#include <functional>
#include <utility>
#include <tuple>
#include <vector>
#include "Common.h"

#include "CGame.h"
#include "CRenderer.h"
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include <iostream>

namespace odb {

    SDL_Surface *video;
    SDL_Surface *backdrop[3];

    SDL_Surface *car[3];

    CRenderer::CRenderer(CControlCallback keyPressedCallback, CControlCallback keyReleasedCallback) :
            mOnKeyPressedCallback(keyPressedCallback), mOnKeyReleasedCallback(keyReleasedCallback) {
        SDL_Init(SDL_INIT_EVERYTHING);
        video = SDL_SetVideoMode(640, 480, 0, 0);
        backdrop[0] = SDL_LoadBMP( "res/1.bmp" );
        backdrop[1] = SDL_LoadBMP( "res/2.bmp" );
        backdrop[2] = SDL_LoadBMP( "res/3.bmp" );

        car[0] = SDL_LoadBMP( "res/lancia0.bmp" );
        car[1] = SDL_LoadBMP( "res/lancia1.bmp" );
        car[2] = SDL_LoadBMP( "res/lancia2.bmp" );

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

    Vec2 project(Vec3 v, Vec3 camera) {
        float xz = (v.x - camera.x) / (1.0f - v.z + camera.z);
        float yz = (v.y - camera.y) / (1.0f - v.z + camera.z);

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
                int numberOfStripeShades = 4;
                float completelyArbitraryCurveEasingFactor = 100.0f;
                int shapeDelta = 0;
                float roadWidth = 1.0f;
                char shape = game.track[game.elementIndex];
                char slope = game.slopes[game.elementIndex];
                auto camera = Vec3( 0.5f * (game.x)/ 640.0f, 0.2f, 0.1f * ( game.carSpeed / 50.0f ) );

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

                int distanceToCurrentShape = 0;

                if (game.distanceToNextElement > (CGame::kSegmentLengthInMeters/2)) {
                    distanceToCurrentShape = (CGame::kSegmentLengthInMeters - game.distanceToNextElement);
                } else {
                    distanceToCurrentShape = (game.distanceToNextElement);
                }

                int halfScreenHeight = 480 / 2;

                auto slopeAddedLines = ( 2 * CGame::kSlopeHeightInMeters * (distanceToCurrentShape/static_cast<float>(CGame::kSegmentLengthInMeters)) * slopeDelta  );

                int modulus = static_cast<int>(game.mHeading * 640) % 640;

                while ( modulus < 0 ) {
                    modulus += 640;
                }


                drawBackdropForHeading( modulus, game.zone );


                for (int y = 0; y < halfScreenHeight + slopeAddedLines + 1; ++y ) {
                    int shade = (y / 4);
                    rect = {0, ( halfScreenHeight - slopeAddedLines ) + y, 640, 1};
                    SDL_FillRect(video, &rect, SDL_MapRGB(video->format, 0, 128 + shade / 2, 0));
                }

                Vec2 previousLeft(-1, -1);
                Vec2 previousRight(-1, -1);

                int shadingStripesCount = 0;

                float initialSlope = CGame::kSlopeHeightInMeters * (distanceToCurrentShape/ static_cast<float>(CGame::kSegmentLengthInMeters)) * slopeDelta;
                float currentStripeHeight = initialSlope;
                float stripeHeightDelta =  (-initialSlope) / 480.0f;

                for (float y = halfScreenHeight; y > -1; y -= (0.5f )) {
                    shadingStripesCount = ( shadingStripesCount + 1) % 1024;

                    currentStripeHeight = initialSlope + ( (2.0f * (halfScreenHeight - y)) * stripeHeightDelta );

                    float curve = (distanceToCurrentShape / static_cast<float>(CGame::kSegmentLengthInMeters)) * shapeDelta * y * y / completelyArbitraryCurveEasingFactor;
                    auto leftPoint = project(Vec3(-( roadWidth / 2.0f ) + curve, -0.5f + currentStripeHeight, -y), camera);
                    auto rightPoint = project(Vec3( (roadWidth / 2.0f ) + curve, -0.5f + currentStripeHeight, -y), camera);

                    //if it's valid
                    if ( previousLeft.y < 0 ) {
                        previousLeft = leftPoint;
                        previousRight = rightPoint;
                    }

                    fill( leftPoint.x, rightPoint.x, leftPoint.y, previousLeft.x, previousRight.x, previousLeft.y, (- shadingStripesCount +
                            static_cast<long>(game.distanceRan) ) % numberOfStripeShades );

                    previousLeft = leftPoint;
                    previousRight = rightPoint;
                }

                auto carProjection = project(Vec3( (game.x)/ 640.0f, 0.0f, 0.5f), camera);
                rect = { carProjection.x - 50, carProjection.y - 26, 100, 53};
                int carSprite = std::max( std::min( static_cast<int>( (carProjection.x - 160 ) / 160.0f), 2), 0 );
                SDL_BlitSurface(car[ carSprite ], nullptr, video, &rect );
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

    void CRenderer::drawBackdropForHeading(int modulus, int zone ) {

        SDL_Rect rectSrc;
        SDL_Rect rectDst;

        rectDst = { modulus, 0, 640 - modulus, 480 };
        SDL_BlitSurface(backdrop[ zone ], nullptr, video, &rectDst );

        rectDst = { 0, 0, modulus, 480 };
        rectSrc = { 640 - modulus, 0, modulus, 480 };
        SDL_BlitSurface(backdrop[ zone ], &rectSrc, video, &rectDst );

    }
}
