#ifndef CGAME_H
#define CGAME_H

#include "Common.h"

namespace odb {
    class CGame {
    public:

        enum class EGameState {
            kTitleScreen, kGame, kVictory, kGameOver
        };

        EGameState gameState = EGameState::kTitleScreen;

        int x = 0;
        int y = 0;
        int xSpeed = 0;
        int height = 0;
        int ySpeed = 0;

        std::array<char, 10>  track = {'|','(','|',')','|','(','|', ')', '(', ')'};

        int distanceToNextElement = 20;

        int elementIndex = 0;

        explicit CGame();

        long timeEllapsed = 0;
        long timeInSegment = 0;

        void tick(long ms);

        void reset();

        CControlCallback getKeyPressedCallback();

        CControlCallback getKeyReleasedCallback();
    };
}
#endif
