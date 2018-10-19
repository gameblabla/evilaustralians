/*

The MIT License (MIT)

Copyright (c) 2017 Gameblabla

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

*/

#include "INPUT.h" 
#include "API.h" 
#include <stdlib.h>
#include <string.h>

#define SIZE_TILE 16

char *collision_map, *background_map;
unsigned char game_mode;
unsigned short map_width, map_height, map_size;
short scroll_x, scroll_progress;
unsigned char active_enemies;
unsigned char enemies_left;
unsigned char lives, current_level;
unsigned long score, highscore;

unsigned short wait_tocontinue;
unsigned short results_mode, results_wait;

const unsigned char human_anim_spr[10][10] =
{
	// Standing still
	{0, 0, 0, 0, 0, 1, 2, 1, 127},
	// Running
	{3, 4, 5, 6, 7, 6, 5, 4, 127},
	// Jumping
	{7, 127},
	// Firing (standing still)
	{8, 9, 9, 8, 127},
	// Unused running animation
	{10, 11, 12, 13, 12, 11, 127},
	// Firing (while running)
	{14, 15, 16, 17, 16, 15, 127},
	// Hit by projectile
	{18, 18, 19, 19, 18, 18, 19, 19, 127},
	// Death a la Mario
	{18, 127},
};

const unsigned char human_anim_spr_delay[10] =
{
	12, 4, 4, 4, 4, 2, 2, 2
};

struct main_player
{
	long x;
	long y;
	long oldx;
	long oldy;
	unsigned char state;
	unsigned short max_hp;
	short hp;
	/*unsigned short ammo;
	unsigned short max_ammo;*/
	unsigned char width;
	unsigned char height;
	unsigned char time;
	unsigned char frame;
	unsigned char pickframe;
	unsigned char incollision[6];
	unsigned char speed;
	unsigned short jumptime;
	unsigned char fallspeed;
	unsigned char isfiring;
	unsigned char state_anim;
	unsigned char moving;
	unsigned char flip;
	unsigned char hit;
	
	/* The following variables are only used for the enemies */
	unsigned char active;
	unsigned char direction;
	unsigned char jump;
	unsigned char tojump;
	unsigned char firetime;
	unsigned char sighted;
	unsigned char latency;
	
} player, enemies[16];


struct bull
{
	long x;
	long y;
	unsigned char direction;
	unsigned char col;
	unsigned char speed;
	unsigned char active;
	unsigned char player;
	unsigned char status;
	unsigned char power;
} bullets[32];

void Change_game(unsigned char mode, unsigned char level);
void Load_Images(unsigned char level);
void Reset_default_values(unsigned char level);

void Press_Start_prompt();

void Results_screen();
void GO_screen();
void END_screen();

void Print_text(unsigned short x, unsigned char y, char *text_ex);

void HUD();
void Missions_Graphics();

void Put_Background(short scroll_x, unsigned char size_tile_w);
struct main_player Animation_sprite(struct main_player pp);
void Put_player();
void Player();
struct main_player Place_thing(unsigned char tile_x, unsigned char tile_y, struct main_player toput, unsigned char playerornot, unsigned char direction, unsigned char flip);
struct main_player Enemy_AI(struct main_player enemy, struct main_player playertofollow);
unsigned char Collisions_MAP(short col_x, short col_y, unsigned short w, unsigned short h);
unsigned char Collision(unsigned short x, unsigned short y, unsigned char w, unsigned char h, unsigned short x2, unsigned short y2, unsigned char w2, unsigned char h2);
void Animate_Player();
struct main_player Enemy(struct main_player enemy, unsigned char id, struct main_player pp);
void Put_Enemy();


void Bullets();
char* itoa_own(unsigned short n);


/*
 *http://www.strudel.org.uk/itoa/
*/

char* itoa_own(unsigned short val)
{
	static char buf[6] = {0, 0, 0, 0, 0, 0};
	unsigned char i = sizeof(buf);
	for(; val && i ; --i, val /= 10)
		buf[i] = "0123456789abcdef"[val % 10];
	return &buf[i+1];
}
