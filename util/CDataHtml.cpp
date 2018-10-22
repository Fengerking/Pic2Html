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

CDataHtml::CDataHtml(void)
	: m_nLeft(0)
	, m_nRight(0)
	, m_nTop(0)
	, m_nBottom(0)
	, m_nLineMaxWords(0)
	, m_nLineMinH(0)
	, m_nFontSize(16)
{
}

CDataHtml::~CDataHtml(void)
{
}

int	CDataHtml::OutTextHtml(CDataWord * pWord, const TCHAR * pFile)
{
	if (pWord == NULL || pFile == NULL)
		return -1;

	ParserWordInfo(pWord);

	CFile outFile;
	if (!outFile.Open(pFile, CFile::modeCreate | CFile::modeWrite, NULL))
		return -1;

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
			int nIndex = 1;
			while (nSpltHeight > nItemHeight * nIndex)
			{
				WriteText(&outFile, "<p style = \"height: 8px;\">&nbsp;</p>\r\n");
				nIndex++;
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
		if (m_nLineMaxWords < _tcslen(pItem->m_pTextWord))
		{
			m_nLineMaxWords = _tcslen(pItem->m_pTextWord);
			if (m_nLineMinH > (pItem->m_rcPos.bottom - pItem->m_rcPos.top))
				m_nLineMinH = pItem->m_rcPos.bottom - pItem->m_rcPos.top;
		}
	}

	return 0;
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
	sprintf(szLine, "        width: %dpx;\r\n", m_nFontSize * m_nLineMaxWords);
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
