/*******************************************************************************
	File:		CJpegFunc.cpp

	Contains:	jpeg func class implement code

	Written by:	Ben King

	Change History (most recent first):
	2019-10-09		ben			Create file

*******************************************************************************/
#include "stdafx.h"
#include <assert.h>  

#include "CJpegFunc.h"

CJpegFunc::CJpegFunc(void)
	: m_nWidth(0)
	, m_nHeight(0)
	, m_hBmpPic(NULL)
	, m_pBmpBuff(NULL)
	, m_hMemDC(NULL)
	, m_hOldBmp(NULL)
{
}

CJpegFunc::~CJpegFunc(void)
{
	Close();
}

int	CJpegFunc::Dec(const TCHAR * pFile)
{
	Close();

	FILE *	hFile;
	struct	jpeg_error_mgr jerr;
	struct	jpeg_decompress_struct cinfo;
	int		i, nBytes, nLine, nRowSize, nStart;

	if ((hFile = _tfopen(pFile, _T("rb"))) == NULL)
		return -1;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, hFile);
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

	if (m_hBmpPic == NULL)
		m_hBmpPic = CreateBitmap(m_nWidth, m_nHeight, 1, 32, NULL);
	SetBitmapBits(m_hBmpPic, m_nWidth * m_nHeight * 4, m_pBmpBuff);

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(hFile);

	return 0;
}

int CJpegFunc::Draw(HWND hWnd, HDC hDC, RECT * pDraw)
{
	if (m_hBmpPic == NULL)
		return -1;
	if (hDC == NULL && hWnd == NULL)
		return -1;

	HDC hDrawDC = hDC;
	if (hDrawDC == NULL)
		hDrawDC = GetDC(hWnd);

	RECT rcDraw;
	if (pDraw != NULL)
	{
		memcpy(&rcDraw, pDraw, sizeof(rcDraw));
	}
	else
	{
		SetRect(&rcDraw, 0, 0, m_nWidth, m_nHeight);
		if (hWnd != NULL)
			GetClientRect(hWnd, &rcDraw);
	}
	
	if (m_hMemDC == NULL)
	{
		m_hMemDC = CreateCompatibleDC(hDrawDC);
		m_hOldBmp = (HBITMAP)SelectObject(m_hMemDC, m_hBmpPic);
	}

	int nDrawW = rcDraw.right - rcDraw.left;
	int nDrawH = rcDraw.bottom - rcDraw.top;
	if (nDrawW * m_nHeight > nDrawH * m_nWidth)
	{
		nDrawW = nDrawH * m_nWidth / m_nHeight;
		rcDraw.left = rcDraw.left + (rcDraw.right - rcDraw.left - nDrawW) / 2;
		rcDraw.right = rcDraw.right - (rcDraw.right - rcDraw.left - nDrawW) / 2;
	}
	else
	{
		nDrawH = nDrawW * m_nHeight / m_nWidth;
		int nHeight = (rcDraw.bottom - rcDraw.top - nDrawH) / 2;
		rcDraw.top = rcDraw.top;
		rcDraw.bottom = rcDraw.bottom - nHeight * 2;
	}

	SetStretchBltMode(hDC, HALFTONE);
	StretchBlt (hDC, rcDraw.left, rcDraw.top, rcDraw.right - rcDraw.left, rcDraw.bottom - rcDraw.top, 
				m_hMemDC, 0, 0, m_nWidth, m_nHeight, SRCCOPY);

	if (hDC == NULL)
		ReleaseDC(hWnd, hDrawDC);

	return 0;
}

int	CJpegFunc::CheckSubImg(RECT * pArea, RECT * pSubImg)
{
	int nImgTop = 0;
	int nImgLeft = pArea->left;
	int nImgBottom = pArea->bottom;
	int nImgRight = pArea->right;

	int nWidth = GetWidth();
	int nHeight = GetHeight();

	int i, j, nFind;
	unsigned char cBlack = 0X2F;
	unsigned char * pBmpBuff = GetBuffer();
	unsigned char * pBuff = NULL;
	for (i = pArea->top; i <= pArea->bottom; i++)
	{
		nFind = 0;
		for (j = pArea->left; j < pArea->right; j++)
		{
			pBuff = pBmpBuff + i * nWidth * 4 + j * 4;
			if (*(pBuff + 0) < cBlack || *(pBuff + 1) < cBlack || *(pBuff + 2) < cBlack)
				nFind++;
		}

		if (nFind > 10) // find 10 points
		{
			nImgTop = i;
			break;
		}
	}
	if (nImgTop == 0)
		return 0;

	// Check the bottom
	for (i = pArea->bottom; i >= pArea->top; i--)
	{
		nFind = 0;
		for (j = pArea->left; j < pArea->right; j++)
		{
			pBuff = pBmpBuff + i * nWidth * 4 + j * 4;
			if (*(pBuff + 0) < cBlack || *(pBuff + 1) < cBlack || *(pBuff + 2) < cBlack)
				nFind++;
		}

		if (nFind > 10) // find 10 points
		{
			nImgBottom = i;
			break;
		}
	}

	// Check the left
	for (j = pArea->left; j < pArea->right; j++)
	{
		nFind = 0;
		for (i = nImgTop; i <= nImgBottom; i++)
		{
			pBuff = pBmpBuff + i * nWidth * 4 + j * 4;
			if (*(pBuff + 0) < cBlack || *(pBuff + 1) < cBlack || *(pBuff + 2) < cBlack)
				nFind++;
		}
		if (nFind > 10) // find 10 points
		{
			nImgLeft = j;
			break;
		}
	}

	// Check the right
	for (j = pArea->right; j >= pArea->left; j--)
	{
		nFind = 0;
		for (i = nImgTop; i <= nImgBottom; i++)
		{
			pBuff = pBmpBuff + i * nWidth * 4 + j * 4;
			if (*(pBuff + 0) < cBlack || *(pBuff + 1) < cBlack || *(pBuff + 2) < cBlack)
				nFind++;
		}
		if (nFind > 10) // find 10 points
		{
			nImgRight = j;
			break;
		}
	}

	if (pSubImg != NULL)
	{
		pSubImg->left = nImgLeft;
		pSubImg->top = nImgTop;
		pSubImg->right = nImgRight;
		pSubImg->bottom = nImgBottom;
	}

	return nFind;
}

int CJpegFunc::Enc(RECT * pRect, int nQuality, const TCHAR * pFile)
{
	if (m_pBmpBuff == NULL)
		return -1;

	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr		jerr;
	FILE *						hFile;
	RECT						rcData;
	unsigned int				nRowStride;
	unsigned char  *			pDataBuff;
	unsigned int				i, j, nDest;
	JSAMPROW					jpgPointer[1]; 

	if ((hFile = _tfopen(pFile, _T("wb"))) == NULL) 
		return -1;
	
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, hFile);

	if (pRect != NULL)
	{
		memcpy(&rcData, pRect, sizeof(rcData));
		if (rcData.right > m_nWidth)
			rcData.right = m_nWidth;
		if (rcData.bottom > m_nHeight)
			rcData.bottom = m_nHeight;
	}
	else
	{
		SetRect(&rcData, 0, 0, m_nWidth, m_nHeight);
	}
	cinfo.image_width = rcData.right - rcData.left; 
	cinfo.image_height = rcData.bottom - rcData.top;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;

	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, nQuality, TRUE);

	jpeg_start_compress(&cinfo, TRUE);
	nRowStride = cinfo.image_width * 3; // JSAMPLEs per row in image_buffer

	pDataBuff = new unsigned char[cinfo.image_height * nRowStride];
	for (i = 0; i < cinfo.image_height; i++)
	{
		nDest = (rcData.top + i) * (m_nWidth * 4) + rcData.left * 4;
		for (j = 0; j < cinfo.image_width; j++)
		{
			pDataBuff[i * nRowStride + j * 3 + 2] = m_pBmpBuff[nDest + j * 4 + 0];
			pDataBuff[i * nRowStride + j * 3 + 1] = m_pBmpBuff[nDest + j * 4 + 1];
			pDataBuff[i * nRowStride + j * 3 + 0] = m_pBmpBuff[nDest + j * 4 + 2];
		}

	}

	while (cinfo.next_scanline < cinfo.image_height)
	{
		jpgPointer[0] = &pDataBuff[cinfo.next_scanline * nRowStride];
		(void)jpeg_write_scanlines(&cinfo, jpgPointer, 1);
	}

	jpeg_finish_compress(&cinfo); 
	jpeg_destroy_compress(&cinfo);
	fclose(hFile); 
	delete[]pDataBuff;

	return 0;
}

int	CJpegFunc::Close(void)
{
	if (m_hOldBmp != NULL && m_hMemDC != NULL)
	{
		SelectObject(m_hMemDC, m_hOldBmp);
		DeleteDC(m_hMemDC);
		m_hMemDC = NULL;
	}

	if (m_pBmpBuff != NULL)
	{
		delete[]m_pBmpBuff;
		m_pBmpBuff = NULL;
	}
	if (m_hBmpPic != NULL)
	{
		DeleteObject(m_hBmpPic);
		m_hBmpPic = NULL;
	}

	return 0;
}
