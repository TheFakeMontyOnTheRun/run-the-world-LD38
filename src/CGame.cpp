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
        distanceRan += carSpeed * ms;
        distanceToNextElement -= carSpeed * ms/1000.0f;

        if (distanceToNextElement <= 0 ) {
            distanceToNextElement = 100;
            elementIndex = ( elementIndex + 1) % track.size(); //len track
            std::cout << "entering " << track[ elementIndex ] << " with slope " << slopes[ elementIndex ] << std::endl;
        }

        std::cout << "speed: " << carSpeed << std::endl;
        x += xSpeed * carSpeed * 0.5f;

        if ( track[ elementIndex ] == ')') {
            x+= 0.35f * carSpeed;
        } else if ( track[ elementIndex ] == '(') {
            x-= 0.35f * carSpeed;
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
                        carSpeed = std::min( carSpeed + 1, 20);
                    }

                    if (command == ECommand::kDown) {
                        std::cout << "down released" << std::endl;
                        carSpeed = std::max( carSpeed - 1, 0);
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
