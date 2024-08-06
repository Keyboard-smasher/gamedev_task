#pragma once
#ifndef player.h
#include "base_object.h"
#include "utils.h"

class Player :BaseObject {
	uint32_t player_inner_circle_color;
	uint32_t player_color;
	int player_inner_circle_size;
	int player_size;
	int rot_speed;
	double angle;
	std::vector<int> center_coordinates;

public:
	Player(
		std::vector<unsigned char> player_inner_circle_colors_,
		std::vector<unsigned char> player_color_,
		int player_inner_circle_size_,
		int player_size_,
		int rot_speed_,
		double angle_,
		std::vector<int> center_coordinates_
	);
	void update(double dt);
	void reverse();
	std::vector<int> ret_center_position();
	int ret_player_size();
	uint32_t ret_color();
	void set_inner_color(uint32_t color);
	std::vector<std::vector<int>> ret_circles_coords();
	void draw_self() override;
};
#endif