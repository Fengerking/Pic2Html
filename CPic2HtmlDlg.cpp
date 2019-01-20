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

CPic2HtmlDlg::~CPic2HtmlDlg()
{
	TCHAR * pFileName = m_lstJpegFile.RemoveHead();
	while (pFileName != NULL)
	{
		delete[]pFileName;
		pFileName = m_lstJpegFile.RemoveHead();
	}
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
	ON_COMMAND(IDM_FILE_FOLDER, &CPic2HtmlDlg::OnFolderOpen)
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
//	OnFolderOpen();

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

// �����Ʒ��ؽ��֪ͨ����������
LRESULT CPic2HtmlDlg::OnCurlMessage(WPARAM wParam, LPARAM lParam)
{
	if (wParam != 0)
	{
		AfxMessageBox(_T("It was failed when it try to get text from server!"), MB_OK);
		return S_OK;
	}

	// ���������Ʒ��ص�JSON����
	if (m_dataJson.ParseData(m_curlFunc.GetResult()) < 0)
	{
		AfxMessageBox(_T("Parse json data error!"), MB_OK);
		return -1;
	}

	// ��ȡ�����������Ϣ
	m_dataWord.SetJpegFunc(&m_jpegFunc);
	if (m_dataWord.ParseData(&m_dataJson) < 0)
	{
		AfxMessageBox(_T("Parse word data error!"), MB_OK);
		return -1;
	}
	
	// ����HTML�ļ�
	m_dataHtml.SetJpegFunc(&m_jpegFunc);
	if (m_dataHtml.OutTextHtml(&m_dataWord, m_strHtmlFile) < 0)
	{
		AfxMessageBox(_T("write html file error!"), MB_OK);
		return -1;
	}

	// ��HTML�ļ��ڽ�������ʾ
	m_webView.Navigate(m_strHtmlFile, NULL, NULL, NULL, NULL);
	m_webView.InvalidateRect(NULL, TRUE);
	InvalidateRect(NULL, TRUE);
	
	if (m_lstJpegFile.GetCount() > 0)
		ParseNextFile();

	return S_OK;
}

// ���ļ�ѡ��Ի���ѡ��JPEG�ļ�
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

void CPic2HtmlDlg::OnFolderOpen()
{
	TCHAR           szFolderPath[MAX_PATH] = { 0 };
/*	BROWSEINFO      sInfo;
	ZeroMemory(&sInfo, sizeof(BROWSEINFO));
	sInfo.lpszTitle = _T("��ѡ��ͼ��ɨ���ļ��洢·��");
	sInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_EDITBOX | BIF_DONTGOBELOWDOMAIN;
	// ��ʾ�ļ���ѡ��Ի���  
	LPITEMIDLIST lpidlBrowse = SHBrowseForFolder(&sInfo);
	if (lpidlBrowse == NULL)
		return;
	SHGetPathFromIDList(lpidlBrowse, szFolderPath);
	CoTaskMemFree(lpidlBrowse);
*/
	_tcscpy(szFolderPath, _T("C:\\work\\Pic2Html\\book\\shediao"));
	_tcscat(szFolderPath, _T("\\"));

	TCHAR * pFileName = m_lstJpegFile.RemoveHead();
	while (pFileName != NULL)
	{
		delete[]pFileName;
		pFileName = m_lstJpegFile.RemoveHead();
	}
	
	TCHAR	szFilter[1024];
	_tcscpy(szFilter, szFolderPath);
	_tcscat(szFilter, _T("*.*"));
	WIN32_FIND_DATA  data;
	HANDLE  hFind = FindFirstFile(szFilter, &data);
	if (hFind == INVALID_HANDLE_VALUE)
		return;
	TCHAR * pExt = NULL;
	hFind = FindFirstFile(szFilter, &data);
	do
	{
		if (_tcslen(data.cFileName) < 3)
			continue;
		if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
			continue;
		pExt = _tcsrchr(data.cFileName, _T('.'));
		if (pExt == NULL)
			continue;
		_tcsupr(pExt);
		if (_tcscmp(pExt, _T(".JPG")) == 0 || _tcscmp(pExt, _T(".JPEG")) == 0)
		{
			int		nLen = _tcslen(szFolderPath) + _tcslen(data.cFileName) + 2;
			TCHAR * pJpegFile = new TCHAR[nLen];
			memset(pJpegFile, 0, nLen);
			_tcscpy(pJpegFile, szFolderPath);
			_tcscat(pJpegFile, data.cFileName);
			m_lstJpegFile.AddTail(pJpegFile);
		}
	} while (FindNextFile(hFind, &data));
	FindClose(hFind);

	TCHAR szSubFolder[1024];
	_tcscpy(szSubFolder, szFolderPath);
	_tcscat(szSubFolder, _T("html"));
	CreateDirectory(szSubFolder, NULL);
	_tcscpy(szSubFolder, szFolderPath);
	_tcscat(szSubFolder, _T("image"));
	CreateDirectory(szSubFolder, NULL);

	m_pCurPos = m_lstJpegFile.GetHeadPosition();
	ParseNextFile();
	return;
}

void CPic2HtmlDlg::OnFileExit()
{
	OnOK();
}

void CPic2HtmlDlg::OnViewHTML()
{
	ShellExecute(NULL, _T("open"), m_strHtmlFile, NULL, NULL, SW_NORMAL);
}

// ���ٲ����ļ�
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
	m_strJpegFile += _T("\\book\\shediao\\IMG_20190120_140252.jpg");
	int nPos = m_strJpegFile.ReverseFind('.');
	m_strHtmlFile = m_strJpegFile.Left(nPos + 1);
	m_strHtmlFile = m_strHtmlFile + _T("html");

	m_jpegFunc.Dec(m_strJpegFile);

	m_curlFunc.ParseFile(m_strJpegFile, m_hWnd);
}

void CPic2HtmlDlg::ParseNextFile(void)
{
	if (m_pCurPos == NULL)
	{
		if (m_lstJpegFile.GetCount() > 0)
			AfxMessageBox(_T("���е�ͼƬת����ɣ�"));
		else
			AfxMessageBox(_T("û���ҵ�ͼƬ��"));
		return;
	}

	m_strJpegFile = m_lstJpegFile.GetNext(m_pCurPos);
	m_strHtmlFile = GetHtmlFileName (0);

	m_dataHtml.SetPrevNextFile(GetHtmlFileName(-1), GetHtmlFileName(1));

	m_jpegFunc.Dec(m_strJpegFile);
	m_curlFunc.ParseFile(m_strJpegFile, m_hWnd);
}

TCHAR * CPic2HtmlDlg::GetHtmlFileName(int nPos)
{
	if (m_lstJpegFile.GetCount() < 0)
		return NULL;

	CString strJpefFile = m_strJpegFile;
	
	if (nPos == -1)
	{
		if (m_pCurPos == m_lstJpegFile.GetHeadPosition())
			return NULL;
		strJpefFile = m_lstJpegFile.GetPrev(m_pCurPos);
		m_lstJpegFile.GetNext(m_pCurPos);
	}
	else if (nPos == 1)
	{
		if (m_pCurPos == NULL || m_pCurPos == m_lstJpegFile.GetTailPosition())
			return NULL;
		strJpefFile = m_lstJpegFile.GetNext(m_pCurPos);
		m_lstJpegFile.GetPrev(m_pCurPos);
	}

	int nPos1 = strJpefFile.ReverseFind(_T('\\'));
	m_strHtmlFile = strJpefFile.Left(nPos1);
	m_strHtmlFile = m_strHtmlFile + _T("\\html");
	int nPos2 = m_strJpegFile.ReverseFind(_T('.'));
	m_strHtmlFile = m_strHtmlFile + strJpefFile.Mid(nPos1, nPos2 - nPos1);
	m_strHtmlFile = m_strHtmlFile + _T(".html");

	return m_strHtmlFile.GetBuffer();
}