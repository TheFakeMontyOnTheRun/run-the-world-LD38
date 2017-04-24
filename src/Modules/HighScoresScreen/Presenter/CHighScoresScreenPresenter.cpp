#include "Vipper/Vipper.h"
#include "Modules/HighScoresScreen/Presenter/CHighScoresScreenPresenter.h"

namespace RunTheWorld {

	CHighScoresScreenPresenter::CHighScoresScreenPresenter(std::shared_ptr<Vipper::IView> view ):
	mView( view ) {
	}


	void CHighScoresScreenPresenter::present() {
		mView->show();	
	}
	void CHighScoresScreenPresenter::onClickOnOkButton() {
		getInteractor()->interact();
	}
}
