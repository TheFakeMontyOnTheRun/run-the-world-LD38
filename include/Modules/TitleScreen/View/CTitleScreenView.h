#ifndef BF_CTITLESCREENVIEW
#define BF_CTITLESCREENVIEW

namespace RunTheWorld {
	class CTitleScreenView : public Vipper::IView, public Vipper::IRenderer::IClickListener {
		std::shared_ptr<CPlayButton> mButton;
		Vipper::IRenderer::BitmapId mTitle;
		Vipper::IRenderer::FontId mTitleTextFont;
		Vipper::IRenderer::FontId mCreditsTextFont;

		Vipper::IRenderer::SoundId mSelectedSound;
		public:
		explicit CTitleScreenView(std::shared_ptr<Vipper::IRenderer> renderer);
		
		void show() override;
		void onClick( std::pair<int, int> position ) override;
	};
}
#endif