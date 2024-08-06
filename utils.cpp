#include "utils.h"


std::vector<std::vector<double>> dot(const std::vector<std::vector<double>>& v1, const std::vector<std::vector<double>>& v2)
{
	if (v1[0].size() != v2.size())
		throw std::exception("Incomparible matrices");
	int m = v1.size();
	int n = v2.size();
	int k = v2[0].size();

	std::vector<std::vector<double>> out(m, std::vector<double>(k));
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < k; ++j) {
			for (int p = 0; p < n; ++p) {

				out[i][j] += v1[i][p] * v2[p][j];
			}
		}
	}
	return out;
}


void draw_line(std::vector<double> v1, std::vector<double> v2, uint32_t color)
{
	double y1 = v1[0];
	double x1 = v1[1];
	double y2 = v2[0];
	double x2 = v2[1];
	
	int len_ = ceil(sqrt(pow(y2 - y1, 2) + pow(x2 - x1, 2)));
	double dy = (y2 - y1) / double(len_);
	double dx = (x2 - x1) / double(len_);

	for (int i = 0; i < len_ + 1; ++i) {
		buffer[int(y1 + i * dy)][int(x1 + i * dx)] = color;
	}
}


void draw_circle(const std::vector<int>& center, int radius, uint32_t color)
{
	int y = center[0];
	int x = center[1];

	int r2 = radius * radius;

	for (int i = - radius; i <= radius; ++i) {
		int j = round(sqrt(r2 - i * i));
		buffer[y - j][x + i] = color;
		buffer[y + j][x + i] = color;
	}
	for (int i = -radius; i <= radius; ++i) {
		int j = round(sqrt(r2 - i * i));
		buffer[y + i][x + j] = color;
		buffer[y + i][x - j] = color;
	}
}


void draw_filled_circle(const std::vector<int>& center, int radius, uint32_t color)
{
	int y = center[0];
	int x = center[1];

	int r2 = radius * radius;

	for (int i = -radius; i <= radius; ++i) {
		for (int j = - radius; j <= radius; ++j) {
			if (i * i + j * j < r2)
				buffer[y + i][x + j] = color;
		}
	}
}


void draw_filled_square(const std::vector<std::vector<double>>& vert, uint32_t color)
{
	double y1 = vert[0][0];
	double x1 = vert[0][1];
	double y2 = vert[1][0];
	double x2 = vert[1][1];
	double y3 = vert[2][0];
	double x3 = vert[2][1];

	int len_ = ceil(sqrt(pow(y1 - y2, 2) + pow(x1 - x2, 2)));
	double dy = (y1 - y2) / double(len_);
	double dx = (x1 - x2) / double(len_);

	int len_ort = ceil(sqrt(pow(y3 - y2, 2) + pow(x3 - x2, 2)));
	double dy_ort = (y3 - y2) / double(len_ort);
	double dx_ort = (x3 - x2) / double(len_ort);

	for (int i = 0; i < len_ + 1; ++i) {
		for (int j = 0; j < len_ort + 1; ++j) {
			int new_x = int(x2 + i * dx + j * dx_ort);
			int new_y = int(y2 + i * dy + j * dy_ort);
			if (0 <= new_x && new_x < SCREEN_WIDTH && 0 <= new_y && new_y < SCREEN_HEIGHT) {
				buffer[new_y][new_x] = color;
			}
		}
	}
}


void draw_filled_square_v2(const std::vector<std::vector<double>>& vert, uint32_t color)
{
	std::vector<std::vector<double>> vert_T = transpose(vert);

	double min_y = *std::min_element(vert_T[0].begin(), vert_T[0].end());
	double max_y = *std::max_element(vert_T[0].begin(), vert_T[0].end());
	double min_x = *std::min_element(vert_T[1].begin(), vert_T[1].end());
	double max_x = *std::max_element(vert_T[1].begin(), vert_T[1].end());

	int min_i = int(floor(std::max(0., min_y)));
	int max_i = int(ceil(std::min(double(SCREEN_HEIGHT) - 0.1, max_y)));
	int min_j = int(floor(std::max(0., min_x)));
	int max_j = int(ceil(std::min(double(SCREEN_WIDTH) - 0.1, max_x)));

	for (int i = min_i; i < max_i; ++i) {
		for (int j = min_j; j < max_j; ++j) {
			bool check = true;
			for (int k = 0; k < vert.size() - 1; ++k) {
				double p1_y = vert[k + 1][0] - vert[k][0];
				double p1_x = vert[k + 1][1] - vert[k][1];
				double p2_y = vert[k][0] - i;
				double p2_x = vert[k][1] - j;
				check &= ((p1_y * p2_x - p2_y * p1_x) >= 0);
			}
			double p1_y = vert[0][0] - vert[vert.size() - 1][0];
			double p1_x = vert[0][1] - vert[vert.size() - 1][1];
			double p2_y = vert[vert.size() - 1][0] - i;
			double p2_x = vert[vert.size() - 1][1] - j;
			check &= ((p1_y * p2_x - p2_y * p1_x) >= 0);

			if (check)
				buffer[i][j] = color;
		}
	}
}


void add_point(std::queue<std::vector<int>>& points, int y, int x, uint32_t color) {
	if (buffer[y][x] != color) {
		buffer[y][x] = color;
		points.push({ y, x });
	}
}


void fill_shape(std::vector<int> starting_point, uint32_t color)
{
	std::queue<std::vector<int>> points;
	points.push(starting_point);

	while (!points.empty()) {
		std::vector<int> point_tmp = points.front();
		points.pop();
		int y = point_tmp[0];
		int x = point_tmp[1];
		add_point(points, y - 1, x, color);
		add_point(points, y, x - 1, color);
		add_point(points, y, x + 1, color);
		add_point(points, y + 1, x, color);
	}
}

void draw_letter(std::vector<std::vector<unsigned char>>& coords, std::vector<int>& topleft, int blocksize, uint32_t color) {
	int y = topleft[0];
	int x = topleft[1];
	for (int i = 0; i < coords.size(); ++i) {
		for (int j = 0; j < coords[0].size(); ++j) {
			if (coords[i][j] == 0) continue;
			for (int k = 0; k < blocksize; ++k) {
				for (int p = 0; p < blocksize; ++p) {
					buffer[y + blocksize * i + k][x + blocksize * j + p] = color;
				}
			}
		}
	}
}

void write_message(std::vector<int> topleft, int blocksize, std::string message, uint32_t color)
{
	for (char& ch : message) {
		std::vector<std::vector<unsigned char>> coords = letters[ch];
		draw_letter(coords, topleft, blocksize, color);
		topleft[1] += (coords[0].size() + 1) * blocksize;
	}
}
