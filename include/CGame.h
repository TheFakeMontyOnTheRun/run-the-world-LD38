#ifndef CGAME_H
#define CGAME_H

#include "Common.h"

namespace odb {

    using CCar = std::tuple<float, float, float>;

    class CGame {
    public:

        enum class EGameState {
            kTitleScreen, kGame, kVictory, kGameOver
        };

        EGameState gameState = EGameState::kTitleScreen;

        static const int kSegmentLengthInMeters = 1000;
        static const int kSlopeHeightInMeters = 100;
        static const int kZones = 3;
        bool smoking = false;
        float x = 0;
        float xSpeed = 0;
        float mHeading = 0;
        int carSpeed = 0;
        int zone = 0;
        long timeLeft = 60000;
        //20 seconds per segment is quite fair
        std::array<char, 3>  track =   {{'(' , ')', ')'}};
        std::array<char, 3>  slopes = {{'/' , '\\', '_' }};
        std::vector<CCar> mCars;

        int distanceToNextElement = kSegmentLengthInMeters;

        int elementIndex = 0;

        explicit CGame();

        long timeEllapsed = 0;
        float distanceRan = 0;
        void tick(long ms);

        void reset();

        CControlCallback getKeyPressedCallback();

        CControlCallback getKeyReleasedCallback();

        std::vector<CCar> getCarsAhead( int range ) const;
    };
}
#endif
