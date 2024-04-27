/*
 * SettingsItem.h
 *
 *  Created on: Mar 7, 2021
 *      Author: dig
 */

#ifndef SETTINGS_MENUSETTINGSITEM_H_
#define SETTINGS_MENUSETTINGSITEM_H_

#include "StringHandler.h"

#ifdef LV_CONF_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif


#include <list>

typedef enum { SETTINGSTYPE_INT, SETTINGSTYPE_FLOAT, SETTINGSTYPE_NONE } settingsTtype_t;
#define MAXLEVELS	 5
//#define MAXITEMS	10

typedef struct {
	const char * name;
	int minVal;
	int maxVal;
	float step;
	settingsTtype_t type;
	void * var;
	void *(*fun_ptr)(void *);
}MenuSetttingsDesrc_t;



class MenuSettings : public StringHandler {
public:
	MenuSettings();
    static lv_obj_t * list1;
	virtual ~MenuSettings();
	void show (MenuSetttingsDesrc_t * );
	static void * exit ( void * arg);
	static volatile bool active;
private:
	lv_obj_t * lastButton;
//	std::list<lv_obj_t *> buttonList;

	static void event_handler(lv_obj_t * obj, lv_event_t event);
	static void nextItem( void);
	static void prevItem( void);
	static void decrItem(lv_obj_t * obj );
	static void incrItem(lv_obj_t * obj );
	static char * printItem ( int item );
	static int nrItems;
	static int selected_btnIdx[MAXLEVELS]; // rememberes last selected button
	static lv_obj_t * selected_btn;
	static MenuSetttingsDesrc_t * table;
	static bool changed;

};

#endif /* SETTINGS_MENUSETTINGSITEM_H_ */
