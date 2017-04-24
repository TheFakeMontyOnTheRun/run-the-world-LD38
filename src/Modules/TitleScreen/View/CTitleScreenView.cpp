#include <string>
#include <array>
#include "Vipper/Vipper.h"

#include "Modules/TitleScreen/View/CRect.h"
#include "Modules/TitleScreen/View/CPlayButton.h"
#include "Modules/TitleScreen/View/CTitleScreenView.h"
#include "Modules/TitleScreen/Presenter/CTitleScreenPresenter.h"

namespace RunTheWorld {
	
	CTitleScreenView::CTitleScreenView( std::shared_ptr<Vipper::IRenderer> renderer ) : IView( renderer ) {
		mButton = std::make_shared<CPlayButton>( renderer, CRect( 310, 400, 400, 450 ), "Play", [&](){
			std::dynamic_pointer_cast<CTitleScreenPresenter>(getPresenter())->onClickOnPlayButton();
		} );
		
		mTitleTextFont = renderer->loadFont( "res/albasuper.ttf", 50 );
		mCreditsTextFont = renderer->loadFont( "res/albasuper.ttf", 30 );
	}
	
	void CTitleScreenView::show() {
		auto renderer = getRenderer();
		renderer->drawSquare( 0,0, 640, 480, {0,0,0,255} );

		mButton->show();
		renderer->drawTextAt( 100, 100, "Run the world!", {255,0,255,255}, mTitleTextFont );
		renderer->drawTextAt( 150, 200, "A game by Daniel Monteiro for the LD 38", {255,255,255,255}, mCreditsTextFont );
	}
	
	void CTitleScreenView::onClick( std::pair<int, int> position ) {
		mButton->click(position);
	}
}
