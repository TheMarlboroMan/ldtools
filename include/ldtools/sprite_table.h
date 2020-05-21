#pragma once

//LibDanSDL2 deps.
#include <ldv/rect.h>

//Tools deps.
#include <tools/text_reader.h>

#include <fstream>
#include <map>
#include <vector>

namespace ldtools {

//!A frame for the sprite table consisting of the size and position of the
//!sprite plus a possible displacement pair.

struct sprite_frame {
	public:

	int             x,	//!< X position in the sheet.
                    y;	//!< Y position in the sheet.
	unsigned int    w, 	//!< Width.
                    h;	//!< Height.
	int             disp_x, //!< Horizontal displacement.
                    disp_y;	//!< Vertical displacement.

	//!Returns a libdansdl2 rect from the sprite.
	ldv::rect       get_rect() const {return ldv::rect{x, y, w, h};}

	//!Can be used to check that the sprite has been loaded. Will discard
	//!unitialized sprites.
	explicit operator bool() const {return x || y || w || h || disp_x || disp_y;}
};

//!Alpha and resource agnostic sprite table: contains a collection of sprite
//!frames loaded from a file with a specific format.

//!The format used uses # to comment lines and whitespace to separate values.
//!# X	Y	W	H	DESPX	DESPY
//!# Standing.
//!0	0	22	32	0	0

class sprite_table {
	public:

	//!Initializes the table with the file at the given path. Will throw
	//!std::runtime error if the file cannot be found or has an invalid
	//!format.
	                        sprite_table(const std::string&);

	//!Default constructor, builds an empty sprite table.
	                        sprite_table();

	//!Adds a frame. Throws if the index is already used.
	sprite_table&           add(size_t, const sprite_frame&);

	//!Removes the frame from the given index.
	sprite_table&           erase(size_t);

	//!Loads/reloads the table with the given file path. Will throw with
	//!std::runtime_error if the file cannot be found or has an invalid
	//!format.On failure, the data is guaranteed to be empty.
	sprite_table&           load(const std::string&);

	//!Returns the frame at the given index.
	const sprite_frame&     at(size_t) const;

	//!Returns the frame at the given index.
	sprite_frame            get(size_t) const;

	//!Returns the size of the table.
	size_t                  size() const {return data.size();}

	private:

	std::map<size_t, sprite_frame>  data;	//!< Internal data storage.

};

}
