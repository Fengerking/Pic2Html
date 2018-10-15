/*******************************************************************************
	File:		CCurlFunc.cpp

	Contains:	curl func class implement code

	Written by:	Ben King

	Change History (most recent first):
	2018-10-10		Ben			Create file

*******************************************************************************/
#include "stdafx.h"
#include <assert.h>  

#include "CCurlFunc.h"

#define		SKIP_PEER_VERIFICATION		1  
//#define	SKIP_HOSTNAME_VERFICATION	1  
//AppKey��		23676169 
//AppSecret:	6db50903b770843d291c9f54308035be
//AppCode:		4779d46346404086ae4d077b9bfe9177

CCurlFunc::CCurlFunc(void)
	: m_pCURL(NULL)
	, m_pFileData(NULL)
	, m_nDataSize(0)
	, m_pResultHead(NULL)
	, m_pResultData(NULL)
	, m_pJsonData(NULL)
	, m_hThread(NULL)
	, m_dwThdID(0)
	, m_hWnd(NULL)
{
	curl_global_init(CURL_GLOBAL_ALL);  
	m_lstHeader.data = new char[256];
	strcpy (m_lstHeader.data, "Authorization:APPCODE 4779d46346404086ae4d077b9bfe9177");
	m_lstHeader.next = new curl_slist();
	m_lstHeader.next->data = new char[256];
	strcpy(m_lstHeader.next->data, "Content-Type:application/json; charset=UTF-8");
	m_lstHeader.next->next = NULL;

	strcpy(m_szURL, "https://ocrapi-document.taobao.com/ocrservice/document");
	memset(m_szFile, 0, sizeof(m_szFile));
}

CCurlFunc::~CCurlFunc(void)
{
	// wait for thread finish
	m_hWnd = NULL;
	while (m_hThread != NULL)
	{
		Sleep(100);
	}

	if (m_pCURL != NULL)
		curl_easy_cleanup(m_pCURL);  
    curl_global_cleanup();  

	if (m_lstHeader.next != NULL)
	{
		delete[]m_lstHeader.next->data;
		delete m_lstHeader.next;
	}
	delete[]m_lstHeader.data;

	if (m_pFileData != NULL)
		delete[]m_pFileData;

	if (m_pJsonData != NULL)
		delete[]m_pJsonData;
}

int CCurlFunc::ParseFile(const TCHAR * pFile, HWND hWnd)
{
	m_hWnd = hWnd;
	_tcscpy(m_szFile, pFile);
	m_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)curlThreadProc, this, 0, &m_dwThdID);
	if (m_hThread == NULL)
		return -1;
	return 0;
}
int	CCurlFunc::DoParseFile(void)
{
	CString strJpegFile = m_szFile;
	int		nDotPos = strJpegFile.ReverseFind('.');
	CString	strJsonFile = strJpegFile.Left(nDotPos);
	strJsonFile += ".json";
	CFile file;
	if (file.Open(strJsonFile, CFile::modeRead, NULL) == TRUE)
	{
		if (m_pJsonData != NULL)
			delete[]m_pJsonData;
		int		nFileSize = (int)file.GetLength();
		m_pJsonData = new char[nFileSize + 1];
		m_pJsonData[nFileSize] = 0;
		file.Read(m_pJsonData, nFileSize);
		file.Close();
		m_pResultData = m_pJsonData;
		return 0;
	}

	if (PrepareData(m_szFile) < 0)
		return -1;

	if (m_pCURL == NULL)
		m_pCURL = curl_easy_init();
	if (m_pCURL == NULL)
		return -1;

	m_szBufferHead.clear();
	m_szBufferData.clear();

	curl_easy_setopt(m_pCURL, CURLOPT_URL, m_szURL);
#ifdef SKIP_PEER_VERIFICATION  
	curl_easy_setopt(m_pCURL, CURLOPT_SSL_VERIFYPEER, 0L);
#endif  
	curl_easy_setopt(m_pCURL, CURLOPT_POST, 1);
	curl_easy_setopt(m_pCURL, CURLOPT_HTTPHEADER, &m_lstHeader);
	curl_easy_setopt(m_pCURL, CURLOPT_POSTFIELDS, m_pFileData);

	curl_easy_setopt(m_pCURL, CURLOPT_HEADERFUNCTION, CURL_CallWriteHead);
	curl_easy_setopt(m_pCURL, CURLOPT_HEADERDATA, &m_szBufferHead);
	curl_easy_setopt(m_pCURL, CURLOPT_WRITEFUNCTION, CURL_CallWriteData);
	curl_easy_setopt(m_pCURL, CURLOPT_WRITEDATA, &m_szBufferData);

	CURLcode nRC = curl_easy_perform(m_pCURL);
	if (nRC == CURLE_OK)
	{
		m_dDownSize = 0;
		nRC = curl_easy_getinfo(m_pCURL, CURLINFO_SIZE_DOWNLOAD, &m_dDownSize);

		m_dDownTime = 0;
		nRC = curl_easy_getinfo(m_pCURL, CURLINFO_TOTAL_TIME, &m_dDownTime);

		m_dDownSpeed = 0;
		nRC = curl_easy_getinfo(m_pCURL, CURLINFO_SPEED_DOWNLOAD, &m_dDownSpeed);

		m_pResultHead = (char *)m_szBufferHead.c_str();
		m_pResultData = (char *)m_szBufferData.c_str();

//		CFile outFile;
//		outFile.Open(_T("C:\\Work\\Temp\\result.txt"), CFile::modeCreate | CFile::modeWrite, NULL);
//		outFile.Write(m_pResultData, m_szBufferData.length());
//		outFile.Close();
	}
	else
	{
		//CURLE_OK						�������һ�ж��� 
		//CURLE_UNSUPPORTED_PROTOCOL	��֧�ֵ�Э�飬��URL��ͷ��ָ�� 
		//CURLE_COULDNT_CONNECT			�������ӵ�remote �������ߴ��� 
		//CURLE_REMOTE_ACCESS_DENIED	���ʱ��ܾ� 
		//CURLE_HTTP_RETURNED_ERROR		Http���ش��� 
		//CURLE_READ_ERROR				�������ļ�����  
		//CURLE_SSL_CACERT				����HTTPSʱ��ҪCA֤��·�� 
		return -1;
	}
	return 0;
}

size_t CCurlFunc::CURL_CallWriteHead (const char * pData, size_t nBlock, size_t nSize, std::string * strStream)  
{  
	assert(strStream != NULL);  
	size_t nLen  = nBlock * nSize;  
	strStream->append(pData, nLen);  
	return nLen;  
} 

size_t CCurlFunc::CURL_CallWriteData (const char * pData, size_t nBlock, size_t nSize, std::string * strStream)  
{  
	assert(strStream != NULL);  
	size_t nLen  = nBlock * nSize;  
	strStream->append(pData, nLen);  
	return nLen;  
} 

int	CCurlFunc::PrepareData(const TCHAR * pFile)
{
	CFile file;
	if (file.Open(pFile, CFile::modeRead, NULL) == FALSE)
		return -1;
	int		nFileSize = (int)file.GetLength();
	char *	pFileBuff = new char[nFileSize];
	file.Read(pFileBuff, nFileSize);
	file.Close();

	if (m_pFileData != NULL)
		delete[]m_pFileData;
	m_pFileData = new char[nFileSize * 2];
	memset(m_pFileData, 0, nFileSize * 2);

	strcpy(m_pFileData, "{\"img\": \"");
	int nPos = strlen(m_pFileData);
	m_nDataSize = BASE64_Encode((unsigned char *)pFileBuff, nFileSize, m_pFileData + nPos);
	delete[]pFileBuff;

	strcat(m_pFileData, "\",");
	strcat(m_pFileData, "\"prob\": true}");

	return 0;
}

/*
curl - i - k - X POST 'https://ocrapi-document.taobao.com/ocrservice/document'
- H 'Authorization:APPCODE ���Լ���AppCode'
--data '{
//ͼ�����ݣ�base64���룬Ҫ��base64������С������4M����̱�����15px��������4096px��
//֧��jpg/png/bmp��ʽ����url����ֻ��ͬʱ����һ��
"img": "",
//ͼ��url��ַ��ͼƬ����URL��URL���Ȳ�����1024�ֽڣ�URL��Ӧ��ͼƬbase64������С������4M��
//��̱�����15px��������4096px��֧��jpg/png/bmp��ʽ����img����ֻ��ͬʱ����һ��
"url" : "",
//�Ƿ���Ҫʶ������ÿһ�е����Ŷȣ�Ĭ�ϲ���Ҫ�� true����Ҫ false������Ҫ
"prob" : false
}'
-H 'Content - Type:application / json; charset = UTF - 8'
*/

/*
1.ͼƬ�������������ֿ�Ϊ��λ���
2.���ֿ���̬1:
�ո���ɵ����ֿ飬�磺����  �������м�������ɿո񣬿��ܻ��γ��������ֿ飺1.�ԣ�2.��
3.���ֿ���̬2:
�����γɵ����ֿ飬����������ÿ��Ϊһ�����ֿ�
{
	//Ψһid���������ⶨλ
	"sid": "bfcb418f71fd057f11ea4e17b15688dc27f9d4c6a47a867a376f251706266353341da54c",
		//�㷨�汾
		"prism_version" : "1.0.6",
		//ʶ������ֿ��������prism_wordsInfo�����С
		"prism_wnum" : 2,
		//ʶ������ֵľ�������
		"prism_wordsInfo" : [
	{
		//���ֿ�
		"word": "2017",
			//���Ŷ�
			"prob" : 99,
			//���ֿ��λ�ã��������ֿ��ĸ��ǵ�����˳ʱ�����У��ֱ�Ϊ����XY���ꡢ����XY���ꡢ����XY���ꡢ����XY����
			"pos" : [
		{
			"x": 107,
				"y" : 203
		},
		{
			"x": 247,
			"y" : 203
		},
		{
			"x": 247,
			"y" : 213
		},
		{
			"x": 107,
			"y" : 213
		}
			]
	},
	
*/
static const char* DATA_BIN2ASCII = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
int CCurlFunc::BASE64_Encode(const unsigned char* inputBuffer, int inputCount, char * outputBuffer)
{
	int i;
	unsigned char b0, b1, b2;

	if ((inputBuffer == NULL) || (inputCount <= 0))
	{
		return -1;  // ��������  
	}

	if (outputBuffer != NULL)
	{
		for (i = inputCount; i > 0; i -= 3)
		{
			if (i >= 3)
			{   // ��3�ֽ�����ת����4��ASCII�ַ�  
				b0 = *inputBuffer++;
				b1 = *inputBuffer++;
				b2 = *inputBuffer++;

				*outputBuffer++ = DATA_BIN2ASCII[b0 >> 2];
				*outputBuffer++ = DATA_BIN2ASCII[((b0 << 4) | (b1 >> 4)) & 0x3F];
				*outputBuffer++ = DATA_BIN2ASCII[((b1 << 2) | (b2 >> 6)) & 0x3F];
				*outputBuffer++ = DATA_BIN2ASCII[b2 & 0x3F];
			}
			else
			{
				b0 = *inputBuffer++;
				if (i == 2)b1 = *inputBuffer++; else b1 = 0;

				*outputBuffer++ = DATA_BIN2ASCII[b0 >> 2];
				*outputBuffer++ = DATA_BIN2ASCII[((b0 << 4) | (b1 >> 4)) & 0x3F];
				*outputBuffer++ = (i == 1) ? char('=') : DATA_BIN2ASCII[(b1 << 2) & 0x3F];
				*outputBuffer++ = char('=');
			}
		} // End for i  

		*outputBuffer++ = char('/0');   // ����ַ����������  
	}

	return ((inputCount + 2) / 3) * 4;  // ������Ч�ַ�����  
}

DWORD CCurlFunc::curlThreadProc(void * pParam)
{
	CCurlFunc * pFunc = (CCurlFunc *)pParam;
	int nRC = pFunc->DoParseFile();
	if (pFunc->m_hWnd != NULL)
		PostMessage(pFunc->m_hWnd, WM_CURL_FINISH, nRC, 0);
	pFunc->m_hThread = NULL;
	return 0;
}