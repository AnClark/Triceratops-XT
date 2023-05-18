

#include <string>
#include <iostream>
#include <gtkmm.h>

// include the URI and global data of this plugin
#include "triceratops.hpp"

// these are our custom widget includes
#include "fader_widget.h"
#include "spacer_widget.h"
#include "wave_widget.h"
#include "filter_widget.h"
#include "knob_widget.h"
#include "toggle_widget.h"

using namespace std;

//--------------------------------------------------------

class dco_gui
{
	public:

	dco_gui(int,string,string);
	virtual ~dco_gui();
	void set_controller(LV2UI_Controller,LV2UI_Write_Function);

	Gtk::HBox* tab;

	toggle* gui_active;
	knob* gui_volume;
	knob* gui_pulsewidth;
	wave_widget* gui_wave;
	knob* gui_octave;
	fader* gui_detune;
	toggle* gui_detune_centre;
	knob* gui_inertia;
	knob* gui_pan;

	private:

};



