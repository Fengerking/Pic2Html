/*******************************************************************************
	File:		CDataWord.h

	Contains:	the data word class header file.

	Written by:	Ben King

	Change History (most recent first):
	2018-10-10		Ben			Create file

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
