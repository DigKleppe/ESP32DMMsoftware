
/*
 * SettingsItem.cpp
 *
 *  Created on: Mar 7, 2021
 *      Author: dig
 */


#include "MenuSettings.h"
#include "settings.h"

#include <stdio.h>
LV_FONT_DECLARE(dejaVuSansMono25);


#define FONT &dejaVuSansMono25
#define PADDING 4

int speed = 23;
int res =5;

static lv_obj_t * screen;
static lv_group_t * g ;

int MenuSettings::nrItems;
int MenuSettings::selected_btnIdx[MAXLEVELS];
lv_obj_t * MenuSettings::list1;
lv_obj_t * MenuSettings::selected_btn;
MenuSetttingsDesrc_t * MenuSettings::table;
bool  MenuSettings::changed;
volatile bool MenuSettings::active;

#define NR_BUTTONS_VISIBLE 6
#define BUTTONHEIGHT (LV_VER_RES_MAX/NR_BUTTONS_VISIBLE)
#define MAXCHARSPERLINE  19// 24 for 16


int level;

static lv_style_t menuStyle;
void * dummy ( void * arg){
	return NULL;
}
typedef void * lv_obj_user_data_t;


void *toDMMsettings( void * arg)
{
	return NULL;
}
void *toWifiSettings( void * arg)
{
	return NULL;
}
void *MenuSettings::exit ( void * arg)
{
	active = false;
	return NULL;
}



MenuSetttingsDesrc_t DMMSettingsDescrTable[] =
{
		{"Averaging",1,100,1,SETTINGSTYPE_INT,&userSettings.averages , dummy},
		{"ConvSpeed",0,5,1,SETTINGSTYPE_INT,&userSettings.conversionSpeed, dummy},
		{"Resolution",-6,6,1,SETTINGSTYPE_INT,&userSettings.precisionOffset, dummy },
		{"UDPport",1, 65500,1,SETTINGSTYPE_INT,&userSettings.udpPort, dummy },
		//	{"Exit",0,0,0,SETTINGSTYPE_NONE,NULL ,MenuSettings::exit},
		{"",0,0,0,SETTINGSTYPE_INT,NULL , dummy} // last
};



MenuSetttingsDesrc_t mainMenuDescrTable[] =
{
		{"DMM settings",0,0,0,SETTINGSTYPE_NONE,NULL , toDMMsettings},
		{"Wifi settings",0,0,0,SETTINGSTYPE_NONE,NULL , toWifiSettings},
		{"Exit",0,0,0,SETTINGSTYPE_NONE,NULL , MenuSettings::exit},

		{"",0,0,0,SETTINGSTYPE_INT,NULL , dummy} // last
};


extern lv_indev_t *kb_indev;
static uint32_t key;

bool keyboard_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);

char* MenuSettings::printItem(int item) {
	static char line[MAXCHARSPERLINE + 1];
	char value[MAXCHARSPERLINE + 1];

	if (table[item].type == SETTINGSTYPE_NONE)
		sprintf(value, " ");
	else {
		void *x = table[item].var;
		int v = *(int*) x;
		snprintf(value, MAXCHARSPERLINE, " %d", v);
	}
	setNameAndValue((char*) table[item].name, value, line, MAXCHARSPERLINE);
	return line;
}

//v_obj_t * MenuSettings::list1;
void MenuSettings::nextItem(void) {
	if (lv_list_get_btn_index(list1, selected_btn) < (nrItems - 1)) {
		selected_btn = lv_list_get_next_btn(list1, selected_btn);
		lv_list_focus_btn(list1, selected_btn);
		selected_btnIdx[level] = lv_list_get_btn_index(list1, selected_btn);
	}
}

void MenuSettings::prevItem(void) {
	if (lv_list_get_btn_index(list1, selected_btn) > 0) {
		selected_btn = lv_list_get_prev_btn(list1, selected_btn); // getnext  button
		lv_list_focus_btn(list1, selected_btn);
		selected_btnIdx[level] = lv_list_get_btn_index(list1, selected_btn);
	}
}

void MenuSettings::decrItem(lv_obj_t *obj) {

	lv_obj_t *button = lv_list_get_btn_selected(list1);
	intptr_t x = (intptr_t) button->user_data;
	int idx = (int) x;
	if (table[idx].var != NULL) {
		int *p = (int*) table[idx].var;
		if (*p > table[idx].minVal) {
			(*p)--;
			changed = true;
			lv_obj_t *label = lv_obj_get_child(button, NULL);
			lv_label_set_text(label, printItem(idx));
		}
	}
}

void MenuSettings::incrItem(lv_obj_t *obj) {
	lv_obj_t *button = lv_list_get_btn_selected(list1);
	intptr_t x = (intptr_t) button->user_data;
	int idx = (int) x;
	if (table[idx].var != NULL) {
		int *p = (int*) table[idx].var;
		if (*p < table[idx].maxVal) {
			(*p)++;
			changed = true;
			lv_obj_t *label = lv_obj_get_child(selected_btn, NULL);
			lv_label_set_text(label, printItem(idx));
		}
	}
}

void MenuSettings::event_handler(lv_obj_t *obj, lv_event_t event) {

	if (event == LV_EVENT_KEY) {

		key = lv_indev_get_key(kb_indev);
		if (key) {
			printf("Key: %s  k %d  e %d\n", lv_list_get_btn_text(obj), key, event);
			switch (key) {
			case LV_KEY_DOWN:
				nextItem();
				break;
			case LV_KEY_UP:
				prevItem();
				break;
			case LV_KEY_LEFT:
				decrItem(obj);
				break;
			case LV_KEY_RIGHT:
				incrItem(obj);
				break;
			case LV_KEY_ENTER:
				active = false;
				//			{
				//				lv_obj_t * button =  lv_list_get_btn_selected(list1);
				//				if ( button != NULL) {
				//					intptr_t x = (intptr_t) button->user_data;
				//					int idx = (int) x;
				//					if (table[idx].fun_ptr != NULL){
				//						table[idx].fun_ptr(NULL);
				//					}
				//				}
				//			}
				break;

			default:
				break;
			}
		}
	}
}

MenuSettings::MenuSettings() {
	lv_style_init(&menuStyle);

	lv_style_set_bg_color(&menuStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_bg_color(&menuStyle, LV_STATE_FOCUSED, LV_COLOR_YELLOW);

	lv_style_set_text_color(&menuStyle, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_text_letter_space(&menuStyle, LV_STATE_DEFAULT, 1);
	lv_style_set_text_font(&menuStyle, LV_STATE_DEFAULT, FONT);

	lv_style_set_radius(&menuStyle, LV_STATE_DEFAULT, 0);
	//	lv_style_set_bg_opa(&menuStyle, LV_STATE_DEFAULT, LV_OPA_COVER);

	lv_style_set_border_width(&menuStyle, LV_STATE_DEFAULT, 0);

	lv_style_set_pad_top(&menuStyle, LV_STATE_DEFAULT, PADDING);
	lv_style_set_pad_bottom(&menuStyle, LV_STATE_DEFAULT, PADDING);
	lv_style_set_pad_left(&menuStyle, LV_STATE_DEFAULT, PADDING + 2);
	lv_style_set_pad_right(&menuStyle, LV_STATE_DEFAULT, PADDING);

	screen = lv_obj_create(NULL, NULL);

	/*Create a list*/
	list1 = lv_list_create(screen, NULL);
	lv_obj_set_size(list1, LV_HOR_RES_MAX, LV_VER_RES_MAX);
	lv_obj_align(list1, NULL, LV_ALIGN_CENTER, 0, 0);

	g = lv_group_create();

}

void MenuSettings::show(MenuSetttingsDesrc_t *menuSettings) {
	int idx = 0;
	char *p;
	nrItems = 0;
	lv_group_remove_all_objs(g);
	lv_list_clean(list1);
	table = menuSettings;
	lv_indev_wait_release(kb_indev);  // prevent from stepping back to first line!

	do {
		lv_obj_t *list_btn;
		lv_obj_t *label;
		p = (char*) table[idx].name;
		if (strlen(p) > 0) {
			list_btn = lv_list_add_btn(list1, NULL, NULL);
			lv_obj_set_user_data(list_btn, (lv_obj_user_data_t) nrItems);
			lv_obj_set_event_cb(list_btn, event_handler);
			lv_group_add_obj(g, list_btn);

			label = lv_label_create(list_btn, NULL);
			lv_label_set_text(label, printItem(nrItems));
			lv_obj_add_style(list_btn, LV_OBJ_PART_MAIN, &menuStyle);
			nrItems++;
		}
		idx++;
	} while (strlen(p) > 0);

	selected_btn =  lv_list_get_next_btn(list1, NULL); // select first button
	lv_obj_clear_state( selected_btn ,LV_STATE_FOCUSED);

	// find previous selected button
	while (selected_btnIdx[level] != lv_list_get_btn_index(list1, selected_btn) && idx-- > 0)
		selected_btn =  lv_list_get_next_btn(list1, selected_btn);

	lv_list_focus_btn(list1, selected_btn);
	lv_scr_load(screen);
	active = true;
	lv_indev_set_group(kb_indev, g);

}

MenuSettings::~MenuSettings() {
	// TODO Auto-generated destructor stub
}

