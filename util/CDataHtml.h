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
#include "CJpegFunc.h"

class CDataHtml
{
public:
    CDataHtml(void);
    virtual ~CDataHtml(void);

	virtual void	SetJpegFunc(CJpegFunc * pJpegFunc) { m_pJpegFunc = pJpegFunc; }

	virtual	int		OutTextHtml(CDataWord * pWord, const TCHAR * pFile);
	virtual	int		OutTextWord(CDataWord * pWord, const TCHAR * pFile);
	virtual	int		OutTextUtf8(CDataWord * pWord, const TCHAR * pFile);

protected:
	virtual int		ParserWordInfo(CDataWord * pWord);
	virtual int		CheckImageInfo(CFile * pIO, RECT * pArea);

	virtual int		WriteText(CFile * pIO, char * pText);
	virtual int		WriteHead(CFile * pIO);
	virtual int		WriteFoot(CFile * pIO);

protected:
	CJpegFunc *		m_pJpegFunc;

	int				m_nLeft;
	int				m_nRight;
	int				m_nTop;
	int				m_nBottom;
	int				m_nLineMaxWords;
	int				m_nLineMinH;
	int				m_nFontSize;

	CString			m_strJpegFile;
	int				m_nJpegIndex;
};

#endif //__CDataHtml_H__
