#include "genesis.h"
#include "resources.h"
#include "music.h"

/*The edges of the play field*/
const int LEFT_EDGE = 0;
const int RIGHT_EDGE = 320;
const int TOP_EDGE = 0;
const int BOTTOM_EDGE = 224;

Sprite* ball;
int ball_pos_x = 100;
int ball_pos_y = 200;
int ball_vel_x = 1;
int ball_vel_y = 2;
int ball_width = 8;
int ball_height = 8;

Sprite* player;
int player_pos_x = 230;
const int player_pos_y = 215;
int player_vel_x = 0;
const int player_width = 32;
const int player_height = 8;


void moveBall(){
	//Check horizontal bounds
	if(ball_pos_x < LEFT_EDGE){
		ball_pos_x = LEFT_EDGE;
		ball_vel_x = -ball_vel_x;
	} else if(ball_pos_x + ball_width > RIGHT_EDGE){
		ball_pos_x = RIGHT_EDGE - ball_width;
		ball_vel_x = -ball_vel_x;
	}

	//Check vertical bounds
	if(ball_pos_y < TOP_EDGE){
		ball_pos_y = TOP_EDGE;
		ball_vel_y = -ball_vel_y;
	} else if(ball_pos_y + ball_height > BOTTOM_EDGE){
		ball_pos_y = BOTTOM_EDGE - ball_height;
		ball_vel_y = -ball_vel_y;
	}
    //Ball movement goes here
	ball_pos_x += ball_vel_x;
	ball_pos_y += ball_vel_y;

SPR_setPosition(ball,ball_pos_x,ball_pos_y);
}

void myJoyHandler( u16 joy, u16 changed, u16 state)
{
	if (joy == JOY_1)
	{
		/*Set player velocity if left or right are pressed;
		 *set velocity to 0 if no direction is pressed */
		if (state & BUTTON_RIGHT)
		{
			player_vel_x = 3;
		}
		else if (state & BUTTON_LEFT)
		{
			player_vel_x = -3;
		} else{
			if( (changed & BUTTON_RIGHT) | (changed & BUTTON_LEFT) ){
				player_vel_x = 0;
			}
		}
	}
}

void positionPlayer(){
	/*Add the player's velocity to its position*/
	player_pos_x += player_vel_x;

	/*Keep the player within the bounds of the screen*/
	if(player_pos_x < LEFT_EDGE) player_pos_x = LEFT_EDGE;
	if(player_pos_x + player_width > RIGHT_EDGE) player_pos_x = RIGHT_EDGE - player_width;

	/*Let the Sprite engine position the sprite*/
	SPR_setPosition(player,player_pos_x,player_pos_y);
}

int main( )
{
	
	// initialise joypad handler
	JOY_init();
	JOY_setEventHandler( &myJoyHandler );
	// load tileset
	VDP_loadTileSet(bgtile.tileset,1,DMA);
	// set pallets from png data
	VDP_setPalette(PAL1, bgtile.palette->data);
	VDP_setPalette(PAL2, imgball.palette->data);
	VDP_setPalette(PAL3, paddle.palette->data);
	// fill background with recuring tile
	//VDP_fillTileMapRect(PLAN_B,TILE_ATTR_FULL(PAL1,0,FALSE,FALSE,1),0,0,40,30);
	SYS_disableInts();
	VDP_drawImage(PLAN_B, &background, 0, 0);
	SYS_enableInts();
	SPR_init(0,0,0);
	ball = SPR_addSprite(&imgball,100,100,TILE_ATTR(PAL2,0, FALSE, FALSE));
	player = SPR_addSprite(&paddle, player_pos_x, player_pos_y, TILE_ATTR(PAL3, 0, FALSE, FALSE));
	XGM_setLoopNumber(0);
	SND_startPlay_XGM(soundtrack);
	while(1)
	{
		
		moveBall();
		positionPlayer();
		SPR_update();
		VDP_waitVSync();
	}
	return 0;
}
