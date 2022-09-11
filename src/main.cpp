#include <Arduino.h>

#include "SPI.h"
#include "TFT_eSPI.h"

#include "../lvgl/src/lvgl.h"

TFT_eSPI tft = TFT_eSPI();

#define YP 26  // Y+ pin
#define XM 25  // X- pin (actually connected to X+ pin)
#define YM 32  // Y- pin
#define XP 33  // X+ pin (actually connected to X- pin)

static lv_disp_buf_t disp_buf; //LVGL stuff;
static lv_color_t buf[LV_HOR_RES_MAX * 10];
lv_disp_drv_t disp_drv;


lv_obj_t* btn1;
lv_obj_t* btn2;
lv_obj_t* screenMain;
lv_obj_t* label;

static void event_handler_btn(lv_obj_t* obj, lv_event_t event) {

	if (event == LV_EVENT_CLICKED) {
		if (obj == btn1)
			lv_label_set_text(label, "YES");
		else if (obj == btn2) {
			lv_label_set_text(label, "NO");
		}
	}
}

void my_disp_flush(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p) {

	uint32_t w = (area->x2 - area->x1 + 1);
	uint32_t h = (area->y2 - area->y1 + 1);

	tft.startWrite();
	tft.setAddrWindow(area->x1, area->y1, w, h);
	tft.pushColors(&color_p->full, w * h, true);
	tft.endWrite();

	lv_disp_flush_ready(disp);
}

void setup() {

	Serial.begin(115200);

	tft.begin();

	tft.setRotation(2);

	analogReadResolution(10);

	lv_init();

	lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);

	/*Initialize the display*/
	lv_disp_drv_init(&disp_drv);
	disp_drv.hor_res = 320;
	disp_drv.ver_res = 480;
	disp_drv.flush_cb = my_disp_flush;
	disp_drv.buffer = &disp_buf;
	lv_disp_drv_register(&disp_drv);

	screenMain = lv_obj_create(NULL, NULL);

	label = lv_label_create(screenMain, NULL);
	lv_label_set_long_mode(label, LV_LABEL_LONG_BREAK);
	lv_label_set_text(label, "Hello World!");
	lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);
	lv_obj_set_size(label, 240, 40);
	lv_obj_set_pos(label, 20, 40);

	btn1 = lv_btn_create(screenMain, NULL);
	lv_obj_set_event_cb(btn1, event_handler_btn);
	lv_obj_set_width(btn1, 70);
	lv_obj_set_height(btn1, 32);
	lv_obj_set_pos(btn1, 32, 130);

	lv_obj_t* label1 = lv_label_create(btn1, NULL);
	lv_label_set_text(label1, "BT 1");

	btn2 = lv_btn_create(screenMain, NULL);
	lv_obj_set_event_cb(btn2, event_handler_btn);
	lv_obj_set_width(btn2, 70);
	lv_obj_set_height(btn2, 32);
	lv_obj_set_pos(btn2, 142, 130);



	lv_obj_t* label2 = lv_label_create(btn2, NULL);
	lv_label_set_text(label2, "BT 2");

	static lv_style_t style1;
	lv_style_init(&style1);
	lv_style_set_text_color(&style1, LV_STATE_DEFAULT, LV_COLOR_GREEN);
	lv_style_set_border_color(&style1, LV_STATE_DEFAULT, LV_COLOR_RED);
	lv_style_set_border_width(&style1, LV_STATE_DEFAULT, 2);
	lv_style_set_bg_color(&style1, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
	lv_style_set_bg_opa(&style1, LV_STATE_DEFAULT, 255);
	lv_style_set_radius(&style1, LV_STATE_DEFAULT, 20);


	static lv_style_t style2;
	lv_style_init(&style2);
	lv_style_set_text_color(&style2, LV_STATE_DEFAULT, LV_COLOR_GRAY);
	lv_style_set_border_color(&style2, LV_STATE_DEFAULT, lv_color_hex(0x222222));
	lv_style_set_border_width(&style2, LV_STATE_DEFAULT, 1);
	lv_style_set_bg_color(&style2, LV_STATE_DEFAULT, lv_color_hex(0x222222));
	lv_style_set_bg_opa(&style2, LV_STATE_DEFAULT, 255);
	lv_style_set_radius(&style2, LV_STATE_DEFAULT, 4);
	lv_style_set_shadow_spread(&style2, LV_STATE_DEFAULT, 1);
	lv_style_set_shadow_color(&style2, LV_STATE_DEFAULT, LV_COLOR_GRAY);
	lv_style_set_shadow_opa(&style2, LV_STATE_DEFAULT, 255);
	lv_style_set_shadow_width(&style2, LV_STATE_DEFAULT, 1);

	static lv_style_t bigStyle;
	lv_style_init(&bigStyle);
	lv_style_set_text_font(&bigStyle, LV_STATE_DEFAULT, &lv_font_montserrat_36);



	lv_obj_add_style(label, LV_LABEL_PART_MAIN, &bigStyle);

	//Uncomment to chabge buutton 1 styling:
	//lv_obj_add_style(btn1, LV_LABEL_PART_MAIN, &style1);

	//Uncomment to chabge buutton 2 styling:
	//lv_obj_add_style(btn2, LV_LABEL_PART_MAIN, &style2);


	lv_scr_load(screenMain);
}

void loop() {

	lv_task_handler();

	delay(1);
}