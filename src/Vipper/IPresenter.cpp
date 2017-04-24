#include <memory>
#include "Vipper/Vipper.h"

namespace Vipper {
	IPresenter::IPresenter() {}
	
	std::shared_ptr<IInteractor> IPresenter::getInteractor() {
		return mInteractor;
	}
	
	void IPresenter::setInteractor( std::shared_ptr<IInteractor> interactor) {
		mInteractor = interactor;
	}
	
	void IPresenter::setRouter( std::shared_ptr<IRouter> router) {
		mRouter = router;
	}
	
	std::shared_ptr<IView> IPresenter::getView() {
		return mView;
	}
	
	void IPresenter::setView( std::shared_ptr<IView> view) {
		mView = view;
	}	
}