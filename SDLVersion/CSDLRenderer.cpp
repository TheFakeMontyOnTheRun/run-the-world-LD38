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
        rect = {0, 0, 640, 480};
        int px = game.x / 40;
        int py = game.y / 40;

        SDL_FillRect(video, &rect, SDL_MapRGB(video->format, 0, 128, 0));

        switch (game.gameState) {
            case CGame::EGameState::kTitleScreen:
            case CGame::EGameState::kVictory:
            case CGame::EGameState::kGameOver:
            case CGame::EGameState::kGame:

                rect = {0, 0, 640, 241};
                SDL_FillRect(video, &rect, SDL_MapRGB(video->format, 0, 0, 255));

                rect = {0, 241, 640, 240};
                SDL_FillRect(video, &rect, SDL_MapRGB(video->format, 0, 255, 0));

                for ( int y = 0; y < 240; ++y ) {
                    rect = {320 - (240 - y) - ( ((240 - y) * game.x) / 240), 240 + (240 - y), 32 + ((240 - y) * 2), 1};
                    SDL_FillRect(video, &rect, SDL_MapRGB(video->format, 64, 64, 64));
                }

                rect = SDL_Rect{0, 0, 80, 40};
                rect.x = 320 + (game.x );
                rect.y = 440;

                SDL_FillRect(video, &rect, SDL_MapRGB(video->format, 255, 0, 0));

                SDL_Flip(video);
        }
    }
}
