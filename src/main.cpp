#include <Arduino.h>
#include "SPI.h"
#include "TFT_eSPI.h"
#include "..\LVGL\src\lvgl.h"

static const uint16_t screenWidth = 480;
static const uint16_t screenHeight = 320;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10];

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */

/* Display flushing */
void my_disp_flush(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p) {
	uint32_t w = (area->x2 - area->x1 + 1);
	uint32_t h = (area->y2 - area->y1 + 1);

	tft.startWrite();
	tft.setAddrWindow(area->x1, area->y1, w, h);
	tft.pushColors((uint16_t*)&color_p->full, w * h, true);
	tft.endWrite();

	lv_disp_flush_ready(disp);
}

void setup() {
	Serial.begin(115200); /* prepare for possible serial debug */

	String LVGL_Arduino = "Hello Arduino! ";
	LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

	Serial.println(LVGL_Arduino);
	Serial.println("I am LVGL_Arduino");

	lv_init();


	tft.begin();          /* TFT init */
	tft.setRotation(1); /* Landscape orientation, flipped */

	lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 10);

	/*Initialize the display*/
	static lv_disp_drv_t disp_drv;
	lv_disp_drv_init(&disp_drv);

	/*Change the following line to your display resolution*/
	disp_drv.hor_res = screenWidth;
	disp_drv.ver_res = screenHeight;
	disp_drv.flush_cb = my_disp_flush;
	disp_drv.draw_buf = &draw_buf;
	lv_disp_drv_register(&disp_drv);


	/* Create simple label */
	lv_obj_t* label = lv_label_create(lv_scr_act());
	lv_label_set_text(label, "Hello World!");
	
	lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);
}

void loop() {
	lv_timer_handler(); /* let the GUI do its work */
	delay(5);
}