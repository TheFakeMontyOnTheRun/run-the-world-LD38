#include "Vipper/Vipper.h"
#include "Modules/HighScoresScreen/Interactor/CHighScoresScreenInteractor.h"
#include "Modules/HighScoresScreen/Router/CHighScoresScreenRouter.h"

namespace RunTheWorld {

	CHighScoresScreenInteractor::CHighScoresScreenInteractor(std::shared_ptr<Vipper::IRouter> router):
	mRouter(router) {
	}
	
	void CHighScoresScreenInteractor::interact(){
		std::dynamic_pointer_cast<CHighScoresScreenRouter>(mRouter)->goToTitle();
	}
}