#pragma once
#include <stdint.h>
#include <vector>
#include "utils.h"


class BaseObject {
protected:
	BaseObject() {}
	virtual void draw_self() = 0;
	uint32_t convert_color(unsigned char R, unsigned char G, unsigned char B);
	std::vector<std::vector<double>> get_rotation_matrix(double angle);
};