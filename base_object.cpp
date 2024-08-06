#include "base_object.h"


uint32_t BaseObject::convert_color(unsigned char R, unsigned char G, unsigned char B) {
	uint32_t color_ = (uint32_t)R * pow(2, 16) + (uint32_t)G * pow(2, 8) + (uint32_t)B;
	return color_;
	}

std::vector<std::vector<double>> BaseObject::get_rotation_matrix(double angle)
{
	angle = angle / 180 * M_PI;
	std::vector<std::vector<double>> rot_matrix = { {cos(angle), -sin(angle)}, {sin(angle), cos(angle)} };
	return rot_matrix;
}
