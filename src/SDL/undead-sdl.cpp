#include <stdio.h>
#include <stdint.h>
#ifdef SDL2
#include <SDL2/SDL.h>
#else
#include <SDL/SDL.h>
#endif

#include "zxemul.h"
#include "fsnapshots.h"
#include "zxkempston.h"

#define interval 20

SDL_TimerID timerID;
SDL_Surface * screen;

#ifdef SDL2
SDL_Window * sdlWindow;
SDL_Renderer * sdlRenderer;
SDL_Texture * sdlTexture;
Uint32 * pixels;
#endif

SDL_Event event;
SDL_Joystick *joystick;


void	checkKeys(int event, int state) {
	switch(event) {
    case SDLK_LSHIFT: z80io.setKey(ZX_KeyRow_CShift, ZX_KeyBit_CShift, state); break;
    case SDLK_z: z80io.setKey(ZX_KeyRow_Z, ZX_KeyBit_Z, state); break;
    case SDLK_x: z80io.setKey(ZX_KeyRow_X, ZX_KeyBit_X, state); break;
    case SDLK_c: z80io.setKey(ZX_KeyRow_C, ZX_KeyBit_C, state); break;
    case SDLK_v: z80io.setKey(ZX_KeyRow_V, ZX_KeyBit_V, state); break;

    case SDLK_a: z80io.setKey(ZX_KeyRow_A, ZX_KeyBit_A, state); break;
    case SDLK_s: z80io.setKey(ZX_KeyRow_S, ZX_KeyBit_S, state); break;
    case SDLK_d: z80io.setKey(ZX_KeyRow_D, ZX_KeyBit_D, state); break;
    case SDLK_f: z80io.setKey(ZX_KeyRow_F, ZX_KeyBit_F, state); break;
    case SDLK_g: z80io.setKey(ZX_KeyRow_G, ZX_KeyBit_G, state); break;

    case SDLK_h: z80io.setKey(ZX_KeyRow_H, ZX_KeyBit_H, state); break;
    case SDLK_j: z80io.setKey(ZX_KeyRow_J, ZX_KeyBit_J, state); break;
    case SDLK_k: z80io.setKey(ZX_KeyRow_K, ZX_KeyBit_K, state); break;
    case SDLK_l: z80io.setKey(ZX_KeyRow_L, ZX_KeyBit_L, state); break;
    case SDLK_RETURN: z80io.setKey(ZX_KeyRow_Enter, ZX_KeyBit_Enter, state); break;

    case SDLK_q: z80io.setKey(ZX_KeyRow_Q, ZX_KeyBit_Q, state); break;
    case SDLK_w: z80io.setKey(ZX_KeyRow_W, ZX_KeyBit_W, state); break;
    case SDLK_e: z80io.setKey(ZX_KeyRow_E, ZX_KeyBit_E, state); break;
    case SDLK_r: z80io.setKey(ZX_KeyRow_R, ZX_KeyBit_R, state); break;
    case SDLK_t: z80io.setKey(ZX_KeyRow_T, ZX_KeyBit_T, state); break;

    case SDLK_p: z80io.setKey(ZX_KeyRow_P, ZX_KeyBit_P, state); break;
    case SDLK_o: z80io.setKey(ZX_KeyRow_O, ZX_KeyBit_O, state); break;
    case SDLK_i: z80io.setKey(ZX_KeyRow_I, ZX_KeyBit_I, state); break;
    case SDLK_u: z80io.setKey(ZX_KeyRow_U, ZX_KeyBit_U, state); break;
    case SDLK_y: z80io.setKey(ZX_KeyRow_Y, ZX_KeyBit_Y, state); break;

    case SDLK_1: z80io.setKey(ZX_KeyRow_1, ZX_KeyBit_1, state); break;
    case SDLK_2: z80io.setKey(ZX_KeyRow_2, ZX_KeyBit_2, state); break;
    case SDLK_3: z80io.setKey(ZX_KeyRow_3, ZX_KeyBit_3, state); break;
    case SDLK_4: z80io.setKey(ZX_KeyRow_4, ZX_KeyBit_4, state); break;
    case SDLK_5: z80io.setKey(ZX_KeyRow_5, ZX_KeyBit_5, state); break;

    case SDLK_0: z80io.setKey(ZX_KeyRow_0, ZX_KeyBit_0, state); break;
    case SDLK_9: z80io.setKey(ZX_KeyRow_9, ZX_KeyBit_9, state); break;
    case SDLK_8: z80io.setKey(ZX_KeyRow_8, ZX_KeyBit_8, state); break;
    case SDLK_7: z80io.setKey(ZX_KeyRow_7, ZX_KeyBit_7, state); break;
    case SDLK_6: z80io.setKey(ZX_KeyRow_6, ZX_KeyBit_6, state); break;

    case SDLK_SPACE: z80io.setKey(ZX_KeyRow_Space, ZX_KeyBit_Space, state); break;
    case SDLK_RSHIFT: z80io.setKey(ZX_KeyRow_SShift, ZX_KeyBit_SShift, state); break;
    case SDLK_m: z80io.setKey(ZX_KeyRow_M, ZX_KeyBit_M, state); break;
    case SDLK_n: z80io.setKey(ZX_KeyRow_N, ZX_KeyBit_N, state); break;
    case SDLK_b: z80io.setKey(ZX_KeyRow_B, ZX_KeyBit_B, state); break;

    case SDLK_BACKSPACE:
        z80io.setKey(ZX_KeyRow_0, ZX_KeyBit_0, state);
        z80io.setKey(ZX_KeyRow_CShift, ZX_KeyBit_CShift, state);
        break;
    case SDLK_QUOTE:
    case SDLK_QUOTEDBL:
        z80io.setKey(ZX_KeyRow_P, ZX_KeyBit_P, state);
        z80io.setKey(ZX_KeyRow_SShift, ZX_KeyBit_SShift, state);
        break;
    case SDLK_RIGHT:
        z80io.setKey(ZX_KeyRow_8, ZX_KeyBit_8, state);
        z80io.setKey(ZX_KeyRow_CShift, ZX_KeyBit_CShift, state);
        break;
    case SDLK_UP:
        z80io.setKey(ZX_KeyRow_7, ZX_KeyBit_7, state);
        z80io.setKey(ZX_KeyRow_CShift, ZX_KeyBit_CShift, state);
        break;
    case SDLK_DOWN:
        z80io.setKey(ZX_KeyRow_6, ZX_KeyBit_6, state);
        z80io.setKey(ZX_KeyRow_CShift, ZX_KeyBit_CShift, state);
        break;
    case SDLK_LEFT:
        z80io.setKey(ZX_KeyRow_5, ZX_KeyBit_5, state);
        z80io.setKey(ZX_KeyRow_CShift, ZX_KeyBit_CShift, state);
        break;
    }
}

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
                case SDLK_F7:
                    loadSNA48k("roms/dizzy3.sna");
                    break;
                case SDLK_F8:
                    loadSNA48k("roms/pacmania.sna");
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
//        zx48.ShowZXscreenNormal();
        zx48.ShowZXscreen();
#ifdef SDL2
    SDL_UpdateTexture(sdlTexture, NULL, pixels, 640 * sizeof (Uint32));
//    SDL_RenderClear(sdlRenderer);
    SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
    SDL_RenderPresent(sdlRenderer);
#else
        SDL_UpdateRect(screen, 0, 0, 0, 0);
#endif
        emul_active = 0;
    }
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
#ifdef SDL2
//    sdlWindow = SDL_CreateWindow("Undead ZX",
//                              SDL_WINDOWPOS_UNDEFINED,
//                              SDL_WINDOWPOS_UNDEFINED,
//                              640, 480,
//                              0);
    SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_OPENGL, &sdlWindow, &sdlRenderer);
    pixels = (Uint32 *)malloc(640*480*4);
    sdlTexture = SDL_CreateTexture(sdlRenderer,
                                   SDL_PIXELFORMAT_ARGB8888,
                                   SDL_TEXTUREACCESS_STREAMING,
                                   640, 480);
    zx48.init(pixels);
#else
//    screen = SDL_SetVideoMode(320 , 240 , 32 , 0);
    SDL_WM_SetCaption("Undead ZX", "undead-zx");
    screen = SDL_SetVideoMode(640 , 480 , 32 , 0);
   	if(screen == NULL) {
   		fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
		exit(1);
	}
    zx48.init((Uint32 *)screen->pixels);
#endif
	loadROM48k("roms/48.rom");

#ifdef SDL2
    timerID = SDL_AddTimer(interval, (SDL_TimerCallback)callback, (void*)"wtf");
#else
    timerID = SDL_AddTimer(interval, (SDL_NewTimerCallback)callback, (void*)"wtf");
#endif
    if(!timerID) fprintf(stderr, "timerID: %s\n", SDL_GetError());

//	Uint32 i = 50*60*60*24*366;
    while (true) {
		checkEvents();
        SDL_Delay(20);
	}
}
