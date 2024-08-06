#pragma once
#include <vector>
#include "obstacle.h"
#include "player.h"
#include <random>
#include <stack>
#include <tuple>
#include <deque>
#include <string>
#include <time.h>


class GameMaster {
	// Inner Settings
	double max_FPS;
	bool pressed;
	double inner_timer;
	Player* player;
	std::vector<Obstacle*> obstacles;
	std::mt19937 randgen;
	bool dead;
	// Obstacle Settings
	std::vector<unsigned char> obstacle_enemy_color;
	std::vector<unsigned char> obstacle_friendly_color;
	int obstacle_linear_size;
	double obstacle_rot_speed;
	double obstacle_initial_angle;
	double obstacle_lin_speed;
	double obstacle_timer;
	double prev_spawn_time;
	// Player Settings
	std::vector<unsigned char> player_inner_circle_color;
	std::vector<unsigned char> player_color;
	std::vector<int> player_center_coordinates;
	int player_inner_circle_size;
	int player_size;
	int player_rot_speed;
	double player_initial_angle;
	// Special Effects
	std::deque<std::tuple<int, int, double>> player_trace;
	double player_trace_decay;
	double player_trace_cooldown;
	double prev_trace_time;
	// Cave Paintings
	int score;
	std::vector<int> score_topleft;
	int score_blocksize;
	std::vector<int>death_message_topleft;
	int death_message_blocksize;
	uint32_t death_message_color;
	std::vector<int>death_undertext_topleft;
	int death_undertext_blocksize;


public:
	GameMaster();
	void initiate();
	Obstacle* generate_obstacle();
	void positive_collision();
	void negative_collision();
	bool collision(Obstacle* ob);
	bool out_of_bounds(Obstacle* ob);
	void spawn_obstacle();
	void delete_obstacles();
	void spawn_trace();
	void update_trace(double dt);
	void delete_trace();
	void draw_trace();
	void draw_score();
	void update(double dt);
	void draw();
	void reverse(bool check);
	void free();
	void respawn_everything();
	void restore();
	void draw_death_message();
	void set_background();
};