/*******************************************************************************
	File:		CDataHtml.cpp

	Contains:	data html class implement code

	Written by:	Ben King

	Change History (most recent first):
	2018-10-10		Ben			Create file

*******************************************************************************/
#include "stdafx.h"
#include "CDataHtml.h"

//<font size = "8" color = "red">
//<p style = "text-align: center;" />
//<p style = "height: 10px;">
//<img src="#" width="100" height="100" style=" margin-left:10px;" 
//<img src = "ad-02.jpg" width = "400px"; height = "200px"; style = "position:absolute; left:100px; top:100px; ">

CDataHtml::CDataHtml(void)
	: m_pJpegFunc(NULL)
	, m_nLeft(0)
	, m_nRight(0)
	, m_nTop(0)
	, m_nBottom(0)
	, m_nLineMaxWords(0)
	, m_nLineMinH(0)
	, m_nFontSize(16)
	, m_nJpegIndex(0)
{
}

CDataHtml::~CDataHtml(void)
{
}

int	CDataHtml::OutTextHtml(CDataWord * pWord, const TCHAR * pFile)
{
	if (pWord == NULL || pFile == NULL)
		return -1;

	CFile outFile;
	if (!outFile.Open(pFile, CFile::modeCreate | CFile::modeWrite, NULL))
		return -1;

	CString strPath = pFile;
	int nPos = strPath.ReverseFind('.');
	m_strJpegFile = strPath.Left(nPos);

	ParserWordInfo(pWord);
	WriteHead(&outFile);
	
	int		nLineIndex = 0;
	int		nWordCount = 0;
	int		nWordWidth = 0;
	int		nPosFlag = 0; // 0 left, 1 center, 2 right
	int		nFontSize = 16; 
	int		nSpaceNum = 0;

	char		szLine[1024];
	wordItem *	pItem = NULL;
	wordItem *	pPrev = NULL;
	NODEPOS pos = pWord->m_lstWord.GetHeadPosition();
	while (pos != NULL)
	{
		pItem = pWord->m_lstWord.GetNext(pos);
		if (pPrev != NULL)
		{
			int nSpltHeight = pItem->m_rcPos.top - pPrev->m_rcPos.bottom;
			int nItemHeight = pItem->m_rcPos.bottom - pItem->m_rcPos.top;
			int nFindImage = 0;
			if (nSpltHeight > nItemHeight * 2)
			{
				RECT rcArea;
				SetRect(&rcArea, m_nLeft, pPrev->m_rcPos.bottom, m_nRight, pItem->m_rcPos.top);
				nFindImage = CheckImageInfo(&outFile, &rcArea);
			}
			if (nFindImage == 0)
			{
				int nIndex = 1;
				while (nSpltHeight > nItemHeight * nIndex)
				{
					WriteText(&outFile, "<p style = \"height: 4px;\">&nbsp;</p>\r\n");
					nIndex++;
				}
			}
		}

		if (nLineIndex == 26)
			nLineIndex = nLineIndex;

		nWordCount = _tcslen (pItem->m_pTextWord);
		nWordWidth = (pItem->m_rcPos.right - pItem->m_rcPos.left) / nWordCount;

		nPosFlag = 0;
		nSpaceNum = 0;
		if (m_nRight - pItem->m_rcPos.right <= nWordWidth * 2)
		{
			if (pItem->m_rcPos.left - m_nLeft > nWordWidth * 4)
			{
				nPosFlag = 2; // the pos is right
			}
		}
		else if (abs((pItem->m_rcPos.left - m_nLeft) - (m_nRight - pItem->m_rcPos.right)) < nWordWidth * 4)
		{
			nPosFlag = 1;
		}

		if (nPosFlag == 0)
		{
			if (pItem->m_rcPos.left - m_nLeft >= nWordWidth)
			{
				nSpaceNum = (pItem->m_rcPos.left - m_nLeft) / nWordWidth + 1;
			}
		}

		if ((pItem->m_rcPos.bottom - pItem->m_rcPos.top) > m_nLineMinH * 3 / 2)
		{
			nFontSize = (pItem->m_rcPos.bottom - pItem->m_rcPos.top) * m_nFontSize / m_nLineMinH;
		}

		strcpy(szLine, "<p");
		if (nPosFlag != 0 || nFontSize != m_nFontSize)
			strcat(szLine, " style= ");
		if (nPosFlag == 1)
			strcat(szLine, "\"text-align: center; ");
		else if (nPosFlag == 2)
			strcat(szLine, "\"text-align: right; ");
		
		//if (nFontSize != m_nFontSize)
		//	sprintf (szLine, "%s \"font-size: %dpx; ", szLine, nFontSize);

		if (nPosFlag != 0 || nFontSize != m_nFontSize)
			strcat(szLine, " \"");
		strcat(szLine, ">");

		WriteText(&outFile, szLine);

		strcpy(szLine, "");
		for (int i = 0; i < nSpaceNum * 2; i++)
			strcat(szLine, "&nbsp;");
		if (nSpaceNum > 0)
			WriteText(&outFile, szLine);

		outFile.Write(pItem->m_pTextJson, strlen(pItem->m_pTextJson));
		WriteText(&outFile, "</p>\r\n");

		pPrev = pItem;
		nLineIndex++;
	}

	WriteFoot(&outFile);
	outFile.Close();

	return 0;
}

int	CDataHtml::ParserWordInfo(CDataWord * pWord)
{
	m_nLeft = 0XFFFF;
	m_nRight = 0;
	m_nTop = 0XFFFF;
	m_nBottom = 0;
	m_nLineMaxWords = 0;
	m_nLineMinH = 0XFFFF;
	wordItem * pItem = NULL;
	NODEPOS pos = pWord->m_lstWord.GetHeadPosition();
	while (pos != NULL)
	{
		pItem = pWord->m_lstWord.GetNext(pos);
		if (pItem->m_rcPos.left < m_nLeft)
			m_nLeft = pItem->m_rcPos.left;
		if (pItem->m_rcPos.right > m_nRight)
			m_nRight = pItem->m_rcPos.right;
		if (pItem->m_rcPos.top < m_nTop)
			m_nTop = pItem->m_rcPos.top;
		if (pItem->m_rcPos.bottom > m_nBottom)
			m_nBottom = pItem->m_rcPos.bottom;
		if (m_nLineMaxWords < (int)_tcslen(pItem->m_pTextWord))
		{
			m_nLineMaxWords = _tcslen(pItem->m_pTextWord);
			if (m_nLineMinH > (pItem->m_rcPos.bottom - pItem->m_rcPos.top))
				m_nLineMinH = pItem->m_rcPos.bottom - pItem->m_rcPos.top;
		}
	}

	return 0;
}

int	CDataHtml::CheckImageInfo(CFile * pIO, RECT * pArea)
{
	if (m_pJpegFunc == NULL || pArea == NULL)
		return 0;

	int nImgTop = 0;
	int nImgLeft = pArea->left;
	int nImgBottom = pArea->bottom;
	int nImgRight = pArea->right;

	int nWidth = m_pJpegFunc->GetWidth();
	int nHeight = m_pJpegFunc->GetHeight();

	int i, j, nFind;
	unsigned char cBlack = 0X2F;
	unsigned char * pBmpBuff = m_pJpegFunc->GetBuffer();
	unsigned char * pBuff = NULL;
	for (i = pArea->top; i <= pArea->bottom; i++)
	{
		nFind = 0;
		for (j = pArea->left; j < pArea->right; j++)
		{
			pBuff = pBmpBuff + i * nWidth * 4 + j * 4;
			if (*(pBuff + 0) < cBlack || *(pBuff + 1) < cBlack || *(pBuff + 2) < cBlack)
				nFind++;
		}

		if (nFind > 10) // find 10 points
		{
			nImgTop = i;
			break;
		}
	}
	if (nImgTop == 0)
		return 0;

	// Check the bottom
	for (i = pArea->bottom; i >= pArea->top; i--)
	{
		nFind = 0;
		for (j = pArea->left; j < pArea->right; j++)
		{
			pBuff = pBmpBuff + i * nWidth * 4 + j * 4;
			if (*(pBuff + 0) < cBlack || *(pBuff + 1) < cBlack || *(pBuff + 2) < cBlack)
				nFind++;
		}

		if (nFind > 10) // find 10 points
		{
			nImgBottom = i;
			break;
		}
	}

	// Check the left
	for (j = pArea->left; j < pArea->right; j++)
	{
		nFind = 0;
		for (i = nImgTop; i <= nImgBottom; i++)
		{
			pBuff = pBmpBuff + i * nWidth * 4 + j * 4;
			if (*(pBuff + 0) < cBlack || *(pBuff + 1) < cBlack || *(pBuff + 2) < cBlack)
				nFind++;
		}
		if (nFind > 10) // find 10 points
		{
			nImgLeft = j;
			break;
		}
	}

	// Check the right
	for (j = pArea->right; j >= pArea->left; j--)
	{
		nFind = 0;
		for (i = nImgTop; i <= nImgBottom; i++)
		{
			pBuff = pBmpBuff + i * nWidth * 4 + j * 4;
			if (*(pBuff + 0) < cBlack || *(pBuff + 1) < cBlack || *(pBuff + 2) < cBlack)
				nFind++;
		}
		if (nFind > 10) // find 10 points
		{
			nImgRight = j;
			break;
		}
	}

	CString strJpegFile;
	strJpegFile.Format(_T("%s_%d.jpeg"), m_strJpegFile, m_nJpegIndex);

	RECT rcEnc;
	SetRect(&rcEnc, nImgLeft, nImgTop, nImgRight, nImgBottom);
	m_pJpegFunc->Enc(&rcEnc, 65, strJpegFile);

	//<img src="#" width="100" height="100" style=" margin-left:10px;" 
	char szName[1024];
	memset(szName, 0, sizeof(szName));
	WideCharToMultiByte(CP_ACP, 0, strJpegFile.GetString(), -1, szName, sizeof(szName), NULL, NULL);

	nWidth = (nImgRight - nImgLeft) * m_nFontSize / m_nLineMinH * 3 / 2;
	nHeight = (nImgBottom - nImgTop) * m_nFontSize / m_nLineMinH * 3 / 2;
	char szLine[1024];
	sprintf(szLine, "<img src=\"%s\" style=\"width: %dpx; height: %dpx>\" \r\n", szName, nWidth, nHeight);

	if (nImgLeft > m_nLeft + m_nFontSize * 4)
		WriteText(pIO, "<p style = \"text-align: center; \"> \r\n");
	else if (nImgRight > m_nRight - m_nFontSize * 4)
		WriteText(pIO, "<p style = \"text-align: right; \"> \r\n");
	else
		WriteText(pIO, "<p style = \"text-align: left; \"> \r\n");

	WriteText(pIO, szLine);
	WriteText(pIO, "</p>\r\n");

	m_nJpegIndex++;
	return 1;
}

int	CDataHtml::WriteText(CFile * pIO, char * pText)
{
	pIO->Write(pText, strlen(pText));
	return 0;
}

int CDataHtml::WriteHead(CFile * pIO)
{
	char szHead[1024];
	char szLine[128];
	memset(szHead, 0, sizeof(szHead));
	strcpy(szHead, "<!DOCTYPE html>\r\n");
	strcat(szHead, "<html>\r\n");
	strcat(szHead, "<head>\r\n");
	strcat(szHead, "<meta charset = \"utf-8\">\r\n");
	strcat(szHead, "<title>pic to html sample </title>\r\n");

	strcat(szHead, "<style>\r\n");
	strcat(szHead, "   #bookpage{\r\n");

	sprintf(szLine, "        font-size: %dpx;\r\n", m_nFontSize);
	strcat(szHead, szLine);
	sprintf(szLine, "        width: %dpx;\r\n", m_nFontSize * (m_nLineMaxWords + 1));
	strcat(szHead, szLine);

	strcat(szHead, "   }\r\n");
	strcat(szHead, "</style>\r\n");

	strcat(szHead, "</head>\r\n\r\n\r\n");
	strcat(szHead, "<body>\r\n");

	strcat(szHead, " <div id=\"bookpage\">\r\n");

	pIO->Write(szHead, strlen(szHead));

	return 0;
}

int	CDataHtml::WriteFoot(CFile * pIO)
{
	char szFoot[1024];
	strcpy(szFoot, "\r\n</div>\r\n");

	strcpy(szFoot, "\r\n</body>\r\n\r\n");
	strcat(szFoot, "\r\n</html>\r\n");

	pIO->Write(szFoot, strlen(szFoot));

	return 0;
}

int	CDataHtml::OutTextWord(CDataWord * pWord, const TCHAR * pFile)
{
	CFile outFile;
	if (!outFile.Open(pFile, CFile::modeCreate | CFile::modeWrite, NULL))
		return -1;

	DWORD dwFlag = 0X00E8FEFF;
	outFile.Write(&dwFlag, 2);

	wordItem * pItem = NULL;
	NODEPOS pos = pWord->m_lstWord.GetHeadPosition();
	while (pos != NULL)
	{
		pItem = pWord->m_lstWord.GetNext(pos);
		outFile.Write(pItem->m_pTextWord, _tcslen(pItem->m_pTextWord) * sizeof (TCHAR));
		outFile.Write(_T("\r\n"), sizeof(TCHAR) * 2);
	}

	outFile.Close();
	return 0;
}

int	CDataHtml::OutTextUtf8(CDataWord * pWord, const TCHAR * pFile)
{
	CFile outFile;
	if (!outFile.Open(pFile, CFile::modeCreate | CFile::modeWrite, NULL))
		return -1;

	DWORD dwFlag = 0X00BFBBEF;
	outFile.Write(&dwFlag, 3);

	wordItem * pItem = NULL;
	NODEPOS pos = pWord->m_lstWord.GetHeadPosition();
	while (pos != NULL)
	{
		pItem = pWord->m_lstWord.GetNext(pos);
		outFile.Write(pItem->m_pTextJson, strlen(pItem->m_pTextJson));
		outFile.Write("\r\n", 2);
	}

	outFile.Close();

	return 0;
}
