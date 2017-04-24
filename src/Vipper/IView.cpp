#include "Vipper/Vipper.h"

namespace Vipper {
	IView::IView(std::shared_ptr<IRenderer> renderer):mRenderer(renderer) {}
	
	void IView::show() {
		
	}

	std::shared_ptr<IRenderer> IView::getRenderer() {
		return mRenderer;	
	}
	
	void IView::setRenderer(std::shared_ptr<IRenderer> renderer) {
		mRenderer = renderer;
	}

	
	std::shared_ptr<IPresenter> IView::getPresenter() {
		return mPresenter;	
	}
	
	void IView::setPresenter(std::shared_ptr<IPresenter> presenter) {
		mPresenter = presenter;
	}
}