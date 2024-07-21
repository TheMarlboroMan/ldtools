#pragma once

//LibdanSDL deps.
#include <ldv/texture.h>
#include <ldv/surface.h>
#include <ldv/screen.h>
#include <ldv/camera.h>
#include <ldv/representation.h>
#include <ldv/ttf_font.h>

//External deps.
#include <rapidjson/document.h>

#include <memory>
#include <map>
#include <string>

namespace ldtools {

//!The view composer creates graphical presentations through configuration files with little coding and compiling.
/**
It works in two stages: a json configuration file which defines the elements
to be drawn and a code snippet to connect textures and fonts to it. Once the
file is done and the resources are connected to the composer, a call to
"parse" with the filename and the view node will mount the view and the call to
"draw" will draw it.

Elements drawn can be internal or external. Internal elements have an id that
can be used in code to manipulate them. External elements exist in code and
are made to correspond with a key in the file.

The structure of the file goes like this:

layout_id:[
	{type:"type_key", attribute: attribute_value},
	{type:"type_key", attribute: attribute_value},
	{type:"type_key", attribute: attribute_value},
	{type:"type_key", attribute: attribute_value}
]

other_layout_id:[
	...
]

External features like textures or fonts need to be linked and given a string
handle through the methods map_texture(handle, texture), map_surface(handle, surface)
or map_font(handle, font). The handle will be used within the file to represent
the resource. These links or "maps" need to be done before "parse" is called!.

In the file, "type" corresponds either to a definition or a representation.
Definitions can be integer or float values that can be used outside the
composer (that is, in the code, through the get_int or get_float values) to
avoid recompilation.

Definitions are expressed like this:

	{type:"define", key:"my_key", value:32}

As for representations, there are common attributes:

	type: expresses the type as a string
	id: string handle used to manipulate the representation from the code
	alpha: integer alpha value. By default, all representations are set to blend_mode alpha.
	order: integer order value. Lower values are drawn sooner.
	visible: boolean value, indicates whether the representation is drawn or not.
	rotate: [30, 40, 40] integer list corresponding to degrees, rotation center x and y.

Different representations and their parameters are

screen:
	rgba:[255,255,255,255] (Screen color fill)
	does not accept the "order" attribute.

ttf:
	location:[16, 16] 	(position)
	text:"this is the text",(text)
	font:"font handle", 	(font handle)
	rgba:[0, 0, 0, 255],	(font color)

external:
	ref:"menu"	(allows a code representation to be included in the view
			and use the "order" attribute. register_as_external
			is used for this purpose). These must be registered
			before loading the file.

bitmap:
	location:[140, 300, 546, 376], 	(position)
	clip:[0, 0, 546, 376],		(texture clip)
	brush:[20,20],			(draw brush size)
	texture:"texture handle"	(texture handle)
	surface:"surface handle"	(surface handle... untested).

box:
	location:[0, 0, 800, 600]		(position)
	rgba:[255,255,255,255]		(color)

polygon:
	points:[[0,0], [10,10], [20,10]]	(points)
	rgba:[255,255,255,255]			(color)
	fill: "fill"|"line"				(type of fill)
*/

class view_composer {
	public:

	typedef std::unique_ptr<ldv::representation> uptr_rep;	//!< Typedef to the internal Reprensetation type.

					view_composer();
	void			parse(const rapidjson::Value&);
/**
*draws the layout upon the screen at its coordinates.
*/
	void			draw(ldv::screen&);
/**
*draws the layout upon the screen at its coordinates taking the camera into 
*account
*/
	void			draw(ldv::screen&, const ldv::camera&);
/**
*draws the layout upon the screen at its coordinates using the given point as
*the origin.
*/
	void			draw(ldv::screen&, ldv::point);
/**
*draws the layout upon the screen at its coordinates taking the camera into 
*account and using the given point as the origin.
*/
	void			draw(ldv::screen&, const ldv::camera&, ldv::point);

	void			map_texture(const std::string&, const ldv::texture *);
	void			map_texture(const std::string&, const ldv::texture&);
	void			map_surface(const std::string&, const ldv::surface *);
	void			map_surface(const std::string&, const ldv::surface&);
	void			map_font(const std::string&, const ldv::ttf_font *);
	void			map_font(const std::string&, const ldv::ttf_font&);
	void			clear_view();
	void			clear_definitions();
/**
 * sets the text for the ttf representation identified by the first parameter.
 * If no representation is found we will just throw.
 */
	void            set_text(const std::string&, const std::string&);
/**
 * changes the visibility of the representation identified by the first
 * parameter. Will throw if no representation is found.
 */
	void            set_visible(const std::string&, bool);
/**
 * changes the alpha value of the representation identified by the first
 * parameter. Will throw if no representation is found.
 */
	void            set_alpha(const std::string&, int);

	//!Empties the representation, allowing for a new call to "parse".
	void			clear()
	{
		clear_view();
		clear_definitions();
	}
	ldv::representation * 	get_by_id(const std::string&);
	bool			id_exists(const std::string&) const;
	int			get_int(const std::string&) const;
	float			get_float(const std::string&) const;
	void			register_as_external(const std::string&, ldv::representation&);

	private:

	//!Internal template helper to get definitions.
	template<typename T>
	T get_definition(const std::string k, const std::map<std::string, T>& map) const {

		try{

			return map.at(k);
		}
		catch(std::exception& e){

			throw std::runtime_error("unable to locate definition of "+k);
		}
	}

	static const char *		type_key;
	static const char *		box_key;
	static const char *		bitmap_key;
	static const char *		text_key;
	static const char *		text_line_height_ratio_key;
	static const char *		ttf_key;
	static const char *		polygon_key;
	static const char *		screen_key;
	static const char *		definition_key;
	static const char *		definition_key_key;
	static const char *		definition_key_value;
	static const char *		order_key;
	static const char *		alpha_key;
	static const char *		id_key;
	static const char *		rgba_key;
	static const char *		location_key;
	static const char *		clip_key;
	static const char *		points_key;
	static const char *		polygon_fill_key;
	static const char *		texture_key;
	static const char *		surface_key;
	static const char *		font_key;
	static const char *		brush_key;
	static const char *		visible_key;
	static const char *		external_key;
	static const char *		external_reference_key;
	static const char *		rotation_key;

	struct position{int x, y;};

	//!Represents a singular drawable.

	struct item {
		uptr_rep			rep;
		ldv::representation *		ptr;
		int 				order;

		item(uptr_rep&& pr, int porder=0)
			:rep(std::move(pr)), ptr(rep.get()), order(porder) {

		}

		item(ldv::representation * p, int porder=0)
			:rep(nullptr), ptr(p), order(porder) {}

		bool operator<(const item& o) const {
			return order < o.order;
		}

		void draw(ldv::screen& p) {
			ptr->draw(p);
		}

		void draw(ldv::screen& p, const ldv::camera& cam) {
			ptr->draw(p, cam);
		}
	};


	uptr_rep		create_box(const rapidjson::Value&);
	uptr_rep		create_bitmap(const rapidjson::Value&);
	uptr_rep		create_ttf(const rapidjson::Value&);
	uptr_rep		create_polygon(const rapidjson::Value&);
	void			do_screen(const rapidjson::Value&);
	void			do_definition(const rapidjson::Value&);

	ldv::rect		box_from_list(const rapidjson::Value&);
	ldv::rgba_color		rgba_from_list(const rapidjson::Value&);
	position		position_from_list(const rapidjson::Value&);

	std::vector<item>				data;
	std::map<std::string, ldv::representation*>	id_map;
	std::map<std::string, ldv::representation*>	external_map;
	std::map<std::string, const ldv::texture*>		texture_map;
	std::map<std::string, const ldv::surface*>		surface_map;
	std::map<std::string, const ldv::ttf_font*>	font_map;
	std::map<std::string, int>			int_definitions;
	std::map<std::string, float>			float_definitions;

	bool 						with_screen;
	ldv::rgba_color				screen_color;

};
}
