#include "main.h" 
#include "maps.h" 

unsigned char game_mode;
unsigned short map_width, map_height;
short scroll_x = 0, scroll_progress = 0;

int main(void)
{
	Init_video();
	game_name = "Evil Australians";
	game_mode = 0;
	
	while (!done)
	{	
		Clear_screen();
		Controls();
		
		if (BUTTON.QUIT) done = 1;
		
		if (game_mode == 0)
		{
			Load_game_memory();
		}
		else if (game_mode == 1)
		{
			Put_Background(scroll_x, 40);
			Draw_player();
			Draw_Pixel(0, 0, 255, 0, 0);
			
			Player();
		}
		else if (game_mode == 2)
		{
			
		}
		
		Update_video();
	}
	
	Clearing();

	return 0;
}


void Load_game_memory()
{
	Clear_screen();
	Load_Image(1,"DATA/player.bmp");
	Load_Image(2,"DATA/temp.bmp");

	Update_video();
	game_mode = 1;
	
	player.x = 144;
	player.y = 40;
	player.height = 22;
	player.width = 16;
	player.speed = 2;
	
	map_width = 20;
	map_height = 15;
	
	player.state = 2; // Fall
	player.fallspeed = 3;

}

void Put_Background(short scroll_x, unsigned char size_tile_w)
{
	unsigned short map_x;
	unsigned short map_y;
	unsigned short i;
	
	size_tile_w = size_tile_w - 1;
	map_x = 0;
	map_y = 0;
	
	for(i=0;i<sizeof(map);i++)
	{
		Put_sprite(2, (map_x*SIZE_TILE)-scroll_x, map_y*SIZE_TILE, SIZE_TILE, SIZE_TILE, map[i]);
		map_x++;
		if (map_x > size_tile_w)
		{
			map_x = 0;
			map_y = map_y + 1;
		}
	}

}

void Draw_player()
{
	Put_sprite(1, player.x, player.y, player.width, player.height, 0);
}


void Player()
{
	player.oldx = player.x;
	player.oldy = player.y;
	
	/* We are checking for collisions when the character is moving */
	if (BUTTON.LEFT)
	{
		if (player.x < 140 && scroll_x > player.speed)
		{
			scroll_progress = player.speed;
			scroll_x -= scroll_progress;
			player.oldx = player.x + player.speed;
		}
		else
		{
			player.x = player.x - player.speed;
		}
		player.incollision[2] = Collisions_MAP(player.x, player.y, 1, 22, scroll_x, sizeof(map), 40);
		if (player.incollision[2] == 1) player.x = player.oldx;
	}
	else if (BUTTON.RIGHT)
	{	
		if (player.x > 140)
		{
			scroll_progress = player.speed;
			scroll_x += scroll_progress;
			player.oldx = player.x - player.speed;
		}
		else
		{
			player.x = player.x + player.speed;
		}
		player.incollision[0] = Collisions_MAP(player.x + 17, player.y, 1, 22, scroll_x, sizeof(map), 40);
		if (player.incollision[0] == 1) player.x = player.oldx;
	}
	
	if (BUTTON.A && player.state == 2)
	{
		player.state = 0;
		player.incollision[1] = 0;
	}
	
	// Character is falling (or on ground)
	if (player.state == 2)
	{
		player.incollision[1] = Collisions_MAP(player.x+6, player.y+22, 10, 1, scroll_x, sizeof(map), 40);
		player.incollision[3] = Collisions_MAP(player.x+6, player.y+24, 10, 1, scroll_x, sizeof(map), 40);
		player.y = player.y + player.fallspeed;
		if (player.incollision[1] == 1) 
		{
			player.y = player.oldy;
		}
		else if (player.incollision[3] == 1) 
		{
			player.y = player.oldy-2;
		}
	}
	else if (player.state == 0)
	{
		player.incollision[2] = Collisions_MAP(player.x+6, player.y, 10, 1, scroll_x, sizeof(map), 40);
		player.y = player.y - player.fallspeed;
		player.jumptime++;
		if (player.jumptime > 16) 
		{
			player.jumptime = 0;
			player.state = 2;
		}
		
		if (player.incollision[2] == 1) 
		{
			player.y = player.oldy;
			player.jumptime = 17;
		}
	}

	// If player hits ground when falling

}

void Scrolling()
{
	if (scroll_x < 1) scroll_x = 0;

}

int Collisions_MAP(short col_x, short col_y, unsigned short width_player, unsigned short height_player, short scroll_x, unsigned short size_map, unsigned short tile_width_map)
{
	unsigned short i, a = 0, y = 0;
	unsigned short temp_x = 0, temp_y = 0;

	for (i=0;i<size_map;i++)	// Y axis
	{
		a++;
		if (a > (tile_width_map-1))
		{
			y++;
			a = 0;
		}
		temp_x = (a * SIZE_TILE)-scroll_x;
		temp_y = y * SIZE_TILE;
		
		if (map[a+(y*tile_width_map)] == 1)
		{
			if ( (col_x + width_player > temp_x) && (col_x < temp_x + SIZE_TILE) )
			{
				if ( (col_y + height_player > temp_y ) && (col_y < temp_y + SIZE_TILE) )
				{
					return 1;
				}
			}
		}
	}
	return 0;
}
