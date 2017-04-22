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
        timeInSegment += ms;

        distanceToNextElement = 100 - (20 * timeInSegment/1000);

        if (distanceToNextElement < 0 ) {
            distanceToNextElement = 100;
            timeInSegment = 0;
            elementIndex = ( elementIndex + 1) % track.size(); //len track
        }

        x += xSpeed;
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
                shape = ')';
            }

            if (command == ECommand::kDown) {
                std::cout << "down pressed" << std::endl;
                shape = '(';
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
                    shape = '|';

                    if (command == ECommand::kLeft) {
                        std::cout << "left released" << std::endl;
                    }

                    if (command == ECommand::kRight) {
                        std::cout << "right released" << std::endl;
                    }

                    if (command == ECommand::kUp) {
                        std::cout << "up released" << std::endl;
                    }

                    if (command == ECommand::kDown) {
                        std::cout << "down released" << std::endl;
                    }

                    if (command == ECommand::kFire1) {
                        std::cout << "fire1 released" << std::endl;
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
