
// Pic2Html.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CPic2HtmlApp:
// See Pic2Html.cpp for the implementation of this class
//

class CPic2HtmlApp : public CWinApp
{
public:
	CPic2HtmlApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CPic2HtmlApp theApp;