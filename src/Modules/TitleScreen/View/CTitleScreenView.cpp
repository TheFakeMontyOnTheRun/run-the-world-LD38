#include <string>
#include <array>
#include "Vipper/Vipper.h"

#include "Modules/TitleScreen/View/CRect.h"
#include "Modules/TitleScreen/View/CPlayButton.h"
#include "Modules/TitleScreen/View/CTitleScreenView.h"
#include "Modules/TitleScreen/Presenter/CTitleScreenPresenter.h"

namespace RunTheWorld {
	
	CTitleScreenView::CTitleScreenView( std::shared_ptr<Vipper::IRenderer> renderer ) : IView( renderer ) {
		mButton = std::make_shared<CPlayButton>( renderer, CRect( 420, 370, 500, 410 ), "Play", [&](){
			std::dynamic_pointer_cast<CTitleScreenPresenter>(getPresenter())->onClickOnPlayButton();
		} );
		
		mTitleTextFont = renderer->loadFont( "albasuper.ttf", 50 );
		mCreditsTextFont = renderer->loadFont( "albasuper.ttf", 30 );
#if defined(__EMSCRIPTEN__) || defined(DOS)
		mTitle = renderer->loadBitmap( "emtitle.png" );
#else
        mTitle = renderer->loadBitmap( "title.png" );
#endif

		mSelectedSound = renderer->loadSound( "selected.wav" );
	}
	
	void CTitleScreenView::show() {
        auto renderer = getRenderer();
        renderer->drawSquare( 0, 0, 640, 480, { 255, 255, 255, 255 } );
        renderer->drawBitmapAt( 0, 0, 640, 480, mTitle );
#ifdef __EMSCRIPTEN__
#else
		mButton->show();
		renderer->drawTextAt( 15, 0, "Run the world!", {0,0,255,255}, mTitleTextFont );
		renderer->drawTextAt( 20, 350, "A game by Daniel Monteiro", {0,0,128,255}, mCreditsTextFont );
        renderer->drawTextAt( 20, 390, "and Livia Valle for the LD 38", {0,0,128,255}, mCreditsTextFont );
#endif
	}

	void CTitleScreenView::onClick( const std::pair<int, int>& position ) {
#ifdef __EMSCRIPTEN__
        getRenderer()->playSound(mSelectedSound);
        std::dynamic_pointer_cast<CTitleScreenPresenter>(getPresenter())->onClickOnPlayButton();
#else
        if ( mButton->click(position) ) {
            getRenderer()->playSound(mSelectedSound);
        }
#endif
	}

    void CTitleScreenView::onKeyUp( const Vipper::ECommand& keyCode ) {
        mButton->activate();
        getRenderer()->playSound(mSelectedSound);
    }

    void CTitleScreenView::onKeyDown( const Vipper::ECommand& keyCode ) {
    }
}
