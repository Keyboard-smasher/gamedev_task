#include "Engine.h"
#include <stdlib.h>
#include <memory.h>
#include "obstacle.h"
#include <time.h>
#include <Windows.h>
#include "game_master.h"

//
//  You are free to modify this file
//

//  is_key_pressed(int button_vk_code) - check if a key is pressed,
//                                       use keycodes (VK_SPACE, VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN, 'A', 'B')
//
//  get_cursor_x(), get_cursor_y() - get mouse cursor position
//  is_mouse_button_pressed(int button) - check if mouse button is pressed (0 - left button, 1 - right button)
//  clear_buffer() - set all pixels in buffer to 'black'
//  is_window_active() - returns true if window is active
//  schedule_quit_game() - quit game after act()

GameMaster gm;

// initialize game data in this function
void initialize()
{
    gm.initiate();
}

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt)
{
  if (is_key_pressed(VK_ESCAPE))
    schedule_quit_game();
  bool check = is_key_pressed(VK_SPACE);
  gm.reverse(check);
  if (is_key_pressed('A'))
    gm.restore();
  gm.update(dt);
}

// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw()
{
  // clear backbuffer
  memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));
  gm.draw();
  /*
  std::vector<unsigned char> color = { 255, 255, 255 };
  std::vector<int> center_coordinates = { 200, 300 };
  bool enemy = false;
  int linear_size = 20;
  int rot_speed = 1;
  std::vector<int> lin_speed = { 20, 20 };
  Obstacle ob(color, center_coordinates, enemy, linear_size, rot_speed, lin_speed);
  ob.draw_self(SCREEN_HEIGHT, SCREEN_HEIGHT, counter * rot_speed);
  counter += 1;
  */

  // Sleep(1000 / 60);
}

// free game data in this function
void finalize()
{
    gm.free();
}

