#include <functional>
#include <utility>
#include <tuple>
#include <vector>
#include <array>
#include <iostream>
#include <random>
#include <algorithm>
#include "Common.h"
#include <array>
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "Vipper/Vipper.h"
#include "Modules/Gameplay/Entities/CLevel.h"

namespace RunTheWorld {

    void CLevel::reset() {
        timeEllapsed = 0;
    }

    CLevel::CLevel() {
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

    void CLevel::tick(long ms) {
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

            auto pz0 = distanceRan;
            auto pz1 = distanceRan + 1;
            auto cz0 = newPosition;
            auto cz1 = newPosition + 1;

            if ((cz0 <= pz0 && pz0 <= cz1) ||
                ( pz0 <= cz0 && cz0 <= pz1 )||
                        (cz0 <= pz1 && pz1 <= cz1) ||
                        ( pz0 <= cz1 && cz1 <= pz1) ) {

                if (((playerLane - 0.2f) <= (lane - 0.2f) && (lane - 0.2f) <= (playerLane + 0.2f)) ||
                    ((playerLane - 0.2f) <= (lane + 0.2f) && (lane + 0.2f) <= (playerLane + 0.2f))) {
                    carSpeed = 0.0;
                    std::cout << "COLLISION!" << std::endl;
                    std::get<0>(car) = distanceRan + std::get<2>(car) / 2 + 10 + carSpeed / 2;
                    std::get<2>(car) += carSpeed / 2;
                    hit = true;
                }
            }
        }

        if ( timeLeft <= 0 ) {
            std::cout << "TIME UP!" << std::endl;
            mIsOver = true;
        }

        if (distanceToNextElement <= 0 ) {
            distanceToNextElement = kSegmentLengthInMeters;
            elementIndex++;
        }

        if ( elementIndex >= track.size() ) {
            elementIndex = 0;
            zone = (zone + 1) % kZones;
            timeLeft += track.size() * 30 * 1000;
            std::cout << "time:" << (timeLeft / 1000 ) << std::endl;
        }

        if ( carSpeed > 0 ) {
            x += xSpeed * (carSpeed + 2) * 0.2f;
        }

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


        if ( x < -640 || x > 640 ) {
            std::cout << "outside " << x << std::endl;
            carSpeed = std::max( carSpeed - 1, 0);
            smoking = smoking || true;
        }
    }

    bool CLevel::isOver() {
        return mIsOver;
    }

    void CLevel::onKeyUp( Vipper::ECommand command ) {

        if (command == Vipper::ECommand::kLeft) {
            std::cout << "left released" << std::endl;
        }

        if (command == Vipper::ECommand::kRight) {
            std::cout << "right released" << std::endl;
        }

        if (command == Vipper::ECommand::kUp) {
            std::cout << "up released" << std::endl;
        }

        if (command == Vipper::ECommand::kDown) {
            std::cout << "down released" << std::endl;
        }

        if (command == Vipper::ECommand::kFire1) {
            std::cout << "fire1 released" << std::endl;
            carSpeed = 0;
            smoking = true;
        }
        return;
    }

    void CLevel::onKeyDown( Vipper::ECommand command ) {
        if (command == Vipper::ECommand::kLeft) {
            std::cout << "left pressed" << std::endl;
            xSpeed = -1;

            char shape = track[elementIndex];
            if (shape == ')') {
                smoking = true;
            }
        }

        if (command == Vipper::ECommand::kRight) {
            std::cout << "Right pressed" << std::endl;
            xSpeed = 1;

            char shape = track[elementIndex];
            if (shape == '(') {
                smoking = true;
            }
        }
        if (command == Vipper::ECommand::kUp) {
            std::cout << "up pressed" << std::endl;
            carSpeed = std::min( carSpeed + 5, 50);
            if ( carSpeed < 50 ) {
                accel = true;
            }
        }

        if (command == Vipper::ECommand::kDown) {
            std::cout << "down pressed" << std::endl;
            carSpeed = std::max( carSpeed - 5, 0);
            smoking = true;
            if ( carSpeed > 0 ) {
                brake = true;
            }
        }

        if (command == Vipper::ECommand::kFire1) {
            std::cout << "fire1 pressed" << std::endl;
        }
    }

    std::vector<CCar> CLevel::getCarsAhead(int range) const {
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
