/*
  This sample code is in public domain.

  This sample code is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

/* 
  This sample sets a Chinese font file and shows a Chinese text.

  It initiates the resource to Chinese by API vm_res_init(), then load the text
  data by API vm_res_get_string(), after that sets the font file path in the 
  phone storage by API vm_graphic_set_font(), at last, draw the text by API
  vm_graphic_draw_text().
  Before run this application, you need to put a Chinese font file to EXTERNAL_FONT_PATH
  and set the EXTERNAL_FONT_SIZE which can be calculated by CheckMemSize.exe tool.

  Modify the macro SCREEN_WIDTH and SCREEN_HEIGHT to fit your display device.
*/

#include "vmtype.h" 
#include "vmstdlib.h"
#include "vmlog.h"
#include "vmwdt.h"
#include "vmgraphic.h"
#include "vmsystem.h"

#include "ResID.h"
#include "vmchset.h"
#include "vmgraphic_font.h"
#include "lcd_sitronix_st7789s.h"
#include "vmdcl.h"
#include "vmdcl_gpio.h"
#include "vmdcl_pwm.h"
#include "gb2312_unicode_convert.h"


#define SCREEN_WIDTH 240
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

#if defined(__HDK_LINKIT_ONE_V1__)
#define BACKLIGHT_PIN  VM_PIN_D1
#elif defined(__HDK_LINKIT_ASSIST_2502__)
#define BACKLIGHT_PIN VM_PIN_P1
#else
#error " Board not support"
#endif

VMUINT8 *font_pool;

/* Set font and draw hello world text */
static void draw_hello(void) {
    VMCSTR str = "All human beings are born free and equal in dignity and rights. They are endowed with reason and conscience and should act towards one another in a spirit of brotherhood.";
	//VMWCHAR string[200];
    VMWCHAR  string1[] = {0x5e8a, 0x524d, 0x660e, 0x6708, 0x5149, 0x00};
    VMWCHAR  string2[] = {0x7591, 0x4f3c, 0x5730, 0x4e0a, 0x971c, 0x00};
    VMWCHAR  string3[] = {0x4e3e, 0x5934, 0x671b, 0x660e, 0x6708, 0x00};
    VMWCHAR  string4[] = {0x4f4e, 0x5934, 0x601d, 0x6545, 0x4e61, 0x00};

    unsigned short demostr[][2] = {1,2,3,4,5,6};

    VMCHAR *srcstr = "个个生而自由，在尊严和权利上一律平等。";
    VMWCHAR desstr[20];
    str_gb2312_to_unicode(srcstr,desstr,20);

    vm_graphic_color_argb_t color;
    vm_graphic_frame_t frame;
    vm_graphic_frame_t* frame_group[1];
    VMWCHAR font_path[FONT_PATH_MAX_LENGTH + 1];
    VMWSTR font_paths_group[1];
    VM_RESULT result;
    VMUINT32 pool_size;
    VMUINT32 size;

    vm_graphic_point_t positions[1] = {0, 0};

    frame.buffer_length = SCREEN_WIDTH * SCREEN_HEIGHT * 2;
    frame.buffer = vm_malloc_dma(frame.buffer_length);
    if(frame.buffer == NULL)
    {
        return;
    }

    frame.color_format = VM_GRAPHIC_COLOR_FORMAT_16_BIT;
    frame.height = SCREEN_HEIGHT;
    frame.width = SCREEN_WIDTH;
    frame_group[0] = &frame;
    
    //string = vm_res_get_string(STR_ID_HELLO, &size);
    //string = "All human beings are born free and equal in dignity and rights. They are endowed with reason and conscience and should act towards one another in a spirit of brotherhood.";
    //string = "Hello world!";
    //vm_chset_ascii_to_ucs2(string,strlen(str),str);
/*
    string[0] = 0x4f60;
    string[1] = 0x597d;
    string[2] = 0x4e16;
    string[3] = 0x754c;
    string[4] = 0x0000;
*/
    color.a = 0;
    color.r = 0;
    color.g = 0;
    color.b = 0;
    vm_graphic_set_color(color);
    vm_graphic_draw_solid_rectangle(&frame, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


    result = vm_graphic_get_font_pool_size(EXTERNAL_FONT_SIZE, 1, FONT_CACHE_SIZE, &pool_size);
    if(VM_IS_SUCCEEDED(result))
    {
        font_pool = vm_malloc(pool_size);
        if(NULL != font_pool)
        {
            result = vm_graphic_init_font_pool(font_pool, pool_size);
        }
        else
        {
            vm_log_info("allocate font pool memory failed");
            return;
        }
    }
    if(!(VM_IS_SUCCEEDED(result)))
    {
        vm_log_info("init font pool failed");
        return;
    }
    vm_chset_ascii_to_ucs2(font_path, (FONT_PATH_MAX_LENGTH + 1) * 2, EXTERNAL_FONT_PATH);
    font_paths_group[0] = font_path;
    vm_graphic_reset_font();
    result = vm_graphic_set_font(font_paths_group, 1);
    if(!(VM_IS_SUCCEEDED(result)))
    {
        vm_log_info("set font failed");
    }

    color.r = 255;
    color.g = 255;
    color.b = 255;
    vm_graphic_set_color(color);
    vm_graphic_set_font_size(28);
    vm_log_info("width=%d, height=%d\n",vm_graphic_get_character_width(desstr[0]), vm_graphic_get_character_height(desstr[0]));

    //vm_log_info("String: %d, %d, %d, %d, %d, %d", string[0], string[1], string[2], string[3], string[4], string[5]);
    //vm_graphic_draw_text(&frame, 1, 10, string1);
    //vm_graphic_draw_text(&frame, 1, 58, string2);
    //vm_graphic_draw_text(&frame, 1, 106, string3);
    //vm_graphic_draw_text(&frame, 1, 154, string4);
    vm_graphic_draw_text(&frame, 0, 0, desstr);
    int i,k;
    unsigned char dotstr[33][33];
    VMUINT16 *p_buf = (VMUINT16 *)(frame_group[0]->buffer);
    for(i=0;i<32;i++) {
    	for(k=0;k<32;k++) {
    		if(p_buf[i*240+k]) dotstr[i][k] = '1';
    		else
    			dotstr[i][k] = '0';
    	}
    	dotstr[i][32] = 0;
    	vm_log_info(dotstr[i]);
    }

#if defined(__HDK_LINKIT_ASSIST_2502__)
    vm_graphic_blt_frame(frame_group, positions, 1);
#endif
    vm_free(frame.buffer);
    vm_free(font_pool);
    font_pool = NULL;
}

void graphic_draw_text_wrap(vm_graphic_frame_t* frame, VMINT x, VMINT y, VMWSTR text)
{

}




/* The callback to be invoked by the system engine. */
void handle_sysevt(VMINT message, VMINT param) {
	switch (message) {
	case VM_EVENT_CREATE:
        vm_res_init(86);    /* Initiate resource as Simple Chinese */
		break;
	case VM_EVENT_PAINT:
		draw_hello();
		break;
	case VM_EVENT_QUIT:
		vm_res_release();

		break;
	}
}

void lcd_backlight_level(VMUINT32 ulValue) {

      VM_DCL_HANDLE pwm_handle;
      vm_dcl_pwm_set_clock_t pwm_clock;
      vm_dcl_pwm_set_counter_threshold_t pwm_config_adv;

      vm_dcl_config_pin_mode(BACKLIGHT_PIN, VM_DCL_PIN_MODE_PWM);

      pwm_handle = vm_dcl_open(PIN2PWM(BACKLIGHT_PIN),vm_dcl_get_owner_id());
      vm_dcl_control(pwm_handle,VM_PWM_CMD_START,0);
      pwm_config_adv.counter = 100;
      pwm_config_adv.threshold = ulValue;
      pwm_clock.source_clock = 0;
      pwm_clock.source_clock_division =3;
      vm_dcl_control(pwm_handle,VM_PWM_CMD_SET_CLOCK,(void *)(&pwm_clock));
      vm_dcl_control(pwm_handle,VM_PWM_CMD_SET_COUNTER_AND_THRESHOLD,(void *)(&pwm_config_adv));
      vm_dcl_close(pwm_handle);
}

/* Entry point */
void vm_main(void) {
#if defined(__HDK_LINKIT_ASSIST_2502__)
	lcd_st7789s_init();
    lcd_backlight_level(60);
#endif
	vm_pmng_register_system_event_callback(handle_sysevt);
}


