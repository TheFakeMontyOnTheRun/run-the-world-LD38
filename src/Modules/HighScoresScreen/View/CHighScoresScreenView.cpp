#include <iostream>
#include <array>
#include <functional>
#include <map>
#include <set>
#include <string>
#include <functional>
#include <algorithm>
#include "Vipper/Vipper.h"
#include <Modules/TitleScreen/View/CRect.h>

#include <array>
#include <iostream>
#include "Vipper/Vipper.h"
#include "Modules/HighScoresScreen/Presenter/CHighScoresScreenPresenter.h"
#include "Modules/HighScoresScreen/View/CHighScoresScreenView.h"

namespace RunTheWorld {
	CHighScoresScreenView::CHighScoresScreenView(std::shared_ptr<Vipper::IRenderer> renderer):
	IView( renderer ){
		mTitleTextFont = renderer->loadFont( "albasuper.ttf", 40 );
		mInstructionsFont = renderer->loadFont( "albaregular.ttf", 15 );
#ifdef __EMSCRIPTEN__
		mBg = renderer->loadBitmap("emgameover.png");
#else
	mBg = renderer->loadBitmap("bg.png");
#endif
	}
	
	void CHighScoresScreenView::show() {
		auto renderer = getRenderer();
		renderer->drawSquare( 0, 0, 640, 480, {0,0,0,255} );
		renderer->drawBitmapAt( 0, 0, 640, 480, mBg );
#ifdef __EMSCRIPTEN__
#else
		renderer->drawTextAt( 30, 50, "Game Over!", {0,0,0,255}, mTitleTextFont );
		renderer->drawTextAt( 20, 440, "Click anywhere to go back to title screen", {255,255,255,255}, mInstructionsFont );
#endif
	}
	
	void CHighScoresScreenView::onClick( std::pair<int, int> position ) {
		std::dynamic_pointer_cast<CHighScoresScreenPresenter>(getPresenter())->onClickOnOkButton();
	}

    void CHighScoresScreenView::onKeyUp( Vipper::ECommand keyCode ) {
        if ( keyCode == Vipper::ECommand::kFire1 ) {
            std::dynamic_pointer_cast<CHighScoresScreenPresenter>(getPresenter())->onClickOnOkButton();
        }
    }

    void CHighScoresScreenView::onKeyDown( Vipper::ECommand keyCode ) {
    }
}
