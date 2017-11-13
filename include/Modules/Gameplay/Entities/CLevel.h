#ifndef CGAME_H
#define CGAME_H

#include "Common.h"

namespace RunTheWorld {

    using CCar = std::tuple<int, int, int>;

    class CLevel : public Vipper::IRenderer::IKeyListener {
    public:

        enum class EGameState {
            kTitleScreen, kGame, kVictory, kGameOver
        };

        EGameState gameState = EGameState::kTitleScreen;

        static const int kSegmentLengthInMeters = 1024;
        static const int kSlopeHeightInMeters = 32;
        static const int kZones = 3;
        bool smoking = false;
        float x = 0;
        float xSpeed = 0;
        float mHeading = 0;
        int carSpeed = 0;
        int zone = 2;
        long timeLeft = 90000;
        //20 seconds per segment is quite fair
        std::array<char, 3>  track =   {{'(' , ')', ')'}};
        std::array<char, 3>  slopes = {{'/' , '\\', '_' }};
        std::vector<CCar> mCars;

        int distanceToNextElement = kSegmentLengthInMeters;

        int elementIndex = 0;

        explicit CLevel();

        long timeEllapsed = 0;
        float distanceRan = 0;
        void tick(long ms);

        void reset();

        std::vector<CCar> getCarsAhead( int range ) const;

        void onKeyUp( const Vipper::ECommand& keyCode );
        void onKeyDown( const Vipper::ECommand& keyCode );
        bool isOver();
        bool mIsOver = false;
        bool hit = false;
        bool accel = false;
        bool brake = false;
    };
}
#endif
