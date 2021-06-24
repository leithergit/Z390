#pragma once

#include <ft2build.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include FT_GLYPH_H
#include FT_FREETYPE_H
#include FT_OUTLINE_H

using namespace cv;

#pragma comment(lib,"freetyped.lib")

class CVText_CN
{

public:
    CVText_CN& operator=(const CVText_CN&) = delete;
    CVText_CN() = delete;
    CVText_CN(CVText_CN&) = delete;
    /// 装载字库文件
    CVText_CN(const char* pFontFace,int nFontSize = 20,bool bBold = false)
    {
		assert(pFontFace != nullptr);

        // 打开字库文件, 创建一个字体
		if (FT_Init_FreeType(&m_library))
            throw;
		if (FT_New_Face(m_library, pFontFace, 0, &m_face)) 
            throw;
        m_bBold = bBold;
        // 设置字体输出参数
		SetFontProp(nFontSize);
        // 设置C语言的字符集环境
		setlocale(LC_ALL, "");
    }

    virtual ~CVText_CN()
    {
		FT_Done_Face(m_face);
		FT_Done_FreeType(m_library);
    }

    /**
      * 获取字体。目前有些参数尚不支持。
      *
      * \param font        字体类型, 目前不支持
      * \param size        字体大小/空白比例/间隔比例/旋转角度
      * \param underline   下画线
      * \param diaphaneity 透明度
      * \sa setFont, restoreFont
      */

    void getFont(int* type, CvScalar* size = nullptr, bool* underline = nullptr, float* diaphaneity = nullptr)
    {
		if (type) *type = m_fontType;
		if (size) *size = m_fontSize;
		if (underline) *underline = m_fontUnderline;
		if (diaphaneity) *diaphaneity = m_fontDiaphaneity;
    }

    /**
   * 设置字体。目前有些参数尚不支持。
   *
   * \param font        字体类型, 目前不支持
   * \param size        字体大小/空白比例/间隔比例/旋转角度
   * \param underline   下画线
   * \param diaphaneity 透明度
   * \sa getFont, restoreFont
   */

    void setFont(int* type, CvScalar* size = nullptr, bool* underline = nullptr, float* diaphaneity = nullptr)
    {
        // 参数合法性检查

		if (type)
		{
			if (type >= 0) m_fontType = *type;
		}
		if (size)
		{
			m_fontSize.val[0] = fabs(size->val[0]);
			m_fontSize.val[1] = fabs(size->val[1]);
			m_fontSize.val[2] = fabs(size->val[2]);
			m_fontSize.val[3] = fabs(size->val[3]);
		}
		if (underline)
		{
			m_fontUnderline = *underline;
		}
		if (diaphaneity)
		{
			m_fontDiaphaneity = *diaphaneity;
		}
    }

    /**
     * 恢复原始的字体设置。
     *
     * \sa getFont, setFont
     */

    void SetFontProp(int nFontSize)
    {
		m_fontType = 0;            		// 字体类型(不支持)
		m_fontSize.val[0] = nFontSize;   // 字体大小
		m_fontSize.val[1] = 0.5;   		// 空白字符大小比例
		m_fontSize.val[2] = 0.1;   		// 间隔大小比例
		m_fontSize.val[3] = 0;     		// 旋转角度(不支持)
		m_fontUnderline = false;   		// 下画线(不支持)
		m_fontDiaphaneity = 1.0;   		// 色彩比例(可产生透明效果)

        // 设置字符大小
		FT_Set_Pixel_Sizes(m_face, (int)m_fontSize.val[0], 0);
    }

    /**
    * 输出汉字(颜色默认为黑色)。遇到不能输出的字符将停止。
    * \param img  输出的影象
    * \param text 文本内容
    * \param pos  文本位置
    * \return 返回成功输出的字符长度，失败返回-1。
    */

    int putText(IplImage* img, const char* text, CvPoint pos)
    {
        
        return putText(img, text, pos, cvScalar(CV_RGB(255, 255, 255)));
    }

    /**
     * 输出汉字(颜色默认为黑色)。遇到不能输出的字符将停止。
     * \param img  输出的影象
     * \param text 文本内容
     * \param pos  文本位置
     * \return 返回成功输出的字符长度，失败返回-1。
     */

    int putText(IplImage* img, const wchar_t* text, CvPoint pos)
    {
        return putText(img, text, pos, cvScalar(CV_RGB(255, 255, 255)));
    }

    /**
      * 输出汉字。遇到不能输出的字符将停止。
      * \param img   输出的影象
      * \param text  文本内容
      * \param pos   文本位置
      * \param color 文本颜色
      * \return 返回成功输出的字符长度，失败返回-1。
      */

    int putText(IplImage* img, const char* text, CvPoint pos, CvScalar color)
    {
		if (img == nullptr) 
            return -1;
		if (text == nullptr) 
            return -1;

		int i;
		for (i = 0; text[i] != '\0'; ++i)
		{
			wchar_t wc = text[i];

            // 解析双字节符号
			if (!isascii(wc)) 
                mbtowc(&wc, &text[i++], 2);

            // 输出当前的字符
			putWChar(img, wc, pos, color);
		}
		return i;
    }

    /**
      * 输出汉字。遇到不能输出的字符将停止。
      *
      * \param img   输出的影象
      * \param text  文本内容
      * \param pos   文本位置
      * \param color 文本颜色
      *
      * \return 返回成功输出的字符长度，失败返回-1。
      */
    int putText(IplImage* img, const wchar_t* text, CvPoint pos, CvScalar color)
    {
		if (img == nullptr) 
            return -1;
		if (text == nullptr) 
            return -1;

		int i;
		for (i = 0; text[i] != '\0'; ++i)
		{
			putWChar(img, text[i], pos, color);
		}
		return i;
    }

private:

    // 输出当前字符, 更新m_pos位置
    void putWChar(IplImage* img, wchar_t wChar, CvPoint& pos, CvScalar color)
    {
        if (wChar == L' ')
        {
            pos.x += m_fontSize.val[0]/2;
            pos.x += 2;
        }
        else
        {
			FT_UInt glyph_index = FT_Get_Char_Index(m_face, wChar);
			uint32_t flags = FT_LOAD_DEFAULT | FT_OUTLINE_HIGH_PRECISION | FT_LOAD_FORCE_AUTOHINT;
			//uint32_t oldFlags = FT_LOAD_FORCE_AUTOHINT;
			FT_Load_Glyph(m_face, glyph_index, flags);
			if (m_bBold)
			{
				int nStrength = 1 << 6;
				FT_Outline_EmboldenXY(&m_face->glyph->outline, nStrength, nStrength);
			}
			FT_BBox ftbox;
			FT_Render_Glyph(m_face->glyph, FT_RENDER_MODE_MONO);
			FT_GlyphSlot slot = m_face->glyph;

			int rows = slot->bitmap.rows;
			int cols = slot->bitmap.width;

			for (int i = 0; i < rows; ++i)
			{
				for (int j = 0; j < cols; ++j)
				{
					int off = ((img->origin == 0) ? i : (rows - 1 - i))
						* slot->bitmap.pitch + j / 8;

					if (slot->bitmap.buffer[off] & (0xC0 >> (j % 8)))
					{
						int r = (img->origin == 0) ? pos.y - (rows - 1 - i) : pos.y + i;;
						int c = pos.x + j;

						if (r >= 0 && r < img->height
							&& c >= 0 && c < img->width)
						{
							CvScalar scalar = cvGet2D(img, r, c);

							float p = m_fontDiaphaneity;
							for (int k = 0; k < 4; ++k)
							{
								scalar.val[k] = scalar.val[k] * (1 - p) + color.val[k] * p;
							}

							cvSet2D(img, r, c, scalar);
						}
					}
				}
			}

			double space = m_fontSize.val[0] * m_fontSize.val[1];
			double sep = m_fontSize.val[0] * m_fontSize.val[2];
			pos.x += (int)((cols ? cols : space) + sep);
        }
		
    }

private:

    FT_Library   m_library;   // 字库
    FT_Face      m_face;      // 字体

   // 默认的字体输出参数
    int         m_fontType;
    bool        m_bBold = false;
    CvScalar   m_fontSize;
    bool      m_fontUnderline;
    float      m_fontDiaphaneity;
};

