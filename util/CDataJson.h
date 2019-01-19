/*******************************************************************************
	文件:	CDataJson.h

	内容:	the data json class header file.

	作者:	Ben King

	修改历史:
	2018-10-10		Ben			创建文件

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
	const char *	GetValue (CJsonNode * pNode, const char * pName);
	CJsonNode *		GetRootNode (void) {return m_pJsonRoot;}

protected:
	virtual int		ParseNode(CJsonNode * pParent, const char * pData);
	virtual int		ParseList(CJsonNode * pParent, const char * pData);
	virtual int		Release (void);

	virtual int		GetName(const char * pData, const char ** ppName, int * pSize);
	CJsonNode *		FindNode(CJsonNode * pParent, const char * pName);

protected:
	CJsonNode *		m_pJsonRoot;
};

#endif //__CDataJson_H__
