

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

class echo_gui
{
	public:

	echo_gui(int,string,string);
	virtual ~echo_gui();
	void set_controller(LV2UI_Controller,LV2UI_Write_Function);

	Gtk::HBox* tab;

	toggle* gui_active;
	knob* gui_speed;
	knob* gui_decay;

	fader* gui_eq_low;
	fader* gui_eq_mid;
	fader* gui_eq_high;




	private:

};



