// pong game for the collaboration project 

#include "genesis.h"
#include "resources.h"
#include "music.h"
// define names for sounds
#define PING        64
#define INTRO        65
#define DIE        66

/*The edges of the play field*/
const int LEFT_EDGE = 0;
const int RIGHT_EDGE = 320;
const int TOP_EDGE = 0;
const int BOTTOM_EDGE = 224;

// create sprites and give them values

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

// added ping sound when ball touches and edge. death sound on bottom_edge.
void moveBall(){
	//Check horizontal bounds
	if(ball_pos_x < LEFT_EDGE){
		SND_startPlayPCM_XGM(PING, 1, SOUND_PCM_CH2);
		ball_pos_x = LEFT_EDGE;
		ball_vel_x = -ball_vel_x;
	} else if(ball_pos_x + ball_width > RIGHT_EDGE){
		SND_startPlayPCM_XGM(PING, 1, SOUND_PCM_CH2);
		ball_pos_x = RIGHT_EDGE - ball_width;
		ball_vel_x = -ball_vel_x;
	}

	//Check vertical bounds
	if(ball_pos_y < TOP_EDGE){
		SND_startPlayPCM_XGM(PING, 1, SOUND_PCM_CH2);
		ball_pos_y = TOP_EDGE;
		ball_vel_y = -ball_vel_y;
	} else if(ball_pos_y + ball_height > BOTTOM_EDGE){
		SND_startPlayPCM_XGM(DIE, 1, SOUND_PCM_CH2);
		ball_pos_y = BOTTOM_EDGE - ball_height;
		ball_vel_y = -ball_vel_y;
	}
    //Ball movement goes here
	ball_pos_x += ball_vel_x;
	ball_pos_y += ball_vel_y;

SPR_setPosition(ball,ball_pos_x,ball_pos_y);
}

// joyhandler for inputs. may add a button for moving faster later
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
	// init sounds
	SND_setPCM_XGM(PING, SFX_PING, sizeof(SFX_PING));
    SND_setPCM_XGM(INTRO, SFX_INTRO, sizeof(SFX_INTRO));
	SND_setPCM_XGM(DIE, SFX_DIE, sizeof(SFX_DIE));
	XGM_setLoopNumber(0);

	// initialise joypad handler
	JOY_init();
	JOY_setEventHandler( &myJoyHandler );
	
	
	SYS_disableInts(); // always disable interupts when setting VDP and enable when finished. not sure why. investigate.
	// intro screen section
	VDP_clearPlan(PLAN_A, TRUE);
	VDP_drawImage(PLAN_B, &START, 0, 0);
	SYS_enableInts(); // always disable interupts when setting VDP and enable when finished. not sure why. investigate.

	SND_startPlayPCM_XGM(INTRO, 1, SOUND_PCM_CH2);
	
    waitMs(5000);
    // End section

	// cleat plane a and draw playfield
	SYS_disableInts(); // always disable interupts when setting VDP and enable when finished. not sure why. investigate.
    VDP_clearPlan(PLAN_A, TRUE);
	VDP_drawImage(PLAN_B, &background, 0, 0);
	VDP_loadTileSet(bgtile.tileset,1,DMA);
	// set pallets from png data. need to investigate pallets further
	VDP_setPalette(PAL1, bgtile.palette->data);
	VDP_setPalette(PAL2, imgball.palette->data);
	VDP_setPalette(PAL3, paddle.palette->data);
	SYS_enableInts(); // always disable interupts when setting VDP and enable when finished. not sure why. investigate.
	SPR_init(0,0,0); // init sprites. will look into spite sheets later.
	ball = SPR_addSprite(&imgball,100,100,TILE_ATTR(PAL2,0, FALSE, FALSE));
	player = SPR_addSprite(&paddle, player_pos_x, player_pos_y, TILE_ATTR(PAL3, 0, FALSE, FALSE));
	
	// background music. currently commented out for sfx testing.
	//SND_startPlayPCM_XGM(PING, 1, SOUND_PCM_CH1);
	// SND_startPlay_XGM(soundtrack);

	while(1)
	{
		// main 'do something' loop
		moveBall(); // move the ball
		positionPlayer(); // read player input, change position
		SPR_update(); // update the sprite position
		VDP_waitVSync(); // syncs with the 50/60hz refresh rate 
	}
	return 0;
}
