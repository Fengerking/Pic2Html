/*******************************************************************************
	File:		CJpegBase.cpp

	Contains:	message manager class implement code

	Written by:	Bangfei Jin

	Change History (most recent first):
	2016-12-01		Bangfei			Create file

*******************************************************************************/
#include <assert.h>  

#include "qcErr.h"
#include "CJpegBase.h"

#include "USystemFunc.h"
#include "ULogFunc.h"

#define PNG_BYTES_TO_CHECK 4  

CJpegBase::CJpegBase(void)
	: CBaseObject ()
	, m_nWidth(0)
	, m_nHeight(0)
	, m_hBmpImage(NULL)
	, m_pBmpBuff(NULL)
{
	SetObjectName ("CJpegBase");
}

CJpegBase::~CJpegBase(void)
{
	Close();
}

int	CJpegBase::OpenSource(const char * pURL)
{
	return QC_ERR_NONE;
}

int	CJpegBase::Close(void)
{
	QC_DEL_A(m_pBmpBuff);
	if (m_hBmpImage != NULL)
	{
		DeleteObject(m_hBmpImage);
		m_hBmpImage = NULL;
	}

	return QC_ERR_NONE;
}

int CJpegBase::Enc(unsigned char * pBmpBuff, RECT * rcData, const char * pFile)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr		jerr;
	unsigned char  * image_buffer;
	int			i = 0;
	FILE *		outfile;  
	JSAMPROW	row_pointer[1]; 
	int			row_stride;

	if ((outfile = fopen(pFile, "wb")) == NULL) {
		return -1;
	}	
	
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, outfile);

	cinfo.image_width = 640; /* image width and height, in pixels */
	cinfo.image_height = 480;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;

	jpeg_set_defaults(&cinfo);

	jpeg_set_quality(&cinfo, 2, TRUE);

	jpeg_start_compress(&cinfo, TRUE);
	row_stride = 640 * 3; /* JSAMPLEs per row in image_buffer */
	image_buffer = (unsigned char*)malloc(640 * 480 * 3);
	for (i = 0; i < 640 * 480; i++)
	{
		image_buffer[i * 3] = 0/*i*255*/;
		image_buffer[i * 3 + 1] = 255/*128-(i*255)&0x7f*/;
		image_buffer[i * 3 + 2] = 120/*255-(i*255)&0xff*/;
	}
	while (cinfo.next_scanline < cinfo.image_height)
	{
		row_pointer[0] = &image_buffer[cinfo.next_scanline * row_stride];
		(void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	jpeg_finish_compress(&cinfo); 
	fclose(outfile);  

	jpeg_destroy_compress(&cinfo);
}