
#include "amp_gui.h"

//-------------------------------------------------------------------

amp_gui::amp_gui(int port_number,string top_colour, string bottom_colour)
{

	tab = new Gtk::HBox();

	gui_volume = new volume();
	gui_volume->top_colour.set(top_colour);
	gui_volume->bottom_colour.set(bottom_colour);
	gui_volume->set_label("VOLUME");
	gui_volume->set_min(0);
	gui_volume->set_max(2);
	gui_volume->set_value(1.5);
	gui_volume->port_number = port_number;
	++port_number;

	gui_tune = new fader();
	gui_tune ->top_colour.set(top_colour);
	gui_tune ->bottom_colour.set(bottom_colour);
	gui_tune->set_label("TUNE");
	gui_tune->set_min(-7);
	gui_tune->set_max(7);
	gui_tune->set_value(0);
	gui_tune->port_number = port_number;
	++port_number;

	gui_drive = new knob();
	gui_drive->top_colour.set(top_colour);
	gui_drive->bottom_colour.set(bottom_colour);
	gui_drive->set_label("DRIVE");
	gui_drive->port_number = port_number;
	++port_number;

	gui_filter_type = new filter_widget();
	gui_filter_type ->top_colour.set(top_colour);
	gui_filter_type ->bottom_colour.set(bottom_colour);
	gui_filter_type->set_label("TYPE");
	gui_filter_type->set_value(1);
	gui_filter_type->port_number = port_number;
	++port_number;

	gui_cutoff = new fader();
	gui_cutoff->top_colour.set(top_colour);
	gui_cutoff->bottom_colour.set(bottom_colour);
	gui_cutoff->set_label("CUTOFF");
	gui_cutoff->set_min(0.0);
	gui_cutoff->set_max(0.88);
	gui_cutoff->set_value(0);
	gui_cutoff->port_number = port_number;
	gui_cutoff->filter_widget_object = gui_filter_type;
	gui_cutoff->filter_widget_connect = true;
	++port_number;

	gui_resonance = new fader();
	gui_resonance->top_colour.set(top_colour);
	gui_resonance->bottom_colour.set(bottom_colour);
	gui_resonance->set_label("RES");
	gui_resonance->set_min(0);
	gui_resonance->set_max(2);
	gui_resonance->set_value(0);
	gui_resonance->port_number = port_number;
	gui_resonance->filter_widget_object = gui_filter_type;
	gui_resonance->filter_widget_connect = true;
	++port_number;

	gui_key_follow = new fader();
	gui_key_follow->top_colour.set(top_colour);
	gui_key_follow->bottom_colour.set(bottom_colour);
	gui_key_follow->set_label("FOLLOW");
	gui_key_follow->port_number = port_number;
	gui_key_follow->set_min(-1);
	gui_key_follow->set_max(1);
	gui_key_follow->set_value(0);
	++port_number;

	gui_legato = new toggle();
	gui_legato ->top_colour.set(top_colour);
	gui_legato ->bottom_colour.set(bottom_colour);
	gui_legato->set_label("LEGATO");
	gui_legato->pos_mode = 1;
	gui_legato->port_number = port_number;
	++port_number;

	gui_sync = new toggle();
	gui_sync->top_colour.set(top_colour);
	gui_sync->bottom_colour.set(bottom_colour);
	gui_sync->set_label("SYNC");
	gui_sync->pos_mode = 1;
	gui_sync->port_number = port_number;
	++port_number;

	gui_warmth = new toggle();
	gui_warmth->top_colour.set(top_colour);
	gui_warmth->bottom_colour.set(bottom_colour);
	gui_warmth->set_label("WARM");
	gui_warmth->port_number = port_number;
	++port_number;

	gui_fm = new toggle();
	gui_fm->top_colour.set(top_colour);
	gui_fm->bottom_colour.set(bottom_colour);
	gui_fm->set_label("FM");
	gui_fm->pos_mode = 2;
	gui_fm->port_number = port_number;
	++port_number;

	gui_panic = new toggle();
	gui_panic->top_colour.set(top_colour);
	gui_panic->bottom_colour.set(bottom_colour);
	gui_panic->set_label("PANIC");
	gui_panic->port_number = port_number;
	++port_number;

	//---------

	Gtk::VBox* vbox1 = new Gtk::VBox();
	vbox1->add(*gui_legato);
	vbox1->add(*gui_drive);
	gui_drive->pos_mode = 2;
	tab->add(*vbox1);

	tab->add(*gui_filter_type);

	tab->add(*gui_cutoff);
	tab->add(*gui_resonance);

	tab->add(*gui_key_follow);
	tab->add(*gui_tune);

	spacer* spacer1 = new spacer();
	spacer1->set_size_request(40,70);
	spacer1->top_colour.set(top_colour);
	spacer1->bottom_colour.set(bottom_colour);
	tab->add(*spacer1);

	Gtk::VBox* vbox2 = new Gtk::VBox();
	vbox2->add(*gui_sync);
	vbox2->add(*gui_fm);
	tab->add(*vbox2);

	tab->add(*gui_volume);
	
}

//-------------------------------------------------------------------

amp_gui::~amp_gui()
{

}

//-------------------------------------------------------------------

void amp_gui::set_controller(LV2UI_Controller controller, LV2UI_Write_Function write_function)
{
	gui_cutoff->controller = controller;
	gui_cutoff->write_function = write_function;

	gui_resonance->controller = controller;
	gui_resonance->write_function = write_function;

	gui_volume->controller = controller;
	gui_volume->write_function = write_function;

	gui_drive->controller = controller;
	gui_drive->write_function = write_function;

	gui_key_follow->controller = controller;
	gui_key_follow->write_function = write_function;

	gui_tune->controller = controller;
	gui_tune->write_function = write_function;
	
	gui_filter_type->controller = controller;
	gui_filter_type->write_function = write_function;

	gui_legato->controller = controller;
	gui_legato->write_function = write_function;

	gui_sync->controller = controller;
	gui_sync->write_function = write_function;

	gui_legato->controller = controller;
	gui_legato->write_function = write_function;

	gui_fm->controller = controller;
	gui_fm->write_function = write_function;

	gui_panic->controller = controller;
	gui_panic->write_function = write_function;

}

//------------------------------------------------------------------------

void amp_gui::nix_press()
{

}
