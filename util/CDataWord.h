/*******************************************************************************
	文件:	CDataWord.h

	内容:	对JSON数据结构处理后，转为简单的数组文字信息。

	作者:	Ben King

	修改历史:
	2018-10-10		Ben			创建文件

*******************************************************************************/
#ifndef __CDataWord_H__
#define __CDataWord_H__

#include "CNodeList.h"
#include "CDataJson.h"
#include "CJpegFunc.h"

// 文字信息结构体
struct wordItem {
	wordItem(void)
	{
		m_pTextJson = NULL;
		m_pTextWord = NULL;
		SetRect(&m_rcPos, 0XFFFF, 0XFFFF, 0, 0);
	}
	~wordItem(void)
	{
		SAFE_DEL_A(m_pTextJson);
		SAFE_DEL_A(m_pTextWord);
	}
	char *	m_pTextJson;		// UTF8 原始文字
	TCHAR *	m_pTextWord;		// UNICODE 文字
	RECT	m_rcPos;			// 文字的位置信息
	POINT	m_ptPos[4];			// 文字上下左右四个点的信息
};

class CDataWord
{
public:
    CDataWord(void);
    virtual ~CDataWord(void);

	// 设置JPEG包装类指针，进行插图判断，插图压缩。
	virtual void	SetJpegFunc(CJpegFunc * pJpegFunc) { m_pJpegFunc = pJpegFunc; }

	// 解析JSON数据结构，转换为文字数组
	virtual	int		ParseData (CDataJson * pJson);

	// 设置图片的大小尺寸。
	virtual void	SetPicSize(int nWidth, int nHeight) { m_nWidth = nWidth; m_nHeight = nHeight; }

protected:
	// 根据文字的位置，判断是不是同一行的文字。
	virtual int		AdjustLine(void);
	// 析构，释放
	virtual int		Release(void);

protected:
	CDataJson *				m_pDataJson;	// JSON数据结构的指针
	CJpegFunc *				m_pJpegFunc;	// JPEG的包装类指针
	int						m_nWidth;		// 图片的宽度
	int						m_nHeight;		// 图片的高度

public:
	CObjectList<wordItem>	m_lstWord;		// 文字信息列表

};

#endif //__CDataWord_H__
