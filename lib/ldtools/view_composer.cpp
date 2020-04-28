#include <ldtools/view_composer.h>

#include <tools/json.h>

#include <ldv/box_representation.h>
#include <ldv/bitmap_representation.h>
#include <ldv/ttf_representation.h>
#include <ldv/polygon_representation.h>

#include <algorithm>

using namespace ldtools;

const char * view_composer::type_key="type";
const char * view_composer::box_key="box";
const char * view_composer::bitmap_key="bitmap";
const char * view_composer::ttf_key="ttf";
const char * view_composer::polygon_key="polygon";
const char * view_composer::screen_key="screen";
const char * view_composer::definition_key="define";
const char * view_composer::definition_key_key="key";
const char * view_composer::definition_key_value="value";
const char * view_composer::alpha_key="alpha";
const char * view_composer::order_key="order";
const char * view_composer::id_key="id";
const char * view_composer::rgba_key="rgba";
const char * view_composer::location_key="location";
const char * view_composer::clip_key="clip";
const char * view_composer::points_key="points";
const char * view_composer::polygon_fill_key="fill";
const char * view_composer::text_key="text";
const char * view_composer::surface_key="surface";
const char * view_composer::font_key="font";
const char * view_composer::texture_key="texture";
const char * view_composer::brush_key="brush";
const char * view_composer::visible_key="visible";
const char * view_composer::external_key="external";
const char * view_composer::external_reference_key="ref";
const char * view_composer::rotation_key="rotate";

//!Default constructor.

view_composer::view_composer()
	:with_screen(false), screen_color{0,0,0,255} {

}

//!Draws the composition to the screen.

void view_composer::draw(ldv::screen& p) {
	if(with_screen)	{
		p.clear(screen_color);
	}

	for(auto& r : data) {
		r.draw(p);
	}
}

//!Draws the composition to the screen using a camera.

void view_composer::draw(ldv::screen& p, const ldv::camera& cam) {
	if(with_screen)	{
		p.clear(screen_color);
	}

	for(auto& r : data) {
		r.draw(p, cam);
	}
}

//!Maps the texture to the given handle.

//!Mapped textures can be referenced in the layout file by their handle. Textures
//!are not copied, so it is required that they exist as long as the layout
//!needs to be drawn. Will throw if the key is used more than once.

void view_composer::map_texture(const std::string& clave, ldv::texture * tex) {
	if(texture_map.count(clave)) {
		throw std::runtime_error("Repeated key for texture map "+clave);
	}

	texture_map[clave]=tex;
}

//!Maps the texture.

void view_composer::map_texture(const std::string& clave, ldv::texture& tex) {
	map_texture(clave, &tex);
}

//!Same as map textures, but with surfaces.

void view_composer::map_surface(const std::string& clave, ldv::surface * sup) {
	if(surface_map.count(clave)) {
		throw std::runtime_error("Repeated key for surface map "+clave);
	}

	surface_map[clave]=sup;
}

//!Same as map textures, but with surfaces.

void view_composer::map_surface(const std::string& clave, ldv::surface& sup) {
	map_surface(clave, &sup);
}

//!Same as map textures, but with ttf_fonts.

void view_composer::map_font(const std::string& clave, const ldv::ttf_font * fuente) {
	if(font_map.count(clave)) {
		throw std::runtime_error("Repeated key for font map "+clave);
	}

	font_map[clave]=fuente;
}

//!Same as map textures, but with ttf_fonts.

void view_composer::map_font(const std::string& clave, const ldv::ttf_font& fuente) {
	map_font(clave, &fuente);
}

//!Returns the representation with the given id.

//!Will throw if there is no representation with that id. The representation
//!is returned as a pointer to its most base type (ldv::representation *)
//!so manipulation of specific characteristics (like textures or texts) need
//!to be done by casting the pointer (static_cast<ldv::bitmap_representation *>)
//!to the necessary type.

ldv::representation * view_composer::get_by_id(const std::string& id) {
	try {
		return id_map.at(id);
	}
	catch(std::exception& e) {
		throw std::runtime_error("Unable to locate element with id "+id+". Is the view mounted?");
	}
}

//!Checks if there exists a representation with the given id.

bool view_composer::id_exists(const std::string& id) const {
	return id_map.count(id);
}

//!Parses the json document.

//!Will if there are malformations in the layout file.
void view_composer::parse(const rapidjson::Value& _root) {

	if(!_root.IsArray()) {

		throw std::runtime_error("root node must be an array");
	}

	for(const auto& token : _root.GetArray()) {

		uptr_rep ptr;
		const auto& tipo=token[type_key].GetString(); //Si no hay tipo vamos a explotar. Correcto.
		int order=0;

		if(tipo==box_key) ptr=std::move(create_box(token));
		else if(tipo==bitmap_key) ptr=std::move(create_bitmap(token));
		else if(tipo==ttf_key) ptr=std::move(create_ttf(token));
		else if(tipo==polygon_key) ptr=std::move(create_polygon(token));
		else if(tipo==external_key) {

			const std::string ref=token[external_reference_key].GetString();
			if(!external_map.count(ref)) {
				throw std::runtime_error("Key for '"+ref+"' has not been externally registered before parsing the file.");
			}

			if(token.HasMember(order_key)) {
				order=token[order_key].GetInt();
			}

			data.push_back(item(external_map[ref], order));
			continue;
		}
		else if(tipo==screen_key) {

			do_screen(token);
			continue;
		}
		else if(tipo==definition_key) {

			do_definition(token);
			continue;
		}
		else {

			throw std::runtime_error(std::string{"Unknown '"}+tipo+"' when parsing view");
		}

		//Tratamiento de cosas comunes...
		if(token.HasMember(order_key))  {

			order=token[order_key].GetInt();
		}

		//Tratamiento de cosas comunes...
		if(token.HasMember(alpha_key)) {

			ptr->set_blend(ldv::representation::blends::alpha);
			ptr->set_alpha((Uint8)token[alpha_key].GetInt());
		}

		if(token.HasMember(rotation_key)) {

			auto values=token[rotation_key].GetArray();
			if(values.Size()!=3) throw std::runtime_error("Rotate needs three parameters");

			auto * rrot=static_cast<ldv::representation *>(ptr.get());
			rrot->set_rotation((int)values[0].GetInt());
			rrot->set_rotation_center((int)values[1].GetInt(), (int)values[2].GetInt());
		}

		if(token.HasMember(visible_key)) {

			ptr->set_visible(token[visible_key].GetBool());
		}

		if(token.HasMember(id_key)) {

			const std::string id=token[id_key].GetString();
			if(id_map.count(id)) {
				throw std::runtime_error(std::string{"Repeated id key '"}+id+"' for view");
			}

			id_map[id]=ptr.get();
		}

		//Y finalmente insertamos.
		data.push_back(item(std::move(ptr), order));
	}

	std::sort(std::begin(data), std::end(data));
}

//!Registers the given representation with the handle.

//!Registered representations can use the order expressed in the layout file.
//!They must be registered before parsing the file, lest a exception is thrown.
//!Will throw if the reference is repeated.

void view_composer::register_as_external(const std::string& clave, ldv::representation& rep) {

	if(external_map.count(clave)) {
		throw std::runtime_error("Repeated key "+clave+" for external representation");
	}

	external_map[clave]=&rep;
}

//!Creates a box from a token. Internal.
view_composer::uptr_rep view_composer::create_box(const rapidjson::Value& token) {

	auto color=rgba_from_list(token[rgba_key]);
	uptr_rep res(new ldv::box_representation(box_from_list(token[location_key]), color, ldv::polygon_representation::type::fill));
	res->set_blend(ldv::representation::blends::alpha);
	return res;
}

//!Creates a polygon from a token. Internal.
view_composer::uptr_rep view_composer::create_polygon(const rapidjson::Value& token) {

	auto color=rgba_from_list(token[rgba_key]);

	ldv::polygon_representation::type t=ldv::polygon_representation::type::fill;
	const std::string sfill=token[polygon_fill_key].GetString();
	if(sfill=="fill")		t=ldv::polygon_representation::type::fill;
	else if(sfill=="line")		t=ldv::polygon_representation::type::line;
	else throw std::runtime_error("Invalid fill type for polygon");

	std::vector<ldv::point>	vp;
	for(const auto& l : token[points_key].GetArray()) {
		vp.push_back({l[0].GetInt(), l[1].GetInt()});
	}

	uptr_rep res(new ldv::polygon_representation(vp, color, t));
	res->set_blend(ldv::representation::blends::alpha);
	return res;
}

//!Creates a bitmap from a token. Internal.
view_composer::uptr_rep view_composer::create_bitmap(const rapidjson::Value& token) {

	const std::string texkey=token[texture_key].GetString();

	if(!texture_map.count(texkey)) {
		throw std::runtime_error(std::string{"Unable to locate texture "}+texkey+" for bitmap");
	}

	uptr_rep res(
		new ldv::bitmap_representation
		(
			*texture_map[texkey],
			box_from_list(token[location_key]),
			box_from_list(token[clip_key])
		)
	);

	if(token.HasMember(brush_key)) {

		const auto& entries=token[brush_key].GetArray();

		static_cast<ldv::bitmap_representation *>(res.get())->set_brush(entries[0].GetInt(), entries[1].GetInt());
	}

	return res;
}

//!Creates a ttf representation from a token. Internal.
view_composer::uptr_rep view_composer::create_ttf(const rapidjson::Value& token) {

	const std::string fontkey=token[font_key].GetString();

	if(!font_map.count(fontkey)) {
		throw std::runtime_error(std::string{"Unable to locate font "}+fontkey+" for ttf");
	}

	uptr_rep res(
		new ldv::ttf_representation(
			*font_map[fontkey],
			rgba_from_list(token[rgba_key]),
			token[text_key].GetString()
		)
	);
	res->set_blend(ldv::representation::blends::alpha);

	auto pos=position_from_list(token[location_key]);
	res->go_to({pos.x, pos.y});
	return res;
}

//!Records screen color fill values. Internal.
void view_composer::do_screen(const rapidjson::Value& token) {

	screen_color=rgba_from_list(token[rgba_key]);
	with_screen=true;
}

//!Records a definition. Internal.
void view_composer::do_definition(const rapidjson::Value& token) {

	const std::string& clave=token[definition_key_key].GetString();

	if(int_definitions.count(clave)) throw std::runtime_error("Repeated definition for "+clave);

	if(token[definition_key_value].IsInt()) int_definitions[clave]=token[definition_key_value].GetInt();
	else if(token[definition_key_value].IsFloat()) float_definitions[clave]=token[definition_key_value].GetFloat();
	else throw std::runtime_error("Invalid data type for definition.");
}

//!Creates a box from a token. Internal.
ldv::rect view_composer::box_from_list(const rapidjson::Value& tok) {

	const auto& values=tok.GetArray();
	int x=values[0].GetInt(), y=values[1].GetInt(), w=values[2].GetInt(), h=values[3].GetInt();
	return ldv::rect{x, y, (unsigned int)w, (unsigned int)h};
}

//!Creates a rgba from a token. Internal.
ldv::rgba_color view_composer::rgba_from_list(const rapidjson::Value& tok) {
	try {
		const auto& values=tok.GetArray();
		int r=values[0].GetInt(), g=values[1].GetInt(), b=values[2].GetInt(), a=values[3].GetInt();
		return ldv::rgba8(r, g, b, a);
	}
	catch(std::exception& e) {
		throw std::runtime_error(std::string("Unable to parse rgba value... did you forget to add the alpha? : ")+e.what());
	}
}

//!Creates a position box from a token. Internal.
view_composer::position view_composer::position_from_list(const rapidjson::Value& tok) {

	const auto& values=tok.GetArray();
	int x=values[0].GetInt(), y=values[1].GetInt();
	return position{x, y};
}

//!Clears all view elements.

//!Representations, id maps and external references are cleared. Definitions
//!are not.
void view_composer::clear_view() {
	data.clear();
	id_map.clear();
	external_map.clear();
}

//!Clears all definitions.
void view_composer::clear_definitions() {
	int_definitions.clear();
	float_definitions.clear();
}

//!Gets a defined integer.

//!Will throw if the definition does not exist.
int view_composer::get_int(const std::string& k) const {
	return get_definition(k, int_definitions);
}

//!Gets a defined float.

//!Will throw if the definition does not exist.
float view_composer::get_float(const std::string& k) const {
	return get_definition(k, float_definitions);
}
