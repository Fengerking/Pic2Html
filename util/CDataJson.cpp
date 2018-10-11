/*******************************************************************************
	File:		CDataJson.cpp

	Contains:	data json class implement code

	Written by:	Ben King

	Change History (most recent first):
	2018-10-10		Ben			Create file

*******************************************************************************/
#include "stdafx.h"
#include "CDataJson.h"

CDataJson::CDataJson(void)
	: m_pJsonRoot (NULL)
{
}

CDataJson::~CDataJson(void)
{
	Release ();
}

int CDataJson::ParseData (const char * pData)
{
	if (pData == NULL)
		return -1;
	Release ();

	int nDataLen = strlen(pData);
	m_pJsonRoot = new CJsonNode ("JSONROOT", 0);
	int nRC = ParseNode (m_pJsonRoot, pData);
	if (nRC == nDataLen)
		return 0;
	else
		return -1;
}

int CDataJson::ParseNode(CJsonNode * pParent, const char * pData)
{
	if (*pData != '{')
		return 0;

	const char *	pText = pData + 1;
	const char *	pNext = NULL;
	const char *	pName = NULL;
	int				nSize = 0;
	int				nRC = 0;
	CJsonNode *		pNode = NULL;
	CJsonItem *		pItem = NULL;
	
	while (*pText != 0)
	{
		nRC = GetName(pText, &pName, &nSize);
		if (nRC <= 0)
			return 0;
		pText = pText + nRC;

		switch (*pText)
		{
		case '[':
			pNode = new CJsonNode(pName, nSize);
			pParent->AddNode(pNode);
			pNode->m_pParent = pParent;
			nRC = ParseList(pNode, pText);
			if (nRC <= 0)
				return nRC;
			pNext = pText + nRC;
			break;

		case '{':
			break;

		default:
			pItem = new CJsonItem(pName, nSize);
			pParent->AddItem(pItem);
			pItem->m_pParent = pParent;
			if (*pText == '"')
			{
				pNext = ++pText;
				while (*pNext != '"' && *pNext != 0) pNext++;
			}
			else
			{
				pNext = pText;
				while (*pNext != ',' && *pNext != '}' && *pNext != 0) pNext++;
			}
			pItem->m_pValue = new char[pNext - pText + 1];
			strncpy(pItem->m_pValue, pText, pNext - pText);
			pItem->m_pValue[pNext - pText] = 0;
			break;
		}

		while (*pNext != ',' && *pNext != '}' && *pNext != 0) pNext++;
		if (*pNext == ',')
			pText = pNext + 1;
		else if (*pNext == '}')
			return pNext - pData + 1;
		else if (*pNext == 0)
			return 0;
		else
			return 0;
	}
	return 0;
}

int CDataJson::ParseList(CJsonNode * pParent, const char * pData)
{
	int				nRC = 0;
	const char *	pText = pData;
	if (*pText != '[')
		return 0;
	pText++;
	while (*pText == ' ') pText++;

	CJsonNode * pNode = NULL;
	char		szName[32];
	int			nIndex = 0;
	while (*pText != ']')
	{
		memset(szName, 0, sizeof(szName));
		sprintf(szName, "%d", nIndex);
		nIndex++;
		pNode = new CJsonNode(szName, 0);
		pParent->AddNode(pNode);
		pNode->m_pParent = pParent;

		nRC = ParseNode(pNode, pText);
		if (nRC <= 0)
			return nRC;
		pText = pText + nRC;
		while (*pText == ' ') pText++;
		if (*pText == ',')
			pText++;
	}
	return pText - pData + 1;
}

int	CDataJson::GetName(const char * pData, const char ** ppName, int * pSize)
{
	const char * pFirst = pData;
	while (*pFirst == ' ') pFirst++;
	if (*pFirst != '"')
		return 0;

	pFirst++;
	const char * pNext = pFirst + 1;
	while (*pNext != '"')
	{
		pNext++;
		if (*pNext == 0)
			return 0;
	}

	*ppName = pFirst;
	*pSize = pNext - pFirst;
	pNext++;

	while (*pNext != ':')
	{
		pNext++;
		if (*pNext == 0)
			return 0;
	}
	pNext++;
	while (*pNext == ' ') pNext++;

	return pNext - pData;
}

CJsonNode *	CDataJson::FindNode(const char * pName)
{
	if (m_pJsonRoot == NULL)
		return NULL;
	return FindNode(m_pJsonRoot, pName);
}

const char * CDataJson::GetValue(CJsonNode * pNode, const char * pName)
{
	CJsonItem * pItem = NULL;
	NODEPOS pos = pNode->m_lstItem.GetHeadPosition();
	while (pos != NULL)
	{
		pItem = pNode->m_lstItem.GetNext(pos);
		if (strcmp(pItem->m_pName, pName) == 0)
		{
			return pItem->m_pValue;
		}
	}
	return NULL;
}

CJsonNode *	CDataJson::FindNode(CJsonNode * pParent, const char * pName)
{
	CJsonNode * pFindNode = NULL;
	NODEPOS pos = pParent->m_lstNode.GetHeadPosition();
	while (pos != NULL)
	{
		pFindNode = pParent->m_lstNode.GetNext(pos);
		if (strcmp(pFindNode->m_pName, pName) == 0)
			return pFindNode;

		pFindNode = FindNode(pFindNode, pName);
		if (pFindNode != NULL)
			return pFindNode;
	}
	return NULL;
}

int CDataJson::Release (void)
{
	SAFE_DEL_P(m_pJsonRoot);
	return 0;
}

CJsonItem::CJsonItem(const char * pName, int nLen)
	: m_pName(NULL)
	, m_pValue(NULL)
	, m_bText(true)
	, m_pParent(NULL)
{
	if (nLen == 0)
		nLen = strlen(pName);
	m_pName = new char[nLen + 1];
	strncpy(m_pName, pName, nLen);
	m_pName[nLen] = 0;
}

CJsonItem::~CJsonItem(void)
{
	SAFE_DEL_A(m_pValue);
	SAFE_DEL_A(m_pName);
}

CJsonNode::CJsonNode(const char * pName, int nLen)
	: m_pName(NULL)
	, m_pParent(NULL)
{
	if (nLen == 0)
		nLen = strlen(pName);
	m_pName = new char[nLen + 1];
	strncpy(m_pName, pName, nLen);
	m_pName[nLen] = 0;
}

CJsonNode::~CJsonNode(void)
{
	CJsonItem * pItem = m_lstItem.RemoveHead ();
	while (pItem != NULL)
	{
		delete pItem;
		pItem = m_lstItem.RemoveHead ();
	}
	CJsonNode * pNode = m_lstNode.RemoveHead ();
	while (pNode != NULL)
	{
		delete pNode;
		pNode = m_lstNode.RemoveHead ();
	}
	SAFE_DEL_A(m_pName);
}
