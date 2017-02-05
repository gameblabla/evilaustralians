#include "main.h" 
#include "maps.h" 

/* I must warn you : currently, the source code is a huge mess ! :P 
 * In addition to that, there are several bugs/limitations :
 * - Vertical maps are not supported (yet)
 * - Collision detection against the map is very slow
 * - There are some hacks
 * - Some functions could be merged together (like Player and Enemy)
 * 
 * That aside, it was designed to be fairly portable thanks to my Gameblabal's Wrapper library.
 * The platforms supported are :
 * PC (Windows 95+, Linux, macOS), Sega Dreamcast, GCW0, TI-Nspire CX or even the 3DO !
 * (and more)
 * */

int main(void)
{
	Init_video();
	Init_sound();
	game_name = "Evil Australians";
	
	Load_SFX(0, "rom://hit.wav");
	Load_SFX(1, "rom://hit2.wav");
	Load_SFX(2, "rom://shoot.wav");
	Load_SFX(3, "rom://victory.wav");
	Load_SFX(4, "rom://shoot_enemy.wav");
	Change_game(0, current_level);
	
	while (!done)
	{	
		Controls();
		
		if (BUTTON.QUIT) done = 1;
		
		switch(game_mode)
		{
			case 0:
				Put_image(4, 0, 0);
				Press_Start_prompt();
			break;
			case 1:
				Put_image(4, 0, 0);
				Put_Background(scroll_x, map_width);
				Put_Enemy();
				Bullets();
				Put_player();
				HUD();
			break;
			case 2:
				Put_image(4, 0, 0);
				Put_Background(scroll_x, map_width);
				Put_sprite_trans(1, player.x, player.y, player.width, player.height, human_anim_spr[player.pickframe][player.frame]+player.flip);
				Results_screen();
			break;
			case 3:
				Missions_Graphics();
			break;
			case 4:
				GO_screen();
			break;
			case 5:
				END_screen();
			break;
		}
		
		Update_video();
	}
	
	Clearing();

	return 0;
}


void Load_Images(unsigned char level)
{
	switch(game_mode)
	{
		case 0:
			Load_Image(4,"/title.sprite");
		break;
		case 1:
			Load_Image(1,"/player.sprite");
			Load_Image(2,"/temp.sprite");
			Load_Image(3,"/bullet.sprite");
			if (level == 3) Load_Image(4,"/background2.sprite");
			else Load_Image(4,"/background.sprite");
		break;
		case 2:
			Load_Image(1,"/player.sprite");
			Load_Image(2,"/temp.sprite");
			Load_Image(3,"/bullet.sprite");
		break;
		case 3:
			Load_Image(1,"/player.sprite");
			Load_Image(2,"/temp.sprite");
			Load_Image(3,"/instructions.sprite");
			Load_Image(4,"/mission.sprite");
		break;
		case 4:
			Load_Image(4,"/go.sprite");
		break;
		case 5:
			Load_Image(4,"/end.sprite");
		break;
	}

	Load_Image(5,"/font.sprite");
}

void HUD()
{
	Print_text(0, 0, "HP");
	Print_text(0, 16, itoa_own(player.hp));
	Print_text(0, 32, "ENEMIES LEFT");
	if (enemies_left == 0) Change_game(2, current_level);
	Print_text(0, 48, itoa_own(enemies_left));
	Print_text(272, 0, "SCORE");
	Print_text(272, 16, itoa_own(score));
}

void Press_Start_prompt()
{
	static unsigned char wait = 0;
	wait++;
	
	if (wait < 20)
	Print_text(85, 160, "PRESS START BUTTON");
	else if (wait > 40) wait = 0;
	
	if (highscore > 0)
	{
		Print_text(120, 196, "HIGHSCORE");
		Print_text(120, 212, itoa_own(highscore));
	}
	
	if (BUTTON.START) 
	{
		lives = 3;
		score = 0;
		current_level = 0;
		Change_game(3, current_level);
	}
}

void Missions_Graphics()
{
	static unsigned char wait = 0;
	wait++;
	wait_tocontinue++;
	Put_image(4, 0, 0);
	
	Put_sprite_trans(1, 120, 184, 16, 22, 0);
	Print_text(144, 180, "LIVES");
	Print_text(156, 196, itoa_own(lives));
	
	if (wait < 20)
	Put_image(3, 75, 12);
	else if (wait > 40) wait = 0;
	
	if (wait_tocontinue > 180)
	{
		Change_game(1, current_level);
	}
}

void Results_screen()
{
	static unsigned char nohit = 0;
	switch(results_mode)
	{
		case 0:
			if (player.hp == player.max_hp)
			{
				nohit = 1;
				score += 5000;
			}
			else
			{
				nohit = 0;
			}
			results_mode = 1;
			Play_SFX(3);
		break;
		case 1:
			results_wait++;

			if (results_wait > 10)
			Print_text(136, 120, "YOU WIN");
			
			if (results_wait > 90)
			{
				if (nohit) Print_text(144, 140, "PERFECT !");
				else Print_text(62, 140, "PREPARE FOR THE NEXT MISSION");
			}
			
			if (results_wait > 240)
			{
				current_level+=1;
				if (current_level > 3)
				Change_game(5, current_level);
				else Change_game(3, current_level);
			}
		break;
	}
	
}

void GO_screen()
{
	Put_image(4, 0, 0);
	results_wait++;
	if (results_wait > 360 || (results_wait > 120 && BUTTON.A))
	{
		if (score > highscore) highscore = score;
		score = 0;
		current_level = 0;
		Change_game(0, current_level);
	}
	
}

void END_screen()
{
	Put_image(4, 0, 0);
	results_wait++;
	if (results_wait > 120 && BUTTON.A)
	{
		if (score > highscore) highscore = score;
		score = 0;
		current_level = 0;
		Change_game(0, current_level);
	}
	
}

void Change_game(unsigned char mode, unsigned char level)
{
	game_mode = mode;
	wait_tocontinue = 0;
	results_mode = 0;
	results_wait = 0;
	Load_Images(level);
	if (mode == 1) Reset_default_values(level);
}

/* Print text on-screen */
void Print_text(unsigned short x, unsigned char y, char *text_ex)
{
	unsigned char i = 0;
	for (i=0;text_ex[i]!='\0';i++)
		Put_sprite_trans(5, x + (8 * i), y, 8, 8, text_ex[i]-33);
}

void Reset_default_values(unsigned char level)
{
	unsigned char i;
	unsigned char player_truex;
	
	scroll_progress = 0;
	
	player.height = 22;
	player.width = 16;
	player.speed = 2;
	player.time = 0;
	player.frame = 0;
	player.pickframe = 0;
	player.state = 2; // Fall
	player.fallspeed = 3;
	player.max_hp = 80;
	player.hp = player.max_hp;
	
	/*if (collision_map) free(collision_map);
	if (background_map) free(background_map);*/
	
	// player_truex is a hack, i need to find a proper solution to this...
	switch(current_level)
	{
		case 0:
			player_truex = 2;
			active_enemies = 5;
			player = Place_thing(player_truex, 10, player, 1, 0, 0);
			enemies[0] = Place_thing(21, 10, enemies[0], 0, 2, 0);
			enemies[1] = Place_thing(31, 8, enemies[1], 0, 2, 20);
			enemies[2] = Place_thing(29, 2, enemies[2], 0, 2, 0);
			enemies[3] = Place_thing(4, 3, enemies[3], 0, 2, 0);
			enemies[4] = Place_thing(38, 6, enemies[4], 0, 2, 20);
			map_width = 40;
			map_height = 15;
			map_size = sizeof(map);
			collision_map = malloc(map_size);
			background_map = malloc(map_size);
			collision_map = map;
			background_map = map;
		break;
		case 1:
			player_truex = 34;
			active_enemies = 6;
			player = Place_thing(player_truex, 4, player, 1, 0, 0);
			enemies[0] = Place_thing(6, 4, enemies[0], 0, 2, 20);
			enemies[1] = Place_thing(12, 7, enemies[1], 0, 2, 20);
			enemies[2] = Place_thing(14, 11, enemies[2], 0, 2, 20);
			enemies[3] = Place_thing(24, 7, enemies[3], 0, 2, 20);
			enemies[4] = Place_thing(27, 11, enemies[4], 0, 2, 20);
			enemies[5] = Place_thing(33, 9, enemies[5], 0, 2, 20);
			map_width = 40;
			map_height = 15;
			map_size = sizeof(map2);
			collision_map = malloc(map_size);
			background_map = malloc(map_size);
			collision_map = map2;
			background_map = map2;
		break;
		case 2:
			player_truex = 19;
			active_enemies = 7;
			player = Place_thing(player_truex, 0, player, 1, 0, 0);
			enemies[0] = Place_thing(6, 4, enemies[0], 0, 2, 0);
			enemies[1] = Place_thing(10, 4, enemies[1], 0, 2, 0);
			enemies[2] = Place_thing(32, 4, enemies[2], 0, 2, 20);
			enemies[3] = Place_thing(15, 8, enemies[3], 0, 2, 20);
			enemies[4] = Place_thing(23, 8, enemies[4], 0, 2, 20);
			enemies[5] = Place_thing(34, 11, enemies[5], 0, 2, 20);
			enemies[6] = Place_thing(1, 11, enemies[6], 0, 2, 0);
			map_width = 40;
			map_height = 15;
			map_size = sizeof(map3);
			collision_map = malloc(map_size);
			background_map = malloc(map_size);
			collision_map = map3;
			background_map = map3;
		break;
		case 3:
			player_truex = 2;
			active_enemies = 9;
			player = Place_thing(player_truex, 1, player, 1, 0, 0);
			enemies[0] = Place_thing(9, 5, enemies[0], 0, 2, 20);
			enemies[1] = Place_thing(15, 5, enemies[1], 0, 2, 20);
			enemies[2] = Place_thing(22, 5, enemies[2], 0, 2, 20);
			enemies[3] = Place_thing(27, 5, enemies[3], 0, 2, 20);
			enemies[4] = Place_thing(32, 5, enemies[4], 0, 2, 20);
			enemies[5] = Place_thing(36, 3, enemies[5], 0, 2, 0);
			enemies[6] = Place_thing(40, 9, enemies[6], 0, 2, 20);
			enemies[7] = Place_thing(43, 8, enemies[7], 0, 2, 0);
			enemies[8] = Place_thing(47, 3, enemies[8], 0, 2, 20);
			map_width = 50;
			map_height = 15;
			map_size = sizeof(map4);
			collision_map = malloc(map_size);
			background_map = malloc(map_size);
			collision_map = map4;
			background_map = map4;
		break;
	}
		
	for(i=0;i<active_enemies;i++)
	{
		// Reset enemies x positions to their actual real ones as they were set (Place_Thing)
		// This is a hack, hopefully i can find a proper solution to this in the near future
		enemies[i].x -= ((player_truex-2)*SIZE_TILE);
		enemies[i].active = 1;
		enemies[i].height = 22;
		enemies[i].width = 16;
		enemies[i].speed = 1;
		enemies[i].time = 0;
		enemies[i].frame = 0;
		enemies[i].pickframe = 0;
		enemies[i].state = 2;
		enemies[i].fallspeed = 3;
		enemies[i].max_hp = 20;
		enemies[i].hp = enemies[0].max_hp;
		enemies[i].moving = 0;
		enemies[i].sighted = 0;
		enemies[i].isfiring = 0;
		enemies[i].firetime = 0;
		enemies[i].hit = 0;
		enemies[i].jump = 0;
		enemies[i].tojump = 0;
		enemies[i].incollision[0] = 0;
		enemies[i].incollision[1] = 0;
		enemies[i].incollision[2] = 0;
		enemies[i].incollision[3] = 0;
		enemies[i].incollision[4] = 0;
	}
	
	for(i=0;i<32;i++)
	{
		bullets[i].active = 0;
		bullets[i].x = 0;
		bullets[i].y = 0;
		bullets[i].col = 0;
	}
	enemies_left = active_enemies;
}

struct main_player Place_thing(unsigned char tile_x, unsigned char tile_y, struct main_player toput, unsigned char playerornot, unsigned char direction, unsigned char flip)
{
	if (playerornot)
	{
		toput.x = 144;
		scroll_x = (tile_x * SIZE_TILE)-144;
	}
	else
	{
		toput.x = (tile_x * SIZE_TILE)+96;
	}
	toput.y = tile_y * SIZE_TILE;
	toput.direction = direction;
	toput.flip = flip;
	
	return toput;
}

void Put_Background(short scroll_x, unsigned char size_tile_w)
{
	unsigned char c, r;
	unsigned short tmp;
	unsigned char drawlimit[2];
	
	drawlimit[0] = 0;
	if (scroll_x > 0) drawlimit[0] = (scroll_x)/16;
	drawlimit[1] = drawlimit[0]+21;
	if (drawlimit[1] > size_tile_w) drawlimit[1] = size_tile_w;

	for (c = drawlimit[0]; c < drawlimit[1]; c++) 
	{
		for (r = 0; r < 15; r++) 
		{
			tmp = c + (size_tile_w * r);
			if (background_map[tmp] != 0) 
			Put_sprite_hardware(2, (c*SIZE_TILE)-scroll_x, r*SIZE_TILE, SIZE_TILE, SIZE_TILE, 1);
		}
	}
}


/* Handles the main player, much simplier than the enemies 
 * Perhaps in the future i could merge the merge logic together with the enemies
 * but because they behave differently, i have to make seperate functions for now
 * */
void Put_player()
{
	player = Animation_sprite(player);
	Put_sprite_trans(1, player.x, player.y, player.width, player.height, human_anim_spr[player.pickframe][player.frame]+player.flip);
	Player();
}

/* This function is used for placing all on the enemies on-screen
 * and gluing the logic to them
 * */
void Put_Enemy()
{
	unsigned char i;
	for(i=0;i<active_enemies;i++)
	{
		if (enemies[i].active)
		{
			enemies[i] = Animation_sprite(enemies[i]);
			enemies[i] = Enemy_AI(enemies[i], player);
			enemies[i] = Enemy(enemies[i], i, player);
			Put_sprite_trans(1, enemies[i].x, enemies[i].y, enemies[i].width, enemies[i].height, human_anim_spr[enemies[i].pickframe][enemies[i].frame]+enemies[i].flip);
		}
	}
}


void Player()
{
	unsigned char bullet_touse = 0;
	unsigned char done_bullet;
	/* The old variables are used for collisions */
	player.oldx = player.x;
	player.oldy = player.y;

	player.moving = 0;
	scroll_progress = 0;
	
	if (player.y > 320)
	{
		lives -= 1;
		if (lives < 1) Change_game(4, current_level);
		else Change_game(3, current_level);
	}
	
	if (player.state == 3)
	{
		player.frame = 19;
		player.y += 4;
	}
	else
	{
		if (player.hit == 0)
		{
			/* This function must be placed before the LEFT & Right functions or else the player
			 * will not stick when firing.
			*/
			if (BUTTON.B && player.isfiring == 0)
			{
				player.isfiring = 1;
				bullet_touse = 0;
				Play_SFX(2);
				done_bullet = 1;
				while(done_bullet)
				{
					if (bullets[bullet_touse].active == 0)
					{
						if (player.flip == 0)
						{
							bullets[bullet_touse].x = player.x+player.width;
							bullets[bullet_touse].direction = 0;
						}
						else
						{
							bullets[bullet_touse].x = player.x;
							bullets[bullet_touse].direction = 1;
						}
						bullets[bullet_touse].y = player.y+11;
						bullets[bullet_touse].power = 4;
						bullets[bullet_touse].player = 1;
						bullets[bullet_touse].active = 1;
						bullets[bullet_touse].speed = 8;
						done_bullet = 0;
					}
					else
					{
						if (bullet_touse > 4) done_bullet = 0;
						else bullet_touse++;
					}
				}
			}
			
			/* Left or Right buttons moves the main character 
			 * 
			*/
			if (BUTTON.LEFT)
			{
				if (player.isfiring==0) player.flip = 20;
				player.moving = 1;
				if (player.x <= 144)
				{
					scroll_progress = -player.speed;
					scroll_x += scroll_progress;
					player.oldx = player.x + player.speed;
				}
				else
				{
					player.x = player.x - player.speed;
				}
				player.incollision[0] = Collisions_MAP(player.x, player.y, 1, player.height);
			}
			else if (BUTTON.RIGHT)
			{	
				if (player.isfiring==0) player.flip = 0;
				player.moving = 1;
				if (player.x >= 144)
				{
					scroll_progress = player.speed;
					scroll_x += scroll_progress;
					player.oldx = player.x - player.speed;
				}
				else
				{
					player.x = player.x + player.speed;
				}
				player.incollision[0] = Collisions_MAP(player.x + (player.width+1), player.y, 1, player.height);
			}
		}
		
		if (player.incollision[0] == 1) player.x = player.oldx;
		
		switch(player.state)
		{
			case 2:
				player.y += player.fallspeed;
				player.incollision[1] = Collisions_MAP(player.x+6, player.y, 10, player.height+1);
				if (player.incollision[1] == 1) 
				{
					while(player.incollision[1]==1)
					{
						player.y -= 1;
						player.incollision[1] = Collisions_MAP(player.x+6, player.y, 10, player.height);
					}
					player.state = 1;
					player.incollision[1] = 0;
				}
			break;
			case 1:
				player.incollision[1] = Collisions_MAP(player.x+6, player.y, 10, player.height);
				if (BUTTON.A)
				{
					player.state = 0;
					player.incollision[1] = 0;
				}
				else if (player.incollision[1] == 0) 
				{
					player.state = 2;
					player.incollision[1] = 0;
				}
			break;
			/* 0 means the player is jumping */
			case 0:
				player.incollision[4] = Collisions_MAP(player.x+6, player.y, 10, 1);
				player.y = player.y - player.fallspeed;
				player.jumptime++;
				
				if (player.jumptime > 20) 
				{
					player.jumptime = 0;
					player.state = 2;
				}
				else if (player.jumptime > 5) 
				{
					if (BUTTON.A==0)
					{
						player.jumptime = 21;
					}
				}
				
				// If Player is hitting the celling, make it fall
				if (player.incollision[4] == 1) 
				{
					player.y = player.oldy;
					while (player.incollision[4])
					{
						player.incollision[4] = Collisions_MAP(player.x+6, player.y, 10, 1);
						player.y += 1;
					}
					player.jumptime = 21;
				}
			break;
			
		}

		if (player.hp < 1)
		{
			Play_SFX(2);
			player.state = 3;
		}
	}
}

struct main_player Animation_sprite(struct main_player pp)
{
	pp.time++;
	if (pp.time > human_anim_spr_delay[pp.pickframe])
	{
		pp.time = 0;
		pp.frame += 1;
		if (human_anim_spr[pp.pickframe][pp.frame] == 127) pp.frame = 0;
		if (pp.isfiring) pp.isfiring = 0;
	}
	
	if (pp.state == 3)
	{
		if (pp.pickframe != 7) 
		{
			pp.pickframe = 7;
			pp.frame = 0;
			pp.time = 0;
		}
	}
	else if (pp.hit > 0)
	{
		if (pp.pickframe != 6) 
		{
			pp.pickframe = 6;
			pp.frame = 0;
			pp.time = 0;
		}
		pp.hit-=5;
	}
	else if (pp.isfiring)
	{
		if (pp.moving)
		{
			if (pp.pickframe != 5) 
			{
				pp.pickframe = 5;
				if (pp.frame > 5) pp.frame = 0;
				pp.time = 0;
			}	
		}
		else
		{
			if (pp.pickframe != 3) 
			{
				pp.pickframe = 3;
				if (pp.frame > 2) pp.frame = 0;
				pp.time = 0;
			}
		}
	}
	else if (pp.state == 0)
	{
		if (pp.pickframe != 2) 
		{
			pp.pickframe = 2;
			pp.frame = 0;
			pp.time = 0;
		}
	}
	else if (pp.moving)
	{
		if (pp.pickframe != 1) 
		{
			pp.pickframe = 1;
			pp.frame = 0;
			pp.time = 0;
		}
	}
	else
	{
		if (pp.pickframe != 0) 
		{
			pp.pickframe = 0;
			pp.frame = 0;
			pp.time = 0;
		}
	}
	
	return pp;
}

struct main_player Enemy(struct main_player enemy, unsigned char id, struct main_player pp)
{
	unsigned char bullet_touse = 0;
	
	/* The old variables are used for collisions */
	enemy.oldx = enemy.x;
	enemy.oldy = enemy.y;
	enemy.tojump = 0;
	
	/* 
	* FIXME : Enemies are also misplaced when it scrolls to the right
	* This moves the enemy according to the scrolling of the main player
	*/
	enemy.x = enemy.x - scroll_progress;
	if (enemy.y > 240) 
	{
		enemies_left -= 1;
		score += 300;
		enemy.active = 0;
	}	
		
	// State equals 3 means that the enemy is dead DEAD DED MEAT
	if (enemy.state == 3)
	{
		enemy.y += 4;
	}
	else
	{	
		switch(enemy.direction)
		{
				case 1:
					enemy.flip = 0;
					enemy.x += (enemy.speed);
					enemy.incollision[0] = Collisions_MAP(enemy.x + (enemy.width+1), enemy.y, 1, 22);
					enemy.moving = 1;
				break;
				case 0:
					enemy.flip = 20;
					enemy.x -= (enemy.speed);
					enemy.incollision[0] = Collisions_MAP(enemy.x, enemy.y, 1, 22);
					enemy.moving = 1;
				break;
				case 2:
					enemy.moving = 0;
				break;
		}

		if (enemy.isfiring)
		{
			bullet_touse = 6+id;
			if (bullets[bullet_touse].active == 0)
			{
				bullets[bullet_touse].active = 1;
				if (enemy.flip == 0)
				{
					bullets[bullet_touse].x = enemy.x+enemy.width;
					bullets[bullet_touse].direction = 0;
				}
				else
				{
					bullets[bullet_touse].x = enemy.x;
					bullets[bullet_touse].direction = 1;
				}
				bullets[bullet_touse].y = enemy.y+11;
				bullets[bullet_touse].power = 4;
				bullets[bullet_touse].player = 0;
				bullets[bullet_touse].speed = 4;
				Play_SFX(4);
			}
			enemy.isfiring = 0;
		}

		// Character is falling (or on ground)
		switch(enemy.state)
		{
			/* This could still be improved, pehaps i should add a new state for on-ground and falling */
			case 2:
				enemy.y += enemy.fallspeed;
				enemy.incollision[1] = Collisions_MAP(enemy.x+6, enemy.y, 10, enemy.height+1);
				if (enemy.incollision[1] == 1) 
				{
					while(enemy.incollision[1]==1)
					{
						enemy.y -= 1;
						enemy.incollision[1] = Collisions_MAP(enemy.x+6, enemy.y, 10, enemy.height);
					}
					enemy.state = 1;
					enemy.incollision[1] = 0;
				}
			break;
			case 1:
				enemy.incollision[1] = Collisions_MAP(enemy.x+6, enemy.y, 10, enemy.height);
				if (enemy.jump)
				{
					enemy.state = 0;
					enemy.incollision[1] = 0;
				}
				else if (enemy.incollision[1] == 0) 
				{
					enemy.state = 2;
					enemy.incollision[1] = 0;
				}
			break;
			case 0:
				enemy.incollision[4] = Collisions_MAP(enemy.x+6, enemy.y, 10, 1);
				enemy.y = enemy.y - enemy.fallspeed;
				enemy.jumptime++;
				
				if (enemy.jumptime > 20) 
				{
					enemy.jumptime = 0;
					enemy.state = 2;
					enemy.jump = 0;
					enemy.tojump = 0;
				}
				
				// If Player is hitting the celling, make it fall
				if (enemy.incollision[4] == 1) 
				{
					enemy.y = enemy.oldy;
					while (enemy.incollision[4])
					{
						enemy.incollision[4] = Collisions_MAP(enemy.x+6, enemy.y, 10, 1);
						enemy.y += 1;
					}
					enemy.jumptime = 21;
				}
			break;
		}
		
		if (enemy.incollision[0] == 1) 
		{
			enemy.x = enemy.oldx;
			switch(enemy.direction)
			{
				case 0:
				while (enemy.incollision[0])
				{
					enemy.incollision[0] = Collisions_MAP(enemy.x, enemy.y, 0, 22);
					if (enemy.incollision[0] == 1) enemy.x += 1;
				}
				break;
				case 1:
				while (enemy.incollision[0])
				{
					enemy.incollision[0] = Collisions_MAP(enemy.x + (enemy.width+1), enemy.y, 0, 22);
					if (enemy.incollision[0] == 1) enemy.x -= 1;
				}
				break;
			}
			enemy.tojump = 1;
		}
		

		if (enemy.hp < 1)
		{
			Play_SFX(1);
			enemy.state = 3;
		}
	}
		
	return enemy;
}

struct main_player Enemy_AI(struct main_player enemy, struct main_player playertofollow)
{
	/* The variable "latency" is to add some delay to the enemy
	 * That's my attempt at making them more "humane" :P
	 * We don't want them to constanly fire at you, oh no
	 * */
	short distance_x, distance_y;
	distance_x = playertofollow.x - enemy.x;
	distance_y = playertofollow.y - enemy.y;

	if (distance_y > -50 && distance_y < 50)
	{
		switch(enemy.flip)
		{
			case 0:
			if (distance_x > -20)
			{
				enemy.sighted = 1;
			}
			break;
			case 20:
			if (distance_x > -150 && distance_x < 40)
			{
				enemy.sighted = 1;
			}
			break;
		}
	}
	
	enemy.latency++;
	if (enemy.latency > 20 && enemy.sighted == 1)
	{
		if (enemy.hit > 0)
		{
			enemy.direction = 2;
		}
		else if (playertofollow.x < enemy.x)
		{
			enemy.direction = 0;
		}
		else if (playertofollow.x > enemy.x)
		{
			enemy.direction = 1;
		}
		
		if (enemy.tojump)
		{
			enemy.jump = 1;
		}
		
		if (player.state == 3)
			enemy.direction = 2;
		else
			enemy.isfiring = 1;

		enemy.latency = 0;
	}
		
	return enemy;
}

/* This function is used for Bullet management 
 * Collisions
 * Movement (very simple for now)
 * Status
 * */
void Bullets()
{
	unsigned char i, a;
	
	// Loops between all the bullets
	for(i=0;i<16;i++)
	{
		// Only activate all the conditions and logic if bullet is enabled
		if (bullets[i].active)
		{
			// Draw the bullet itself
			Put_image_trans(3, bullets[i].x, bullets[i].y);
			
			// 0 is right
			if (bullets[i].direction == 0)
				bullets[i].x += bullets[i].speed - (scroll_progress);
			// 1 is Left
			else
				bullets[i].x -= bullets[i].speed + (scroll_progress);
			
			// test collisions against the map
			bullets[i].col = Collisions_MAP(bullets[i].x, bullets[i].y, 5, 5);
			
			// .player == 0 means that the bullet belongs to the enemy
			if (bullets[i].player == 0)
			{
				// If Enemy bullets touch you, you get hit
				player.incollision[5] = Collision(bullets[i].x-8, bullets[i].y, 16, 5, player.x, player.y, player.width, player.height);
				if (player.incollision[5])
				{
					if (player.hit == 0)
					{
						player.hp -= bullets[i].power;
						player.hit = 60;
						Play_SFX(1);
					}
					bullets[i].col = 1;
				}
			}
			// .player == 1 means that the bullet comes from you
			else
			{
				// Test collision againt the numerous enemies
				for(a=0;a<active_enemies;a++)
				{
					// Only test collisions if enemy is "active"
					if (enemies[a].active)
					{
						enemies[a].incollision[5] = Collision(bullets[i].x-8, bullets[i].y, 16, 6, enemies[a].x, enemies[a].y, enemies[a].width, enemies[a].height);
						if (enemies[a].incollision[5])
						{
							if (enemies[a].hit==0)
							{
								enemies[a].hp -= bullets[i].power;
								enemies[a].sighted = 1;
								enemies[a].hit = 60;
								Play_SFX(0);
								score += 10;
							}
							bullets[i].col = 1;
							break;
						}
					}
				}
			}
			
			/* If bullet is out of the map then destroy it */
			if (bullets[i].x > map_width*16 || bullets[i].x < 0) bullets[i].col = 1;
			
			// Desactivate Bullet if it hits something
			if (bullets[i].col == 1)
			bullets[i].active = 0;
		}
	}
}

/* Collisions
 * Checks collisions against the map
 * Thanks Cumred Snektron for the functions, it's definitively better than i came up with !
 * 
 * LICENSE
 * gameblabla	btw, is your code public domain ?	11:38:56 PM
 * Snektron	use it as you like gameblabla	11:55:25 PM
 * */
 
/*
	We can convert a pixel-coordinate to map-coordinates by dividing by the tile's size in pixels.
	because the tiles are square we can use the same function for x and y coordinates.
*/
#define PIX_TO_MAP(x) ((x) / 16)
/* This converts a map-coordinate to an index of the map array. */
#define MAP_TO_INDEX(x, y, a) ((y) * a + x)

unsigned char Collisions_MAP(short col_x, short col_y, unsigned short w, unsigned short h)
{
    unsigned short map_x = PIX_TO_MAP(col_x+scroll_x);
	unsigned short map_y = PIX_TO_MAP(col_y+h-1);
	unsigned short tile = MAP_TO_INDEX(map_x, map_y, map_width);
	/* If it's checking a tile that does not exist, return 0 */
	if (tile > map_size) return 0;
    return !!collision_map[tile];
}

/*
 * This function handles collisions.
 * It is quite simple : it only checks if it is inside the collision box.
 * No color detection or some shit like that.
 * 
*/

unsigned char Collision(unsigned short x, unsigned short y, unsigned char w, unsigned char h, unsigned short x2, unsigned short y2, unsigned char w2, unsigned char h2)
{
	if ((x + w > x2) && (x < x2 + w2) && (y + h > y2) && (y < y2 + h2))
	{
		return 1;
	}	
	return 0;
}

