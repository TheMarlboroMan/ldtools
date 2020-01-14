#pragma once

#include <ldv/ttf_font.h>

#include <map>
#include <string>

namespace ldtools {

/*
Un gestor de data ttf. Almacena una clave de texto o y un entero con una fuente
ttf y un sizeaño. Se trabaja en dos pasos:

- 1: registrar la fuente con registrar_fuente("alias", size, "ruta_ttf");
- 2: obtener la referencia a la fuente con obtener_fuente("alias"; sizeaño);

En caso de no existir la fuente se lanzará un std::runtime_error.
*/

//!Manages TTF fonts for the lidbdansdl2 library through SDL font.

//!Each entry in the manager consists in a key (a composite object containing
//!a string (font identifier) and in integer (font size)) that is matched to
//!the libdansdl2 font data. There are two steps to using the class: first
//!register the font with insert("alias", size, ttf_font path) and then
//!getting it with get("alias", size").

class ttf_manager {
	public:

	//!Returs the stored font with the alias and size. Will throw std::runtime_error if the font is not registered in the given size.
	const ldv::ttf_font&				get(const std::string&, int) const;
	//!Inserts a font with the given alias and size using the path to the ttf file. Returns false if the font was already inserted.
	bool						insert(const std::string&, int, const std::string&);
	//!Returns true if the font with the given alias and size exists.
	bool						exists(const std::string&, int) const;
	//!Erases the fond with the given alias and size. Will throw if the font is not registered.
	void						erase(const std::string&, int);

	private:

	//!This structure is used as the key to retrieve the font from the internal storage.
	struct font_info {
		std::string name;				//!< Font alias 
		int size;					//!< Font size in px.
		//!Comparison operator.... TODO: I honestly don't remember what this does.
		bool operator<(const font_info& o) const
		{
			if(name < o.name) return false;
			else if(name > o.name) return true;
			else return size < o.size;
		}
	};

	std::map<font_info, ldv::ttf_font>	data;		//!< Internal data storage
};

}
