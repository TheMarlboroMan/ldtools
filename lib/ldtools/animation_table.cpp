#include <ldtools/animation_table.h> 

//Tools deps.
#include <tools/string_utils.h>
#include <tools/compatibility_patches.h>

using namespace ldtools;

animation::animation()
	:name(), duration(0.0f) {

}

int animation_line::get_rotation() const {

	int result=0;

	if(flags & degree_rotation_add_90) {

		result+=90;
	}

	if(flags & degree_rotation_add_180) {

		result+=180;
	}

	return result;
}

void animation::adjust_frame_time() {

	duration=0;
	for(auto& l: data) {
		duration+=l.duration;
		l.begin_time=duration;
	}
}

const animation_line& animation::get_for_time(float t) const {

	return get_for_time(t, duration);
}


const animation_line& animation::get_for_time(float t, float total) const {

	if(data.size()==1) return data.at(0);

	float mult=total / duration;
	float transformado=fmod(t, total);
	for(const animation_line& fr : data) {
		if(transformado <= fr.begin_time * mult) {
			return fr;
		}
	}

	return data.at(0);
}

std::size_t animation::index_for_time(
	float _t
) const {

	return index_for_time(_t, duration);
}

std::size_t animation::index_for_time(
	float _t, 
	float _duration
) const {

	if(data.size()==1) return 0;

	std::size_t res{0};
	float mult=_duration / duration;
	float transformado=fmod(_t, _duration);
	for(const animation_line& fr : data) {
		if(transformado <= fr.begin_time * mult) {
			return res;
		}
		++res;
	}

	return 0;
}

animation_table::animation_table(const sprite_table& t)
	:table(t) {
}

animation_table::animation_table(const sprite_table& t, const std::string& ruta)
	:table(t) {
	load(ruta);
}

void animation_table::load(const std::string& ruta) {

	tools::text_reader L(ruta, '#');

	if(!L) {
		throw std::runtime_error(std::string("Unable to locate animation file ")+ruta);
	}
	else {
		std::string linea;
		const char inicio_titulo='*';
		const char inicio_cabecera='!';
		size_t id=0;
		animation animacion;

		auto insertar_anim=[this](animation panimacion, size_t pid) {
			panimacion.adjust_frame_time();
			data[pid]=panimacion;
		};

		try {
			while(true) {
				linea=L.read_line();
				if(L.is_eof()) {
					//Insertar la última animación...
					if(animacion) {
						insertar_anim(animacion, id);
					}
					break;
				}

				const char inicio=linea[0];
				switch(inicio) {
					case inicio_titulo:
						if(animacion) insertar_anim(animacion, id);
						animacion=animation(); //Reset animación...
						animacion.name=linea.substr(1);
					break;
					case inicio_cabecera:
						id=read_header(linea.substr(1));
					break;
					default:
						read_line(linea, animacion);
					break;
				}
			}
		}
		catch(std::exception& e) {
			std::string error=e.what()+std::string(" : line ")+compat::to_string(L.get_line_number())+std::string(" ["+linea+"]. aborting.");
			throw std::runtime_error(error);
		}
	}
}

size_t animation_table::read_header(const std::string& linea) {

	const char separador='\t';
	const std::vector<std::string> valores=tools::explode(linea, separador);
	if(valores.size()==1) {

		int id=std::atoi(valores[0].c_str());
		return id;
	}
	else {
		throw std::runtime_error("Error reading animation header.");
	}
}

void animation_table::read_line(const std::string& linea, animation& animacion) {

	const char separador='\t';
	std::vector<std::string> valores=tools::explode(linea, separador);
	if(valores.size()==2 || valores.size()==3) {

		int duration=std::atoi(valores[0].c_str());
		int indice_frame=std::atoi(valores[1].c_str());
		int flags=0;

		if(3 == valores.size()) {

			flags=std::stoi(valores[2]);
		}

		try {
			const auto& frame=table.get(indice_frame);

			float dur=(float)duration / 1000.f;
			animacion.duration+=dur;
			animacion.data.push_back(animation_line(dur, 0.0f, frame, flags));
			animacion.adjust_frame_time();
		}
		catch(std::out_of_range& e) {
			throw e;
		}
	}
	else {
		throw std::runtime_error("Error reading animation line.");
	}
}
