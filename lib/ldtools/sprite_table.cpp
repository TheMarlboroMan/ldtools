#include <ldtools/sprite_table.h>

#include<fstream>
#include<sstream>

using namespace ldtools;
   
int sprite_frame::get_rotation() const {

	int result=0;

	if(flags & degree_rotation_add_90) {

		result+=90;
	}

	if(flags & degree_rotation_add_180) {

		result+=180;
	}

	return result;
}

sprite_table::sprite_table() {

}

sprite_table::sprite_table(const std::string& _path) {

	load(_path);
}

bool sprite_table::exists(size_t _index) const {

	return data.count(_index);
}

const sprite_frame& sprite_table::get(size_t _index) const {

	if(!data.count(_index)) {

		throw sprite_table_exception(std::string{"cannot get invalid sprite index "}+std::to_string(_index));
	}

	return data.at(_index);
}

sprite_table& sprite_table::load(const std::string& _path) {

	std::ifstream input_file(_path);

	if(!input_file){
		throw sprite_table_exception(std::string{"Unable to locate sprite file "}+_path);
	}

	std::stringstream ss{};
	std::string line;
	while(true) {

		std::getline(input_file, line);
		if(input_file.eof()) {
			break;
		}

		if(!line.size()) {
			continue;
		}

		if('#'==line[0]) {
			continue;
		}

		ss.clear();
		ss.str(line);

		sprite_frame f{};
		size_t index;
		ss>>index>>f.box.origin.x>>f.box.origin.y>>f.box.w>>f.box.h>>f.disp_x>>f.disp_y;

		if(ss.fail()) {

			data.clear();
			throw sprite_table_exception(std::string{"Malformed sprite line in "}+_path+" : "+line);
		}

		ss>>f.flags;
		if(ss.fail()) {

			//no sweat, older file format.
		}

		data.insert(std::make_pair(index, f));
	}

	return *this;
}
