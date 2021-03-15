#include <ldtools/animation_event_handler.h> 

#include <stdexcept>

using namespace ldtools;

void animation_event_handler::set(
	animation_event_handler::cb _callback
) {

	callback=_callback;
}

void animation_event_handler::update(
	std::size_t _animation_index, 
	std::size_t _frame_index
) {

	if(!callback) {

		throw std::runtime_error("ldtools::animation_event_handler, callback not set");
	}

	if(_animation_index != animation
		|| _frame_index != frame
	) {

		callback(_animation_index, _frame_index);
		animation=_animation_index;
		frame=_frame_index;
	}
}
