

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
#include "toggle_widget.h"
#include "spacer_widget.h"

using namespace std;

//--------------------------------------------------------

class lfo_gui
{
	public:

	lfo_gui(int,string,string);
	virtual ~lfo_gui();
	void set_controller(LV2UI_Controller,LV2UI_Write_Function);

	Gtk::HBox* tab;

	toggle* gui_retrig;
	knob* gui_speed;
	fader* gui_dco1_pitch;
	fader* gui_dco2_pitch;
	fader* gui_dco3_pitch;
	fader* gui_cutoff;
	fader* gui_route1;
	fader* gui_route2;
	wave_widget* gui_wave;


	private:

};



