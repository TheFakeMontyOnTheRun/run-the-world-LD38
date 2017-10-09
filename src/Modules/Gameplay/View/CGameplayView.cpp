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

    Vec2 project(Vec3 v, Vec3 camera) {
        FixP one{1};
        auto xz = (v.x - camera.x) / (one - v.z + camera.z);
        auto yz = (v.y - camera.y) / (one - v.z + camera.z);

        Vec2 v2(FixP{320} + (xz * FixP{640}), FixP{240} - (yz * FixP{480}));
        return v2;
    }

    void CGameplayView::drawBackdropForHeading(int modulus, int zone) {
        getRenderer()->drawBitmapAt(-640 + modulus, 0, 640, 480, mBackdrop[zone]);
        getRenderer()->drawBitmapAt(modulus, 0, 640, 480, mBackdrop[zone]);
    }

    CGameplayView::CGameplayView(std::shared_ptr<CGameSession> session, std::shared_ptr<Vipper::IRenderer> renderer)
            : IView(renderer), mGameSession(session) {

        mBackdrop[0] = renderer->loadBitmap("res/3.png");
        mBackdrop[1] = renderer->loadBitmap("res/2.png");
        mBackdrop[2] = renderer->loadBitmap("res/1.png");

        mCar[0][0] = renderer->loadBitmap("res/big0.png");
        mCar[1][0] = renderer->loadBitmap("res/big1.png");
        mCar[2][0] = renderer->loadBitmap("res/big2.png");

        mCar[0][1] = renderer->loadBitmap("res/med0.png");
        mCar[1][1] = renderer->loadBitmap("res/med1.png");
        mCar[2][1] = renderer->loadBitmap("res/med2.png");

        mCar[0][2] = renderer->loadBitmap("res/small0.png");
        mCar[1][2] = renderer->loadBitmap("res/small1.png");
        mCar[2][2] = renderer->loadBitmap("res/small2.png");

        mOtherCar[0][0] = renderer->loadBitmap("res/obig0.png");
        mOtherCar[1][0] = renderer->loadBitmap("res/obig1.png");
        mOtherCar[2][0] = renderer->loadBitmap("res/obig2.png");

        mOtherCar[0][1] = renderer->loadBitmap("res/omed0.png");
        mOtherCar[1][1] = renderer->loadBitmap("res/omed1.png");
        mOtherCar[2][1] = renderer->loadBitmap("res/omed2.png");

        mOtherCar[0][2] = renderer->loadBitmap("res/osmall0.png");
        mOtherCar[1][2] = renderer->loadBitmap("res/osmall1.png");
        mOtherCar[2][2] = renderer->loadBitmap("res/osmall2.png");


        mSmoke = renderer->loadBitmap("res/smoke.png");

        mHitSound = renderer->loadSound("res/hit.wav");
        mBrakeSound = renderer->loadSound("res/brake.wav");
        mAccelerateSound = renderer->loadSound("res/accel.wav");

        mUIFont = renderer->loadFont("res/albaregular.ttf", 15);
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
        auto one = FixP{1};
        auto two = FixP{2};
        auto four = FixP{4};
        auto misterious = two;

        int numberOfStripeShades = 4;
        int completelyArbitraryCurveEasingFactor = 100;
        int shapeDelta = 0;
        char shape = game->track[game->elementIndex];
        char slope = game->slopes[game->elementIndex];
        Vec3 camera = Vec3( FixP{(game->x) / (640 * 2)}, halfPart, FixP{(game->carSpeed / 500)});

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

        if (game->distanceToNextElement > (CLevel::kSegmentLengthInMeters / 2)) {
            distanceToCurrentShape = (CLevel::kSegmentLengthInMeters - game->distanceToNextElement);
        } else {
            distanceToCurrentShape = (game->distanceToNextElement);
        }

        int halfScreenHeight = 480 / 2;

        float slopeAddedLines = (2 * CLevel::kSlopeHeightInMeters *
                                (distanceToCurrentShape / static_cast<float>(CLevel::kSegmentLengthInMeters)) *
                                slopeDelta);

        int modulus = static_cast<int>(game->mHeading * 640) % 640;

        while (modulus < 0) {
            modulus += 640;
        }


        int shadingStripesCount = 0;


//        drawBackdropForHeading( modulus, game->zone );

        renderer->drawSquare(0, halfScreenHeight - slopeAddedLines, 640, 480, {0, 64, 0, 255});
        renderer->drawSquare(0, 0, 640, halfScreenHeight - slopeAddedLines, {0, 0, 0, 255});


        Vec2 previousLeft(-1, -1);
        Vec2 previousRight(-1, -1);

        shadingStripesCount = 0;

        float segmentLength = static_cast<float>(CLevel::kSegmentLengthInMeters);

        auto initialSlope = CLevel::kSlopeHeightInMeters *
                             (distanceToCurrentShape / segmentLength ) * slopeDelta;
        float currentStripeHeight = initialSlope;
        float stripeHeightDelta = (-initialSlope) / 480;

        for (int y = halfScreenHeight; y > -1; y--) {
            shadingStripesCount = (shadingStripesCount + 1) % 1024;

            currentStripeHeight = initialSlope + ((2 * (halfScreenHeight - y)) * stripeHeightDelta);

            float curve =
                    (distanceToCurrentShape / static_cast<float>(CLevel::kSegmentLengthInMeters)) * shapeDelta * y * y /
                    completelyArbitraryCurveEasingFactor;

            Vec2 leftPoint = project(Vec3(curve - 1, -1 + currentStripeHeight, -y), camera);
            Vec2 rightPoint = project(Vec3(curve + 1, -1 + currentStripeHeight, -y), camera);

            //if it's valid
            if (previousLeft.y < 0) {
                previousLeft = leftPoint;
                previousRight = rightPoint;
            }

            int count = (-shadingStripesCount + static_cast<long>(game->distanceRan)) % numberOfStripeShades;

            int shade = ((count + 16) * 256) / 32;

            renderer->fill(leftPoint.x, rightPoint.x, leftPoint.y, previousLeft.x, previousRight.x, previousLeft.y,
                           {shade, shade, shade, 255});

            previousLeft = leftPoint;
            previousRight = rightPoint;
        }

        initialSlope = CLevel::kSlopeHeightInMeters *
                       (distanceToCurrentShape / static_cast<float>(CLevel::kSegmentLengthInMeters)) * slopeDelta;
        stripeHeightDelta = (-initialSlope) / 480;

        auto cars = game->getCarsAhead(1024);

        float playerLane = (game->x) / 640;

        for (auto foe : cars) {
            float y = std::get<0>(foe) - game->distanceRan;
            float curve =
                    (distanceToCurrentShape / static_cast<float>(CLevel::kSegmentLengthInMeters)) * shapeDelta * y * y /
                    completelyArbitraryCurveEasingFactor;

            float lane = std::get<1>(foe);
            currentStripeHeight = initialSlope + ((2 * (halfScreenHeight - y + 1)) * stripeHeightDelta);
            Vec2 carProjection0 = project(Vec3(curve + lane - 0.2f, -1 + currentStripeHeight, -y + 0), camera);
            Vec2 carProjection1 = project(Vec3(curve + lane + 0.2f, -1 + currentStripeHeight, -y + 0), camera);
            currentStripeHeight = initialSlope + ((2 * (halfScreenHeight - y - 1)) * stripeHeightDelta);
            Vec2 carProjection2 = project(Vec3(curve + lane - 0.2f, -1 + currentStripeHeight, -y - 1), camera);
            Vec2 carProjection3 = project(Vec3(curve + lane + 0.2f, -1 + currentStripeHeight, -y - 1), camera);

            int carSprite = (lane + 1) + 1;
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
            auto lane = FixP{game->x} / FixP{640};

            auto fixCurrentStripeHeight = initialSlope + ((2 * (halfScreenHeight - 4)) * stripeHeightDelta);
            Vec2 carProjection2 = project(Vec3(lane - halfPart, -one + fixCurrentStripeHeight, -two - one), camera);
            Vec2 carProjection3 = project(Vec3(lane + halfPart, -one + fixCurrentStripeHeight, -two - one), camera);
            currentStripeHeight = initialSlope + ((two * (halfScreenHeight - misterious)) * stripeHeightDelta);
            Vec2 carProjection0 = project(Vec3(lane - halfPart, -one + fixCurrentStripeHeight, -two), camera);
            Vec2 carProjection1 = project(Vec3(lane + halfPart, -one + fixCurrentStripeHeight, -two), camera);

            auto centerX = carProjection0.x + ((carProjection1.x - carProjection0.x) / two);
            auto centerY = carProjection0.y + ((carProjection2.y - carProjection0.y) / two);
            int carSprite = (static_cast<int>(lane) + 1) + 1;
            renderer->fill(static_cast<int>(carProjection0.x),
                           static_cast<int>(carProjection1.x),
                           static_cast<int>(carProjection0.y),
                           static_cast<int>(carProjection2.x),
                           static_cast<int>(carProjection3.x),
                           static_cast<int>(centerY),
                           {0, 0, 0, 0});

            renderer->drawBitmapAt(static_cast<int>(centerX) - 64, static_cast<int>(centerY) - 32, 128, 32, mCar[carSprite][0]);

            if (game->smoking) {
                renderer->drawBitmapAt(static_cast<int>(centerX) - 64, static_cast<int>(centerY), 100, 33, mSmoke);
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
    }

    void CGameplayView::onClick(std::pair<int, int> position) {
        if (position.first > 420) {
            this->mGameSession->getLevel()->onKeyDown(Vipper::ECommand::kRight);
        } else if (position.first < 210) {
            this->mGameSession->getLevel()->onKeyDown(Vipper::ECommand::kLeft);
        }

        if (position.second > 320) {
            this->mGameSession->getLevel()->onKeyDown(Vipper::ECommand::kDown);
        } else if (position.second < 160) {
            this->mGameSession->getLevel()->onKeyDown(Vipper::ECommand::kUp);
        }
    }

    void CGameplayView::onKeyUp(Vipper::ECommand keyCode) {
        this->mGameSession->getLevel()->onKeyUp(keyCode);
    }

    void CGameplayView::onKeyDown(Vipper::ECommand keyCode) {
        this->mGameSession->getLevel()->onKeyDown(keyCode);
    }
}
