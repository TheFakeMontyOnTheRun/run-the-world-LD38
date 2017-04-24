#ifndef BF_CHIGHSCORESSCREENROUTER_H
#define BF_CHIGHSCORESSCREENROUTER_H

namespace RunTheWorld {
	class CHighScoresScreenRouter : public Vipper::IRouter {
		public:

		explicit CHighScoresScreenRouter( std::shared_ptr<Vipper::IRenderer> renderer );

		void goToTitle();
		void initWithDefaults() override;
		void onFocus() override;
    	void onRelinquishFocus() override;
	};	
}
#endif