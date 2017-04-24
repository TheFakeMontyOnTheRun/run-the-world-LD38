#include <functional>
#include <array>
#include <iostream>
#include <random>
#include "Common.h"
#include "CGame.h"

namespace odb {

    void CGame::reset() {
        timeEllapsed = 0;
    }

    CGame::CGame() {
        gameState = EGameState::kGame;
        reset();
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


        if ( x < -160 || x > 160 ) {

        }
    }

    CControlCallback CGame::getKeyPressedCallback() {
        return [&](ECommand command) {
            if (command == ECommand::kLeft) {
                std::cout << "left pressed" << std::endl;
                xSpeed = -1;
            }

            if (command == ECommand::kRight) {
                std::cout << "Right pressed" << std::endl;
                xSpeed = 1;
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
                    }

                    if (command == ECommand::kFire1) {
                        std::cout << "fire1 released" << std::endl;
                        carSpeed = 0;
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
}
