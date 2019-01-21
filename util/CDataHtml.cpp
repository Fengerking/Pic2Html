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
	, m_nTxtLeft(0)
	, m_nTxtRight(0)
	, m_nTxtTop(0)
	, m_nTxtBottom(0)
	, m_nLineMaxWords(0)
	, m_nLineMinH(0)
	, m_nFontSize(16)
	, m_dTxtScale(1)
	, m_nJpegIndex(0)
{
	memset(m_szPrevFile, 0, sizeof(m_szPrevFile));
	memset(m_szNextFile, 0, sizeof(m_szNextFile));
}

CDataHtml::~CDataHtml(void)
{
}

void CDataHtml::SetPrevNextFile(TCHAR * strPrevFile, TCHAR * strNextFile)
{
	memset(m_szPrevFile, 0, sizeof(m_szPrevFile));
	memset(m_szNextFile, 0, sizeof(m_szNextFile));

	TCHAR * pFileName = 0;
	if (strPrevFile != NULL && _tcslen(strPrevFile) > 0)
	{
		pFileName = _tcsrchr(strPrevFile, _T('\\')) + 1;
		WideCharToMultiByte(CP_ACP, 0, pFileName, -1, m_szPrevFile, sizeof(m_szPrevFile), NULL, NULL);
	}
	if (strNextFile != NULL && _tcslen(strNextFile) > 0)
	{
		pFileName = _tcsrchr(strNextFile, _T('\\')) + 1;
		WideCharToMultiByte(CP_ACP, 0, pFileName, -1, m_szNextFile, sizeof(m_szNextFile), NULL, NULL);
	}
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
	int			nSpltHeight = 0;
	int			nItemHeight = 0;
	int			nFindImage = 0;
	NODEPOS pos = pWord->m_lstWord.GetHeadPosition();
	while (pos != NULL)
	{
		pItem = pWord->m_lstWord.GetNext(pos);
		nFindImage = 0;
		nItemHeight = pItem->m_rcPos.bottom - pItem->m_rcPos.top;
		if (pPrev != NULL)
			nSpltHeight = pItem->m_rcPos.top - pPrev->m_rcPos.bottom;
		else
			nSpltHeight = pItem->m_rcPos.top;
		if (nSpltHeight > nItemHeight * 2)
		{
			RECT rcArea;
			if (pPrev != NULL)
				SetRect(&rcArea, m_nTxtLeft, pPrev->m_rcPos.bottom, m_nTxtRight, pItem->m_rcPos.top);
			else
				SetRect(&rcArea, m_nTxtLeft, 10, m_nTxtRight, pItem->m_rcPos.top);
			nFindImage = CheckImageInfo(&outFile, &rcArea);
		}
		if (nFindImage == 0)
		{
			int nIndex = 1;
			while (nSpltHeight > nItemHeight * nIndex)
			{
				WriteText(&outFile, "<p style = \"height: 2px;\">&nbsp;</p>\r\n");
				nIndex++;
			}
		}

		nWordCount = _tcslen (pItem->m_pTextWord);
		nWordWidth = (pItem->m_rcPos.right - pItem->m_rcPos.left) / nWordCount;

		nPosFlag = 0;
		nSpaceNum = 0;
		if (pItem->m_rcPos.left - m_nTxtLeft < nWordWidth * 2)
		{
			nPosFlag = 0;
		}
		else if (m_nTxtRight - pItem->m_rcPos.right <= nWordWidth * 2)
		{
			if (pItem->m_rcPos.left - m_nTxtLeft > nWordWidth * 4)
			{
				nPosFlag = 2; // the pos is right
			}
		}
		else if (abs((pItem->m_rcPos.left - m_nTxtLeft) - (m_nTxtRight - pItem->m_rcPos.right)) < nWordWidth * 4)
		{
			nPosFlag = 1;
		}

		if (nPosFlag == 0)
		{
			if (pItem->m_rcPos.left - m_nTxtLeft >= nWordWidth)
			{
				nSpaceNum = (pItem->m_rcPos.left - m_nTxtLeft) / nWordWidth + 1;
			}
		}

		if ((pItem->m_rcPos.bottom - pItem->m_rcPos.top) > m_nLineMinH * 3 / 2)
		{
			nFontSize = (pItem->m_rcPos.bottom - pItem->m_rcPos.top) * m_nFontSize / m_nLineMinH;
		}

		strcpy(szLine, "<p");
		if (nPosFlag != 0)// || nFontSize != m_nFontSize)
			strcat(szLine, " style= ");
		if (nPosFlag == 1)
			strcat(szLine, "\"text-align: center; ");
		else if (nPosFlag == 2)
			strcat(szLine, "\"text-align: right; ");
		
//		if (nFontSize != m_nFontSize)
//			sprintf (szLine, "%s \"font-size: %dpx; ", szLine, nFontSize);

		if (nPosFlag != 0)// || nFontSize != m_nFontSize)
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

	int nBottom = m_pJpegFunc->GetHeight() - 10;
	if (pItem != NULL && pItem->m_rcPos.bottom + (pItem->m_rcPos.bottom - pItem->m_rcPos.top) * 3 < nBottom)
	{
		RECT rcArea;
		SetRect(&rcArea, m_nTxtLeft, pItem->m_rcPos.bottom, m_nTxtRight, nBottom);
		CheckImageInfo(&outFile, &rcArea);
	}

	WriteFoot(&outFile);
	outFile.Close();

	return 0;
}

int	CDataHtml::ParserWordInfo(CDataWord * pWord)
{
	m_nTxtLeft = 0XFFFF;
	m_nTxtRight = 0;
	m_nTxtTop = 0XFFFF;
	m_nTxtBottom = 0;
	m_nLineMaxWords = 0;
	m_nLineMinH = 0XFFFF;
	wordItem * pItem = NULL;
	NODEPOS pos = pWord->m_lstWord.GetHeadPosition();
	while (pos != NULL)
	{
		pItem = pWord->m_lstWord.GetNext(pos);
		if (pItem->m_rcPos.left < m_nTxtLeft)
			m_nTxtLeft = pItem->m_rcPos.left;
		if (pItem->m_rcPos.right > m_nTxtRight)
			m_nTxtRight = pItem->m_rcPos.right;
		if (pItem->m_rcPos.top < m_nTxtTop)
			m_nTxtTop = pItem->m_rcPos.top;
		if (pItem->m_rcPos.bottom > m_nTxtBottom)
			m_nTxtBottom = pItem->m_rcPos.bottom;
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

	RECT rcEnc;
	if (m_pJpegFunc->CheckSubImg(pArea, &rcEnc) <= 0)
		return 0;

	CString strJpegFile;
	strJpegFile.Format(_T("%s_%d.jpeg"), m_strJpegFile, m_nJpegIndex);

	m_pJpegFunc->Enc(&rcEnc, 65, strJpegFile);

	//<img src="#" width="100" height="100" style=" margin-left:10px;" 
	char szName[1024];
	memset(szName, 0, sizeof(szName));
	int nPos = strJpegFile.ReverseFind(_T('\\'));
	strJpegFile = strJpegFile.Right(strJpegFile.GetLength() - nPos - 1);
	WideCharToMultiByte(CP_ACP, 0, strJpegFile.GetString(), -1, szName, sizeof(szName), NULL, NULL);

	int nWidth = (rcEnc.right - rcEnc.left) * (m_nFontSize * (m_nLineMaxWords + 1)) / (m_nTxtRight - m_nTxtLeft);
	int nHeight = nWidth * (rcEnc.bottom - rcEnc.top) / (rcEnc.right - rcEnc.left);
	char szLine[1024];
	sprintf(szLine, "<img src=\"%s\" style=\"width: %dpx; height: %dpx\"> \r\n", szName, nWidth, nHeight);

	if (abs((rcEnc.left - m_nTxtLeft) - (m_nTxtRight - rcEnc.right)) < (int)((m_nFontSize * 4) * m_dTxtScale))
		WriteText(pIO, "<p style = \"text-align: center; \"> \r\n");
	else if ((m_nTxtRight - rcEnc.right) < (int)((m_nFontSize * 4) * m_dTxtScale))
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
	strcat(szHead,  "        line-height: 4px;\r\n");
	sprintf(szLine, "        width: %dpx;\r\n", m_nFontSize * (m_nLineMaxWords + 1));
	strcat(szHead, szLine);
	m_dTxtScale = (double)(m_nTxtRight - m_nTxtLeft) / (m_nFontSize * (m_nLineMaxWords + 1));

	strcat(szHead, "   }\r\n");
	strcat(szHead, "</style>\r\n");

	strcat(szHead, "</head>\r\n\r\n\r\n");
	strcat(szHead, "<body>\r\n");

	strcat(szHead, " <div id=\"bookpage\">\r\n");
	strcat(szHead, "<p>&nbsp</p>\r\n");

	pIO->Write(szHead, strlen(szHead));

	if (strlen(m_szPrevFile) > 0 || strlen(m_szNextFile) > 0)
	{
		memset(szHead, 0, sizeof(szHead));
		if (strlen(m_szPrevFile) > 0)
		{
			sprintf(szLine, "<a href=\"%s\">prev</a> &nbsp &nbsp &nbsp \r\n", m_szPrevFile);
			strcat(szHead, szLine);
		}
		if (strlen(m_szNextFile) > 0)
		{
			sprintf(szLine, "<a href=\"%s\">next</a>\r\n", m_szNextFile);
			strcat(szHead, szLine);
		}
		pIO->Write(szHead, strlen(szHead));
	}

	return 0;
}

int	CDataHtml::WriteFoot(CFile * pIO)
{
	char szFoot[1024];
	strcpy(szFoot, "\r\n</div>\r\n");

	strcat(szFoot, "\r\n</body>\r\n\r\n");
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
