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
#include "graphic_cjk_bitmap.h"


#if defined(__HDK_LINKIT_ONE_V1__)
#define BACKLIGHT_PIN  VM_PIN_D1
#elif defined(__HDK_LINKIT_ASSIST_2502__)
#define BACKLIGHT_PIN VM_PIN_P1
#else
#error " Board not support"
#endif


/* Set font and draw hello world text */
static void draw_hello(void) {

    VMCHAR *teststr = "人人生而自由，在尊严和权利上一律平等。";
    VMINT font_width, font_height;
    VMUINT8 *glyph_bitmap;

    graphic_cjk_engine_font_t ext_font = {
    		SIMHEI_FONT_PATH,
			SIMHEI_FONT_SIZE,
			28,
			VM_FALSE,
			VM_FALSE,
			VM_FALSE
    };

    graphic_cjk_engine_set_font(ext_font);

    graphic_cjk_engine_measure_character(teststr[0]*256 + teststr[1], &font_width, &font_height);
    glyph_bitmap = vm_malloc((font_height+7)/8 * font_width);

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


