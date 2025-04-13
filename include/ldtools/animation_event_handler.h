#pragma once

#include <functional>

namespace ldtools {

/**
*Monitors an animation for the ocurrence of a specific frame. The idea here
*is to instantiate as many of these as needed to monitor the animations of
*specific entities. The use case looks like:
* - instantiate this with the callback, a function that will get the animation
*   and frame indexes.
* - when a frame is chosen for an entity call "update" with the animation and frame.
* - the callback will fire each time the frame / animation changes.
*/
class animation_event_handler {

	public:

	using                       cb=std::function<void (std::size_t, std::size_t)>;

	//! Sets the callback.
	void                        set(cb);
	//! Must be called each time the animation or frame changes.
	void                        update(std::size_t, std::size_t);

	private:

	cb                          callback;
	std::size_t                 animation{0},
	                            frame{0};
};
}
