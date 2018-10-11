/*******************************************************************************
	File:		CDataWord.cpp

	Contains:	data word class implement code

	Written by:	Ben King

	Change History (most recent first):
	2018-10-10		Ben			Create file

*******************************************************************************/
#include "stdafx.h"
#include "CDataWord.h"

CDataWord::CDataWord(void)
	: m_pDataJson (NULL)
{
}

CDataWord::~CDataWord(void)
{
	Release();
}

int CDataWord::ParseData (CDataJson * pJson)
{
	m_pDataJson = pJson;
	CJsonNode * pWordList = m_pDataJson->FindNode("prism_wordsInfo");
	if (pWordList == NULL)
		return -1;
	Release();

	wordItem *		pItemWord = NULL;
	const char *	pValue = NULL;
	int				nSize = 0;
	CJsonNode *		pNodeWord = NULL;
	CJsonNode *		pNodePos1 = NULL;
	CJsonNode *		pNodePos2 = NULL;
	NODEPOS			pPosPos1 = NULL;
	NODEPOS			pPosPos2 = NULL;
	NODEPOS			pPosWord = pWordList->m_lstNode.GetHeadPosition();
	while (pPosWord != NULL)
	{
		pNodeWord = pWordList->m_lstNode.GetNext(pPosWord);
		pValue = m_pDataJson->GetValue(pNodeWord, "word");
		if (pValue == NULL)
			continue;

		pItemWord = new wordItem();
		m_lstWord.AddTail(pItemWord);
		nSize = strlen(pValue) + 1;
		pItemWord->m_pTextJson = new char[nSize];
		strcpy(pItemWord->m_pTextJson, pValue);
		pItemWord->m_pTextWord = new TCHAR[nSize];
		memset(pItemWord->m_pTextWord, 0, sizeof(TCHAR) * nSize);
		MultiByteToWideChar(CP_UTF8, 0, pValue, -1, pItemWord->m_pTextWord, nSize);

		pPosPos1 = pNodeWord->m_lstNode.GetHeadPosition();
		while (pPosPos1 != NULL)
		{
			pNodePos1 = pNodeWord->m_lstNode.GetNext(pPosPos1);
			pPosPos2 = pNodePos1->m_lstNode.GetHeadPosition();
			int nIndex = 0;
			while (pPosPos2 != NULL)
			{
				pNodePos2 = pNodePos1->m_lstNode.GetNext(pPosPos2);
				pValue = m_pDataJson->GetValue(pNodePos2, "x");
				if (pValue != NULL)
				{
					pItemWord->m_ptPos[nIndex].x = atoi(pValue);
					if (pItemWord->m_rcPos.left > pItemWord->m_ptPos[nIndex].x)
						pItemWord->m_rcPos.left = pItemWord->m_ptPos[nIndex].x;
					if (pItemWord->m_rcPos.right < pItemWord->m_ptPos[nIndex].x)
						pItemWord->m_rcPos.right = pItemWord->m_ptPos[nIndex].x;
				}
				pValue = m_pDataJson->GetValue(pNodePos2, "y");
				if (pValue != NULL)
				{
					pItemWord->m_ptPos[nIndex].y = atoi(pValue);
					if (pItemWord->m_rcPos.top > pItemWord->m_ptPos[nIndex].y)
						pItemWord->m_rcPos.top = pItemWord->m_ptPos[nIndex].y;
					if (pItemWord->m_rcPos.bottom < pItemWord->m_ptPos[nIndex].y)
						pItemWord->m_rcPos.bottom = pItemWord->m_ptPos[nIndex].y;
				}
				nIndex++;
			}
		}
	}

	return 0;
}

int	CDataWord::Release(void)
{
	wordItem * pItem = m_lstWord.RemoveHead();
	while (pItem != NULL)
	{
		SAFE_DEL_A(pItem->m_pTextJson);
		SAFE_DEL_A(pItem->m_pTextWord);
		delete pItem;
		pItem = m_lstWord.RemoveHead();
	}
	return 0;
}
