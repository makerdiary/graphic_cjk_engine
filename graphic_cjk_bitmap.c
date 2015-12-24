
#include "vmtype.h"
#include "vmstdlib.h"
#include "vmlog.h"
#include "vmgraphic.h"
#include "vmchset.h"
#include "vmgraphic_font.h"
#include "gb2312_unicode_convert.h"
#include "graphic_cjk_bitmap.h"


vm_graphic_frame_t g_frame;
vm_graphic_frame_t* g_frame_group[1];
VMUINT8 *g_font_pool;


VMINT graphic_cjk_engine_set_font(graphic_cjk_engine_font_t ext_font)
{
	vm_graphic_color_argb_t color;

	VM_RESULT result;
	VMUINT32 pool_size;
    VMWCHAR font_path[FONT_PATH_MAX_LENGTH + 1];
    VMWSTR font_paths_group[1];

	g_frame.buffer_length = SCREEN_WIDTH * SCREEN_HEIGHT * 2;
	g_frame.buffer = vm_malloc_dma(g_frame.buffer_length);
	if(g_frame.buffer == NULL){
		return VM_FAIL;
	}
    g_frame.color_format = VM_GRAPHIC_COLOR_FORMAT_16_BIT;
	g_frame.height = SCREEN_HEIGHT;
	g_frame.width = SCREEN_WIDTH;
	g_frame_group[0] = &g_frame;

    color.a = 0;
    color.r = 0;
    color.g = 0;
    color.b = 0;
    vm_graphic_set_color(color);
    vm_graphic_draw_solid_rectangle(&g_frame, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	result = vm_graphic_get_font_pool_size(ext_font.max_external_font_size, 1, FONT_CACHE_SIZE, &pool_size);
	if(VM_IS_SUCCEEDED(result)){
		g_font_pool = vm_malloc(pool_size);
		if(NULL != g_font_pool){
			result = vm_graphic_init_font_pool(g_font_pool, pool_size);
		}
		else{
			vm_log_info("allocate font pool memory failed");
			return VM_FAIL;
		}
	}
	if(!(VM_IS_SUCCEEDED(result)))
	{
		vm_log_info("init font pool failed");
		return VM_FAIL;
	}
    vm_chset_ascii_to_ucs2(font_path, (FONT_PATH_MAX_LENGTH + 1) * 2, ext_font.path);
    font_paths_group[0] = font_path;
    vm_graphic_reset_font();
    result = vm_graphic_set_font(font_paths_group, 1);
    if(!(VM_IS_SUCCEEDED(result))){
    	vm_log_info("set font failed");
    }
    color.r = 255;
    color.g = 255;
    color.b = 255;
    vm_graphic_set_color(color);
    vm_graphic_set_font_size(ext_font.font_size);

    return VM_SUCCESS;
}

void graphic_cjk_engine_deinit()
{
    vm_free(g_frame.buffer);
    vm_free(g_font_pool);
    g_font_pool = NULL;
}

void graphic_cjk_engine_set_font_size(VMINT font_size)
{
	vm_graphic_set_font_size(font_size);
}

void graphic_cjk_engine_set_font_style(VMINT bold, VMINT italic, VMINT underline)
{
	vm_graphic_set_font_style(bold, italic, underline);
}

VM_RESULT graphic_cjk_engine_measure_character(VMUWCHAR gbcode_euc, VMINT* width, VMINT* height)
{
	VMUWCHAR character = char_gb2312_to_unicode(gbcode_euc);
	vm_graphic_measure_character(character, width, height);

	return VM_SUCCESS;
}


VMINT graphic_cjk_engine_get_bitmap(VMUWCHAR gbcode_euc, graphic_cjk_engine_bitmap_t *p_bitmap)
{
	VMUWCHAR des_code[] = {0,0};
	des_code[0] = char_gb2312_to_unicode(gbcode_euc);

	vm_graphic_draw_text(&g_frame, 0, 0, des_code);

	VMINT width, height;
	vm_graphic_measure_character(des_code[0], &width, &height);

	p_bitmap->width = width;
	p_bitmap->height = height;

	VMUINT16 *p_buf = (VMUINT16 *)(g_frame_group[0]->buffer);

	VMINT i,j;

	for(i=0; i<height; i++) {
		for(j=0; j<width; j++) {
			*(p_bitmap->glyph_bitmap+i*width +j) = (p_buf[i*SCREEN_WIDTH+j]<<3)&0xf8;
		}
	}

	return VM_SUCCESS;
}

VMUINT8 graphic_cjk_engine_get_avg_gray(graphic_cjk_engine_bitmap_t bitmap) {
	VMINT i,j;
	VMUINT32 sum = 0;
	VMINT count = 0;

	for(i=0;i<bitmap.height;i++) {
		for(j=0;j<bitmap.width;j++) {
			if(bitmap.glyph_bitmap[i*bitmap.width+j]>0 && bitmap.glyph_bitmap[i*bitmap.width+j]>0 < 248) {
				count++;
				sum += bitmap.glyph_bitmap[i*bitmap.width+j];
			}
		}
	}

	return (sum/count);
}

VMINT graphic_cjk_engine_show_bitmap(VMINT x, VMINT y, graphic_cjk_engine_bitmap_t bitmap)
{
	VMINT width = bitmap.width;
	VMINT height = bitmap.height;
	VMINT i,j;

	VMUINT8 avg_gray = graphic_cjk_engine_get_avg_gray(bitmap);
	for(i=0; i<height; i++) {
		for(j=0; j<width; j++) {
			if(bitmap.glyph_bitmap[i*width+j] > avg_gray) {
				vm_graphic_draw_point(&g_frame, x+j, y+i);
			}
		}
	}

    return VM_SUCCESS;
}


VMINT graphic_cjk_engine_blt_frame(void)
{
	vm_graphic_point_t positions[1] = {0, 0};

#if defined(__HDK_LINKIT_ASSIST_2502__)
    vm_graphic_blt_frame(g_frame_group, positions, 1);
#endif

    return VM_SUCCESS;
}
