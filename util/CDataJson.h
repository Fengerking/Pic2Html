/*******************************************************************************
	文件:	CDataJson.h

	内容:	把阿里云服务器返回的JSON数据转换为树结构，方便操作.

	作者:	Ben King

	修改历史:
	2018-10-10		Ben			创建文件

*******************************************************************************/
#ifndef __CDataJson_H__
#define __CDataJson_H__

#include "CNodeList.h"

// 宏定义，安全删除 new object 的指针。
#define	SAFE_DEL_P(p)   \
	if (p != NULL) {	\
		delete p;		\
		p = NULL; }		

// 宏定义，安全删除new 【】的数据。
#define	SAFE_DEL_A(a)   \
	if (a != NULL) {	\
		delete []a;		\
		a = NULL; }		\

class CJsonNode;

// 结构中的内容类
class CJsonItem
{
public:
	CJsonItem(const char * pName, int nLen);
	virtual ~CJsonItem(void);

public:
	char *			m_pName;		// 内容名字
	CJsonNode *		m_pParent;		// 父节点指针
	char *			m_pValue;		// 内容值
	bool			m_bText;		// 是字符串，还是数字
};

// 结构中的节点类
class CJsonNode
{
public:
	CJsonNode(const char * pName, int nLen);
	virtual ~CJsonNode(void);

	// 增加子节点
	virtual void	AddNode (CJsonNode * pNode) {m_lstNode.AddTail (pNode);}
	// 增加内容
	virtual void	AddItem (CJsonItem * pItem) {m_lstItem.AddTail (pItem);}

public:
	char *					m_pName;		// 节点名字
	CJsonNode *				m_pParent;		// 父节点
	CObjectList<CJsonNode>	m_lstNode;		// 节点列表
	CObjectList<CJsonItem>	m_lstItem;		// 内容列表
};

class CDataJson
{
public:
    CDataJson(void);
    virtual ~CDataJson(void);

	// 解析阿里云返回的JSON数据
	// 参数： pData， 阿里云返回的JSON数据
	virtual	int		ParseData (const char * pData);

	// 根据名字查找节点。返回节点指针。
	CJsonNode *		FindNode (const char * pName);
	// 在指定的节点里面，查找内容。返回内容的值。
	const char *	GetValue (CJsonNode * pNode, const char * pName);
	// 返回跟节点。
	CJsonNode *		GetRootNode (void) {return m_pJsonRoot;}

protected:
	// 解析剩下的数据，{ 开始为节点结构。把解析出来的信息增加到父节点里面。
	virtual int		ParseNode(CJsonNode * pParent, const char * pData);
	// 解析剩下的数据，[ 开始为数组结构。把解析出来的信息增加到父节点里面。
	virtual int		ParseList(CJsonNode * pParent, const char * pData);
	// 析构，释放结构，内存。
	virtual int		Release (void);

	// 在字符串中，提取名字。
	virtual int		GetName(const char * pData, const char ** ppName, int * pSize);
	// 在父节点里面，根据名字查找子节点。
	CJsonNode *		FindNode(CJsonNode * pParent, const char * pName);

protected:
	CJsonNode *		m_pJsonRoot;	// 根节点指针。
};

#endif //__CDataJson_H__
