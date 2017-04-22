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

    struct vec3 {
        vec3(float aX, float aY, float aZ) : x(aX), y(aY), z(aZ) {
        }

        float x = 0;
        float y = 0;
        float z = 0;
    };

    struct vec2 {
        vec2(float aX, float aY) : x(aX), y(aY) {
        }

        float x = 0;
        float y = 0;
    };

    vec2 project(vec3 v) {
        float xz = v.x / (1.0f - v.z);
        float yz = v.y / (1.0f - v.z);

        vec2 v2(320 + (xz * 640), 240 - (yz * 480));
        return v2;
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

                char shape = game.track[game.elementIndex];
                if (shape == ')') {
                    delta = -1;
                }

                if (shape == '(') {
                    delta = 1;
                }

                int distance = 0;

                if (game.distanceToNextElement > 50) {
                    distance = (100 - game.distanceToNextElement);
                } else {
                    distance = (game.distanceToNextElement);
                }


                int perspectiveFactor = 0;
                int height = 240;

                for (int y = height; y > 0; y--) {
                    perspectiveFactor = y;

                    int curveFactor = ((height * distance * delta) / y);
                    int cameraFactor = -((y * game.x) / height);
                    int roadX = curveFactor + 320 - (perspectiveFactor);
                    int roadDeltaX = (perspectiveFactor * 2);
                    int shade = (y / 4);

                    rect = {0, y, 640, 1};
                    SDL_FillRect(video, &rect, SDL_MapRGB(video->format, 0, 0, 128 + shade / 2));

                    rect = {0, height + y, 640, 1};
                    SDL_FillRect(video, &rect, SDL_MapRGB(video->format, 0, 128 + shade / 2, 0));

                    rect = {roadX, height + y, roadDeltaX, 1};
                    SDL_FillRect(video, &rect, SDL_MapRGB(video->format, 64 + shade, 64 + shade, 64 + shade));

                    float curve = (distance / 20.0f) * delta * y * y / 50.0f;

                    auto v0 = project(vec3(-0.4 + curve, 0.5f, -y));
                    rect = {(int) v0.x, (int) v0.y, 3, 3};
                    SDL_FillRect(video, &rect, SDL_MapRGB(video->format, 255, 255, 255));


                    auto v1 = project(vec3(0.4 + curve, 0.5f, -y));
                    rect = {(int) v1.x, (int) v1.y, 5, 5};
                    SDL_FillRect(video, &rect, SDL_MapRGB(video->format, 255, 255, 0));

                    auto v2 = project(vec3(curve, -0.5f, -y));
                    rect = {(int) v2.x, (int) v2.y, 5, 5};
                    SDL_FillRect(video, &rect, SDL_MapRGB(video->format, 255, 0, 0));

                }




                rect = SDL_Rect{0, 0, 80, 40};
                rect.x = 320 + (game.x);
                rect.y = 440;

                SDL_FillRect(video, &rect, SDL_MapRGB(video->format, 255, 0, 0));

                SDL_Flip(video);
        }
    }
}
