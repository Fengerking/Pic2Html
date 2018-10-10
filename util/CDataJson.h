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

#define	SAFE_DEL_P(p)   \
	if (p != NULL) {	\
		delete p;		\
		p = NULL; }		

#define	SAFE_DEL_A(a)   \
	if (a != NULL) {	\
		delete []a;		\
		a = NULL; }		\

class CJsonNode;

class CJsonItem
{
public:
    CJsonItem(void);
	CJsonItem(const char * pName, int nLen);
	virtual ~CJsonItem(void);

public:
	char *			m_pName;
	CJsonNode *		m_pParent;
	char *			m_pValue;
	bool			m_bText;
};

class CJsonNode
{
public:
    CJsonNode(void);
	CJsonNode(const char * pName, int nLen);
	virtual ~CJsonNode(void);

	virtual void	AddNode (CJsonNode * pNode) {m_lstNode.AddTail (pNode);}
	virtual void	AddItem (CJsonItem * pItem) {m_lstItem.AddTail (pItem);}

public:
	char *					m_pName;
	CJsonNode *				m_pParent;
	CObjectList<CJsonNode>	m_lstNode;	
	CObjectList<CJsonItem>	m_lstItem;
};

class CDataJson
{
public:
    CDataJson(void);
    virtual ~CDataJson(void);

	virtual	int		ParseData (const char * pData);

	CJsonNode *		FindNode (const char * pName);
	CJsonNode *		GetRootNode (void) {return m_pJsonRoot;}

protected:
	virtual int		ParseNode(CJsonNode * pParent, const char * pData);
	virtual int		ParseList(CJsonNode * pParent, const char * pData);
	virtual int		Release (void);

	virtual int		GetName(const char * pData, const char ** ppName, int * pSize);

	CJsonNode *		FindNode (CJsonNode * pNode, const char * pName);

protected:
	CJsonNode *		m_pJsonRoot;
};

#endif //__CDataJson_H__
