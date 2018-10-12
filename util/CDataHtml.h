/*******************************************************************************
	File:		CDataHtml.h

	Contains:	the data html class header file.

	Written by:	Ben King

	Change History (most recent first):
	2018-10-10		Ben			Create file

*******************************************************************************/
#ifndef __CDataHtml_H__
#define __CDataHtml_H__

#include "CDataWord.h"

class CDataHtml
{
public:
    CDataHtml(void);
    virtual ~CDataHtml(void);

	virtual void	SetPicSize(int nWidth, int nHeight) { m_nWidth = nWidth; m_nHeight = nHeight; }

	virtual	int		OutTextHtml(CDataWord * pWord, const TCHAR * pFile);
	virtual	int		OutTextWord(CDataWord * pWord, const TCHAR * pFile);
	virtual	int		OutTextUtf8(CDataWord * pWord, const TCHAR * pFile);

protected:
	virtual int		WriteText(CFile * pIO, char * pText);
	virtual int		WriteHead(CFile * pIO);
	virtual int		WriteFoot(CFile * pIO);

protected:
	int				m_nWidth;
	int				m_nHeight;

};

#endif //__CDataHtml_H__
