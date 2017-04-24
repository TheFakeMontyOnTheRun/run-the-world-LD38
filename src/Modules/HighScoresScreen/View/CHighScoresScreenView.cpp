#include <array>
#include <iostream>
#include "Vipper/Vipper.h"
#include "Modules/HighScoresScreen/Presenter/CHighScoresScreenPresenter.h"
#include "Modules/HighScoresScreen/View/CHighScoresScreenView.h"

namespace RunTheWorld {
	CHighScoresScreenView::CHighScoresScreenView(std::shared_ptr<Vipper::IRenderer> renderer):
	IView( renderer ){
		mTitleTextFont = renderer->loadFont( "res/albasuper.ttf", 40 );
		mInstructionsFont = renderer->loadFont( "res/albaregular.ttf", 15 );
	}
	
	void CHighScoresScreenView::show() {
		auto renderer = getRenderer();
		renderer->drawSquare( 0, 0, 640, 480, {0,0,0,255} );
		renderer->drawTextAt( 30, 50, "Game Over!", {255,0,255,255}, mTitleTextFont );	
		renderer->drawTextAt( 20, 440, "Click anywhere to go back to title screen", {255,255,255,255}, mInstructionsFont );
	}
	
	void CHighScoresScreenView::onClick( std::pair<int, int> position ) {
		std::dynamic_pointer_cast<CHighScoresScreenPresenter>(getPresenter())->onClickOnOkButton();
	}
}
