

#ifndef _GRAPHIC_CJK_BITMAP_H_
#define _GRAPHIC_CJK_BITMAP_H_


#define SCREEN_WIDTH    240
#define SCREEN_HEIGHT   240

#define NOTOSANS_BOLD_FONT_SIZE           (13927)
#define NOTOSANS_BOLDITALIC_FONT_SIZE     (13950)
#define NOTOSANS_ITALIC_FONT_SIZE         (13932)
#define NOTOSANS_REGULAR_FONT_SIZE        (13936)
#define SIMHEI_FONT_SIZE                  (12802)
#define SIMKAI_FONT_SIZE                  (12582)
#define SIMLI_FONT_SIZE                   (11857)
#define SIMSUN_FONT_SIZE                  (14935)
#define DEMO_FONT_SIZE                    (26676)
#define SHSK_LIGHT_FONT_SIZE              (247230)
#define SIMSUNB_FONT_SIZE                 (10993)

#define NOTOSANS_BOLD_FONT_PATH           ("E:\\font\\NotoSans-Bold.ttf")
#define NOTOSANS_BOLDITALIC_FONT_PATH     ("E:\\font\\NotoSans-BoldItalic.ttf")
#define NOTOSANS_BOLDITALIC_FONT_PATH1    ("E:\\font\\NotoSans-BoldItalic.ttf")
#define NOTOSANS_ITALIC_FONT_PATH         ("E:\\font\\NotoSans-Italic.ttf")
#define NOTOSANS_REGULAR_FONT_PATH        ("E:\\font\\NotoSans-Regular.ttf")
#define SIMHEI_FONT_PATH                  ("E:\\font\\simhei.ttf")
#define SIMKAI_FONT_PATH                  ("E:\\font\\simkai.ttf")
#define SIMLI_FONT_PATH                   ("E:\\font\\simli.ttf")
#define SIMSUN_FONT_PATH                  ("E:\\font\\simsun.ttf")
#define DEMO_FONT_PATH                    ("E:\\font\\demo.ttf")
#define SHSK_LIGHT_FONT_PATH              ("E:\\font\\SourceHanSansK-Light.ttf")
#define SIMSUNB_FONT_PATH                 ("E:\\font\\simsunb.ttf")

#define EXTERNAL_FONT_SIZE  SIMHEI_FONT_SIZE   /* external font size calculated by CheckMemSize.exe tool, 32 * 1024 */
#define FONT_CACHE_SIZE     131072   /* 128 * 1024 */
#define EXTERNAL_FONT_PATH  SIMHEI_FONT_PATH
#define FONT_PATH_MAX_LENGTH    100


typedef struct
{
	const VMCHAR *path;
	VMINT max_external_font_size; //Bytes, the actual font size when font is loaded in RAM, it can be get by tool.
	VMINT font_size;
	VMINT bold;
	VMINT italic;
	VMINT underline;
} graphic_cjk_engine_font_t;

typedef struct
{
	VMINT width;
	VMINT height;
	VMUINT8 *glyph_bitmap;

} graphic_cjk_engine_bitmap_t;

VMINT graphic_cjk_engine_set_font(graphic_cjk_engine_font_t ext_font);

void graphic_cjk_engine_deinit();

void graphic_cjk_engine_set_font_size(VMINT font_size);

void graphic_cjk_engine_set_font_style(VMINT bold, VMINT italic, VMINT underline);

VM_RESULT graphic_cjk_engine_measure_character(VMUWCHAR gbcode_euc, VMINT* width, VMINT* height);

VMINT graphic_cjk_engine_get_bitmap(VMUWCHAR gbcode_euc, graphic_cjk_engine_bitmap_t *p_bitmap);

VMINT graphic_cjk_engine_show_bitmap(VMINT x, VMINT y, graphic_cjk_engine_bitmap_t bitmap);

VMINT graphic_cjk_engine_blt_frame(void);

#endif
