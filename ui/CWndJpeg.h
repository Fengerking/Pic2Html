/*******************************************************************************
	File:		CWndJpeg.h

	Contains:	the window view header file

	Written by:	Bangfei Jin

	Change History (most recent first):
	2018-10-08		Bangfei			Create file

*******************************************************************************/
#ifndef __CWndJpeg_H__
#define __CWndJpeg_H__

#include "CWndBase.h"

class CWndJpeg : public CWndBase
{
public:
	CWndJpeg(HINSTANCE hInst);
	virtual ~CWndJpeg(void);

	virtual bool	CreateWnd (HWND hParent, RECT rcView, COLORREF clrBG);
	virtual LRESULT	OnReceiveMessage (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	virtual LRESULT	OnResize (void);

protected:
	char				m_szFile[1024];

};
#endif //__CWndJpeg_H__