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

        static const int kSegmentLengthInMeters = 1000;
        static const int kSlopeHeightInMeters = 100;
        float x = 0;
        float xSpeed = 0;
        int carSpeed = 0;
        std::array<char, 10>  track =   {'(' , ')', ')', ')', '|' , ')', ')', ')', ')' , ')'};
        std::array<char, 10>  slopes = {'_' , '\\', '_', '\\', '_', '/', '_', '\\', '_', '_'};

        int distanceToNextElement = kSegmentLengthInMeters;

        int elementIndex = 0;

        explicit CGame();

        long timeEllapsed = 0;
        float distanceRan = 0;
        void tick(long ms);

        void reset();

        CControlCallback getKeyPressedCallback();

        CControlCallback getKeyReleasedCallback();
    };
}
#endif
