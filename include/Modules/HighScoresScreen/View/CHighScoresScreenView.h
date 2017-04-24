#ifndef BF_CHIGHSCORESSCREENVIEW
#define BF_CHIGHSCORESSCREENVIEW

namespace RunTheWorld {
	class CHighScoresScreenView : public Vipper::IView, public Vipper::IRenderer::IClickListener {
		
		Vipper::IRenderer::FontId mTitleTextFont;
		Vipper::IRenderer::FontId mInstructionsFont;
		Vipper::IRenderer::BitmapId mBg;
		public:
		explicit CHighScoresScreenView(std::shared_ptr<Vipper::IRenderer> renderer);
		
		void show() override;
		void onClick( std::pair<int, int> position ) override;
	};
}
#endif