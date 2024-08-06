#include "player.h"


Player::Player(
	std::vector<unsigned char> player_inner_circle_colors_,
	std::vector<unsigned char> player_color_,
	int player_inner_circle_size_,
	int player_size_,
	int rot_speed_,
	double angle_,
	std::vector<int> center_coordinates_
) :
	BaseObject::BaseObject(),
	player_inner_circle_color{ 0 },
	player_color{ 0 },
	player_inner_circle_size{ player_inner_circle_size_ },
	player_size(player_size_),
	rot_speed(rot_speed_),
	angle{ angle_ },
	center_coordinates{ center_coordinates_ }
{
	this->player_inner_circle_color = Player::convert_color(player_inner_circle_colors_[0], player_inner_circle_colors_[2], player_inner_circle_colors_[2]);
	this->player_color = Player::convert_color(player_color_[0], player_color_[1], player_color_[2]);
}
void Player::update(double dt) {
	this->angle += this->rot_speed * dt;
}
void Player::reverse() {
	this->rot_speed *= -1;
}
std::vector<int> Player::ret_center_position() {
	return this->center_coordinates;
}
int Player::ret_player_size() {
	return this->player_size;
}
uint32_t Player::ret_color() {
	return this->player_color;
}
void Player::set_inner_color(uint32_t color) {
	this->player_inner_circle_color = color;
}
std::vector<std::vector<int>> Player::ret_circles_coords() {
	double angle_ = this->angle * M_PI / 180.;
	std::vector<std::vector<int>> out = {
		{
			int(this->player_inner_circle_size * cos(this->angle)) + this->center_coordinates[0],
			int(this->player_inner_circle_size * sin(this->angle)) + this->center_coordinates[1]
		},
		{
			int(-this->player_inner_circle_size * cos(this->angle)) + this->center_coordinates[0],
			int(-this->player_inner_circle_size * sin(this->angle)) + this->center_coordinates[1]
		}
	};
	return out;
}
void Player::draw_self()
{
	int h = this->center_coordinates[0];
	int w = this->center_coordinates[1];

	draw_filled_circle(this->center_coordinates, this->player_inner_circle_size, this->player_inner_circle_color);
	// fill_shape(this->center_coordinates, this->player_inner_circle_color);

	std::vector<std::vector<int>> centers = this->ret_circles_coords();

	draw_filled_circle(centers[0], this->player_size, this->player_color);
	draw_filled_circle(centers[1], this->player_size, this->player_color);

	// fill_shape(centers[0], this->player_color);
	// fill_shape(centers[1], this->player_color);
}