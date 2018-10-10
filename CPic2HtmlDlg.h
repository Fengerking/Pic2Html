#pragma once
#include "explorer1.h"

#include "CJpegFunc.h"
#include "CCurlFunc.h"
#include "CDataJson.h"

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

	virtual void InitAboutDialog(void);


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnCurlMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnFileOpen();
	afx_msg void OnFileExit();
	afx_msg void OnViewJPEG();
	afx_msg void OnViewHTML();
	afx_msg void OnViewBOTH();
	afx_msg void OnHelpAbout();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CExplorer1		m_webView;
	CJpegFunc		m_jpegFunc;
	CCurlFunc 		m_curlFunc;
	CDataJson		m_dataJson;
};
