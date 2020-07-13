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

//!Exception thrown by the sprite_table.

class sprite_table_exception:
	public std::runtime_error {

	public:
	                sprite_table_exception(const std::string& _msg)
		:std::runtime_error(_msg) {

	}
};

//!Alpha and resource agnostic sprite table: contains a collection of sprite
//!frames loaded from a file with a specific format.

//!The format used uses # to comment lines and whitespace to separate values.
//!# X	Y	W	H	DESPX	DESPY
//!# Standing.
//!0	0	22	32	0	0
//!There's a reason it does not provide write capabilities: this class must 
//!only interpret a format and provide read-only access to it. Whatever is used
//!to write the format is something else's responsibility, so no insert, update
//!or delete.

class sprite_table {
	public:

	using container=std::map<size_t, sprite_frame>;
	using iterator=typename container::iterator;
	using const_iterator=typename container::const_iterator;

	//!Initializes the table with the file at the given path. Will throw
	//!std::runtime error if the file cannot be found or has an invalid
	//!format.
	                        sprite_table(const std::string&);

	//!Default constructor, builds an empty sprite table.
	                        sprite_table();

	//!Loads/reloads the table with the given file path. Will throw with
	//!std::runtime_error if the file cannot be found or has an invalid
	//!format.On failure, the data is guaranteed to be empty.
	sprite_table&           load(const std::string&);

	//!Returns the frame at the given index. Will throw if the index is invalid.
	const sprite_frame&     get(size_t) const;

	//!Returns true if there's something in the given index.
	bool                    exists(size_t) const;

	//!Returns the size of the table.
	size_t                  size() const {return data.size();}

	//!Implementation of an iterator using the underlying map: the easiest way.
	iterator                begin() {return data.begin();}
	iterator                end() {return data.end();}
	const_iterator          begin() const {return data.begin();}
	const_iterator          end() const {return data.end();}

	private:

	//! Internal data storage.is interpreted in terms of a map to enable skips
	//! in the indexes content (such as frames 0-60 being scenery, 100-140 
	//! items...).

	container  data;

};

}
