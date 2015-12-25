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
#include <string.h>
#include "vmfs.h"
#include "ResID.h"
#include "vmchset.h"
#include "vmgraphic_font.h"
#include "lcd_sitronix_st7789s.h"
#include "vmdcl.h"
#include "vmdcl_gpio.h"
#include "vmdcl_pwm.h"
#include "graphic_cjk_bitmap.h"


#if defined(__HDK_LINKIT_ONE_V1__)
#define BACKLIGHT_PIN  VM_PIN_D1
#elif defined(__HDK_LINKIT_ASSIST_2502__)
#define BACKLIGHT_PIN VM_PIN_P1
#else
#error " Board not support"
#endif


void file_write(const char* fileName, const char* strBuf, long pos)
{
	VMINT drv ;
	VMCHAR file_name[VM_FS_MAX_PATH_LENGTH];
	VMWCHAR w_file_name[VM_FS_MAX_PATH_LENGTH];
	VM_FS_HANDLE filehandle = -1;
	VMUINT writelen = 0;
	VMINT ret = 0;

	/* If there is a removable letter (SD Card) use it,
	 * otherwise stored it in the flash storage
	 * (vm_fs_get_internal_drive_letter).
	 * */
	drv = vm_fs_get_removable_drive_letter();
	if(drv <0){
		drv = vm_fs_get_internal_drive_letter();
	    if(drv <0){
	    	vm_log_fatal("not find driver");
	    	return ;
	    }
	  }
	sprintf(file_name, "%c:\\%s", drv, fileName);
	vm_chset_ascii_to_ucs2(w_file_name, VM_FS_MAX_PATH_LENGTH, file_name);

	// write file
	if((filehandle = vm_fs_open(w_file_name, VM_FS_MODE_CREATE_ALWAYS_WRITE, TRUE)) < 0)
	{
		vm_log_info("Write failed to open file: %s",file_name);
		return;
	}
	vm_log_info("Write success to open file: %s", file_name);

	vm_fs_seek(filehandle, pos, VM_FS_BASE_END);

	ret = vm_fs_write(filehandle, (void*)strBuf, strlen(strBuf), &writelen);
	if(ret < 0)
	{
		vm_log_info("Failed to write file");
		return;
	}
	vm_log_info("Success to write file: %s", file_name);
	vm_fs_close(filehandle);
}

/* Set font and draw hello world text */
static void draw_hello(void) {

    VMUCHAR teststr[][12] = {"床前明月光", "疑似地上霜", "举头望明月", "低头思故乡"};
    VMINT font_width, font_height;
    VMUINT8 *glyph_bitmap;
    graphic_cjk_engine_bitmap_t bitmap;
    VMUINT glyph_size;

    graphic_cjk_engine_font_t ext_font = {
    		SIMHEI_FONT_PATH,
			SIMHEI_FONT_SIZE,
			36,
			VM_FALSE,
			VM_FALSE,
			VM_FALSE
    };

    graphic_cjk_engine_set_font(ext_font);

    graphic_cjk_engine_measure_character(teststr[0][0]*256 + teststr[0][1], &font_width, &font_height);


    VMCHAR demostr[100];
    sprintf(demostr, "font_width=%d, font_height=%d\n", font_width, font_height);
    file_write("angel_font.txt", demostr, 0);

    vm_log_info("font_width=%d, font_height=%d\n", font_width, font_height);

    glyph_size = font_height * font_width;
    glyph_bitmap = vm_malloc(glyph_size);
	if(glyph_bitmap == NULL){
		return ;
	}

    bitmap.glyph_bitmap = glyph_bitmap;

    graphic_cjk_engine_set_font_style(0,0,0);

    VMINT i,j,k;
	for(i=0;i<4;i++) {
		for(j=0,k=0;j<10;j+=2,k++) {
			memset(glyph_bitmap,0,glyph_size);
			graphic_cjk_engine_get_bitmap(teststr[i][j]*256 + teststr[i][j+1], &bitmap);
			graphic_cjk_engine_show_bitmap(k*bitmap.width,(i+1)*48-bitmap.height,bitmap);
		}
	}

	graphic_cjk_engine_blt_frame();

}

/* The callback to be invoked by the system engine. */
void handle_sysevt(VMINT message, VMINT param) {
	switch (message) {
	case VM_EVENT_CREATE:
        vm_res_init(86);    /* Initiate resource as Simple Chinese */
        graphic_cjk_engine_init();
		break;
	case VM_EVENT_PAINT:
		draw_hello();
		break;
	case VM_EVENT_QUIT:
		graphic_cjk_engine_deinit();
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


