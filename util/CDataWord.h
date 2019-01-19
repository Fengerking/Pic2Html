/*******************************************************************************
	文件:		CDataWord.h

	内容:	the data word class header file.

	作者:	Ben King

	修改历史:
	2018-10-10		Ben			创建文件

*******************************************************************************/
#ifndef __CDataWord_H__
#define __CDataWord_H__

#include "CNodeList.h"
#include "CDataJson.h"

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
	char *	m_pTextJson;
	TCHAR *	m_pTextWord;
	RECT	m_rcPos;
	POINT	m_ptPos[4];
};

class CDataWord
{
public:
    CDataWord(void);
    virtual ~CDataWord(void);

	virtual	int		ParseData (CDataJson * pJson);
	virtual void	SetPicSize(int nWidth, int nHeight) { m_nWidth = nWidth; m_nHeight = nHeight; }

protected:
	virtual int		AdjustLine(void);
	virtual int		Release(void);

protected:
	CDataJson *				m_pDataJson;
	int						m_nWidth;
	int						m_nHeight;

public:
	CObjectList<wordItem>	m_lstWord;

};

#endif //__CDataWord_H__
