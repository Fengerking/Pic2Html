#pragma once
#include "explorer1.h"

#include "CJpegFunc.h"
#include "CCurlFunc.h"
#include "CDataJson.h"
#include "CDataWord.h"
#include "CDataHtml.h"

// CPic2HtmlDlg dialog
class CPic2HtmlDlg : public CDialog
{
// Construction
public:
	CPic2HtmlDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CPic2HtmlDlg();

// Dialog Data
	enum { IDD = IDD_PIC2HTML_DIALOG };

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	virtual void	InitAboutDialog(void);
	virtual void	TempTest(void);

	virtual void	ParseNextFile(void);
	virtual TCHAR * GetHtmlFileName(int nPos);


// Implementation
protected:
	HICON m_hIcon;


	// 初始化对话框
	virtual BOOL OnInitDialog();
	// CURL发回消息处理
	afx_msg LRESULT OnCurlMessage(WPARAM wParam, LPARAM lParam);
	// 选择JPEG文件
	afx_msg void OnFileOpen();
	// 选择图书目录
	afx_msg void OnFolderOpen();
	// 退出程序
	afx_msg void OnFileExit();
	afx_msg void OnViewHTML();
	afx_msg void OnHelpAbout();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	// 重画窗口。
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CExplorer1		m_webView;		// 显示HTML文件的窗口
	CJpegFunc		m_jpegFunc;		// JPEG包装类
	CCurlFunc 		m_curlFunc;		// CURL包装类
	CDataJson		m_dataJson;		// JSON包装类
	CDataWord		m_dataWord;		// 文字包装类
	CDataHtml		m_dataHtml;		// HTML包装类

	CString			m_strJpegFile;	// JPEG 文件名
	CString			m_strHtmlFile;	// HTML 文件名

	CObjectList<TCHAR>	m_lstJpegFile;
	NODEPOS				m_pCurPos;
};
