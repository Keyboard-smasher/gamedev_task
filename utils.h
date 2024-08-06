#pragma once
#ifndef utils.h
#define _USE_MATH_DEFINES

#include <vector>
#include <iostream>
#include <string>
#include <queue>
#include "Engine.h"
#include <math.h>
#include <map>
#include "score_table.h"


std::vector<std::vector<double>> dot(const std::vector<std::vector<double>>& v1, const std::vector<std::vector<double>>& v2);
void draw_line(std::vector<double> v1, std::vector<double>  v2, uint32_t color);
void draw_circle(const std::vector<int>& center, int radius, uint32_t color);
void draw_filled_circle(const std::vector<int>& center, int radius, uint32_t color);
void draw_filled_square(const std::vector<std::vector<double>>& vert, uint32_t color);
void draw_filled_square_v2(const std::vector<std::vector<double>>& vert, uint32_t color);
void fill_shape(std::vector<int> starting_point, uint32_t color);
void write_message(std::vector<int> topleft, int blocksize, std::string message, uint32_t color);

template <typename T>
std::vector<std::vector<T>> transpose(const std::vector<std::vector<T>>& vec) {
	auto s0 = vec.size();
	auto s1 = vec[0].size();
	std::vector<std::vector<T>> out(s1, std::vector<T>(s0));
	for (int i = 0; i < s0; ++i) {
		for (int j = 0; j < s1; ++j) {
			out[j][i] = vec[i][j];
		}
	}
	return out;
}

#endif