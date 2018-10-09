
// CPic2HtmlDlg.h : header file
//

#pragma once
#include "explorer1.h"


// CPic2HtmlDlg dialog
class CPic2HtmlDlg : public CDialog
{
// Construction
public:
	CPic2HtmlDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PIC2HTML_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CExplorer1	m_webView;
};
