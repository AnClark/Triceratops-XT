

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

class amp_gui
{
	public:

	amp_gui(int,string,string);
	virtual ~amp_gui();
	void set_controller(LV2UI_Controller,LV2UI_Write_Function);

	Gtk::HBox* tab;

	volume* gui_volume;
	knob* gui_drive;


	fader* gui_cutoff;
	fader* gui_resonance;
	fader* gui_key_follow;
	fader* gui_tune;
	filter_widget* gui_filter_type;

	toggle* gui_legato;
	toggle* gui_sync;
	toggle* gui_warmth;
	toggle* gui_fm;
	toggle* gui_panic;

	void nix_press();

	private:

};



