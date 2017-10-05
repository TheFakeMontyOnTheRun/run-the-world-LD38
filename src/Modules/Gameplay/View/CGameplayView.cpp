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

        mBackdrop[0] = renderer->loadBitmap( "res/3.png" );
        mBackdrop[1] = renderer->loadBitmap( "res/2.png" );
        mBackdrop[2] = renderer->loadBitmap( "res/1.png" );

        mCar[0][0] = renderer->loadBitmap( "res/big0.png" );
        mCar[1][0] = renderer->loadBitmap( "res/big1.png" );
        mCar[2][0] = renderer->loadBitmap( "res/big2.png" );

        mCar[0][1] = renderer->loadBitmap( "res/med0.png" );
        mCar[1][1] = renderer->loadBitmap( "res/med1.png" );
        mCar[2][1] = renderer->loadBitmap( "res/med2.png" );

        mCar[0][2] = renderer->loadBitmap( "res/small0.png" );
        mCar[1][2] = renderer->loadBitmap( "res/small1.png" );
        mCar[2][2] = renderer->loadBitmap( "res/small2.png" );

        mOtherCar[0][0] = renderer->loadBitmap( "res/obig0.png" );
        mOtherCar[1][0] = renderer->loadBitmap( "res/obig1.png" );
        mOtherCar[2][0] = renderer->loadBitmap( "res/obig2.png" );

        mOtherCar[0][1] = renderer->loadBitmap( "res/omed0.png" );
        mOtherCar[1][1] = renderer->loadBitmap( "res/omed1.png" );
        mOtherCar[2][1] = renderer->loadBitmap( "res/omed2.png" );

        mOtherCar[0][2] = renderer->loadBitmap( "res/osmall0.png" );
        mOtherCar[1][2] = renderer->loadBitmap( "res/osmall1.png" );
        mOtherCar[2][2] = renderer->loadBitmap( "res/osmall2.png" );


        mSmoke = renderer->loadBitmap( "res/smoke.png" );

		mHitSound = renderer->loadSound( "res/hit.wav" );
        mBrakeSound = renderer->loadSound( "res/brake.wav" );
        mAccelerateSound = renderer->loadSound( "res/accel.wav" );

        mUIFont = renderer->loadFont( "res/albaregular.ttf", 15 );
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
//		renderer->drawSquare( 0, 0, 640 - 64, 480, {0,0,0,255} );


                int numberOfStripeShades = 4;
                int completelyArbitraryCurveEasingFactor = 100;
                int shapeDelta = 0;
                int roadWidth = 1;
                char shape = game->track[game->elementIndex];
                char slope = game->slopes[game->elementIndex];
                auto camera = Vec3( (game->x)/ (640 * 2), 0.2f,  ( game->carSpeed / 500 ) );

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
                    renderer->fill( 0, 640, line, 0, 640, line + 1, {0, 64, 0, 0});
                }

                Vec2 previousLeft(-1, -1);
                Vec2 previousRight(-1, -1);

                shadingStripesCount = 0;

                float initialSlope = CLevel::kSlopeHeightInMeters * (distanceToCurrentShape/ static_cast<float>(CLevel::kSegmentLengthInMeters)) * slopeDelta;
                float currentStripeHeight = initialSlope;
                float stripeHeightDelta =  (-initialSlope) / 480.0f;

                for (int y = halfScreenHeight; y > -1; y--) {
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
                    renderer->fill( leftPoint.x, rightPoint.x, leftPoint.y, previousLeft.x, previousRight.x, previousLeft.y, {255 * ( count + 10 ) / 20, 255 * ( count + 10 ) / 20, 255 * ( count + 10 ) / 20, 255 } );

                    previousLeft = leftPoint;
                    previousRight = rightPoint;
                }

                initialSlope = CLevel::kSlopeHeightInMeters * (distanceToCurrentShape/ static_cast<float>(CLevel::kSegmentLengthInMeters)) * slopeDelta;
                stripeHeightDelta =  (-initialSlope) / 480;

                auto cars = game->getCarsAhead(1024);
                auto playerLane = (game->x)/640;

                for ( auto foe : cars ) {
                    auto y = std::get<0>(foe) - game->distanceRan;
                    float curve = (distanceToCurrentShape / static_cast<float>(CLevel::kSegmentLengthInMeters)) * shapeDelta * y * y / completelyArbitraryCurveEasingFactor;

                    auto lane = std::get<1>(foe);
                    currentStripeHeight = initialSlope + ( (2 * (halfScreenHeight - y + 1)) * stripeHeightDelta );
                    auto carProjection0 = project( Vec3( curve + lane - 0.2f, -1 + currentStripeHeight, -y + 0), camera);
                    auto carProjection1 = project( Vec3( curve + lane + 0.2f, -1 + currentStripeHeight, -y + 0), camera);
                    currentStripeHeight = initialSlope + ( (2 * (halfScreenHeight - y - 1)) * stripeHeightDelta );
                    auto carProjection2 = project( Vec3( curve + lane - 0.2f, -1 + currentStripeHeight, -y - 1), camera);
                    auto carProjection3 = project( Vec3( curve + lane + 0.2f, -1 + currentStripeHeight, -y - 1), camera);

                    int carSprite = (lane + 1) + 1;
                    int carSize = 0;

                    int sizeX = 32;
                    int sizeY = 8;

                    if ( y <= 5 ) {
                        carSize = 0;
                        sizeX = 128;
                        sizeY = 64;
                    } else if ( 5 <= y && y <= 25 ) {
                        carSize = 1;
                        sizeX = 64;
                        sizeY = 32;
                    } else if ( 25 <= y && y <= 75 ) {
                        carSize = 2;
                        sizeX = 32;
                        sizeY = 16;
                    } else {
                        continue;
                    }


                    auto centerX = carProjection0.x + ((carProjection1.x - carProjection0.x) / 2);
                    auto centerY = carProjection0.y + ((carProjection2.y - carProjection0.y) / 2);
                    renderer->fill( carProjection0.x, carProjection1.x, carProjection0.y, carProjection2.x, carProjection3.x, centerY, {0,0,0,0} );
                    renderer->drawBitmapAt(centerX - (sizeX/2), centerY - (sizeY/2), sizeX, sizeY, mOtherCar[carSprite][carSize]);
                }


                {
                    auto lane = (game->x) / 640;
                    currentStripeHeight = initialSlope + ( (2 * (halfScreenHeight -4)) * stripeHeightDelta );
                    auto carProjection2 = project(Vec3(lane -0.2f, -1 + currentStripeHeight, -2 -1), camera);
                    auto carProjection3 = project(Vec3(lane +0.2f, -1 + currentStripeHeight, -2 -1), camera);
                    currentStripeHeight = initialSlope + ( (2 * (halfScreenHeight -2.1)) * stripeHeightDelta );
                    auto carProjection0 = project(Vec3(lane -0.2f, -1 + currentStripeHeight, -2 ), camera);
                    auto carProjection1 = project(Vec3(lane +0.2f, -1 + currentStripeHeight, -2 ), camera);

                    int black[3] = {0,0,0};
                    auto centerX = carProjection0.x + ((carProjection1.x - carProjection0.x) / 2);
                    auto centerY = carProjection0.y + ((carProjection2.y - carProjection0.y) / 2);
                    int carSprite = (lane + 1) + 1;
                    renderer->fill( carProjection0.x, carProjection1.x, carProjection0.y, carProjection2.x, carProjection3.x, centerY, {0,0,0,0} );
                    renderer->drawBitmapAt(centerX - 64, centerY - 32, 128, 32, mCar[carSprite][0]);

                    if (game->smoking) {
                        renderer->drawBitmapAt(centerX - 64, centerY, 100, 33, mSmoke);
                    }
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
        if ( position.first > 420 ) {
            this->mGameSession->getLevel()->onKeyDown(Vipper::ECommand::kRight);
        } else if ( position.first < 210 ) {
            this->mGameSession->getLevel()->onKeyDown(Vipper::ECommand::kLeft);
        }

        if ( position.second > 320 ) {
            this->mGameSession->getLevel()->onKeyDown(Vipper::ECommand::kDown);
        } else if ( position.second < 160 ) {
            this->mGameSession->getLevel()->onKeyDown(Vipper::ECommand::kUp);
        }
	}

	void CGameplayView::onKeyUp( Vipper::ECommand keyCode ) {
        this->mGameSession->getLevel()->onKeyUp(keyCode);
	}

    void CGameplayView::onKeyDown( Vipper::ECommand keyCode ) {
        this->mGameSession->getLevel()->onKeyDown(keyCode);
    }
}
