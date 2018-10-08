/*******************************************************************************
	File:		CJpegBase.h

	Contains:	the message manager class header file.

	Written by:	Bangfei Jin

	Change History (most recent first):
	2018-10-08		Bangfei			Create file

*******************************************************************************/
#ifndef __CJpegBase_H__
#define __CJpegBase_H__
#include <string>  
#include <iostream>  

#include "CBaseObject.h"
#include "jpeglib.h"

class CJpegBase : public CBaseObject
{
public:
    CJpegBase(void);
    virtual ~CJpegBase(void);

	virtual int		OpenSource(const char * pURL);
	virtual int		Close(void);

	virtual int		Enc(unsigned char * pBmpBuff, RECT * rcData, const char * pFile);

	virtual int		GetWidth(void) { return m_nWidth; }
	virtual int		GetHeight(void) { return m_nHeight; }

	virtual HBITMAP	GetBitmap(void) { return m_hBmpImage; }

protected:
	int				m_nColorType;
	int				m_nWidth;
	int				m_nHeight;

	unsigned char *	m_pBuffData;
	int				m_nBuffSize;
	int				m_nBuffRead;

	HBITMAP			m_hBmpImage;
	unsigned char *	m_pBmpBuff;

public:

};

#endif //__CJpegBase_H__
