#include <ldtools/sprite_table.h>

#include<fstream>
#include<sstream>

using namespace ldtools;

sprite_table::sprite_table() {

}

sprite_table::sprite_table(const std::string& _path) {

	load(_path);
}

const sprite_frame& sprite_table::at(size_t _index) const {

	return data.at(_index);
}

sprite_frame sprite_table::get(size_t _index) const {

	return data.count(_index)
		? data.at(_index) //TODO: Could const-cast out the constness out and use [].
		: sprite_frame();
}

sprite_table& sprite_table::load(const std::string& _path) {

	std::ifstream input_file(_path);

	if(!input_file){
		throw std::runtime_error(std::string("Unable to locate sprite file ")+_path);
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
		ss>>index>>f.x>>f.y>>f.w>>f.h>>f.disp_x>>f.disp_y;

		if(ss.fail()) {

			data.clear();
			throw std::runtime_error(std::string("Malformed sprite line in ")+_path+std::string(" : ")+line);
		}

		add(index, f);
	}

	return *this;
}

sprite_table& sprite_table::add(size_t _index, const sprite_frame& _f) {

	if(data.count(_index)) {

		throw std::runtime_error("frame already exists");
	}

	data[_index]=_f;
	return *this;
}

sprite_table& sprite_table::erase(size_t _index) {

	if(!data.count(_index)) {
	
		throw std::runtime_error("invalid index "+std::to_string(_index)+" for erase");
	}

	data.erase(_index);
	return *this;
}
