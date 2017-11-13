#include <iostream>
#include <array>
#include <functional>
#include <map>
#include <set>
#include <algorithm>
#include "Vipper/Vipper.h"

#include "Modules/Gameplay/Entities/CLevel.h"
#include "Modules/Gameplay/Entities/CGameSession.h"
#include "Modules/Gameplay/View/CGameplayView.h"


namespace RunTheWorld {
    
    const static auto xRes =
#ifdef DOS
            320;
#else
            640;
#endif

    const static auto xHalfRes =
#ifdef DOS
            160;
#else
            320;
#endif


    const static auto yRes =
#ifdef DOS
            200;
#else
            480;
#endif

    const static auto yHalfRes =
#ifdef DOS
            100;
#else
            240;
#endif


    Vec2 project(Vec3 v, Vec3 camera) {
        FixP one{1};
        auto xz = (v.x - camera.x) / (one - v.z + camera.z);
        auto yz = (v.y - camera.y) / (one - v.z + camera.z);

        Vec2 v2(FixP{xHalfRes} + (xz * FixP{xRes}), FixP{yHalfRes} - (yz * FixP{yRes}));
        return v2;
    }

    void CGameplayView::drawBackdropForHeading(int modulus, int zone) {
        getRenderer()->drawBitmapAt(-xRes + modulus, 0, xRes, yRes, mBackdrop[zone]);
        getRenderer()->drawBitmapAt(modulus, 0, xRes, yRes, mBackdrop[zone]);
    }

    CGameplayView::CGameplayView(std::shared_ptr<CGameSession> session, std::shared_ptr<Vipper::IRenderer> renderer)
            : IView(renderer), mGameSession(session) {

        mBackdrop[0] = renderer->loadBitmap("3.png");
        mBackdrop[1] = renderer->loadBitmap("2.png");
        mBackdrop[2] = renderer->loadBitmap("1.png");

        mCar[0][0] = renderer->loadBitmap("big0.png");
        mCar[1][0] = renderer->loadBitmap("big1.png");
        mCar[2][0] = renderer->loadBitmap("big2.png");

        mCar[0][1] = renderer->loadBitmap("med0.png");
        mCar[1][1] = renderer->loadBitmap("med1.png");
        mCar[2][1] = renderer->loadBitmap("med2.png");

        mCar[0][2] = renderer->loadBitmap("small0.png");
        mCar[1][2] = renderer->loadBitmap("small1.png");
        mCar[2][2] = renderer->loadBitmap("small2.png");

        for ( int  direction = 0; direction < 3; ++direction ) {
            for ( int distance = 0; distance < 3; ++distance ) {
                auto sprite = renderer->getBitmapSize( mCar[ distance ][ direction ] );
                mWidth[ distance ][ direction ] = sprite.first;
                mHeight[ distance ][ direction ] = sprite.second;
            }
        }

        mOtherCar[0][0] = renderer->loadBitmap("obig0.png");
        mOtherCar[1][0] = renderer->loadBitmap("obig1.png");
        mOtherCar[2][0] = renderer->loadBitmap("obig2.png");

        mOtherCar[0][1] = renderer->loadBitmap("omed0.png");
        mOtherCar[1][1] = renderer->loadBitmap("omed1.png");
        mOtherCar[2][1] = renderer->loadBitmap("omed2.png");

        mOtherCar[0][2] = renderer->loadBitmap("osmall0.png");
        mOtherCar[1][2] = renderer->loadBitmap("osmall1.png");
        mOtherCar[2][2] = renderer->loadBitmap("osmall2.png");


        mSmoke = renderer->loadBitmap("smoke.png");

        mHitSound = renderer->loadSound("hit.wav");
        mBrakeSound = renderer->loadSound("brake.wav");
        mAccelerateSound = renderer->loadSound("accel.wav");

        mUIFont = renderer->loadFont("albaregular.ttf", 15);
    }

    void CGameplayView::drawTextAt(std::pair<int, int> position, std::string text) {
        auto renderer = getRenderer();
        renderer->drawTextAt(position.first, position.second, text, {0xFF, 0xFF, 0x00, 0xFF}, mUIFont);
    }

    void CGameplayView::drawGaugeAt(std::pair<int, int> position, int howFilled) {
        auto renderer = getRenderer();
        renderer->drawSquare(position.first, position.second, 100, position.second + 20, {0, 0, 255, 255});
        renderer->drawSquare(position.first, position.second, howFilled, position.second + 20, {255, 0, 0, 255});
    }

    void CGameplayView::show() {
        auto game = this->mGameSession->getLevel();

        auto renderer = getRenderer();

        auto halfPart = FixP{2} / FixP{10};
        auto zero = FixP{0};
        auto one = FixP{1};
        auto two = FixP{2};
        auto four = FixP{4};
        auto misterious = two;

        int numberOfStripeShades = 4;
        int completelyArbitraryCurveEasingFactor = 128;
        int shapeDelta = 0;
        char shape = game->track[game->elementIndex];
        char slope = game->slopes[game->elementIndex];
        Vec3 camera = Vec3( FixP{(game->x) / (xRes * 2)}, halfPart, FixP{(game->carSpeed / 500)});

        if (shape == ')') {
            shapeDelta = -1;
        }

        if (shape == '(') {
            shapeDelta = 1;
        }

        int slopeDelta = 0;

        if (slope == '/') {
            slopeDelta = 1;
        }

        if (slope == '\\') {
            slopeDelta = -1;
        }

        int distanceToCurrentShape = 0;

        if (game->distanceToNextElement > (CLevel::kSegmentLengthInMeters / 2)) {
            distanceToCurrentShape = (CLevel::kSegmentLengthInMeters - game->distanceToNextElement);
        } else {
            distanceToCurrentShape = (game->distanceToNextElement);
        }

        int halfScreenHeight = yRes / 2;

        int slopeAddedLines = ( (2 * CLevel::kSlopeHeightInMeters * slopeDelta * distanceToCurrentShape) / (CLevel::kSegmentLengthInMeters) );

        int modulus = static_cast<int>(game->mHeading * xRes) % xRes;

        while (modulus < 0) {
            modulus += xRes;
        }

        modulus = 0;

        int shadingStripesCount = 0;


        if (mDrawBackdrop) {
            drawBackdropForHeading( modulus, game->zone );
        } else {
            renderer->drawSquare(0, 0, xRes, halfScreenHeight - slopeAddedLines, {0, 0, 0, 255});
        }

        renderer->drawSquare(0, halfScreenHeight - slopeAddedLines, xRes, yRes, {0, 64, 0, 255});

        Vec2 previousLeft(-1, -1);
        Vec2 previousRight(-1, -1);

        shadingStripesCount = 0;

        int segmentLength = (CLevel::kSegmentLengthInMeters);


        float initialSlope = getInitialSlope(slopeDelta, distanceToCurrentShape, segmentLength);


        float stripeHeightDelta = (-initialSlope) / yRes;

        for (int y = 0; y < halfScreenHeight; ++y) {

            if ( --shadingStripesCount <= 0 ) {
                shadingStripesCount = 1024;
            }

            auto currentStripeHeight = initialSlope + ((2 * (halfScreenHeight - y)) * stripeHeightDelta);

            float curve = getCurve(completelyArbitraryCurveEasingFactor, shapeDelta, distanceToCurrentShape, y);

            Vec2 leftPoint = project(Vec3(FixP{curve} - one, -one + FixP{currentStripeHeight}, FixP{-y}), camera);
            Vec2 rightPoint = project(Vec3(FixP{curve} + one, -one + FixP{currentStripeHeight}, FixP{-y}), camera);

            //if it's valid
            if (previousLeft.y < 0) {
                previousLeft = leftPoint;
                previousRight = rightPoint;
            }

            int count = (-shadingStripesCount + (static_cast<long>(game->distanceRan) % ( CLevel::kSegmentLengthInMeters / 2 )  )) % numberOfStripeShades;

            int shade = ((count + 16) * 256) / 32;

            int diff = std::abs(static_cast<int>(leftPoint.y) - static_cast<int>(previousLeft.y) );
            if ( diff < 2) {
                renderer->drawSquare(
                                        static_cast<int>(leftPoint.x),
                                        static_cast<int>(leftPoint.y),
                                      static_cast<int>(rightPoint.x),
                                      static_cast<int>(leftPoint.y) + diff,
                               {shade, shade, shade, 255});
            } else {
                renderer->fill(
                               static_cast<int>(previousLeft.x),
                               static_cast<int>(previousRight.x),
                               static_cast<int>(previousLeft.y),
                               static_cast<int>(leftPoint.x),
                               static_cast<int>(rightPoint.x),
                               static_cast<int>(leftPoint.y),
                               {shade, shade, shade, 255});

            }

            previousLeft = leftPoint;
            previousRight = rightPoint;
        }

        for (auto foe : game->mCars) {

            int y = std::get<0>(foe) - game->distanceRan;

            if ( y < 0 ) {
                continue;
            }

            float curve = getCurve(completelyArbitraryCurveEasingFactor, shapeDelta, distanceToCurrentShape, y);

            int lane = std::get<1>(foe);
            auto curveAndLane = FixP{lane} + FixP{curve};
            auto fixedStripeHeight = -one + FixP{initialSlope + ((2 * (halfScreenHeight - y - 1)) * stripeHeightDelta)};
            auto bottom = FixP{-y};
            Vec2 carProjection0 = project(Vec3(curveAndLane - halfPart, fixedStripeHeight, bottom + zero), camera);
            Vec2 carProjection1 = project(Vec3(curveAndLane + halfPart, fixedStripeHeight, bottom + zero), camera);
            Vec2 carProjection2 = project(Vec3(curveAndLane - halfPart, fixedStripeHeight, bottom - one), camera);
            Vec2 carProjection3 = project(Vec3(curveAndLane + halfPart, fixedStripeHeight, bottom - one), camera);

            int carSprite = std::max( 0, std::min((static_cast<int>(lane) + 1), 2) );

            int carSize = 0;

            int sizeX = 32;
            int sizeY = 8;

            if (y <= 5) {
                carSize = 0;
                sizeX = 128;
                sizeY = 64;
            } else if (5 <= y && y <= 25) {
                carSize = 1;
                sizeX = 64;
                sizeY = 32;
            } else if (25 <= y && y <= 75) {
                carSize = 2;
                sizeX = 32;
                sizeY = 16;
            } else {
                continue;
            }

            sizeX = mWidth[ carSprite ][ carSize ];
            sizeY = mHeight[ carSprite ][ carSize ];


            auto centerX = carProjection0.x + ((carProjection1.x - carProjection0.x) / two);
            auto centerY = carProjection0.y + ((carProjection2.y - carProjection0.y) / two);

            renderer->fill(static_cast<int>(carProjection0.x),
                           static_cast<int>(carProjection1.x),
                           static_cast<int>(carProjection0.y),
                           static_cast<int>(carProjection2.x),
                           static_cast<int>(carProjection3.x),
                           static_cast<int>(centerY),
                           {0, 0, 0, 0});


            renderer->drawBitmapAt(static_cast<int>(centerX) - (sizeX / 2),
                                   static_cast<int>(centerY) - (sizeY / 2),
                                   sizeX,
                                   sizeY,
                                   mOtherCar[carSprite][carSize]);
        }


        {
            auto lane = FixP{game->x} / FixP{xRes};

            auto fixCurrentStripeHeight = FixP{ initialSlope + ((2 * (halfScreenHeight - 4)) * stripeHeightDelta) };
            Vec2 carProjection2 = project(Vec3(lane - halfPart, -one + fixCurrentStripeHeight, -two - one), camera);
            Vec2 carProjection3 = project(Vec3(lane + halfPart, -one + fixCurrentStripeHeight, -two - one), camera);
            Vec2 carProjection0 = project(Vec3(lane - halfPart, -one + fixCurrentStripeHeight, -two), camera);
            Vec2 carProjection1 = project(Vec3(lane + halfPart, -one + fixCurrentStripeHeight, -two), camera);

            auto centerX = carProjection0.x + ((carProjection1.x - carProjection0.x) / two);
            auto centerY = carProjection0.y + ((carProjection2.y - carProjection0.y) / two);

            renderer->fill(static_cast<int>(carProjection0.x),
                           static_cast<int>(carProjection1.x),
                           static_cast<int>(carProjection0.y),
                           static_cast<int>(carProjection2.x),
                           static_cast<int>(carProjection3.x),
                           static_cast<int>(centerY),
                           {0, 0, 0, 0});

            int carSprite = std::max( 0, std::min((static_cast<int>(lane) + 1), 2) );

            renderer->drawBitmapAt(static_cast<int>(centerX) - (mWidth[ carSprite ][ 0 ] / 2), static_cast<int>(centerY) - (mHeight[ carSprite ][ 0 ] / 2), 128, 32, mCar[carSprite][0]);

            if (game->smoking) {
                renderer->drawBitmapAt(static_cast<int>(centerX) - (mWidth[ carSprite ][ 0 ] / 2), static_cast<int>(centerY), 100, 33, mSmoke);
            }
        }


        if (game->hit) {
            renderer->playSound(mHitSound);
            game->hit = false;
        }

        if (game->brake) {
            renderer->playSound(mBrakeSound);
            game->brake = false;
        }

        if (game->accel) {
            renderer->playSound(mAccelerateSound);
            game->accel = false;
        }

        auto barWidth = ( xRes * this->mGameSession->getLevel()->timeLeft ) / this->mGameSession->getLevel()->totalTimeForRound;
        renderer->drawSquare(0, 0, barWidth, 16, {255, 0, 0, 0 } );
    }

    float CGameplayView::getInitialSlope(int slopeDelta, int distanceToCurrentShape, int segmentLength) const {
        return ( CLevel::kSlopeHeightInMeters * distanceToCurrentShape * slopeDelta / segmentLength );
    }

    float CGameplayView::getCurve(int completelyArbitraryCurveEasingFactor, int shapeDelta, int distanceToCurrentShape, int y) const {
        return (128 * (distanceToCurrentShape * shapeDelta * y * y )) / (completelyArbitraryCurveEasingFactor * CLevel::kSegmentLengthInMeters) / 128.0f;
    }

    void CGameplayView::onClick(const std::pair<int, int>& position) {
        if (position.first > 420) {
            this->mGameSession->getLevel()->onKeyDown(Vipper::ECommand::kRight);
        } else if (position.first < 210) {
            this->mGameSession->getLevel()->onKeyDown(Vipper::ECommand::kLeft);
        }

        if (position.second > xHalfRes) {
            this->mGameSession->getLevel()->onKeyDown(Vipper::ECommand::kDown);
        } else if (position.second < 160) {
            this->mGameSession->getLevel()->onKeyDown(Vipper::ECommand::kUp);
        }
    }

    void CGameplayView::onKeyUp(const Vipper::ECommand& keyCode) {
        this->mGameSession->getLevel()->onKeyUp(keyCode);
    }

    void CGameplayView::onKeyDown(const Vipper::ECommand& keyCode) {
        this->mGameSession->getLevel()->onKeyDown(keyCode);
    }
}
