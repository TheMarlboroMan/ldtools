#include "../../include/ldtools/sprite_table.h"

#include <iostream>
#include <stdexcept>

bool check_frame(
	const ldtools::sprite_frame _f,
	int _x,
	int _y,
	unsigned _w, 
	unsigned _h, 
	int _dx, 
	int _dy
);

int main(int, char **) {

	try {
		//Create an empty sprite table...
		ldtools::sprite_table table{};

		//Assert that the table is empty.
		if(0!=table.size()) {
			throw std::runtime_error("failed to assert that the table is empty");
		}

		//Assert that regular get will return an empty sprite.
		//Tests also the bool operator for the sprite.
		auto sprite=table.get(0);
		if(sprite) {

			throw std::runtime_error("failed to assert that default frame is empty");
		}

		const std::string errsentry{"error"};

		//Assert that reference get will throw.
		try {
			const ldtools::sprite_frame& ref_sprite=table.at(1);
			std::cout<<ref_sprite.x<<std::endl; //Silence the compiler warning....
			throw std::runtime_error(errsentry);
		}
		catch(std::exception& e) {

			if(e.what() == errsentry) {
				throw std::runtime_error("failed to assert that 'at' not work with invalid indexes");
			}
		}

		//Assert error when loading.
		try {
			table.load("no_file");
			throw std::runtime_error(errsentry);
		}
		catch(std::exception& e) {
		
			if(e.what() == errsentry) {

				throw std::runtime_error("failed to assert that the table cannot be loaded with invalid files");
			}
		}

		//Load error table.
		try {
			table.load("broken_table.txt");
			throw std::runtime_error(errsentry);
		}
		catch(std::exception& e) {
		
			if(e.what() == errsentry) {

				throw std::runtime_error("failed to assert that the invalid table cannot be loaded ");
			}

			if(0!=table.size()) {

				throw std::runtime_error("failed to assert that the table is empty after an invalid load");
			}
		}

		//Load good table.
		table.load("table.txt");

		//Assert size.
		if(5!=table.size()) {
			throw std::runtime_error("failed to assert size after loading table");
		}

		//Assert get.
		sprite=table.get(1);
		if(!check_frame(sprite, 1, 2, 3, 4, 5, 6)) {
			throw std::runtime_error("failed to assert validity of frame 1");
		}

		//Assert reference get
		const auto& refsprite=table.get(2);
		if(!check_frame(refsprite, 7, 8, 9, 10, 11, 12)) {
			throw std::runtime_error("failed to assert validity of frame 2");
		}

		if(!check_frame(table.get(3), 13, 14, 15, 16, 17, 18)) {
			throw std::runtime_error("failed to assert validity of frame 3");
		}

		if(!check_frame(table.get(4), 19, 20, 21, 22, 23, 24)) {
			throw std::runtime_error("failed to assert validity of frame 4");
		}

		//Assert good table values.
		if(!check_frame(table.get(5), 25, 26, 27, 28, 29, 30)) {
			throw std::runtime_error("failed to assert validity of frame 5");
		}

		std::cout<<"all good"<<std::endl;

		return 0;
	}
	catch(std::runtime_error &e) {

		std::cerr<<"error: "<<e.what()<<std::endl;
		return 1;
	}
}

bool check_frame(
	const ldtools::sprite_frame _f,
	int _x,
	int _y,
	unsigned _w, 
	unsigned _h, 
	int _dx, 
	int _dy
) {
	
	return _f.x==_x
		&& _f.y==_y
		&& _f.w==_w
		&& _f.h==_h
		&& _f.disp_x==_dx
		&& _f.disp_y==_dy;
}
