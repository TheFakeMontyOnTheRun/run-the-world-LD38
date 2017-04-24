#include <functional>
#include <utility>
#include <array>
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

    SDL_Surface *car[3][3];

    CRenderer::CRenderer(CControlCallback keyPressedCallback, CControlCallback keyReleasedCallback) :
            mOnKeyPressedCallback(keyPressedCallback), mOnKeyReleasedCallback(keyReleasedCallback) {
        SDL_Init(SDL_INIT_EVERYTHING);
        video = SDL_SetVideoMode(640, 480, 0, 0);
        backdrop[0] = SDL_LoadBMP( "res/1.bmp" );
        backdrop[1] = SDL_LoadBMP( "res/2.bmp" );
        backdrop[2] = SDL_LoadBMP( "res/3.bmp" );

        car[0][0] = SDL_LoadBMP( "res/big0.bmp" );
        car[1][0] = SDL_LoadBMP( "res/big1.bmp" );
        car[2][0] = SDL_LoadBMP( "res/big2.bmp" );

        car[0][1] = SDL_LoadBMP( "res/med0.bmp" );
        car[1][1] = SDL_LoadBMP( "res/med1.bmp" );
        car[2][1] = SDL_LoadBMP( "res/med2.bmp" );

        car[0][2] = SDL_LoadBMP( "res/small0.bmp" );
        car[1][2] = SDL_LoadBMP( "res/small1.bmp" );
        car[2][2] = SDL_LoadBMP( "res/small2.bmp" );


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

                    auto line = ( halfScreenHeight - slopeAddedLines ) + y;
                    int shade[3] = { 0, (y / 4), 0 };
                    fill( 0, 640, line, 0, 640, line + 1, shade );
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

                    int count = (- shadingStripesCount + static_cast<long>(game.distanceRan) ) % numberOfStripeShades;
                    int shade[3] = {255 * ( count + 10 ) / 20, 255 * ( count + 10 ) / 20, 255 * ( count + 10 ) / 20 };
                    fill( leftPoint.x, rightPoint.x, leftPoint.y, previousLeft.x, previousRight.x, previousLeft.y, shade );

                    previousLeft = leftPoint;
                    previousRight = rightPoint;
                }

                initialSlope = CGame::kSlopeHeightInMeters * (distanceToCurrentShape/ static_cast<float>(CGame::kSegmentLengthInMeters)) * slopeDelta;
                stripeHeightDelta =  (-initialSlope) / 480.0f;

                auto cars = game.getCarsAhead(1024);
                auto playerLane = (game.x - 320)/320.0f;

                for ( auto foe : cars ) {
                    auto y = std::get<0>(foe) - game.distanceRan;
                    float curve = (distanceToCurrentShape / static_cast<float>(CGame::kSegmentLengthInMeters)) * shapeDelta * y * y / completelyArbitraryCurveEasingFactor;
                    currentStripeHeight = initialSlope + ( (2.0f * (halfScreenHeight - y)) * stripeHeightDelta );
                    auto lane = std::get<1>(foe);
                    auto carProjection0 = project( Vec3( curve + lane + 0.0f, -1.0f + currentStripeHeight, -y), camera);
                    auto carProjection1 = project( Vec3( curve + lane + 1.0f, -1.0f + currentStripeHeight, -y), camera);
                    auto size = carProjection1.x - carProjection0.x;
                    int carSprite = std::max( std::min( static_cast<int>( lane - playerLane + 1), 2), 0 );
                    int carSize = 0;

                    if ( size >= 75 ) {
                        carSize = 0;
                        size = 100;
                    } else if ( 40 <= size && size <= 75 ) {
                        carSize = 1;
                        size = 50;
                    } else {
                        carSize = 2;
                        size = 25;
                    }

                    rect.x = carProjection0.x;
                    rect.y = carProjection0.y - (size / 2);
                    rect.w = size;
                    rect.h = size / 2;

                    SDL_BlitSurface(car[ carSprite ][carSize], nullptr, video, &rect );
                }

                currentStripeHeight = initialSlope + ( (2.0f * (halfScreenHeight)) * stripeHeightDelta );
                auto carProjection = project(Vec3( (game.x)/ 640.0f, -1.0f + currentStripeHeight, -3.0f), camera);

                rect.x = carProjection.x - 50;
                rect.y = carProjection.y - 26;
                rect.w = 100;
                rect.h = 53;

                int carSprite = std::max( std::min( static_cast<int>( (carProjection.x - 160 ) / 160.0f), 2), 0 );
                SDL_BlitSurface(car[ carSprite ][0], nullptr, video, &rect );

                SDL_Flip(video);
        }
    }

    void CRenderer::fill(float x0, float x1, float y0, float x2, float x3, float y1, int shade[3]) {
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

            rect.x = x;
            rect.y = line;
            rect.w = ( fx - x);
            rect.h = 1;

            x -= ratiox0x2;
            fx -= ratiox1x3;

            SDL_FillRect(video, &rect, SDL_MapRGB(video->format, shade[0], shade[1],  shade[2] ));
        }
    }

    void CRenderer::drawBackdropForHeading(int modulus, int zone ) {

        SDL_Rect rectSrc;
        SDL_Rect rectDst;

        rectDst.x = modulus;
        rectDst.y = 0;
        rectDst.w = 640 - modulus;
        rectDst.h = 480;

        SDL_BlitSurface(backdrop[ zone ], nullptr, video, &rectDst );

        rectDst.x = 0;
        rectDst.y = 0;
        rectDst.w = modulus;
        rectDst.h = 480;

        rectSrc.x = 640 - modulus;
        rectSrc.y = 0;
        rectSrc.w = modulus;
        rectSrc.h = 480;

        SDL_BlitSurface(backdrop[ zone ], &rectSrc, video, &rectDst );
    }
}
