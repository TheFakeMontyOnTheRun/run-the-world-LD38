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

    Vec2 project(Vec3 v, Vec2 camera) {
        float xz = (v.x - camera.x) / (1.0f - v.z);
        float yz = (v.y - camera.y) / (1.0f - v.z);

        Vec2 v2(320 + (xz * 640), 240 - (yz * 480));
        return v2;
    }


    void CRenderer::render(const CGame &game, long ms) {

        SDL_Rect rect;

        switch (game.gameState) {
            case CGame::EGameState::kTitleScreen:
            case CGame::EGameState::kVictory:
            case CGame::EGameState::kGameOver:
            case CGame::EGameState::kGame:

                int shapeDelta = 0;
                float roadWidth = 1.0f;
                char shape = game.track[game.elementIndex];
                char slope = game.slopes[game.elementIndex];
                auto camera = Vec2( 0.5f * (game.x - 320)/ 640.0f, 0.2f );

                if (shape == ')') {
                    shapeDelta = -1;
                }

                if (shape == '(') {
                    shapeDelta = 1;
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

                int height = 480 / 2;

                auto addedLines = ( distance / 50.0f * 100 * slopeDelta  );

                for (int y = 1; y < height - addedLines; ++y ) {
                    int shade = (y / 4);

                    rect = {0, y, 640, 1};
                    SDL_FillRect(video, &rect, SDL_MapRGB(video->format, 0, 0, 128 + shade / 2));
                }

                for (int y = 0; y < height + addedLines + 1; ++y ) {
                    int shade = (y / 4);

                    rect = {0, ( height - addedLines ) + y, 640, 1};
                    SDL_FillRect(video, &rect, SDL_MapRGB(video->format, 0, 128 + shade / 2, 0));
                }

                Vec2 p0(-1, -1);
                Vec2 p1(-1, -1);
                int count = 0;
                float initialSlope = distance * slopeDelta;
                float acc = initialSlope;
                float deltaAcc =  (-initialSlope) / 480.0f;

                for (float y = height; y > 0; y -= (0.5f )) {
                    count = ( count + 1) % 1024;

                    acc += deltaAcc;

                    float curve = (distance / 20.0f) * shapeDelta * y * y / 50.0f;
                    auto v0 = project(Vec3(-( roadWidth / 2.0f ) + curve, -0.5f + acc, -y + 0.5f), camera);
                    auto v1 = project(Vec3( (roadWidth / 2.0f ) + curve, -0.5f + acc, -y + 0.5f), camera);

                    if ( p0.y < 0 ) {
                        p0 = v0;
                        p1 = v1;
                    }

                    fill( v0.x, v1.x, v0.y, p0.x, p1.x, p0.y, (- count + ( game.distanceRan / 1000 ) ) % 4 );

                    p0 = v0;
                    p1 = v1;
                }

                auto car = project(Vec3( (game.x - 320)/ 640.0f, 0.0f, 0.5f), camera);
                rect = { car.x - 40, car.y - 20, 80, 40};

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
