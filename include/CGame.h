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

        float x = 0;
        float xSpeed = 0;
        int carSpeed = 20;
        std::array<char, 10>  track =   {'(' , '|', ')', ')', '|' , ')', ')', ')', ')' , ')'};
        std::array<char, 10>  slopes = {'_' , '/', '_', '\\', '_', '/', '_', '\\', '_', '_'};

        int distanceToNextElement = 100;

        int elementIndex = 0;

        explicit CGame();

        long timeEllapsed = 0;
        long distanceRan = 0;
        void tick(long ms);

        void reset();

        CControlCallback getKeyPressedCallback();

        CControlCallback getKeyReleasedCallback();
    };
}
#endif
