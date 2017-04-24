#include "Vipper/Vipper.h"
#include "Modules/TitleScreen/View/CRect.h"
#include "Modules/TitleScreen/View/CPlayButton.h"
#include "Modules/TitleScreen/View/CTitleScreenView.h"
#include "Modules/TitleScreen/Presenter/CTitleScreenPresenter.h"

namespace RunTheWorld {
	CTitleScreenPresenter::CTitleScreenPresenter(std::shared_ptr<Vipper::IView> view): mView( view )
	 {
	 }

	void CTitleScreenPresenter::present() {
		mView->show();
	}
	
	void CTitleScreenPresenter::onClickOnPlayButton() {
		getInteractor()->interact();
	}
}
