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

int CDataJson::ParseData (char * pData, int nSize)
{
	if (pData == NULL || *pData != '{')
		return -1;
	Release ();

	m_pJsonRoot = new CJsonNode ();
	ParseNode (pData, m_pJsonRoot);

	return 0;
}

int CDataJson::ParseNode (char * pData, CJsonNode * pNode)
{
	int		nUsed = 0;
	int		nLen = strlen (pData);
	char *	pPos = pData;
	char *	pNext = pData;
	int		nUsedSize = 0;

	if (pPos == NULL || *pPos != '{')
		return 0;

	while (pPos - pData < nLen)
	{
		while (*pPos != '"')
		{
			pPos++;
			if (pPos - pData >= nLen)
				return 0;
		}
		pPos++;
		pNext = pPos;
		while (*pNext != '"')
		{
			pNext++;
			if (pNext - pData >= nLen)
				return 0;
		}
		CJsonItem * pItem = new CJsonItem ();
		pNode->AddItem (pItem);
		if (pNext - pPos > 0)
		{
			pItem->m_pName = new char[pNext - pPos + 1];
			strncpy(pItem->m_pName, pPos, pNext - pPos);
		}
		pPos = pNext + 1;
		if (*pPos != ':')
			return 0;
		while (*pPos++ == ' ');
		pNext = pPos + 1;
		if (*pPos == '{')
		{
			pItem->m_pChild = new CJsonNode ();
			nUsedSize =  ParseNode (pPos, pItem->m_pChild);
			pPos += nUsedSize;
		}
		else if (*pPos == '[')
		{
			pItem->m_pChild = new CJsonNode ();
			nUsedSize =  ParseList (pPos, pItem->m_pChild);
			pPos += nUsedSize;
		}
		else
		{
			if (*pPos == '"')
			{
				pPos++;
				pNext = pPos;
				while (*pNext != '"')
				{
					pNext++;
					if (pNext - pData >= nLen)
						return 0;
				}
			}
			else
			{
				while (*pNext != ',')
				{
					pNext++;
					if (pNext - pData >= nLen)
						return 0;
				}
			}
			pItem->m_pValue = new char[pNext - pPos + 2];
			memset (pItem->m_pValue, 0, (pNext - pPos) + 2);
			if (pNext - pPos > 0)
				strncpy (pItem->m_pValue, pPos, pNext - pPos);
			pPos = pNext+1;
		}
		if (*pPos == '}')
		{
			pPos++;
			return pPos - pData;
		}

	}
	return pPos - pData;
}

int CDataJson::ParseList (char * pData, CJsonNode * pNode)
{
	char *	pPos = pData;
	int		nLen = strlen (pData);
	int		nUsedSize = 0;
	if (*pPos != '[')
		return 0;
	pPos++;
	while (pPos - pData < nLen)
	{
		CJsonNode * pNewNode = new CJsonNode ();
		pNode->AddNode (pNewNode);
		nUsedSize = ParseNode (pPos, pNewNode);
		if (nUsedSize <= 0)
			break;
		pPos += nUsedSize;
		if (*pPos == ']')
		{
			pPos++;
			break;
		}
		if (*pPos == ',')
			pPos++;
	}
	return pPos - pData;
}

int	CDataJson::GetName(char * pData, int nLen, char ** ppName, int * pSize)
{
	char * pFirst = pData;
	while (*pFirst++ == ' ');
	if (*pFirst != '"')
		return 0;

	char * pNext = pFirst + 1;
	while (*pNext != '"')
	{
		pNext++;
		if (pNext - pData >= nLen)
			return 0;
	}

	*ppName = pFirst;
	*pSize = pNext - pFirst;
	return pNext - pData;
}

CJsonNode *	CDataJson::FindNode(char * pName)
{
	if (m_pJsonRoot == NULL)
		return NULL;
	return FindNode(m_pJsonRoot, pName);
}


CJsonNode *	CDataJson::FindNode(CJsonNode * pNode, char * pName)
{
	CJsonNode * pFindNode = NULL;
	CJsonItem * pItem = NULL;
	NODEPOS pos = pNode->m_lstItem.GetHeadPosition();
	while (pos != NULL)
	{
		pItem = pNode->m_lstItem.GetNext(pos);
		if (!strcmp(pItem->m_pName, pName) && (pItem->m_pChild != NULL))
		{
			return pItem->m_pChild;
		}
		if (pItem->m_pChild != NULL)
		{
			pFindNode = FindNode(pItem->m_pChild, pName);
			if (pFindNode != NULL)
				return pFindNode;
		}
	}
	pos = pNode->m_lstNode.GetHeadPosition();
	while (pos != NULL)
	{
		pFindNode = pNode->m_lstNode.GetNext(pos);
		pFindNode = FindNode(pFindNode, pName);
		if (pFindNode != NULL)
			return pFindNode;
	}
	return NULL;
}

int CDataJson::Release (void)
{
	if (m_pJsonRoot != NULL)
	{
		delete m_pJsonRoot;
		m_pJsonRoot = NULL;
	}
	return 0;
}

CJsonItem::CJsonItem(void)
	: m_pName(NULL)
	, m_pValue (NULL)
	, m_bText(true)
	, m_pChild (NULL)
{
}

CJsonItem::~CJsonItem(void)
{
}

CJsonNode::CJsonNode(void)
	: m_pName(NULL)
{
}

CJsonNode::~CJsonNode(void)
{
	CJsonItem * pItem = m_lstItem.RemoveHead ();
	while (pItem != NULL)
	{
		if (pItem->m_pValue != NULL)
		{
			delete[]pItem->m_pValue;
			pItem->m_pValue = NULL;
		}
		if (pItem->m_pChild != NULL)
		{
			delete pItem->m_pChild;
			pItem->m_pChild = NULL;
		}
		delete pItem;
		pItem = m_lstItem.RemoveHead ();
	}
	CJsonNode * pNode = m_lstNode.RemoveHead ();
	while (pNode != NULL)
	{
		delete pNode;
		pNode = m_lstNode.RemoveHead ();
	}
}
