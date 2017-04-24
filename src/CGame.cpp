#include <functional>
#include <utility>
#include <tuple>
#include <vector>
#include <array>
#include <iostream>
#include <random>
#include <algorithm>
#include "Common.h"
#include "CGame.h"

namespace odb {

    void CGame::reset() {
        timeEllapsed = 0;
    }

    CGame::CGame() {
        gameState = EGameState::kGame;
        reset();

        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_real_distribution<> speedRandom(1.0f, 50.0f);
        std::uniform_real_distribution<> laneRandom(-1.0f, 1.0f);
        std::uniform_real_distribution<> positionRandom( 10, track.size() * kSegmentLengthInMeters * kZones);

        for( int c = 0; c < 200; ++c ) {
            mCars.push_back( std::make_tuple( positionRandom( gen ), laneRandom( gen ), speedRandom( gen ) ) );
        }
    }

    void CGame::tick(long ms) {
        timeEllapsed += ms;

        //20 km/h
        //1h = 60 minutes * 60 seconds * 1000 ms
        float secondsSinceLastTick = ms / 1000.0f;
        float distanceSinceLastTick = carSpeed * secondsSinceLastTick;
        timeLeft -= ms;
        distanceRan += distanceSinceLastTick;
        distanceToNextElement -= distanceSinceLastTick;

        smoking = ( carSpeed > 0 && carSpeed < 20 );

        auto playerLane = x / 640.0f;



        for ( auto& car : mCars ) {
            auto oldPosition = std::get<0>(car);
            auto speed = std::get<2>(car);
            auto newPosition = oldPosition + ( speed * secondsSinceLastTick );
            std::get<0>(car) = newPosition;
            auto lane = std::get<1>(car);

            if ((oldPosition <= distanceRan && distanceRan <= newPosition) ||
                (oldPosition <= (distanceRan - distanceSinceLastTick) &&
                 (distanceRan - distanceSinceLastTick) <= newPosition)) {

                if (((playerLane - 0.2f) <= (lane - 0.2f) && (lane - 0.2f) <= (playerLane + 0.2f)) ||
                    ((playerLane - 0.2f) <= (lane + 0.2f) && (lane + 0.2f) <= (playerLane + 0.2f))) {
                    carSpeed = 0;
                }
            }
        }

        if ( timeLeft <= 0 ) {
            std::cout << "TIME UP!" << std::endl;
            exit(0);
        }

        if (distanceToNextElement <= 0 ) {
            distanceToNextElement = kSegmentLengthInMeters;
            elementIndex++;
        }

        if ( elementIndex >= track.size() ) {
            elementIndex = 0;
            ++zone;
            timeLeft += track.size() * 20 * 1000;
            std::cout << "time:" << (timeLeft / 1000 ) << std::endl;
        }

        x += xSpeed * carSpeed * 0.2f;

        if ( distanceToNextElement < ( kSegmentLengthInMeters / 2) ) {

            char shape = track[elementIndex];
            int shapeDelta = 0;

            if (shape == ')') {
                shapeDelta = -1;
                x+= 0.15f * carSpeed;
            }

            if (shape == '(') {
                shapeDelta = 1;
                x-= 0.15f * carSpeed;
            }

            mHeading -= carSpeed * (shapeDelta / 1000.0f) + ( x / (640.0f * 10000.0f) );
        }


        if ( x < -320 || x > 320 ) {
            std::cout << "outside " << x << std::endl;
            carSpeed = std::max( carSpeed - 1, 0);
            smoking = smoking || true;
        }
    }

    CControlCallback CGame::getKeyPressedCallback() {
        return [&](ECommand command) {
            if (command == ECommand::kLeft) {
                std::cout << "left pressed" << std::endl;
                xSpeed = -1;

                char shape = track[elementIndex];
                if (shape == ')') {
                    smoking = true;
                }
            }

            if (command == ECommand::kRight) {
                std::cout << "Right pressed" << std::endl;
                xSpeed = 1;

                char shape = track[elementIndex];
                if (shape == '(') {
                    smoking = true;
                }
            }
            if (command == ECommand::kUp) {
                std::cout << "up pressed" << std::endl;
            }

            if (command == ECommand::kDown) {
                std::cout << "down pressed" << std::endl;
            }

            if (command == ECommand::kFire1) {
                std::cout << "fire1 pressed" << std::endl;
            }
        };
    }

    CControlCallback CGame::getKeyReleasedCallback() {
        return [&](ECommand command) {

            xSpeed = 0;

            switch (gameState) {
                case EGameState::kGame:

                    if (command == ECommand::kLeft) {
                        std::cout << "left released" << std::endl;
                    }

                    if (command == ECommand::kRight) {
                        std::cout << "right released" << std::endl;
                    }

                    if (command == ECommand::kUp) {
                        std::cout << "up released" << std::endl;
                        carSpeed = std::min( carSpeed + 5, 50);
                    }

                    if (command == ECommand::kDown) {
                        std::cout << "down released" << std::endl;
                        carSpeed = std::max( carSpeed - 5, 0);
                        smoking = true;
                    }

                    if (command == ECommand::kFire1) {
                        std::cout << "fire1 released" << std::endl;
                        carSpeed = 0;
                        smoking = true;
                    }
                    return;

                case EGameState::kGameOver:
                case EGameState::kVictory:
                    if (command == ECommand::kFire1) {
                        std::cout << "fire1 released" << std::endl;
                        gameState = EGameState::kGame;
                        reset();
                    }
                    return;

                case EGameState::kTitleScreen:
                    gameState = EGameState::kGame;
                    reset();
                    return;
            }
        };
    }

    std::vector<CCar> CGame::getCarsAhead(int range) const {
        std::vector<CCar> toReturn;

        int min = distanceRan;
        int max = distanceRan + range;

        for ( auto& car : mCars ) {
            if ( min <= std::get<0>(car) && std::get<0>(car) <= max ) {
                toReturn.push_back( car );
            }
        }

        std::sort( std::begin( toReturn), std::end( toReturn ), []( CCar c1, CCar c2 ) {
            return std::get<0>( c1 ) > std::get<0>(c2);
        });

        return toReturn;
    }
}
