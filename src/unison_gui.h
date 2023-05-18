

#include <string>
#include <iostream>
#include <gtkmm.h>

// include the URI and global data of this plugin
#include "triceratops.hpp"

// these are our custom widget includes
#include "fader_widget.h"
#include "wave_widget.h"
#include "filter_widget.h"
#include "knob_widget.h"
#include "volume_widget.h"
#include "toggle_widget.h"
#include "spacer_widget.h"

using namespace std;

//--------------------------------------------------------

class unison_gui
{
	public:

	unison_gui(int,string,string);
	virtual ~unison_gui();
	void set_controller(LV2UI_Controller,LV2UI_Write_Function);

	Gtk::HBox* tab;

	toggle* gui_active;

	fader* gui_dco1_unison;
	fader* gui_dco2_unison;
	fader* gui_dco3_unison;




	private:

};



