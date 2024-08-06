#include "obstacle.h"


Obstacle::Obstacle(
	std::vector<unsigned char> color_,
	std::vector<double> center_coordinates_,
	bool enemy_,
	int linear_size_,
	double rot_speed_,
	double angle_,
	std::vector<double> lin_speed_
) :
	BaseObject::BaseObject(),
	center_coordinates{ center_coordinates_ },
	enemy{ enemy_ },
	linear_size{ linear_size_ },
	rot_speed{ rot_speed_ },
	angle{ angle_ },
	lin_speed{ lin_speed_ }
{
	this->color = Obstacle::convert_color(color_[0], color_[1], color_[2]);
}
std::vector<double> Obstacle::ret_center_coordinates() {
	return this->center_coordinates;
}
int Obstacle::ret_linear_size() {
	return this->linear_size;
}
bool Obstacle::is_enemy() {
	return this->enemy;
}
void Obstacle::update(double dt) {
	this->center_coordinates[0] += dt * this->lin_speed[0];
	this->center_coordinates[1] += dt * this->lin_speed[1];
	this->angle += rot_speed * dt;
}
std::vector<std::vector<double>> Obstacle::calculate_position() {
	int center_y = int(this->center_coordinates[0]);
	int center_x = int(this->center_coordinates[1]);

	std::vector<std::vector<double>> v = {
		{double(-this->linear_size), double(-this->linear_size)},
		{double(-this->linear_size), double(this->linear_size)},
		{double(this->linear_size), double(this->linear_size)},
		{double(this->linear_size), double(-this->linear_size)}
	};
	v = transpose(v);
	auto rot_matrix = Obstacle::get_rotation_matrix(this->angle);
	v = dot(rot_matrix, v);
	v = transpose(v);
	for (auto& v_ : v) {
		v_[0] += center_y;
		v_[1] += center_x;
	}
	return v;
}


void Obstacle::draw_self()
{

	std::vector<std::vector<double>> v = this->calculate_position();
	draw_filled_square_v2(v, this->color);
	/*
	for (int i = 0; i < v.size() - 1; ++i) {
		draw_line(v[i], v[i + 1], this->color);
	}
	draw_line(v[v.size()-1], v[0], this->color);
	fill_shape(this->center_coordinates, this->color);
	*/
}
