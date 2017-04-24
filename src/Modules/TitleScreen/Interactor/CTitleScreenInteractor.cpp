#include <string>
#include <utility>
#include <vector>

#include "Vipper/Vipper.h"
#include "Modules/Gameplay/Router/CGameplayRouter.h"
#include "Modules/TitleScreen/Router/CTitleScreenRouter.h"
#include "Modules/TitleScreen/Interactor/CTitleScreenInteractor.h"

namespace RunTheWorld {
	CTitleScreenInteractor::CTitleScreenInteractor( std::shared_ptr<Vipper::IRouter> router):
	mRouter( router ) {
	}
	
	void CTitleScreenInteractor::interact() {
		std::dynamic_pointer_cast<CTitleScreenRouter>(mRouter)->goToGameplay();
	}
}
