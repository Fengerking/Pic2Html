/*******************************************************************************
	�ļ�:	CDataJson.h

	����:	�Ѱ����Ʒ��������ص�JSON����ת��Ϊ���ṹ���������.

	����:	Ben King

	�޸���ʷ:
	2018-10-10		Ben			�����ļ�

*******************************************************************************/
#ifndef __CDataJson_H__
#define __CDataJson_H__

#include "CNodeList.h"

// �궨�壬��ȫɾ�� new object ��ָ�롣
#define	SAFE_DEL_P(p)   \
	if (p != NULL) {	\
		delete p;		\
		p = NULL; }		

// �궨�壬��ȫɾ��new ���������ݡ�
#define	SAFE_DEL_A(a)   \
	if (a != NULL) {	\
		delete []a;		\
		a = NULL; }		\

class CJsonNode;

// �ṹ�е�������
class CJsonItem
{
public:
	CJsonItem(const char * pName, int nLen);
	virtual ~CJsonItem(void);

public:
	char *			m_pName;		// ��������
	CJsonNode *		m_pParent;		// ���ڵ�ָ��
	char *			m_pValue;		// ����ֵ
	bool			m_bText;		// ���ַ�������������
};

// �ṹ�еĽڵ���
class CJsonNode
{
public:
	CJsonNode(const char * pName, int nLen);
	virtual ~CJsonNode(void);

	// �����ӽڵ�
	virtual void	AddNode (CJsonNode * pNode) {m_lstNode.AddTail (pNode);}
	// ��������
	virtual void	AddItem (CJsonItem * pItem) {m_lstItem.AddTail (pItem);}

public:
	char *					m_pName;		// �ڵ�����
	CJsonNode *				m_pParent;		// ���ڵ�
	CObjectList<CJsonNode>	m_lstNode;		// �ڵ��б�
	CObjectList<CJsonItem>	m_lstItem;		// �����б�
};

class CDataJson
{
public:
    CDataJson(void);
    virtual ~CDataJson(void);

	// ���������Ʒ��ص�JSON����
	// ������ pData�� �����Ʒ��ص�JSON����
	virtual	int		ParseData (const char * pData);

	// �������ֲ��ҽڵ㡣���ؽڵ�ָ�롣
	CJsonNode *		FindNode (const char * pName);
	// ��ָ���Ľڵ����棬�������ݡ��������ݵ�ֵ��
	const char *	GetValue (CJsonNode * pNode, const char * pName);
	// ���ظ��ڵ㡣
	CJsonNode *		GetRootNode (void) {return m_pJsonRoot;}

protected:
	// ����ʣ�µ����ݣ�{ ��ʼΪ�ڵ�ṹ���ѽ�����������Ϣ���ӵ����ڵ����档
	virtual int		ParseNode(CJsonNode * pParent, const char * pData);
	// ����ʣ�µ����ݣ�[ ��ʼΪ����ṹ���ѽ�����������Ϣ���ӵ����ڵ����档
	virtual int		ParseList(CJsonNode * pParent, const char * pData);
	// �������ͷŽṹ���ڴ档
	virtual int		Release (void);

	// ���ַ����У���ȡ���֡�
	virtual int		GetName(const char * pData, const char ** ppName, int * pSize);
	// �ڸ��ڵ����棬�������ֲ����ӽڵ㡣
	CJsonNode *		FindNode(CJsonNode * pParent, const char * pName);

protected:
	CJsonNode *		m_pJsonRoot;	// ���ڵ�ָ�롣
};

#endif //__CDataJson_H__
