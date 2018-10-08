/*******************************************************************************
	File:		CJpegDec.h

	Contains:	the message manager class header file.

	Written by:	Bangfei Jin

	Change History (most recent first):
	2016-12-01		Bangfei			Create file

*******************************************************************************/
#ifndef __CJpegDec_H__
#define __CJpegDec_H__

#include "CJpegBase.h"


class CJpegDec : public CJpegBase
{
public:
    CJpegDec(void);
    virtual ~CJpegDec(void);

	virtual int		OpenSource(const char * pURL);
	virtual int		Close(void);

	virtual int		Draw(HDC hdc);


protected:
	HDC				m_hMemDC;
	HBITMAP			m_hOldBmp;

};

#endif //__CJpegDec_H__
