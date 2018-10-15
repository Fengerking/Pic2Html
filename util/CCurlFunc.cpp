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
//AppKey：		23676169 
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
		//CURLE_OK						任务完成一切都好 
		//CURLE_UNSUPPORTED_PROTOCOL	不支持的协议，由URL的头部指定 
		//CURLE_COULDNT_CONNECT			不能连接到remote 主机或者代理 
		//CURLE_REMOTE_ACCESS_DENIED	访问被拒绝 
		//CURLE_HTTP_RETURNED_ERROR		Http返回错误 
		//CURLE_READ_ERROR				读本地文件错误  
		//CURLE_SSL_CACERT				访问HTTPS时需要CA证书路径 
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
- H 'Authorization:APPCODE 你自己的AppCode'
--data '{
//图像数据：base64编码，要求base64编码后大小不超过4M，最短边至少15px，最长边最大4096px，
//支持jpg/png/bmp格式，和url参数只能同时存在一个
"img": "",
//图像url地址：图片完整URL，URL长度不超过1024字节，URL对应的图片base64编码后大小不超过4M，
//最短边至少15px，最长边最大4096px，支持jpg/png/bmp格式，和img参数只能同时存在一个
"url" : "",
//是否需要识别结果中每一行的置信度，默认不需要。 true：需要 false：不需要
"prob" : false
}'
-H 'Content - Type:application / json; charset = UTF - 8'
*/

/*
1.图片文字内容以文字块为单位输出
2.文字块形态1:
空格造成的文字块，如：“淘  宝”，中间存在若干空格，可能会形成两个文字块：1.淘，2.宝
3.文字块形态2:
换行形成的文字块，即连续文字每行为一个文字块
{
	//唯一id，用于问题定位
	"sid": "bfcb418f71fd057f11ea4e17b15688dc27f9d4c6a47a867a376f251706266353341da54c",
		//算法版本
		"prism_version" : "1.0.6",
		//识别的文字块的数量，prism_wordsInfo数组大小
		"prism_wnum" : 2,
		//识别的文字的具体内容
		"prism_wordsInfo" : [
	{
		//文字块
		"word": "2017",
			//置信度
			"prob" : 99,
			//文字块的位置，按照文字块四个角的坐标顺时针排列，分别为左上XY坐标、右上XY坐标、右下XY坐标、左下XY坐标
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
		return -1;  // 参数错误  
	}

	if (outputBuffer != NULL)
	{
		for (i = inputCount; i > 0; i -= 3)
		{
			if (i >= 3)
			{   // 将3字节数据转换成4个ASCII字符  
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

		*outputBuffer++ = char('/0');   // 添加字符串结束标记  
	}

	return ((inputCount + 2) / 3) * 4;  // 返回有效字符个数  
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