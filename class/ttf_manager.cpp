#include "ttf_manager.h"

#include <stdexcept>

using namespace tools;

const ldv::ttf_font& ttf_manager::get(const std::string& f, int t) const {

	if(!exists(f, t)) {
		throw std::runtime_error("TTF font "+f+" was not registered in the requested size");
	}

	return data.at({f,t});
}

bool ttf_manager::insert(const std::string& f, int t, const std::string& r)
{
	if(!exists(f, t)) {
		data.insert( std::pair<font_info, ldv::ttf_font>({f, t}, ldv::ttf_font(r, t) ) );
		return true;
	}

	return true;
}

bool ttf_manager::exists(const std::string& _fontname, int _fontsize) const {

	return data.count({_fontname, _fontsize});
}

void ttf_manager::erase(const std::string& _fontname, int _fontsize) {

	if(!exists(_fontname, _fontsize)) {
		throw std::runtime_error("ttf font "+f+" could not be erased");
	}

	data.erase({_fontname, _fontsize});
}
