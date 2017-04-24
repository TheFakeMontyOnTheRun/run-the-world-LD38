#include <memory>
#include "Vipper/Vipper.h"

namespace Vipper {
	IInteractor::IInteractor() {}
	
	
	std::shared_ptr<IPresenter> IInteractor::getPresenter() {
		return mPresenter;	
	}
	
	void IInteractor::setPresenter(std::shared_ptr<IPresenter> presenter) {
		mPresenter = presenter;
	}
}