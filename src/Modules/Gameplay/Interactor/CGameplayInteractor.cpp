#include <utility>
#include <vector>
#include <memory>
#include <functional>
#include <utility>
#include <functional>
#include <utility>
#include <tuple>
#include <vector>
#include <array>
#include <iostream>
#include <random>
#include <algorithm>
#include <tuple>
#include <vector>
#include <array>
#include <iostream>
#include <random>
#include <algorithm>

#include "Vipper/Vipper.h"
#include "Modules/Gameplay/Entities/CLevel.h"
#include "Modules/Gameplay/Entities/CGameSession.h"
#include "Modules/Gameplay/Router/CGameplayRouter.h"
#include "Modules/Gameplay/Interactor/CGameplayInteractor.h"

namespace RunTheWorld {
		CGameplayInteractor::CGameplayInteractor(std::shared_ptr<CGameSession> session, std::shared_ptr<Vipper::IRouter> router) : mRouter( router ), mSession( session ) {	
		}
	
		void CGameplayInteractor::interact() {
			if ( mSession->isOver() ) {
				std::dynamic_pointer_cast<CGameplayRouter>(mRouter)->goToGameOver();				
			}			
		}
}