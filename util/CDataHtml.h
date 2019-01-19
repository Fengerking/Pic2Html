/*******************************************************************************
	文件:	CDataHtml.h

	内容:	通过文字，文字的位置，转换成HTML网页的格式输出。
			在没有文字的区域，扫描Bitmap内容，判断是否有插图，并写入网页。

	作者:	Ben King

	修改历史:
	2018-10-10		Ben			创建文件

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

	// 设置JPEG包装类指针，进行插图判断，插图压缩。
	virtual void	SetJpegFunc(CJpegFunc * pJpegFunc) { m_pJpegFunc = pJpegFunc; }

	// 输出HTML格式文件
	// 参数：	pWord, 包含文字信息类的指针。
	//			pFile, 输出HTML的文件名
	virtual	int		OutTextHtml(CDataWord * pWord, const TCHAR * pFile);

	// 输出UNICODE 文本格式文件
	// 参数：	pWord, 包含文字信息类的指针。
	//			pFile, 输出文本的文件名
	virtual	int		OutTextWord(CDataWord * pWord, const TCHAR * pFile);

	// 输出UTF8格式文件
	// 参数：	pWord, 包含文字信息类的指针。
	//			pFile, 输出UTF8文本的文件名
	virtual	int		OutTextUtf8(CDataWord * pWord, const TCHAR * pFile);

protected:
	// 分析所有的文字位置，判断出图片的大小。
	virtual int		ParserWordInfo(CDataWord * pWord);

	// 判断在指定的区域里面有没有插图。
	// 参数：	pIO, 写HTML文件的指针，写入插图信息。
	//			pArea, 指定需要判断的区域
	virtual int		CheckImageInfo(CFile * pIO, RECT * pArea);

	// 写字符串到文件里面。
	// 参数：	pIO，文件读写的指针。
	//			pText，要写到文件中的字符串。
	virtual int		WriteText(CFile * pIO, char * pText);

	// 写HTML的头部信息。
	virtual int		WriteHead(CFile * pIO);
	// 写HTML的尾部信息。
	virtual int		WriteFoot(CFile * pIO);

protected:
	CJpegFunc *		m_pJpegFunc;			// JPEG的包装类指针

	int				m_nLeft;				// 文字的最左边位置
	int				m_nRight;				// 文字的最右边位置
	int				m_nTop;					// 文字的最上边位置
	int				m_nBottom;				// 文字的最下面位置
	int				m_nLineMaxWords;		// 文字一行最多文字数字
	int				m_nLineMinH;			// 文字一行最小的高度
	int				m_nFontSize;			// 文字的字体尺寸

	CString			m_strJpegFile;			// 插图压缩后的JPEG文件名
	int				m_nJpegIndex;			// 插图的索引号
};

#endif //__CDataHtml_H__
