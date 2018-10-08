/*******************************************************************************
	File:		CJpegDec.cpp

	Contains:	message manager class implement code

	Written by:	Bangfei Jin

	Change History (most recent first):
	2016-12-01		Bangfei			Create file

*******************************************************************************/
#include <assert.h>  

#include "qcErr.h"
#include "CJpegDec.h"

#include "USystemFunc.h"
#include "ULogFunc.h"


CJpegDec::CJpegDec(void)
	: CJpegBase()
	, m_hMemDC(NULL)
	, m_hOldBmp(NULL)
{
	SetObjectName ("CJpegDec");
}

CJpegDec::~CJpegDec(void)
{
	Close();
}

int	CJpegDec::OpenSource(const char * pURL)
{
	FILE *	pFile;
	struct	jpeg_error_mgr jerr;
	struct	jpeg_decompress_struct cinfo;
	int		i, nBytes, nLine, nRowSize, nStart;

	if ((pFile = fopen(pURL, "rb")) == NULL)
		return -1;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, pFile);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);
	m_nWidth = cinfo.output_width;
	m_nHeight = cinfo.output_height;
	nBytes = cinfo.output_components;
	nRowSize = m_nWidth * nBytes;

	m_pBmpBuff = new unsigned char[m_nWidth * m_nHeight * 4];
	memset(m_pBmpBuff, 0, m_nWidth * m_nHeight * 4);
	JSAMPARRAY pBuffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, nRowSize, 1);
	while (cinfo.output_scanline < cinfo.output_height)
	{
		nLine = cinfo.output_scanline;
		jpeg_read_scanlines(&cinfo, pBuffer, 1);
		for (i = 0; i < m_nWidth; i++)
		{
			nStart = m_nWidth * 4 * nLine + i * 4;
			m_pBmpBuff[nStart + 2] = (unsigned char)(*pBuffer)[i * nBytes];
			m_pBmpBuff[nStart + 1] = (unsigned char)(*pBuffer)[i * nBytes + 1];
			m_pBmpBuff[nStart + 0] = (unsigned char)(*pBuffer)[i * nBytes + 2];
		}
	}

	if (m_hBmpImage == NULL)
		m_hBmpImage = CreateBitmap(m_nWidth, m_nHeight, 1, 32, NULL);
	SetBitmapBits(m_hBmpImage, m_nWidth * m_nHeight * 4, m_pBmpBuff);

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(pFile);

	return QC_ERR_NONE;
}

int	CJpegDec::Close(void)
{
	if (m_hOldBmp != NULL && m_hMemDC != NULL)
	{
		SelectObject(m_hMemDC, m_hOldBmp);
		DeleteDC(m_hMemDC);
	}	
	
	CJpegBase::Close();
	return QC_ERR_NONE;
}

int	CJpegDec::Draw(HDC hdc)
{
	if (m_hBmpImage == NULL)
		return QC_ERR_STATUS;

	if (m_hMemDC == NULL)
	{
		m_hMemDC = CreateCompatibleDC(hdc);
		m_hOldBmp = (HBITMAP)SelectObject(m_hMemDC, m_hBmpImage);
	}

	BitBlt(hdc, 0, 0, m_nWidth, m_nHeight, m_hMemDC, 0, 0, SRCCOPY);

	return QC_ERR_NONE;
}