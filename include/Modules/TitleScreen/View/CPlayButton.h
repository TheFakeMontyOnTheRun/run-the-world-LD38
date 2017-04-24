#ifndef BF_CPLAYBUTTON_H
#define BF_CPLAYBUTTON_H

namespace RunTheWorld {
	class CPlayButton: public Vipper::IView {
		
		CRect mBounds;
		std::string mLabel;
		std::function<void()> mOnClick;
		Vipper::IRenderer::FontId mLabelFont;
		public:
		CPlayButton( std::shared_ptr<Vipper::IRenderer> renderer, CRect area, std::string label, std::function<void()> onClick );
		void show() override;
		void click( std::pair<int,int> position );
	};
}
#endif