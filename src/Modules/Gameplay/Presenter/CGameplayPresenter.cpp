#include <set>
#include <map>
#include <sstream>
#include <functional>
#include <utility>
#include <tuple>
#include <vector>
#include <array>
#include <iostream>
#include <random>
#include <algorithm>
#include <iostream>
#include "Vipper/Vipper.h"
#include "Modules/Gameplay/Entities/CLevel.h"
#include "Modules/Gameplay/Entities/CGameSession.h"
#include "Modules/Gameplay/View/CGameplayView.h"
#include "Modules/Gameplay/Presenter/CGameplayPresenter.h"


namespace RunTheWorld {
  CGameplayPresenter::CGameplayPresenter(std::shared_ptr<CGameSession> gameSession, std::shared_ptr<CGameplayView> view ):
  mGameSession( gameSession ), mView( view ) {
  }
     
    
  void CGameplayPresenter::setScore( int score ) {
    
  }
  
  void CGameplayPresenter::setPullTime( int timeToPull ) {
  }
  
  void CGameplayPresenter::setPointsUntilNextLevel( int points ) {
  }
    
  void CGameplayPresenter::present() {

    mGameSession->update();
    getInteractor()->interact();

    mView->show();

    mView->drawTextAt( std::pair<int, int>(0,460), "Pull" );
    std::stringstream ss;
    ss << "Score: ";
    ss << mGameSession->getScore();

    mView->drawTextAt( std::pair<int, int>(120,460), ss.str() );
  }
}