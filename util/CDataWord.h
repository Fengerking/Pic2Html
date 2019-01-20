/*******************************************************************************
	�ļ�:	CDataWord.h

	����:	��JSON���ݽṹ�����תΪ�򵥵�����������Ϣ��

	����:	Ben King

	�޸���ʷ:
	2018-10-10		Ben			�����ļ�

*******************************************************************************/
#ifndef __CDataWord_H__
#define __CDataWord_H__

#include "CNodeList.h"
#include "CDataJson.h"
#include "CJpegFunc.h"

// ������Ϣ�ṹ��
struct wordItem {
	wordItem(void)
	{
		m_pTextJson = NULL;
		m_pTextWord = NULL;
		SetRect(&m_rcPos, 0XFFFF, 0XFFFF, 0, 0);
	}
	~wordItem(void)
	{
		SAFE_DEL_A(m_pTextJson);
		SAFE_DEL_A(m_pTextWord);
	}
	char *	m_pTextJson;		// UTF8 ԭʼ����
	TCHAR *	m_pTextWord;		// UNICODE ����
	RECT	m_rcPos;			// ���ֵ�λ����Ϣ
	POINT	m_ptPos[4];			// �������������ĸ������Ϣ
};

class CDataWord
{
public:
    CDataWord(void);
    virtual ~CDataWord(void);

	// ����JPEG��װ��ָ�룬���в�ͼ�жϣ���ͼѹ����
	virtual void	SetJpegFunc(CJpegFunc * pJpegFunc) { m_pJpegFunc = pJpegFunc; }

	// ����JSON���ݽṹ��ת��Ϊ��������
	virtual	int		ParseData (CDataJson * pJson);

	// ����ͼƬ�Ĵ�С�ߴ硣
	virtual void	SetPicSize(int nWidth, int nHeight) { m_nWidth = nWidth; m_nHeight = nHeight; }

protected:
	// �������ֵ�λ�ã��ж��ǲ���ͬһ�е����֡�
	virtual int		AdjustLine(void);
	// �������ͷ�
	virtual int		Release(void);

protected:
	CDataJson *				m_pDataJson;	// JSON���ݽṹ��ָ��
	CJpegFunc *				m_pJpegFunc;	// JPEG�İ�װ��ָ��
	int						m_nWidth;		// ͼƬ�Ŀ��
	int						m_nHeight;		// ͼƬ�ĸ߶�

public:
	CObjectList<wordItem>	m_lstWord;		// ������Ϣ�б�

};

#endif //__CDataWord_H__
