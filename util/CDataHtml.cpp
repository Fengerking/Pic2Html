/*******************************************************************************
	File:		CDataHtml.cpp

	Contains:	data html class implement code

	Written by:	Ben King

	Change History (most recent first):
	2018-10-10		Ben			Create file

*******************************************************************************/
#include "stdafx.h"
#include "CDataHtml.h"

CDataHtml::CDataHtml(void)
	: m_nWidth(0)
	, m_nHeight(0)
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

	WriteText(&outFile, "<center>");

	wordItem * pItem = NULL;
	NODEPOS pos = pWord->m_lstWord.GetHeadPosition();
	while (pos != NULL)
	{
		pItem = pWord->m_lstWord.GetNext(pos);

		WriteText(&outFile, "<p>");
		outFile.Write(pItem->m_pTextJson, strlen(pItem->m_pTextJson));
		WriteText(&outFile, "</p>\r\n");
	}
	WriteText(&outFile, "</center>");

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
	strcat(szHead, "</head>\r\n");
	strcat(szHead, "<body>\r\n");

	pIO->Write(szHead, strlen(szHead));

	return 0;
}

int	CDataHtml::WriteFoot(CFile * pIO)
{
	char szFoot[1024];
	strcpy(szFoot, "</body>\r\n");
	strcpy(szFoot, "</html>\r\n");

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
