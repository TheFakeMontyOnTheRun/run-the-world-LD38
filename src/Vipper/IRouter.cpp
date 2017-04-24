#include <iostream>
#include "Vipper/Vipper.h"

namespace Vipper {
	IRouter::IRouter( std::shared_ptr<Vipper::IRenderer> renderer, std::string name ): mName( name ), mNextRoute( nullptr ), mRenderer( renderer ) {
		mIsActive = true;
	}
	
	std::shared_ptr<IRouter> IRouter::route() {
		auto toReturn = mNextRoute;
		mNextRoute = nullptr;
		
		return toReturn;
	}

	bool IRouter::isFinished() {
		return !mIsActive;
	}
	
	void IRouter::setNextRoute( std::shared_ptr<IRouter> nextRoute ) {
		mNextRoute = nextRoute;
	}
	
	void IRouter::setPresenter( std::shared_ptr<IPresenter> presenter ) {
		mPresenter = presenter;	
	}
	
	std::shared_ptr<IPresenter> IRouter::getPresenter() {
		return mPresenter;
	}
	
	void IRouter::setName( std::string name ) {
		mName = name;
	}
	
	std::string IRouter::getName() {
		return mName;
	}

		
	std::shared_ptr<Vipper::IRenderer> IRouter::getRenderer() {
		return mRenderer;
	}

	void IRouter::finish() {
		mIsActive = false;
	}

	void IRouter::onFocus() {
		// mIsActive = true;
	}
		
	void IRouter::onRelinquishFocus() {
		// mIsActive = false;
	}	
}