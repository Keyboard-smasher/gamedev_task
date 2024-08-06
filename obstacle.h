#pragma once
#ifndef obstacle.h
#include "base_object.h"


class Obstacle :BaseObject {
	uint32_t color;
	std::vector<double> center_coordinates;
	bool enemy;
	int linear_size;
	double rot_speed;
	double angle;
	std::vector<double> lin_speed;
public:
	Obstacle(
		std::vector<unsigned char> color_,
		std::vector<double> center_coordinates_,
		bool enemy_,
		int linear_size_,
		double rot_speed_,
		double angle_,
		std::vector<double> lin_speed_
	);
	std::vector<double> ret_center_coordinates();
	int ret_linear_size();
	bool is_enemy();
	void update(double dt);
	void draw_self() override;
	std::vector<std::vector<double>> calculate_position();
};
#endif