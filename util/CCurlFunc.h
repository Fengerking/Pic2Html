/*******************************************************************************
	File:		CCurlFunc.h

	Contains:	the curl func class header file.

	Written by:	Ben King

	Change History (most recent first):
	2018-10-10		Ben			Create file

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

	virtual int		ParseFile (const TCHAR * pFile, HWND hWnd);
	virtual char *	GetResult(void) { return m_pResultData; }

protected:
	virtual int		DoParseFile(void);
	virtual int		PrepareData(const TCHAR * pFile);
	virtual int		BASE64_Encode(const unsigned char* inputBuffer, int inputCount, char * outputBuffer);

protected:
	CURL *			m_pCURL;
	curl_slist		m_lstHeader;

    std::string		m_szBufferData;  
    std::string		m_szBufferHead;  
	double			m_dDownSize;
	double			m_dDownTime;
	double			m_dDownSpeed;

	char			m_szURL[1024];
	TCHAR			m_szFile[1024];

	char *			m_pFileData;
	int				m_nDataSize;
	char *			m_pResultHead;
	char *			m_pResultData;

public:
	static size_t	CURL_CallWriteHead (const char * pData, size_t nBlock, size_t nSize, std::string * strStream);
	static size_t	CURL_CallWriteData (const char * pData, size_t nBlock, size_t nSize, std::string * strStream);

protected:
	HANDLE			m_hThread;
	DWORD			m_dwThdID;
	HWND			m_hWnd;

	static DWORD	curlThreadProc(void * pParam);
};

#endif //__CCurlFunc_H__
