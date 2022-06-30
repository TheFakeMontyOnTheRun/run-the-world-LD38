#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <typeinfo>
#include <memory>
#include <cmath>
#include <iostream>
#include <array>
#include <functional>
#include <map>
#include <set>
#include <string>
#include <functional>
#include <algorithm>
#include "Vipper/Vipper.h"

namespace Vipper {
	void IRenderer::registerClickListener( std::shared_ptr<IClickListener> listener ) {
		mClickListeners.push_back( listener );
	}
	
    void IRenderer::unregisterClickListener( std::shared_ptr<IClickListener> listener ) {
		mClickListeners.erase( std::remove( mClickListeners.begin(), mClickListeners.end(), listener ), mClickListeners.end() );
	}
	
	void IRenderer::registerKeyListener( std::shared_ptr<IKeyListener> listener ) {
		mKeyListeners.push_back( listener );
	}
	
    void IRenderer::unregisterKeyListener( std::shared_ptr<IKeyListener> listener ) {
		mKeyListeners.erase( std::remove( mKeyListeners.begin(), mKeyListeners.end(), listener ), mKeyListeners.end() );		
	}

	void IRenderer::dispatchKeyUpToListeners( const Vipper::ECommand& keyCode ) {
		for ( auto& listener : mKeyListeners ) {	
			listener->onKeyUp( keyCode );
		}
	}

	void IRenderer::dispatchKeyDownToListeners( const Vipper::ECommand& keyCode ) {
		for ( auto& listener : mKeyListeners ) {
			listener->onKeyDown( keyCode );
		}
	}
		
	void IRenderer::dispatchClickToListeners( const std::pair<int, int >& pointerPosition ) {
		for ( auto& listener : mClickListeners ) {	
			listener->onClick( pointerPosition );
		}
	}
}


