
#include "modifier_gui.h"

//-------------------------------------------------------------------

modifier_gui::modifier_gui(int port_number,string top_colour, string bottom_colour)
{

	tab = new Gtk::HBox();

	gui_modifier_dirt = new fader();
	gui_modifier_dirt ->top_colour.set(top_colour);
	gui_modifier_dirt ->bottom_colour.set(bottom_colour);
	gui_modifier_dirt->set_label("DIRT");
	gui_modifier_dirt->set_min(0);
	gui_modifier_dirt->set_max(1);
	gui_modifier_dirt->set_value(0);
	gui_modifier_dirt->port_number = port_number;
	++port_number;

	gui_modifier_stereo_mode= new toggle();
	gui_modifier_stereo_mode->top_colour.set(top_colour);
	gui_modifier_stereo_mode->bottom_colour.set(bottom_colour);
	gui_modifier_stereo_mode->set_label("STEREO");
	gui_modifier_stereo_mode->set_value(1);
	gui_modifier_stereo_mode->port_number = port_number;
	gui_modifier_stereo_mode->pos_mode = 2;
	++port_number;

	gui_modifier_ring= new toggle();
	gui_modifier_ring->top_colour.set(top_colour);
	gui_modifier_ring->bottom_colour.set(bottom_colour);
	gui_modifier_ring->set_label("RING");
	gui_modifier_ring->set_value(0);
	gui_modifier_ring->port_number = port_number;
	gui_modifier_ring->pos_mode = 1;
	++port_number;

	gui_pitch_bend_range = new fader();
	gui_pitch_bend_range->top_colour.set(top_colour);
	gui_pitch_bend_range->bottom_colour.set(bottom_colour);
	gui_pitch_bend_range->set_label("PB-RANGE");
	gui_pitch_bend_range->set_min(0);
	gui_pitch_bend_range->set_max(24);
	gui_pitch_bend_range->port_number = port_number;
	gui_pitch_bend_range->set_snap(true);
	gui_pitch_bend_range->scroll_wheel_speed = 1;
	++port_number;

	gui_midi_channel = new fader();
	gui_midi_channel->top_colour.set(top_colour);
	gui_midi_channel->bottom_colour.set(bottom_colour);
	gui_midi_channel->set_label("MIDICHAN");
	gui_midi_channel->set_min(1);
	gui_midi_channel->set_max(16);
	gui_midi_channel->port_number = port_number;
	gui_midi_channel->set_snap(true);
	gui_midi_channel->scroll_wheel_speed = 1;
	++port_number;

	spacer* spacer1 = new spacer();
	spacer1 ->top_colour.set(top_colour);
	spacer1 ->bottom_colour.set(bottom_colour);
	spacer1->set_size_request(60,70);

	spacer* spacer2 = new spacer();
	spacer2->top_colour.set(top_colour);
	spacer2->bottom_colour.set(bottom_colour);
	spacer2->set_size_request(200,70);

	spacer* spacer3 = new spacer();
	spacer3->top_colour.set(top_colour);
	spacer3->bottom_colour.set(bottom_colour);
	spacer3->set_size_request(40,70);
	spacer3->pos_mode = 1;

	Gtk::VBox* vbox1 = new Gtk::VBox();
	vbox1->add(*gui_modifier_ring);
	vbox1->add(*gui_modifier_stereo_mode);

	tab->add(*gui_modifier_dirt);
	tab->add(*gui_pitch_bend_range);
	tab->add(*vbox1);
	tab->add(*gui_midi_channel);
	tab->add(*spacer1);
	tab->add(*spacer2);


}

//-------------------------------------------------------------------

modifier_gui::~modifier_gui()
{

}

//-------------------------------------------------------------------

void modifier_gui::set_controller(LV2UI_Controller controller, LV2UI_Write_Function write_function)
{

	gui_modifier_dirt->controller = controller;
	gui_modifier_dirt->write_function = write_function;

	gui_modifier_stereo_mode->controller = controller;
	gui_modifier_stereo_mode->write_function = write_function;

	gui_modifier_ring->controller = controller;
	gui_modifier_ring->write_function = write_function;

	gui_pitch_bend_range->controller = controller;
	gui_pitch_bend_range->write_function = write_function;

	gui_midi_channel->controller = controller;
	gui_midi_channel->write_function = write_function;
}

