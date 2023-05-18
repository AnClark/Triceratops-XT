

#ifndef PRESETCAT
#define PRESETCAT

#include <cairomm/context.h>

#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <gtkmm/drawingarea.h>
#include <iostream>
#include <string>
#include <sstream>

// LV2UI stuff
#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"

#include "dco_gui.h"
#include "lfo_gui.h"
#include "unison_gui.h"
#include "adsr_gui.h"
#include "adsr_lfo_gui.h"
#include "amp_gui.h"
#include "echo_gui.h"
#include "reverb_gui.h"
#include "modifier_gui.h"
#include "widget_button.h"

using namespace std;


// LV2UI stuff
#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"

class presets : public Gtk::DrawingArea
{
	public:

	vector<string>preset_file_names;

	typedef struct
	{
		string name;
		string directory;
		
	} preset;

	typedef struct
	{
		string name;
		vector <preset> presets;
		
	} category;

	vector <category> categories;

	// POINTERS TO TRICERATOPS GUI OBJECTS

	dco_gui* dco1;
	dco_gui* dco2;
	dco_gui* dco3;
	unison_gui* unison;

	lfo_gui* lfo1;
	lfo_gui* lfo2;
	lfo_gui* lfo3;

	adsr_gui* adsr_amp;
	adsr_gui* adsr_filter;
	adsr_lfo_gui* adsr_lfo;

	amp_gui* amp_and_filter;
	echo_gui* echo;
	reverb_gui* reverb;
	modifier_gui* modifier;
    
	widget_button* button_go_back;
	widget_button* button_save_preset;

	int port_number;

	// LV2 COMMUNICATION STUFF

	LV2UI_Controller controller;
	LV2UI_Write_Function write_function;

	// VARIABLES & VECTORS

	Gdk::Color top_colour;
	Gdk::Color bottom_colour;

	int current_preset_number;
	int current_category;
	int preset_scroll_offset;
	int old_category;

	string bundle_path;

	enum
	{
		PRESET_TYPE_NULL,
		PRESET_TYPE_CATEGORY,
		PRESET_TYPE_PRESET	
	};

	typedef struct
	{
		int type;	// applies to both presets & categories
		int category;	// applies only to presets
		string name;	// applies to voth presets & categories
		string dir;	// applies only to presets
		bool unfold;	// applies only to categories
	} presets_object;

	vector<presets_object>preset_list;

	static bool sortByName(const presets_object &lhs, const presets_object &rhs) { return lhs.name < rhs.name; }

	// FUNCTIONS USED IN THIS CLASS

	presets(const std::string& bundle_path_in);
	~presets();
	void position_top(bool);
	int pos_mode;
	void set_value(int);
	void set_gui_widget(int,float);
	void init();
	void load_preset(int);
	void create_new_preset(string);
	int get_symbol_port(string);

  protected:

	int val;
	bool hover_create_new_preset;
	string text;    
	stringstream text_stream; 
	vector<int>display_list;
    	float grad_top_colour;
    	float grad_bottom_colour;
	int number_of_categories;

	enum
	{
		gui_status_category,
		gui_status_presets	
	};

	int gui_status;

	void get_preset_names();

	virtual bool on_expose_event(GdkEventExpose*);
	virtual bool on_button_press_event(GdkEventButton*);
	virtual bool on_scroll_event(GdkEventScroll*);
	virtual bool on_button_release_event(GdkEventButton*);
	virtual bool on_key_press_event(GdkEventKey* eventData); 

	bool read_category_file();
};

#endif



