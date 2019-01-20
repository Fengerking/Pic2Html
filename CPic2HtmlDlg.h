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


	// ��ʼ���Ի���
	virtual BOOL OnInitDialog();
	// CURL������Ϣ����
	afx_msg LRESULT OnCurlMessage(WPARAM wParam, LPARAM lParam);
	// ѡ��JPEG�ļ�
	afx_msg void OnFileOpen();
	// ѡ��ͼ��Ŀ¼
	afx_msg void OnFolderOpen();
	// �˳�����
	afx_msg void OnFileExit();
	afx_msg void OnViewHTML();
	afx_msg void OnHelpAbout();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	// �ػ����ڡ�
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CExplorer1		m_webView;		// ��ʾHTML�ļ��Ĵ���
	CJpegFunc		m_jpegFunc;		// JPEG��װ��
	CCurlFunc 		m_curlFunc;		// CURL��װ��
	CDataJson		m_dataJson;		// JSON��װ��
	CDataWord		m_dataWord;		// ���ְ�װ��
	CDataHtml		m_dataHtml;		// HTML��װ��

	CString			m_strJpegFile;	// JPEG �ļ���
	CString			m_strHtmlFile;	// HTML �ļ���

	CObjectList<TCHAR>	m_lstJpegFile;
	NODEPOS				m_pCurPos;
};
