

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

class modifier_gui
{
	public:

	modifier_gui(int,string,string);
	virtual ~modifier_gui();
	void set_controller(LV2UI_Controller,LV2UI_Write_Function);
 
	Gtk::HBox* tab;

	fader* gui_modifier_dirt;
	toggle* gui_modifier_stereo_mode;
	toggle* gui_modifier_ring;
	fader* gui_pitch_bend_range;
	fader* gui_midi_channel;

	private:

};



