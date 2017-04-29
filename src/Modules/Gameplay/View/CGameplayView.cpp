#include <iostream>
#include <array>
#include <functional>
#include <map>
#include <set>
#include <string>
#include <functional>
#include <algorithm>
#include "Vipper/Vipper.h"
#include <Modules/TitleScreen/View/CRect.h>

#include "Modules/Gameplay/Entities/CLevel.h"
#include "Modules/Gameplay/Entities/CGameSession.h"
#include "Modules/Gameplay/View/CGameplayView.h"


namespace RunTheWorld {

    Vec2 project(Vec3 v, Vec3 camera) {
        float xz = (v.x - camera.x) / (1.0f - v.z + camera.z);
        float yz = (v.y - camera.y) / (1.0f - v.z + camera.z);

        Vec2 v2(320 + (xz * 640), 240 - (yz * 480));
        return v2;
    }

    void CGameplayView::drawBackdropForHeading(int modulus, int zone  ) {
        getRenderer()->drawBitmapAt( -640 + modulus, 0, 640, 480, mBackdrop[ zone ] );
        getRenderer()->drawBitmapAt( modulus, 0, 640, 480, mBackdrop[ zone ] );
    }

	CGameplayView::CGameplayView(std::shared_ptr<CGameSession> session, std::shared_ptr<Vipper::IRenderer> renderer) : IView( renderer ), mGameSession( session ) {


        mBackdrop[0] = renderer->loadBitmap( "./res/3.png" );
        mBackdrop[1] = renderer->loadBitmap( "./res/2.png" );
        mBackdrop[2] = renderer->loadBitmap( "./res/1.png" );

        mCar[0][0] = renderer->loadBitmap( "./res/big0.png" );
        mCar[1][0] = renderer->loadBitmap( "./res/big1.png" );
        mCar[2][0] = renderer->loadBitmap( "./res/big2.png" );

        mCar[0][1] = renderer->loadBitmap( "./res/med0.png" );
        mCar[1][1] = renderer->loadBitmap( "./res/med1.png" );
        mCar[2][1] = renderer->loadBitmap( "./res/med2.png" );

        mCar[0][2] = renderer->loadBitmap( "./res/small0.png" );
        mCar[1][2] = renderer->loadBitmap( "./res/small1.png" );
        mCar[2][2] = renderer->loadBitmap( "./res/small2.png" );

        mSmoke = renderer->loadBitmap( "./res/smoke.png" );

		mHitSound = renderer->loadSound( "./res/hit.wav" );
        mBrakeSound = renderer->loadSound( "./res/brake.wav" );
        mAccelerateSound = renderer->loadSound( "./res/accel.wav" );

        mUIFont = renderer->loadFont( "./res/albaregular.ttf", 15 );
	}
	
    void CGameplayView::drawTextAt( std::pair<int, int> position, std::string text ) {
		auto renderer = getRenderer();
		renderer->drawTextAt( position.first, position.second, text, {0xFF, 0xFF, 0x00, 0xFF }, mUIFont  );
	}
	
    void CGameplayView::drawGaugeAt( std::pair<int, int> position, int howFilled) {
		auto renderer = getRenderer();
		renderer->drawSquare(position.first, position.second, 100, position.second + 20, {0,0,255,255});
		renderer->drawSquare(position.first, position.second, howFilled, position.second + 20, {255,0,0,255});
	}
	
    void CGameplayView::show() {
        auto game = this->mGameSession->getLevel();
        
		auto renderer = getRenderer();
		renderer->drawSquare( 0, 0, 640 - 64, 480, {0,0,0,255} );


                int numberOfStripeShades = 4;
                float completelyArbitraryCurveEasingFactor = 100.0f;
                int shapeDelta = 0;
                float roadWidth = 1.0f;
                char shape = game->track[game->elementIndex];
                char slope = game->slopes[game->elementIndex];
                auto camera = Vec3( 0.5f * (game->x)/ 640.0f, 0.2f, 0.1f * ( game->carSpeed / 50.0f ) );

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

                if (game->distanceToNextElement > (CLevel::kSegmentLengthInMeters/2)) {
                    distanceToCurrentShape = (CLevel::kSegmentLengthInMeters - game->distanceToNextElement);
                } else {
                    distanceToCurrentShape = (game->distanceToNextElement);
                }

                int halfScreenHeight = 480 / 2;

                auto slopeAddedLines = ( 2 * CLevel::kSlopeHeightInMeters * (distanceToCurrentShape/static_cast<float>(CLevel::kSegmentLengthInMeters)) * slopeDelta  );

                int modulus = static_cast<int>(game->mHeading * 640) % 640;

                while ( modulus < 0 ) {
                    modulus += 640;
                }


                drawBackdropForHeading( modulus, game->zone );

                int shadingStripesCount = 0;
                for (int y = 0; y < halfScreenHeight + slopeAddedLines + 1; ++y ) {
                    auto line = ( halfScreenHeight - slopeAddedLines ) + y;


                    int shade[3] = {0, 64, 0};
                    renderer->fill( 0, 640, line, 0, 640, line + 1, shade );
                }

                Vec2 previousLeft(-1, -1);
                Vec2 previousRight(-1, -1);

                shadingStripesCount = 0;

                float initialSlope = CLevel::kSlopeHeightInMeters * (distanceToCurrentShape/ static_cast<float>(CLevel::kSegmentLengthInMeters)) * slopeDelta;
                float currentStripeHeight = initialSlope;
                float stripeHeightDelta =  (-initialSlope) / 480.0f;

                for (float y = halfScreenHeight; y > -1; y -= (0.5f )) {
                    shadingStripesCount = ( shadingStripesCount + 1) % 1024;

                    currentStripeHeight = initialSlope + ( (2.0f * (halfScreenHeight - y)) * stripeHeightDelta );

                    float curve = (distanceToCurrentShape / static_cast<float>(CLevel::kSegmentLengthInMeters)) * shapeDelta * y * y / completelyArbitraryCurveEasingFactor;
                    auto leftPoint = project(Vec3(-( roadWidth / 2.0f ) + curve, -0.5f + currentStripeHeight, -y), camera);
                    auto rightPoint = project(Vec3( (roadWidth / 2.0f ) + curve, -0.5f + currentStripeHeight, -y), camera);

                    //if it's valid
                    if ( previousLeft.y < 0 ) {
                        previousLeft = leftPoint;
                        previousRight = rightPoint;
                    }

                    int count = (- shadingStripesCount + static_cast<long>(game->distanceRan) ) % numberOfStripeShades;
                    int shade[3] = {255 * ( count + 10 ) / 20, 255 * ( count + 10 ) / 20, 255 * ( count + 10 ) / 20 };
                    renderer->fill( leftPoint.x, rightPoint.x, leftPoint.y, previousLeft.x, previousRight.x, previousLeft.y, shade );

                    previousLeft = leftPoint;
                    previousRight = rightPoint;
                }

                initialSlope = CLevel::kSlopeHeightInMeters * (distanceToCurrentShape/ static_cast<float>(CLevel::kSegmentLengthInMeters)) * slopeDelta;
                stripeHeightDelta =  (-initialSlope) / 480.0f;

                auto cars = game->getCarsAhead(1024);
                auto playerLane = (game->x)/640.0f;

                for ( auto foe : cars ) {
                    auto y = std::get<0>(foe) - game->distanceRan;
                    float curve = (distanceToCurrentShape / static_cast<float>(CLevel::kSegmentLengthInMeters)) * shapeDelta * y * y / completelyArbitraryCurveEasingFactor;
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

                    renderer->drawBitmapAt( carProjection0.x, carProjection0.y - (size / 2), size, size / 2, mCar[ carSprite ][carSize] );
                }

                currentStripeHeight = initialSlope + ( (2.0f * (halfScreenHeight)) * stripeHeightDelta );
                auto carProjection = project(Vec3( (game->x)/ 640.0f, -1.0f + currentStripeHeight, -3.0f), camera);
                int carSprite = std::max( std::min( static_cast<int>( (carProjection.x - 160 ) / 160.0f), 2), 0 );

                renderer->drawBitmapAt( carProjection.x - 128, carProjection.y - 32, 100, 53, mCar[ carSprite ][0] );

                if ( game->smoking ) {
                    renderer->drawBitmapAt( carProjection.x - 128, carProjection.y, 100, 33, mSmoke );
                }



        if ( game->hit ) {
            renderer->playSound( mHitSound );
            game->hit = false;
        }

        if ( game->brake ) {
            renderer->playSound( mBrakeSound );
            game->brake = false;
        }

        if ( game->accel ) {
            renderer->playSound( mAccelerateSound );
            game->accel = false;
        }
    }

	void CGameplayView::onClick( std::pair<int, int> position ) {
	}

	void CGameplayView::onKeyUp( Vipper::ECommand keyCode ) {
        this->mGameSession->getLevel()->onKeyUp(keyCode);
	}

    void CGameplayView::onKeyDown( Vipper::ECommand keyCode ) {
        this->mGameSession->getLevel()->onKeyDown(keyCode);
    }
}
