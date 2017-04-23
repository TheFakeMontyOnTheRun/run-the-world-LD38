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

                rect = {0, 0, 640, 480};
                SDL_FillRect(video, &rect, SDL_MapRGB(video->format, 0, 0, 0));


                int delta = 0;

                char shape = game.track[game.elementIndex];
                char slope = game.slopes[game.elementIndex];

                if (shape == ')') {
                    delta = -1;
                }

                if (shape == '(') {
                    delta = 1;
                }

                float slopeDelta = 0;

                if (slope == '/') {
                    slopeDelta = 1;
                }

                if (slope == '\\') {
                    slopeDelta = -1;
                }



                int distance = 0;

                if (game.distanceToNextElement > 50) {
                    distance = (100 - game.distanceToNextElement);
                } else {
                    distance = (game.distanceToNextElement);
                }


                int perspectiveFactor = 0;
                int subida = 2;
                int descida = 1;
                int height = 240;
                vec2 p0(-1, -1);
                vec2 p1(-1, -1);

                int count = 0;

                float initialSlope = distance * slopeDelta;
                float acc = initialSlope;
                float deltaAcc =  (-initialSlope) / 480.0f;

                auto addedLines = ( distance / 50.0f * 80 * slopeDelta  );

                for (int y = 1; y < 240 - addedLines; ++y ) {
                    int shade = (y / 4);

                    rect = {0, y, 640, 1};
                    SDL_FillRect(video, &rect, SDL_MapRGB(video->format, 0, 0, 128 + shade / 2));
                }

                for (int y = 1; y < 240 + addedLines; ++y ) {
                    int shade = (y / 4);

                    rect = {0, ( 240 - addedLines ) + y, 640, 1};
                    SDL_FillRect(video, &rect, SDL_MapRGB(video->format, 0, 128 + shade / 2, 0));
                }


                for (float y = height; y > 0; y -= (0.5f )) {
                    count = ( count + 1) % 1024;

                    std::cout << game.distanceRan << std::endl;

                    acc += deltaAcc;


//                    rect = {roadX, height + y, roadDeltaX, 1};
//                    SDL_FillRect(video, &rect, SDL_MapRGB(video->format, 64 + shade, 64 + shade, 64 + shade));

                    float curve = (distance / 20.0f) * delta * y * y / 50.0f;


                    auto v0 = project(vec3(-0.4 + curve, -0.5f + acc, -y + 0.5f));
                    auto v1 = project(vec3(0.4 + curve, -0.5f + acc, -y + 0.5f));

                    if ( p0.y < 0 ) {
                        p0 = v0;
                        p1 = v1;
                    }

                    fill( v0.x, v1.x, v0.y, p0.x, p1.x, p0.y, (- count + ( game.distanceRan / 1000 ) ) % 4 );

                    p0 = v0;
                    p1 = v1;
                }

                rect = SDL_Rect{0, 0, 80, 40};
                rect.x = 320 + (game.x);
                rect.y = 440;

                SDL_FillRect(video, &rect, SDL_MapRGB(video->format, 255, 0, 0));

                SDL_Flip(video);
        }
    }

    void CRenderer::fill(float x0, float x1, float y0, float x2, float x3, float y1, int count) {
        float fromY = std::min( y0, y1 );
        float toY = std::max( y0, y1 );
        SDL_Rect rect;
        float deltaY = toY - fromY;

        float ratiox0x2 = 1.0f;
        float ratiox1x3 = 1.0f;

        if ( toY - fromY > 0.0f  ) {
            ratiox0x2 = ( x0 - x2 ) / deltaY;
            ratiox1x3 = ( x1 - x3 ) / deltaY;
        }

        float x = x0;
        float fx = x1;
        for ( int line = toY; line >= fromY; line--) {
            rect = { x, line, ( fx - x), 1};
            x -= ratiox0x2;
            fx -= ratiox1x3;
            int shade = 255 * ( count + 10 ) / 20.0f;
            SDL_FillRect(video, &rect, SDL_MapRGB(video->format, shade, shade,  shade ));

        }
    }
}
