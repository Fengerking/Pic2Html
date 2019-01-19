/*******************************************************************************
	文件:	CCurlFunc.h

	内容:	用类的方法包装CURL库，方便使用。

	作者:	Ben King

	修改历史:
	2018-10-10		Ben			创建文件

*******************************************************************************/
#ifndef __CCurlFunc_H__
#define __CCurlFunc_H__
#include <string>  
#include <iostream>  

#include "curl.h"

#define	WM_CURL_FINISH		WM_USER+100

class CCurlFunc
{
public:
    CCurlFunc(void);
    virtual ~CCurlFunc(void);

	// 接收扫描好的JPEG文件，处理以后，通过CURL发给阿里云服务器。
	// 参数：pFile，	JPEG文件名，
	//      hWnd,   主窗口句柄，等阿里云返回结果后通知主窗口。
	virtual int		ParseFile (const TCHAR * pFile, HWND hWnd);

	// 返回阿里云返回的识别结果。
	virtual char *	GetResult(void) { return m_pResultData; }

protected:
	// 初始化CURL，设置参数，调用CURL的接口，发送数据给阿里云服务器。
	virtual int		DoParseFile(void);

	// 在发送数据前，读取文件到内存，把JPEG的内容进行大64码转换。
	// 参数：	pFile。JPEG的文件名。
	virtual int		PrepareData(const TCHAR * pFile);

	// 把数据转换成da64码。
	// 参数：	inputBuffer,	输入数据指针。
	//			inputCount,		输入数据长度。
	//			outputBuffer,	输出数据指针
	virtual int		BASE64_Encode(const unsigned char* inputBuffer, int inputCount, char * outputBuffer);

protected:
	CURL *			m_pCURL;			// CURL的指针。
	curl_slist		m_lstHeader;		// 调用CURL时，需要的头部信息列表。

    std::string		m_szBufferData;		// CURL返回的内容数据。
    std::string		m_szBufferHead;		// CURL返回的头部数据。
	double			m_dDownSize;		// CURL下载的数据大小。
	double			m_dDownTime;		// CURL下载使用的时间。
	double			m_dDownSpeed;		// CURL下载的速度。

	char			m_szURL[1024];		// 访问阿里云服务器的地址。
	TCHAR			m_szFile[1024];		// 保存的JPEG文件名。

	char *			m_pFileData;		// JPEG文件的内存指针。
	int				m_nDataSize;		// JPEG文件的内存大小。
	char *			m_pResultHead;		// 阿里云返回的头部数据指针。
	char *			m_pResultData;		// 阿里云返回的内容数据指针。

	char *			m_pJsonData;		// 已经保存好的JSON数据指针。

public:
	// CURL的回调函数。获取头部数据。
	static size_t	CURL_CallWriteHead (const char * pData, size_t nBlock, size_t nSize, std::string * strStream);
	// CURL的回调函数。获取内容数据。
	static size_t	CURL_CallWriteData (const char * pData, size_t nBlock, size_t nSize, std::string * strStream);

protected:
	HANDLE			m_hThread;			// 线程指针。
	DWORD			m_dwThdID;			// 线程ID。
	HWND			m_hWnd;				// 窗口句柄。收到阿里云数据时通知窗口。

	// 子线程入口函数。
	static DWORD	curlThreadProc(void * pParam);
};

#endif //__CCurlFunc_H__
