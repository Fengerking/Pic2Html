/*******************************************************************************
	文件:	CJpegFunc.h

	内容:	JPEG文件的解码，编码，绘图

	作者:	Ben King

	修改历史:
	2018-10-08		Ben			创建文件

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

	// 解码JPEG文件。
	virtual int		Dec(const TCHAR * pFile);
	// 编码JPEG文件。
	virtual int		Enc(RECT * pRect, int nQuality, const TCHAR * pFile);
	// 画JPEG文件到指定的区域的窗口上。
	virtual int		Draw(HWND hWnd, HDC hDC, RECT * pDraw);

	// 返回宽度
	virtual int		GetWidth(void) { return m_nWidth; }
	// 返回高度
	virtual int		GetHeight(void) { return m_nHeight; }
	// 返回BITMAP句柄
	virtual HBITMAP	GetBitmap(void) { return m_hBmpPic; }
	// 返回BITMAP的内存指针
	unsigned char *	GetBuffer(void) { return m_pBmpBuff; }


protected:
	// 关闭并释放内存
	virtual int		Close(void);

protected:
	int				m_nWidth;		// 宽度
	int				m_nHeight;		// 高度
	HBITMAP			m_hBmpPic;		// BITMAP 句柄
	unsigned char *	m_pBmpBuff;		// BITMAP 内存指针

	HDC				m_hMemDC;		// 内存DC句柄
	HBITMAP			m_hOldBmp;		// 临时BITMAP句柄

};

#endif //__CJpegFunc_H__
