#include <iostream>
#include <functional>
#include <utility>
#include <tuple>
#include <vector>
#include <array>
#include <iostream>
#include <random>
#include <algorithm>
#include <set>
#include "Vipper/Vipper.h"
#include "Modules/Gameplay/Entities/CLevel.h"
#include "Modules/Gameplay/Entities/CGameSession.h"

namespace RunTheWorld {

    CGameSession::CGameSession( int initialTimeLeft, int pointsNeeded, int levelOfDificulty) : mTimeLeft( initialTimeLeft ), mScore( 0 ), mLevel( std::make_shared<CLevel>()) {
    }
       
    std::shared_ptr<CLevel> CGameSession::getLevel() {
        return mLevel;
    }

    int CGameSession::getTimeLeft() {
        return mTimeLeft;
    }

    int CGameSession::getScore() {
        return mScore;
    }

    bool CGameSession::isOver() {
//        return mLevel->isOver();
        return false;
    }

    void CGameSession::addToScore( int score ) {
        mScore += score;
    }

    void CGameSession::update() {
        mLevel->tick( 33 );
    }
}
