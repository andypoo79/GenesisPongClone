#include "genesis.h"
#include "resources.h"
// look at me
// again

/*The edges of the play field*/
const int LEFT_EDGE = 0;
const int RIGHT_EDGE = 320;
const int TOP_EDGE = 0;
const int BOTTOM_EDGE = 224;

Sprite* ball;
int ball_pos_x = 100;
int ball_pos_y = 100;
int ball_vel_x = 3;
int ball_vel_y = 2;
int ball_width = 8;
int ball_height = 8;

void moveBall(){
	//Check horizontal bounds
	if(ball_pos_x < LEFT_EDGE){
		ball_pos_x = LEFT_EDGE;
		ball_vel_x + 10;
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


int main( )
{
	VDP_loadTileSet(bgtile.tileset,1,DMA);
	VDP_setPalette(PAL1, bgtile.palette->data);
	VDP_fillTileMapRect(PLAN_A,TILE_ATTR_FULL(PAL1,0,FALSE,FALSE,1),0,0,40,30);
	VDP_setPalette(PAL2, imgball.palette->data);
	SPR_init(0,0,0);
	ball = SPR_addSprite(&imgball,100,100,TILE_ATTR(PAL2,0, FALSE, FALSE));

	while(1)
	{
		moveBall();
		SPR_update();
		VDP_waitVSync();
	}
	return 0;
}
