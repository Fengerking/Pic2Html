/*******************************************************************************
	�ļ�:	CDataHtml.h

	����:	ͨ�����֣����ֵ�λ�ã�ת����HTML��ҳ�ĸ�ʽ�����
			��û�����ֵ�����ɨ��Bitmap���ݣ��ж��Ƿ��в�ͼ����д����ҳ��

	����:	Ben King

	�޸���ʷ:
	2018-10-10		Ben			�����ļ�

*******************************************************************************/
#ifndef __CDataHtml_H__
#define __CDataHtml_H__

#include "CDataWord.h"
#include "CJpegFunc.h"

class CDataHtml
{
public:
    CDataHtml(void);
    virtual ~CDataHtml(void);

	// ����JPEG��װ��ָ�룬���в�ͼ�жϣ���ͼѹ����
	virtual void	SetJpegFunc(CJpegFunc * pJpegFunc) { m_pJpegFunc = pJpegFunc; }

	// ���HTML��ʽ�ļ�
	// ������	pWord, ����������Ϣ���ָ�롣
	//			pFile, ���HTML���ļ���
	virtual	int		OutTextHtml(CDataWord * pWord, const TCHAR * pFile);

	// ���UNICODE �ı���ʽ�ļ�
	// ������	pWord, ����������Ϣ���ָ�롣
	//			pFile, ����ı����ļ���
	virtual	int		OutTextWord(CDataWord * pWord, const TCHAR * pFile);

	// ���UTF8��ʽ�ļ�
	// ������	pWord, ����������Ϣ���ָ�롣
	//			pFile, ���UTF8�ı����ļ���
	virtual	int		OutTextUtf8(CDataWord * pWord, const TCHAR * pFile);

protected:
	// �������е�����λ�ã��жϳ�ͼƬ�Ĵ�С��
	virtual int		ParserWordInfo(CDataWord * pWord);

	// �ж���ָ��������������û�в�ͼ��
	// ������	pIO, дHTML�ļ���ָ�룬д���ͼ��Ϣ��
	//			pArea, ָ����Ҫ�жϵ�����
	virtual int		CheckImageInfo(CFile * pIO, RECT * pArea);

	// д�ַ������ļ����档
	// ������	pIO���ļ���д��ָ�롣
	//			pText��Ҫд���ļ��е��ַ�����
	virtual int		WriteText(CFile * pIO, char * pText);

	// дHTML��ͷ����Ϣ��
	virtual int		WriteHead(CFile * pIO);
	// дHTML��β����Ϣ��
	virtual int		WriteFoot(CFile * pIO);

protected:
	CJpegFunc *		m_pJpegFunc;			// JPEG�İ�װ��ָ��

	int				m_nLeft;				// ���ֵ������λ��
	int				m_nRight;				// ���ֵ����ұ�λ��
	int				m_nTop;					// ���ֵ����ϱ�λ��
	int				m_nBottom;				// ���ֵ�������λ��
	int				m_nLineMaxWords;		// ����һ�������������
	int				m_nLineMinH;			// ����һ����С�ĸ߶�
	int				m_nFontSize;			// ���ֵ�����ߴ�

	CString			m_strJpegFile;			// ��ͼѹ�����JPEG�ļ���
	int				m_nJpegIndex;			// ��ͼ��������
};

#endif //__CDataHtml_H__
