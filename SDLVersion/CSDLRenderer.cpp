#include <functional>
#include "Common.h"

#include "CGame.h"
#include "CRenderer.h"
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include <iostream>

namespace odb {

    SDL_Surface *video;

    CRenderer::CRenderer(CControlCallback keyPressedCallback, CControlCallback keyReleasedCallback) :
            mOnKeyPressedCallback(keyPressedCallback), mOnKeyReleasedCallback(keyReleasedCallback) {
        SDL_Init(SDL_INIT_EVERYTHING);
        video = SDL_SetVideoMode(640, 480, 0, 0);
    }

    void CRenderer::sleep(long ms) {
        SDL_Delay(33);
    }

    void CRenderer::handleSystemEvents() {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT) {
#ifndef __EMSCRIPTEN__
                exit(0);
#endif
            }

            if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case SDLK_q:
#ifndef __EMSCRIPTEN__
                        exit(0);
#endif
                    case SDLK_LEFT:
                        mOnKeyReleasedCallback(ECommand::kLeft);
                        break;

                    case SDLK_RIGHT:
                        mOnKeyReleasedCallback(ECommand::kRight);
                        break;

                    case SDLK_UP:
                        mOnKeyReleasedCallback(ECommand::kUp);
                        break;

                    case SDLK_DOWN:
                        mOnKeyReleasedCallback(ECommand::kDown);
                        break;
                    case SDLK_SPACE:
                        mOnKeyReleasedCallback(ECommand::kFire1);
                        break;
                    default:
                        break;
                }
            }

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        mOnKeyPressedCallback(ECommand::kLeft);
                        break;
                    case SDLK_RIGHT:
                        mOnKeyPressedCallback(ECommand::kRight);
                        break;

                    case SDLK_UP:
                        mOnKeyPressedCallback(ECommand::kUp);
                        break;

                    case SDLK_DOWN:
                        mOnKeyPressedCallback(ECommand::kDown);
                        break;
                    case SDLK_SPACE:
                        mOnKeyPressedCallback(ECommand::kFire1);
                        break;
                    default:
                        break;
                }
            }

        }
    }

    void CRenderer::render(const CGame &game, long ms) {

        SDL_Rect rect;

        switch (game.gameState) {
            case CGame::EGameState::kTitleScreen:
            case CGame::EGameState::kVictory:
            case CGame::EGameState::kGameOver:
            case CGame::EGameState::kGame:

                rect = {0, 0, 640, 241};
                SDL_FillRect(video, &rect, SDL_MapRGB(video->format, 0, 0, 255));



                int delta = 0;


                if (game.track[ game.elementIndex ] == ')') {
                    delta = -1;
                }

                if ( game.track[ game.elementIndex ] == '(') {
                    delta = 1;
                }

                int distance = 0;

                if ( game.distanceToNextElement > 50 ) {
                    distance = ( 100 - game.distanceToNextElement );
                } else {
                    distance = ( game.distanceToNextElement );
                }



                for ( int y = 0; y < 240; ++y ) {
                    int roadX = ( (240 * distance * delta) / (240-y) ) +  320 - (240 - y) - ( ((240 - y) * game.x) / 240);
                    int roadDeltaX = 32 + ((240 - y) * 2);
                    int shade = ( (240 - y) / 4);

                    rect = {0, (240 - y), 640, 1};
                    SDL_FillRect(video, &rect, SDL_MapRGB(video->format, 0, 0, 128 + shade / 2));


                    rect = {0, 240 + (240 - y), 640, 1};
                    SDL_FillRect(video, &rect, SDL_MapRGB(video->format, 0, 128 + shade / 2, 0));

                    rect = { roadX, 240 + (240 - y), roadDeltaX, 1};
                    SDL_FillRect(video, &rect, SDL_MapRGB(video->format, 64 + shade, 64 + shade, 64 + shade));

                }

                rect = SDL_Rect{0, 0, 80, 40};
                rect.x = 320 + (game.x );
                rect.y = 440;

                SDL_FillRect(video, &rect, SDL_MapRGB(video->format, 255, 0, 0));

                SDL_Flip(video);
        }
    }
}
