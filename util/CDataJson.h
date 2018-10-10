/*******************************************************************************
	File:		CDataJson.h

	Contains:	the data json class header file.

	Written by:	Ben King

	Change History (most recent first):
	2018-10-10		Ben			Create file

*******************************************************************************/
#ifndef __CDataJson_H__
#define __CDataJson_H__

#include "CNodeList.h"

class CJsonNode;

class CJsonItem
{
public:
    CJsonItem(void);
    virtual ~CJsonItem(void);

public:
	char *			m_pName;
	char *			m_pValue;
	bool			m_bText;
	CJsonNode *		m_pChild;
};

class CJsonNode
{
public:
    CJsonNode(void);
    virtual ~CJsonNode(void);

	virtual void	AddItem (CJsonItem * pItem) {m_lstItem.AddTail (pItem);}
	virtual void	AddNode (CJsonNode * pNode) {m_lstNode.AddTail (pNode);}

public:
	char *					m_pName;
	CObjectList<CJsonItem>	m_lstItem;
	CObjectList<CJsonNode>	m_lstNode;	
};

class CDataJson
{
public:
    CDataJson(void);
    virtual ~CDataJson(void);

	virtual	int		ParseData (char * pData, int nSize);

	CJsonNode *		FindNode (char * pName);
	CJsonNode *		GetRootNode (void) {return m_pJsonRoot;}

protected:
	virtual int		ParseNode (char * pData, CJsonNode * pNode);
	virtual int		ParseList (char * pData, CJsonNode * pNode);
	virtual int		Release (void);

	virtual int		GetName(char * pData, int nSize, char ** ppName, int * nNameLen);

	CJsonNode *		FindNode (CJsonNode * pNode, char * pName);

protected:
	CJsonNode *		m_pJsonRoot;
};

#endif //__CDataJson_H__
