#include <stdio.h>
#include <stdint.h>
#include <SDL/SDL.h>
#include "zxemul.h"
#include "fsnapshots.h"

#define interval 20

SDL_TimerID timerID;
SDL_Surface * screen;
SDL_Event event;
SDL_Joystick *joystick;


void	checkKeys(int event, int state) {
	switch(event) {
	case SDLK_LSHIFT:
			if (state)
				z80io.ZXKeyboard.kfe &= (~0x1);
			else
				z80io.ZXKeyboard.kfe |= (0x1);
		break;
	case SDLK_z:
			if (state)
				z80io.ZXKeyboard.kfe &= (~0x2);
			else
				z80io.ZXKeyboard.kfe |= (0x2);
		break;
	case SDLK_x:
			if (state)
				z80io.ZXKeyboard.kfe &= (~0x4);
			else
				z80io.ZXKeyboard.kfe |= (0x4);
		break;
	case SDLK_c:
			if (state)
				z80io.ZXKeyboard.kfe &= (~0x8);
			else
				z80io.ZXKeyboard.kfe |= (0x8);
		break;
	case SDLK_v:
			if (state)
				z80io.ZXKeyboard.kfe &= (~0x10);
			else
				z80io.ZXKeyboard.kfe |= (0x10);
		break;

	case SDLK_a:
			if (state)
				z80io.ZXKeyboard.kfd &= (~0x1);
			else
				z80io.ZXKeyboard.kfd |= (0x1);
		break;
	case SDLK_s:
			if (state)
				z80io.ZXKeyboard.kfd &= (~0x2);
			else
				z80io.ZXKeyboard.kfd |= (0x2);
		break;
	case SDLK_d:
			if (state)
				z80io.ZXKeyboard.kfd &= (~0x4);
			else
				z80io.ZXKeyboard.kfd |= (0x4);
		break;
	case SDLK_f:
			if (state)
				z80io.ZXKeyboard.kfd &= (~0x8);
			else
				z80io.ZXKeyboard.kfd |= (0x8);
		break;
	case SDLK_g:
			if (state)
				z80io.ZXKeyboard.kfd &= (~0x10);
			else
				z80io.ZXKeyboard.kfd |= (0x10);
		break;

	case SDLK_h:
			if (state)
				z80io.ZXKeyboard.kbf &= (~0x10);
			else
				z80io.ZXKeyboard.kbf |= (0x10);
		break;
	case SDLK_j:
			if (state)
				z80io.ZXKeyboard.kbf &= (~0x8);
			else
				z80io.ZXKeyboard.kbf |= (0x8);
		break;
	case SDLK_k:
			if (state)
				z80io.ZXKeyboard.kbf &= (~0x4);
			else
				z80io.ZXKeyboard.kbf |= (0x4);
		break;
	case SDLK_l:
			if (state)
				z80io.ZXKeyboard.kbf &= (~0x2);
			else
				z80io.ZXKeyboard.kbf |= (0x2);
		break;
	case SDLK_RETURN:
			if (state)
				z80io.ZXKeyboard.kbf &= (~0x1);
			else
				z80io.ZXKeyboard.kbf |= (0x1);
		break;

	case SDLK_q:
			if (state)
				z80io.ZXKeyboard.kfb &= (~0x1);
			else
				z80io.ZXKeyboard.kfb |= (0x1);
		break;
	case SDLK_w:
			if (state)
				z80io.ZXKeyboard.kfb &= (~0x2);
			else
				z80io.ZXKeyboard.kfb |= (0x2);
		break;
	case SDLK_e:
			if (state)
				z80io.ZXKeyboard.kfb &= (~0x4);
			else
				z80io.ZXKeyboard.kfb |= (0x4);
		break;
	case SDLK_r:
			if (state)
				z80io.ZXKeyboard.kfb &= (~0x8);
			else
				z80io.ZXKeyboard.kfb |= (0x8);
		break;
	case SDLK_t:
			if (state)
				z80io.ZXKeyboard.kfb &= (~0x10);
			else
				z80io.ZXKeyboard.kfb |= (0x10);
		break;

	case SDLK_p:
			if (state)
				z80io.ZXKeyboard.kdf &= (~0x1);
			else
				z80io.ZXKeyboard.kdf |= (0x1);
		break;
	case SDLK_o:
			if (state)
				z80io.ZXKeyboard.kdf &= (~0x2);
			else
				z80io.ZXKeyboard.kdf |= (0x2);
		break;
	case SDLK_i:
			if (state)
				z80io.ZXKeyboard.kdf &= (~0x4);
			else
				z80io.ZXKeyboard.kdf |= (0x4);
		break;
	case SDLK_u:
			if (state)
				z80io.ZXKeyboard.kdf &= (~0x8);
			else
				z80io.ZXKeyboard.kdf |= (0x8);
		break;
	case SDLK_y:
			if (state)
				z80io.ZXKeyboard.kdf &= (~0x10);
			else
				z80io.ZXKeyboard.kdf |= (0x10);
		break;

	case SDLK_1:
			if (state)
				z80io.ZXKeyboard.kf7 &= (~0x1);
			else
				z80io.ZXKeyboard.kf7 |= (0x1);
		break;
	case SDLK_2:
			if (state)
				z80io.ZXKeyboard.kf7 &= (~0x2);
			else
				z80io.ZXKeyboard.kf7 |= (0x2);
		break;
	case SDLK_3:
			if (state)
				z80io.ZXKeyboard.kf7 &= (~0x4);
			else
				z80io.ZXKeyboard.kf7 |= (0x4);
		break;
	case SDLK_4:	//z80.debug=1;
			if (state)
				z80io.ZXKeyboard.kf7 &= (~0x8);
			else
				z80io.ZXKeyboard.kf7 |= (0x8);
		break;
	case SDLK_5:
			if (state)
				z80io.ZXKeyboard.kf7 &= (~0x10);
			else
				z80io.ZXKeyboard.kf7 |= (0x10);
		break;

	case SDLK_0:
			if (state)
				z80io.ZXKeyboard.kef &= (~0x1);
			else
				z80io.ZXKeyboard.kef |= (0x1);
		break;
	case SDLK_9:
			if (state)
				z80io.ZXKeyboard.kef &= (~0x2);
			else
				z80io.ZXKeyboard.kef |= (0x2);
		break;
	case SDLK_8:
			if (state)
				z80io.ZXKeyboard.kef &= (~0x4);
			else
				z80io.ZXKeyboard.kef |= (0x4);
		break;
	case SDLK_7:
			if (state)
				z80io.ZXKeyboard.kef &= (~0x8);
			else
				z80io.ZXKeyboard.kef |= (0x8);
		break;
	case SDLK_6:
			if (state)
				z80io.ZXKeyboard.kef &= (~0x10);
			else
				z80io.ZXKeyboard.kef |= (0x10);
		break;

	case SDLK_SPACE:
			if (state)
				z80io.ZXKeyboard.k7f &= (~0x1);
			else
				z80io.ZXKeyboard.k7f |= (0x1);
		break;
	case SDLK_RSHIFT:
			if (state)
				z80io.ZXKeyboard.k7f &= (~0x2);
			else
				z80io.ZXKeyboard.k7f |= (0x2);
		break;
	case SDLK_m:
			if (state)
				z80io.ZXKeyboard.k7f &= (~0x4);
			else
				z80io.ZXKeyboard.k7f |= (0x4);
		break;
	case SDLK_n:
			if (state)
				z80io.ZXKeyboard.k7f &= (~0x8);
			else
				z80io.ZXKeyboard.k7f |= (0x8);
		break;
	case SDLK_b:
			if (state)
				z80io.ZXKeyboard.k7f &= (~0x10);
			else
				z80io.ZXKeyboard.k7f |= (0x10);
		break;

	default:
		;
	};
};

void checkEvents(){
   		while(SDL_PollEvent(&event) != 0) {
			switch(event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
                case SDLK_F12:
                    z80.reset();
                    break;
                case SDLK_F3:
                    loadSNA48k("roms/elite.sna");
                    break;
                case SDLK_F4:
                    loadSNA48k("roms/ikari.sna");
                    break;
                case SDLK_F5:
                    loadSNA48k("roms/rraid.sna");
                    break;
                case SDLK_F6:
                    loadSNA48k("roms/deathchase.sna");
                    break;
                default:
				    checkKeys(event.key.keysym.sym, 1);
                }
				break;
			case SDL_KEYUP:
				checkKeys(event.key.keysym.sym,0);
				break;
			case (SDL_QUIT):
			    SDL_RemoveTimer(timerID);
				SDL_FreeSurface(screen);
				exit(0);
				break;
    		case SDL_JOYAXISMOTION:
    				if ( ( event.jaxis.value < -3200 ) || (event.jaxis.value > 3200 ) ) {
        				if( event.jaxis.axis == 0) {
                            if(event.jaxis.value > 0) {
                                z80io.kempston |= kLeft;
                                z80io.kempston &=~ kRigth;
                            } else if(event.jaxis.value < 0) {
                                z80io.kempston &= ~kLeft;
                                z80io.kempston |= kRigth;
                            }
        				} else if( event.jaxis.axis == 1) {
						    if(event.jaxis.value > 0) {
                                z80io.kempston |= kUp;
                                z80io.kempston &= ~kDown;
						    } else if (event.jaxis.value < 0) {
                                z80io.kempston |= kDown;
                                z80io.kempston &= ~kUp;
						    }
					    }
                    } else {
                            if( event.jaxis.axis == 0) {
                            z80io.kempston &= ~kLeft;z80io.kempston &= ~kRigth;
                        } else if( event.jaxis.axis == 1) {
                                z80io.kempston &= ~kDown;
                                z80io.kempston &= ~kUp;
                        }
                    }
    		    break;
    		case SDL_JOYBUTTONDOWN:
				z80io.kempston |= kFire;
    			break;
    		case SDL_JOYBUTTONUP:
				z80io.kempston &=~ kFire;
    			break;
			}
			break;

		}
}

int emul_active = 0;
Uint32 callback (Uint32 minterval, void* param) {
    if(!emul_active) {
        emul_active = 1;
		zx48.emul();
		zx48.ShowZXscreen();
	    SDL_UpdateRect(screen, 0, 0, 0, 0);
        emul_active = 0;
    } else fprintf(stderr, "skip frame");
    return minterval;
}

int main() {

	if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_JOYSTICK ) < 0 ) {
		fprintf(stderr, "Unable init SDL: %s\n", SDL_GetError());
		exit(1);
	}
    SDL_JoystickEventState(SDL_ENABLE);
    joystick = SDL_JoystickOpen(0);
	atexit(SDL_Quit);
   	screen = SDL_SetVideoMode(320*2 , 240*2 , 32 , 0);
   	if(screen == NULL) {
   		fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
		exit(1);
	}

	zx48.init((Uint32 *)screen->pixels);
	loadROM48k("roms/48.rom");

    timerID = SDL_AddTimer(interval, (SDL_NewTimerCallback)callback, (void*)"wtf");
    if(!timerID) fprintf(stderr, "timerID: %s\n", SDL_GetError());

	Uint32 i = 50*60*60*24*366;
	while (i--) {
		checkEvents();
		SDL_Delay(10);
	}
}
