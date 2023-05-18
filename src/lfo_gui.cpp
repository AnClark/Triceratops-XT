
#include "lfo_gui.h"

//-------------------------------------------------------------------

lfo_gui::lfo_gui (int port_number,string top_colour, string bottom_colour)
{
	tab = new Gtk::HBox();

	gui_retrig = new toggle();
	gui_retrig->top_colour.set(top_colour);
	gui_retrig->bottom_colour.set(bottom_colour);
	gui_retrig->set_label("RETRIG");	
	gui_retrig->port_number = port_number;
	gui_retrig->pos_mode = 1;
	++port_number;

	gui_speed = new knob();
	gui_speed ->top_colour.set(top_colour);
	gui_speed ->bottom_colour.set(bottom_colour);
	gui_speed->set_min(1);
	gui_speed->set_max(600);
	gui_speed->set_value(120);
	gui_speed->set_snap(true);
	gui_speed->set_label("SPEED");
	gui_speed->pos_mode = 2;
	gui_speed->scroll_wheel_speed = 1;
	gui_speed->port_number = port_number;
	++port_number;

	gui_wave = new wave_widget();
	gui_wave->top_colour.set(top_colour);
	gui_wave->bottom_colour.set(bottom_colour);
	gui_wave->set_label("WAVE");
	gui_wave->set_lfo_mode(true);
	gui_wave->port_number = port_number;
	++port_number;

	gui_dco1_pitch = new fader();
	gui_dco1_pitch->top_colour.set(top_colour);
	gui_dco1_pitch->bottom_colour.set(bottom_colour);
	gui_dco1_pitch->set_label("DCO1");
	gui_dco1_pitch->port_number = port_number;
	++port_number;

	gui_dco2_pitch = new fader();
	gui_dco2_pitch->top_colour.set(top_colour);
	gui_dco2_pitch->bottom_colour.set(bottom_colour);
	gui_dco2_pitch->set_label("DCO2");
	gui_dco2_pitch->port_number = port_number;
	++port_number;

	gui_dco3_pitch = new fader();
	gui_dco3_pitch->top_colour.set(top_colour);
	gui_dco3_pitch->bottom_colour.set(bottom_colour);
	gui_dco3_pitch->set_label("DCO3");
	gui_dco3_pitch->port_number = port_number;
	++port_number;

	gui_cutoff = new fader();
	gui_cutoff->top_colour.set(top_colour);
	gui_cutoff->bottom_colour.set(bottom_colour);
	gui_cutoff->set_label("CUTOFF");
	gui_cutoff->set_min(0);
	gui_cutoff->set_max(0.25);
	gui_cutoff->set_value(0);
	gui_cutoff->scroll_wheel_speed = 0.01;
	gui_cutoff->port_number = port_number;
	++port_number;

	gui_route1 = new fader();
	gui_route1->top_colour.set(top_colour);
	gui_route1->bottom_colour.set(bottom_colour);
	gui_route1->set_label("ROUTE 1");
	gui_route1->port_number = port_number;
	++port_number;
	gui_route1->route_port_number = port_number;
	++port_number;

	gui_route2 = new fader();
	gui_route2->top_colour.set(top_colour);
	gui_route2->bottom_colour.set(bottom_colour);
	gui_route2->set_label("ROUTE 2");
	gui_route2->port_number = port_number;
	++port_number;
	gui_route2->route_port_number = port_number;
	++port_number;

	Gtk::VBox* vbox1 = new Gtk::VBox();
	vbox1->add(*gui_retrig);
	vbox1->add(*gui_speed);

	tab->add(*vbox1);

	tab->add(*gui_wave);

	spacer* spacer1 = new spacer();
	spacer1->top_colour.set(top_colour);
	spacer1->bottom_colour.set(bottom_colour);
	spacer1->set_size_request(40,70);
	spacer1->pos_mode = 0;

	tab->add(*gui_dco1_pitch);
	tab->add(*gui_dco2_pitch);
	tab->add(*gui_dco3_pitch);
	tab->add(*spacer1);
	tab->add(*gui_cutoff);
	tab->add(*gui_route1);
	tab->add(*gui_route2);
	
}

//-------------------------------------------------------------------

lfo_gui::~lfo_gui()
{

}

//-------------------------------------------------------------------

void lfo_gui::set_controller(LV2UI_Controller controller, LV2UI_Write_Function write_function)
{
	gui_retrig->controller = controller;
	gui_retrig->write_function = write_function;

	gui_speed->controller = controller;
	gui_speed->write_function = write_function;

	gui_dco1_pitch->controller = controller;
	gui_dco1_pitch->write_function = write_function;

	gui_dco2_pitch->controller = controller;
	gui_dco2_pitch->write_function = write_function;

	gui_dco3_pitch->controller = controller;
	gui_dco3_pitch->write_function = write_function;

	gui_cutoff->controller = controller;
	gui_cutoff->write_function = write_function;

	gui_route1->controller = controller;
	gui_route1->write_function = write_function;

	gui_route2->controller = controller;
	gui_route2->write_function = write_function;

	gui_wave->controller = controller;
	gui_wave->write_function = write_function;

}

