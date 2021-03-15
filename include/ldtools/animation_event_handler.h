#pragma once

#include <functional>

namespace ldtools {

class animation_event_handler {

	public:

	using                       cb=std::function<void (std::size_t, std::size_t)>;

	void                        set(cb);
	void                        update(std::size_t, std::size_t);

	private:

	cb                          callback;
	std::size_t                 animation{0},
	                            frame{0};
};
}
