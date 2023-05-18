
#include "dco_gui.h"

//-------------------------------------------------------------------

dco_gui::dco_gui(int port_number,string top_colour, string bottom_colour)
{
	tab = new Gtk::HBox();

	gui_active = new toggle();
	gui_active->top_colour.set(top_colour);
	gui_active->bottom_colour.set(bottom_colour);
	gui_active->set_label("DCO 1");
	gui_active->pos_mode = 1;
	gui_active->port_number = port_number;
	++port_number;

	gui_volume = new knob();
	gui_volume->top_colour.set(top_colour);
	gui_volume->bottom_colour.set(bottom_colour);
	gui_volume->set_label("VOL");
	gui_volume->port_number = port_number;
	gui_volume->pos_mode = 2;
	++port_number;

	gui_pulsewidth = new knob();
	gui_pulsewidth->top_colour.set(top_colour);
	gui_pulsewidth->bottom_colour.set(bottom_colour);
	gui_pulsewidth->set_label("PW");
	gui_pulsewidth->set_min(-.5);
	gui_pulsewidth->set_max(0.5);
	gui_pulsewidth->set_value(0);
	gui_pulsewidth->pos_mode = 2;
	gui_pulsewidth->port_number = port_number;
	++port_number;

	gui_wave = new wave_widget();
	gui_wave->top_colour.set(top_colour);
	gui_wave->bottom_colour.set(bottom_colour);
	gui_wave->set_label("WAVE");
	gui_wave->port_number = port_number;
	gui_wave->last_waveform = 3;
	++port_number;

	gui_octave = new knob();
	gui_octave->top_colour.set(top_colour);
	gui_octave->bottom_colour.set(bottom_colour);
	gui_octave->set_label("OCTAVE");
	gui_octave->set_min(-5);
	gui_octave->set_max(5);
	gui_octave->set_value(0);
	gui_octave->set_snap(true);
	gui_octave->pos_mode = 1;
	gui_octave->scroll_wheel_speed = 1;
	gui_octave->port_number = port_number;
	++port_number;

	gui_detune = new fader();
	gui_detune->top_colour.set(top_colour);
	gui_detune->bottom_colour.set(bottom_colour);
	gui_detune->set_label("DETUNE");
	gui_detune->set_min(-7);
	gui_detune->set_max(7);
	gui_detune->set_value(0);
	gui_detune->port_number = port_number;
	++port_number;

	gui_detune_centre = new toggle();
	gui_detune_centre->top_colour.set(top_colour);
	gui_detune_centre->bottom_colour.set(bottom_colour);
	gui_detune_centre->set_label("CENTRE");
	gui_detune_centre->port_number = port_number;
	gui_detune_centre->pos_mode = 1;
	++port_number;

	gui_inertia = new knob();
	gui_inertia->top_colour.set(top_colour);
	gui_inertia->bottom_colour.set(bottom_colour);
	gui_inertia->set_label("INERTIA");
	gui_inertia->set_min(0);
	gui_inertia->set_max(1);
	gui_inertia->set_value(0);
	gui_inertia->pos_mode = 2;
	gui_inertia->port_number = port_number;
	++port_number;

	gui_pan = new knob();
	gui_pan->top_colour.set(top_colour);
	gui_pan->bottom_colour.set(bottom_colour);
	gui_pan->set_label("PAN");
	gui_pan->set_min(0);
	gui_pan->set_max(1);
	gui_pan->set_value(0.5);
	gui_pan->pos_mode = 2;
	gui_pan->port_number = port_number;
	gui_pan->pos_mode = 2;
	++port_number;

	Gtk::VBox* vbox1 = new Gtk::VBox();
	vbox1->add(*gui_active);
	vbox1->add(*gui_volume);
	gui_volume->set_value(1);

	tab->add(*vbox1);

	spacer* spacer1 = new spacer();
	spacer1->top_colour.set(top_colour);
	spacer1->bottom_colour.set(bottom_colour);
	spacer1->pos_mode = 1;

	Gtk::VBox* vbox4 = new Gtk::VBox();
	vbox4->add(*spacer1);
	vbox4->add(*gui_pan);
	tab->add(*vbox4);

	tab->add(*gui_wave);

	tab->add(*gui_detune);

	Gtk::VBox* vbox2 = new Gtk::VBox();
	vbox2->add(*gui_detune_centre);
	vbox2->add(*gui_inertia);

	tab->add(*vbox2);

	Gtk::VBox* vbox3 = new Gtk::VBox();
	vbox3->add(*gui_octave);
	vbox3->add(*gui_pulsewidth);

	tab->add(*vbox3);



}

//-------------------------------------------------------------------

dco_gui::~dco_gui()
{

}

//-------------------------------------------------------------------

void dco_gui::set_controller(LV2UI_Controller controller, LV2UI_Write_Function write_function)
{
	gui_active->controller = controller;
	gui_active->write_function = write_function;

	gui_volume->controller = controller;
	gui_volume->write_function = write_function;

	gui_pulsewidth->controller = controller;
	gui_pulsewidth->write_function = write_function;

	gui_wave->controller = controller;
	gui_wave->write_function = write_function;

	gui_octave->controller = controller;
	gui_octave->write_function = write_function;

	gui_detune->controller = controller;
	gui_detune->write_function = write_function;

	gui_detune_centre->controller = controller;
	gui_detune_centre->write_function = write_function;

	gui_inertia->controller = controller;
	gui_inertia->write_function = write_function;

	gui_pan->controller = controller;
	gui_pan->write_function = write_function;

}

