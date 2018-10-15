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
{
}

CDataHtml::~CDataHtml(void)
{
}

int	CDataHtml::OutTextHtml(CDataWord * pWord, const TCHAR * pFile)
{
	CFile outFile;
	if (!outFile.Open(pFile, CFile::modeCreate | CFile::modeWrite, NULL))
		return -1;

	WriteHead(&outFile);

	m_nLeft = 0XFFFF;
	m_nRight = 0;
	m_nTop = 0XFFFF;
	m_nBottom = 0;
	wordItem * pItem = NULL;
	wordItem * pPrev = NULL;
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
	}
	
	bool	bCenterPos = false;
	int		nCenterWdt = (m_nRight - m_nLeft) / 100;
	int		nCenterItm = 0;
	int		nCenterLin = (m_nRight + m_nLeft) / 2;
	int		nWordCount = 0;
	pos = pWord->m_lstWord.GetHeadPosition();
	while (pos != NULL)
	{
		pItem = pWord->m_lstWord.GetNext(pos);
		if (pPrev != NULL)
		{
			int nSpltHeight = pItem->m_rcPos.top - pPrev->m_rcPos.bottom;
			int nItemHeight = pItem->m_rcPos.bottom - pItem->m_rcPos.top;
			int nIndex = -10;
			while (nSpltHeight > nItemHeight * nIndex)
			{
				WriteText(&outFile, "<p style = \"height: 10px;\"> </p>\r\n");
				nIndex++;
			}
		}

		nWordCount = _tcslen (pItem->m_pTextWord);
		nCenterWdt = (pItem->m_rcPos.right - pItem->m_rcPos.left) / nWordCount * 2;

		WriteText(&outFile, "<p");
		nCenterItm = (pItem->m_rcPos.right + pItem->m_rcPos.left) / 2;
		if (abs(nCenterLin - nCenterItm) < nCenterWdt)
		{
			WriteText(&outFile, " style= \"text-align: center;\"");
		}
		WriteText(&outFile, ">");


		outFile.Write(pItem->m_pTextJson, strlen(pItem->m_pTextJson));
		WriteText(&outFile, "</p>\r\n");

		pPrev = pItem;
	}

	WriteFoot(&outFile);
	outFile.Close();

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
	memset(szHead, 0, sizeof(szHead));
	strcpy(szHead, "<!DOCTYPE html>\r\n");
	strcat(szHead, "<html>\r\n");
	strcat(szHead, "<head>\r\n");
	strcat(szHead, "<meta charset = \"utf-8\">\r\n");
	strcat(szHead, "<title>pic to html sample </title>\r\n");
	strcat(szHead, "</head>\r\n\r\n\r\n");
	strcat(szHead, "<body>\r\n");

	pIO->Write(szHead, strlen(szHead));

	return 0;
}

int	CDataHtml::WriteFoot(CFile * pIO)
{
	char szFoot[1024];
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
