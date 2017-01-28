#include "INPUT.h" 
#include "API.h" 

#define SIZE_TILE 16

struct main_player
{
	long x;
	long y;
	long oldx;
	long oldy;
	unsigned char state;
	unsigned short max_hp;
	short hp;
	unsigned short ammo;
	unsigned short max_ammo;
	unsigned char width;
	unsigned char height;
	unsigned char incollision[4];
	unsigned char speed;
	unsigned short jumptime;
	unsigned char fallspeed;
	
} player;

void Load_game_memory();
void Put_Background(short scroll_x, unsigned char size_tile_w);
void Draw_player();
void Player();
int Collisions_MAP(short col_x, short col_y, unsigned short width_player, unsigned short height_player, short scroll_x, unsigned short size_map, unsigned short tile_width_map);


