/*******************************************************************************
	�ļ�:	CCurlFunc.h

	����:	����ķ�����װCURL�⣬����ʹ�á�

	����:	Ben King

	�޸���ʷ:
	2018-10-10		Ben			�����ļ�

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

	// ����ɨ��õ�JPEG�ļ��������Ժ�ͨ��CURL���������Ʒ�������
	// ������pFile��	JPEG�ļ�����
	//      hWnd,   �����ھ�����Ȱ����Ʒ��ؽ����֪ͨ�����ڡ�
	virtual int		ParseFile (const TCHAR * pFile, HWND hWnd);

	// ���ذ����Ʒ��ص�ʶ������
	virtual char *	GetResult(void) { return m_pResultData; }

protected:
	// ��ʼ��CURL�����ò���������CURL�Ľӿڣ��������ݸ������Ʒ�������
	virtual int		DoParseFile(void);

	// �ڷ�������ǰ����ȡ�ļ����ڴ棬��JPEG�����ݽ��д�64��ת����
	// ������	pFile��JPEG���ļ�����
	virtual int		PrepareData(const TCHAR * pFile);

	// ������ת����da64�롣
	// ������	inputBuffer,	��������ָ�롣
	//			inputCount,		�������ݳ��ȡ�
	//			outputBuffer,	�������ָ��
	virtual int		BASE64_Encode(const unsigned char* inputBuffer, int inputCount, char * outputBuffer);

protected:
	CURL *			m_pCURL;			// CURL��ָ�롣
	curl_slist		m_lstHeader;		// ����CURLʱ����Ҫ��ͷ����Ϣ�б�

    std::string		m_szBufferData;		// CURL���ص��������ݡ�
    std::string		m_szBufferHead;		// CURL���ص�ͷ�����ݡ�
	double			m_dDownSize;		// CURL���ص����ݴ�С��
	double			m_dDownTime;		// CURL����ʹ�õ�ʱ�䡣
	double			m_dDownSpeed;		// CURL���ص��ٶȡ�

	char			m_szURL[1024];		// ���ʰ����Ʒ������ĵ�ַ��
	TCHAR			m_szFile[1024];		// �����JPEG�ļ�����

	char *			m_pFileData;		// JPEG�ļ����ڴ�ָ�롣
	int				m_nDataSize;		// JPEG�ļ����ڴ��С��
	char *			m_pResultHead;		// �����Ʒ��ص�ͷ������ָ�롣
	char *			m_pResultData;		// �����Ʒ��ص���������ָ�롣

	char *			m_pJsonData;		// �Ѿ�����õ�JSON����ָ�롣

public:
	// CURL�Ļص���������ȡͷ�����ݡ�
	static size_t	CURL_CallWriteHead (const char * pData, size_t nBlock, size_t nSize, std::string * strStream);
	// CURL�Ļص���������ȡ�������ݡ�
	static size_t	CURL_CallWriteData (const char * pData, size_t nBlock, size_t nSize, std::string * strStream);

protected:
	HANDLE			m_hThread;			// �߳�ָ�롣
	DWORD			m_dwThdID;			// �߳�ID��
	HWND			m_hWnd;				// ���ھ�����յ�����������ʱ֪ͨ���ڡ�

	// ���߳���ں�����
	static DWORD	curlThreadProc(void * pParam);
};

#endif //__CCurlFunc_H__
