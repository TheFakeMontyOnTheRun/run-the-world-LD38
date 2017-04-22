#ifndef CGAME_H
#define CGAME_H

namespace odb {
    class CGame {
    public:

        enum class EGameState {
            kTitleScreen, kGame, kVictory, kGameOver
        };

        EGameState gameState = EGameState::kTitleScreen;

        int x = 0;
        int y = 0;

        explicit CGame();

        long timeEllapsed = 0;

        void tick(long ms);

        void reset();

        CControlCallback getKeyPressedCallback();

        CControlCallback getKeyReleasedCallback();
    };
}
#endif
