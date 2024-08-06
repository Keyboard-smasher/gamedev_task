#include "game_master.h"


GameMaster::GameMaster() :
	max_FPS{ 60 },
	pressed{ false },
	inner_timer{ 0 },
	player{ nullptr },
	obstacles(0, nullptr),
	randgen(time(NULL)),
	dead{ false },

	obstacle_friendly_color{ 255, 255, 254 },
	obstacle_enemy_color{ 255, 127, 127 },
	obstacle_linear_size{ 20 },
	obstacle_rot_speed{ 40 },
	obstacle_initial_angle{ 0 },
	obstacle_lin_speed{ 400 },
	obstacle_timer{ 1 },
	prev_spawn_time{ 0 },

	player_inner_circle_color{ 100, 100, 100 },
	player_color{ 255, 255, 255 },
	player_center_coordinates{ SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2 },
	player_inner_circle_size{ 150 },
	player_size{ 30 },
	player_rot_speed{ 4 },
	player_initial_angle{ 0 },

	player_trace(),
	player_trace_decay{ 80 },
	player_trace_cooldown{ 0.1 },
	prev_trace_time{ 0 },

	score{ 0 },
	score_topleft{ 5, int(SCREEN_WIDTH * 0.75) },
	score_blocksize{ 6 },
	death_message_topleft{ int(SCREEN_HEIGHT * 0.4), int(SCREEN_WIDTH * 0.2) },
	death_message_blocksize{ 20 },
	death_message_color{ 0x00FF0000 },
	death_undertext_topleft{ int(SCREEN_HEIGHT * 0.55), int(SCREEN_WIDTH * 0.30) },
	death_undertext_blocksize{ 6 }
{}
void GameMaster::initiate() {
	this->player = new Player(
		this->player_inner_circle_color,
		this->player_color,
		this->player_inner_circle_size,
		this->player_size,
		this->player_rot_speed,
		this->player_initial_angle,
		this->player_center_coordinates
	);
}


Obstacle* GameMaster::generate_obstacle() {
	bool enemy = (this->randgen() % 3) == 0;
	std::vector<unsigned char> color = !enemy ? this->obstacle_friendly_color : this->obstacle_enemy_color;
	std::vector<double> ob_center_coordinates{
		double(this->randgen() % (SCREEN_HEIGHT - 2 * this->obstacle_linear_size) + this->obstacle_linear_size),
		double(-this->obstacle_linear_size)
	};
	std::vector<double> ob_target_coordinates{
		double(this->randgen() % (this->player_inner_circle_size * 2) + (SCREEN_HEIGHT - this->player_inner_circle_size * 2) / 2.),
		double(this->player_center_coordinates[1])
	};
	double thrown_angle = atan((ob_target_coordinates[0] - ob_center_coordinates[0]) / (ob_target_coordinates[1] - ob_center_coordinates[1]));
	std::vector<double> ob_initial_speed{
		this->obstacle_lin_speed * sin(thrown_angle),
		this->obstacle_lin_speed * cos(thrown_angle)
	};
	Obstacle* ob = new Obstacle{
		color,
		ob_center_coordinates,
		enemy,
		this->obstacle_linear_size,
		this->obstacle_rot_speed,
		this->obstacle_initial_angle + int(this->randgen() % 360),
		ob_initial_speed
	};
	return ob;
}
void GameMaster::positive_collision() {
	this->score += 1;
}
void GameMaster::negative_collision() {
	this->dead = true;
}
bool GameMaster::collision(Obstacle* ob) {
	std::vector<std::vector<double>> ob_vert = ob->calculate_position();
	std::vector<double> ob_pos = ob->ret_center_coordinates();
	int ob_size = ob->ret_linear_size();
	std::vector<std::vector<int>> player_coords = this->player->ret_circles_coords();
	int r = this->player->ret_player_size();
	bool check = false;
	for (const auto& player_coord : player_coords) {
		for (const auto& vert : ob_vert) {
			check |= (sqrt(pow(vert[0] - player_coord[0], 2) + pow(vert[1] - player_coord[1], 2)) <= r);
		}
		check |= (sqrt(pow(ob_pos[0] - player_coord[0], 2) + pow(ob_pos[1] - player_coord[1], 2)) <= r + ob_size);
	}
	if (check and !ob->is_enemy())
		this->positive_collision();
	if (check and ob->is_enemy())
		this->negative_collision();

	return check;
}
bool GameMaster::out_of_bounds(Obstacle* ob) {
	std::vector<double> ob_pos = ob->ret_center_coordinates();
	int ob_size = ob->ret_linear_size();
	return (ob_pos[1] > ob_size + SCREEN_WIDTH);
}
void GameMaster::spawn_obstacle() {
	if (this->prev_spawn_time + this->obstacle_timer < this->inner_timer) {
		this->prev_spawn_time = this->inner_timer;
		Obstacle* ob = this->generate_obstacle();
		this->obstacles.push_back(ob);
	}
}
void GameMaster::delete_obstacles() {
	std::stack<int> to_delete;
	for (int i = 0; i < this->obstacles.size(); ++i) {
		bool collide = this->collision(this->obstacles[i]);
		bool out_of_bnds = this->out_of_bounds(this->obstacles[i]);
		if (collide or out_of_bnds) {
			to_delete.push(i);
		}
	}
	while (!to_delete.empty()) {
		int pos = to_delete.top();
		to_delete.pop();
		delete this->obstacles[pos];
		this->obstacles.erase(this->obstacles.begin() + pos);
	}
}
void GameMaster::spawn_trace() {
	if (this->prev_trace_time > this->player_trace_cooldown) {
		this->prev_trace_time = 0;
		std::vector<std::vector<int>> player_coords = this->player->ret_circles_coords();
		double r = double(this->player->ret_player_size());
		this->player_trace.push_back({ player_coords[0][0], player_coords[0][1], r });
		this->player_trace.push_back({ player_coords[1][0], player_coords[1][1], r });
	}
}
void GameMaster::update_trace(double dt) {
	for (int i = 0; i < this->player_trace.size(); ++i) {
		auto tmp = std::get<2>(this->player_trace[i]) - this->player_trace_decay * dt;
		std::get<2>(this->player_trace[i]) = tmp;
	}
}
void GameMaster::delete_trace() {
	while (!this->player_trace.empty() && std::get<2>(this->player_trace.front()) <= 2)
		this->player_trace.pop_front();
}
void GameMaster::draw_trace() {
	for (const auto& tr : this->player_trace) {
		std::vector<int> center = { std::get<0>(tr), std::get<1>(tr) };
		draw_filled_circle(center, int(std::get<2>(tr)), this->player->ret_color());
	}
}
void GameMaster::draw_score() {
	std::string message = "SCORE: " + std::to_string(this->score);
	write_message(this->score_topleft, this->score_blocksize, message, this->player->ret_color());
}
void GameMaster::update(double dt) {
	if (this->dead) return;
	this->prev_trace_time += dt;
	this->inner_timer += dt;
	this->spawn_obstacle();
	this->player->update(dt);
	this->update_trace(dt);
	this->spawn_trace();
	for (Obstacle* ob : this->obstacles) {
		ob->update(dt);
	}
	this->delete_obstacles();
	this->delete_trace();
}
void GameMaster::draw() {
	this->set_background();
	this->player->draw_self();
	for (Obstacle* ob : this->obstacles) {
		ob->draw_self();
	}
	this->draw_trace();
	this->draw_score();
	if (this->dead) {
		this->draw_death_message();
	}
}
void GameMaster::reverse(bool check) {
	if (check && !this->pressed) {
		this->player->reverse();
	}
	this->pressed = check;
}
void GameMaster::free() {
	delete this->player;
	for (Obstacle* ob : this->obstacles) {
		delete ob;
	}
	this->obstacles.clear();
}
void GameMaster::respawn_everything() {
	this->free();
	this->score = 0;
	this->inner_timer = 0;
	this->prev_spawn_time = 0;
	this->prev_trace_time = 0;
	this->initiate();
}
void GameMaster::restore() {
	if (this->dead) {
		this->respawn_everything();
		this->dead = false;
	}
}
void GameMaster::draw_death_message() {
	std::string message = "YOU DIED";
	std::string instruction = "PRESS A TO RESTART";
	write_message(this->death_message_topleft, this->death_message_blocksize, message, this->death_message_color);
	write_message(this->death_undertext_topleft, this->death_undertext_blocksize, instruction, this->death_message_color);
}
void GameMaster::set_background() {
	uint32_t color = this->score < 10 ? 0x0077FF77 : 0x00FFAAFF;
	uint32_t circle_color = this->score < 10 ? 0x0022FF22 : 0x00FF77FF;
	this->player->set_inner_color(circle_color);
	for (int i = 0; i < SCREEN_HEIGHT; ++i) {
		for (int j = 0; j < SCREEN_WIDTH; ++j) {
			buffer[i][j] = color;
		}
	}
}
