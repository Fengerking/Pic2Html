/*******************************************************************************
	File:		CJpegFunc.h

	Contains:	the jpeg func class header file.

	Written by:	Ben King

	Change History (most recent first):
	2018-10-08		Ben			Create file

*******************************************************************************/
#ifndef __CJpegFunc_H__
#define __CJpegFunc_H__
#include <string>  
#include <iostream>  

#include "jpeglib.h"

class CJpegFunc
{
public:
    CJpegFunc(void);
    virtual ~CJpegFunc(void);

	virtual int		Dec(const TCHAR * pFile);
	virtual int		Enc(RECT * pRect, int nQuality, const TCHAR * pFile);
	virtual int		Draw(HWND hWnd, HDC hDC, RECT * pDraw);

	virtual int		GetWidth(void) { return m_nWidth; }
	virtual int		GetHeight(void) { return m_nHeight; }
	virtual HBITMAP	GetBitmap(void) { return m_hBmpPic; }
	unsigned char *	GetBuffer(void) { return m_pBmpBuff; }


protected:
	virtual int		Close(void);

protected:
	int				m_nWidth;
	int				m_nHeight;
	HBITMAP			m_hBmpPic;
	unsigned char *	m_pBmpBuff;

	HDC				m_hMemDC;
	HBITMAP			m_hOldBmp;

};

#endif //__CJpegFunc_H__
