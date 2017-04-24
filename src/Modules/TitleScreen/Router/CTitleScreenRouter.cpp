#include <iostream>
#include <set>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <functional>
#include <utility>
#include <tuple>
#include <vector>
#include <array>
#include <iostream>
#include <random>
#include <algorithm>
#include <map>
#include <vector>
#include <set>
#include "Vipper/Vipper.h"

#include "Modules/Gameplay/Entities/CLevel.h"
#include "Modules/Gameplay/Entities/CGameSession.h"

#include "Modules/Gameplay/Router/CGameplayRouter.h"

#include "Modules/TitleScreen/Router/CTitleScreenRouter.h"
#include "Modules/TitleScreen/View/CRect.h"
#include "Modules/TitleScreen/View/CPlayButton.h"
#include "Modules/TitleScreen/View/CTitleScreenView.h"
#include "Modules/TitleScreen/Presenter/CTitleScreenPresenter.h"
#include "Modules/TitleScreen/Interactor/CTitleScreenInteractor.h"
#include "Modules/TitleScreen/Router/CTitleScreenRouter.h"

namespace RunTheWorld {

		CTitleScreenRouter::CTitleScreenRouter(std::shared_ptr<Vipper::IRenderer> renderer): Vipper::IRouter( renderer,"title" ){	
		}

		void CTitleScreenRouter::goToGameplay() {

			auto gameplayRouter = std::make_shared<RunTheWorld::CGameplayRouter>(getRenderer());
  			gameplayRouter->initWithDefaults();
			setNextRoute( gameplayRouter );		
		}
		
		void CTitleScreenRouter::initWithDefaults() {
			auto view = std::make_shared<RunTheWorld::CTitleScreenView>(getRenderer());
			auto interactor = std::make_shared<RunTheWorld::CTitleScreenInteractor>(shared_from_this());
			auto presenter = std::make_shared<RunTheWorld::CTitleScreenPresenter>(view);
				
			setPresenter( presenter );
			presenter->setView( view );
			presenter->setInteractor( interactor );
			view->setPresenter( presenter );
		}
		
		void CTitleScreenRouter::onFocus(){
			IRouter::onFocus();
			getRenderer()->registerClickListener( std::dynamic_pointer_cast<CTitleScreenView>(getPresenter()->getView()) );
		}
		
    	void CTitleScreenRouter::onRelinquishFocus(){
			IRouter::onRelinquishFocus();
			setNextRoute( nullptr );
			getRenderer()->unregisterClickListener( std::dynamic_pointer_cast<CTitleScreenView>(getPresenter()->getView()) );
		}		
}
