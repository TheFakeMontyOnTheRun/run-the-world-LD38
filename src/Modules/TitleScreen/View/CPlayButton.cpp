#include <functional>
#include <array>

#include "Vipper/Vipper.h"
#include "Modules/TitleScreen/View/CRect.h"
#include "Modules/TitleScreen/View/CPlayButton.h"

namespace RunTheWorld {
    CPlayButton::CPlayButton( std::shared_ptr<Vipper::IRenderer> renderer, CRect area, std::string label, std::function<void()> onClick ):
    IView( renderer ), mBounds( area ), mLabel( label ), mOnClick( onClick ), mLabelFont( renderer->loadFont( "./res/albaregular.ttf", 15 ) ) {
    }

    bool CPlayButton::click( std::pair<int,int> position ) {
        if ( mBounds.isInside( position.first, position.second ) ) {
			if ( mOnClick != nullptr ) {
                mOnClick();
            }
            return true;
        }
        return false;
    }

    void CPlayButton::show() {
        auto renderer = getRenderer();
        renderer->drawSquare( mBounds.mX0, mBounds.mY0, mBounds.mX1, mBounds.mY1, std::array<int,4>({ 255, 0,0,255}) );
        renderer->drawTextAt( mBounds.mX0, mBounds.mY0, mLabel, std::array<int,4>({255,255,255,255}), mLabelFont );
    }

    void CPlayButton::activate() {
        mOnClick();
    }
}
