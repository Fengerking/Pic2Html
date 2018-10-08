/*******************************************************************************
	File:		CWndJpeg.cpp

	Contains:	Window slide pos implement code

	Written by:	Bangfei Jin

	Change History (most recent first):
	2018-10-08		Bangfei			Create file

*******************************************************************************/
#include "windows.h"
#include "tchar.h"

#include "CWndJpeg.h"

CWndJpeg::CWndJpeg(HINSTANCE hInst)
	: CWndBase(hInst)
{
	_tcscpy (m_szClassName, _T("jpegViewWindow"));
	_tcscpy (m_szWindowName, _T("jpegViewWindow"));
}

CWndJpeg::~CWndJpeg(void)
{
}

LRESULT CWndJpeg::OnResize (void)
{
	return S_OK;
}

bool CWndJpeg::CreateWnd (HWND hParent, RECT rcView, COLORREF clrBG)
{
	if (!CWndBase::CreateWnd(hParent, rcView, clrBG))
		return false;

	OnResize ();

	return true;
}

LRESULT CWndJpeg::OnReceiveMessage (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{

	default:
		break;
	}

	return	CWndBase::OnReceiveMessage(hwnd, uMsg, wParam, lParam);
}

