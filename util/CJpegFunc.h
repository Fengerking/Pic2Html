/*******************************************************************************
	�ļ�:	CJpegFunc.h

	����:	JPEG�ļ��Ľ��룬���룬��ͼ

	����:	Ben King

	�޸���ʷ:
	2018-10-08		Ben			�����ļ�

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

	// ����JPEG�ļ���
	virtual int		Dec(const TCHAR * pFile);
	// ����JPEG�ļ���
	virtual int		Enc(RECT * pRect, int nQuality, const TCHAR * pFile);
	// ��JPEG�ļ���ָ��������Ĵ����ϡ�
	virtual int		Draw(HWND hWnd, HDC hDC, RECT * pDraw);

	// ���ؿ��
	virtual int		GetWidth(void) { return m_nWidth; }
	// ���ظ߶�
	virtual int		GetHeight(void) { return m_nHeight; }
	// ����BITMAP���
	virtual HBITMAP	GetBitmap(void) { return m_hBmpPic; }
	// ����BITMAP���ڴ�ָ��
	unsigned char *	GetBuffer(void) { return m_pBmpBuff; }


protected:
	// �رղ��ͷ��ڴ�
	virtual int		Close(void);

protected:
	int				m_nWidth;		// ���
	int				m_nHeight;		// �߶�
	HBITMAP			m_hBmpPic;		// BITMAP ���
	unsigned char *	m_pBmpBuff;		// BITMAP �ڴ�ָ��

	HDC				m_hMemDC;		// �ڴ�DC���
	HBITMAP			m_hOldBmp;		// ��ʱBITMAP���

};

#endif //__CJpegFunc_H__
