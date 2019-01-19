// CPic2HtmlDlg.cpp : implementation file

#include "stdafx.h"
#include "Pic2Html.h"
#include "CPic2HtmlDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CPic2HtmlDlg dialog
CPic2HtmlDlg::CPic2HtmlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPic2HtmlDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPic2HtmlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_webView);
}

BEGIN_MESSAGE_MAP(CPic2HtmlDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_CURL_FINISH, &CPic2HtmlDlg::OnCurlMessage)
	ON_COMMAND(IDM_FILE_OPEN, &CPic2HtmlDlg::OnFileOpen)
	ON_COMMAND(ID_FILE_EXIT, &CPic2HtmlDlg::OnFileExit)
	ON_COMMAND(ID_VIEW_OPENHTML, &CPic2HtmlDlg::OnViewHTML)
	ON_COMMAND(ID_HELP_ABOUT, &CPic2HtmlDlg::OnHelpAbout)
END_MESSAGE_MAP()

// CPic2HtmlDlg message handlers
BOOL CPic2HtmlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	InitAboutDialog();

//	TempTest();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPic2HtmlDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CPic2HtmlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		if (m_jpegFunc.GetBitmap() != NULL)
		{
			CPaintDC dc(this);
			RECT rcDraw;
			GetClientRect(&rcDraw);
			rcDraw.right = rcDraw.right / 2;
			m_jpegFunc.Draw(m_hWnd, dc.GetSafeHdc(), &rcDraw);
		}
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPic2HtmlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPic2HtmlDlg::InitAboutDialog(void)
{
	// Add "About..." menu item to system menu.
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
}

// 阿里云返回结果通知，继续处理
LRESULT CPic2HtmlDlg::OnCurlMessage(WPARAM wParam, LPARAM lParam)
{
	if (wParam != 0)
	{
		AfxMessageBox(_T("It was failed when it try to get text from server!"), MB_OK);
		return S_OK;
	}

	// 解析阿里云返回的JSON数据
	if (m_dataJson.ParseData(m_curlFunc.GetResult()) < 0)
	{
		AfxMessageBox(_T("Parse json data error!"), MB_OK);
		return -1;
	}

	// 提取里面的文字信息
	if (m_dataWord.ParseData(&m_dataJson) < 0)
	{
		AfxMessageBox(_T("Parse word data error!"), MB_OK);
		return -1;
	}
	
	// 生成HTML文件
	m_dataHtml.SetJpegFunc(&m_jpegFunc);
	if (m_dataHtml.OutTextHtml(&m_dataWord, m_strHtmlFile) < 0)
	{
		AfxMessageBox(_T("write html file error!"), MB_OK);
		return -1;
	}

	// 把HTML文件在界面上显示
	m_webView.Navigate(m_strHtmlFile, NULL, NULL, NULL, NULL);
	m_webView.InvalidateRect(NULL, TRUE);
	InvalidateRect(NULL, TRUE);
	
	return S_OK;
}

// 打开文件选择对话框，选择JPEG文件
void CPic2HtmlDlg::OnFileOpen()
{
	CString filter = L"Picture File (*.jpeg; *.jpg)|*.jpeg;*.jpg||";
	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_READONLY, filter, NULL);
	if (dlgFile.DoModal() != IDOK)
		return;

	m_strJpegFile = dlgFile.GetPathName();
	int nRC = m_jpegFunc.Dec(m_strJpegFile);
	if (nRC != 0)
		return;

	InvalidateRect(NULL, TRUE);
	int nPos = m_strJpegFile.ReverseFind('.');
	m_strHtmlFile = m_strJpegFile.Left(nPos + 1);
	m_strHtmlFile = m_strHtmlFile + _T("html");

	m_curlFunc.ParseFile(m_strJpegFile, m_hWnd);
//	m_webView.Navigate(m_strHtmlFile, NULL, NULL, NULL, NULL);
}

void CPic2HtmlDlg::OnFileExit()
{
	OnOK();
}

void CPic2HtmlDlg::OnViewHTML()
{
	ShellExecute(NULL, _T("open"), m_strHtmlFile, NULL, NULL, SW_NORMAL);
}

// 快速测试文件
void CPic2HtmlDlg::OnHelpAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CPic2HtmlDlg::TempTest(void)
{
	TCHAR szPath[1024];
	GetCurrentDirectory(sizeof(szPath), szPath);

	m_strJpegFile = szPath;
//	m_strJpegFile += _T("\\TestPic\\liuyong_01.jpeg");
	m_strJpegFile += _T("\\TestPic\\Test02.jpg");
	int nPos = m_strJpegFile.ReverseFind('.');
	m_strHtmlFile = m_strJpegFile.Left(nPos + 1);
	m_strHtmlFile = m_strHtmlFile + _T("html");

	m_jpegFunc.Dec(m_strJpegFile);

	m_curlFunc.ParseFile(m_strJpegFile, m_hWnd);
}