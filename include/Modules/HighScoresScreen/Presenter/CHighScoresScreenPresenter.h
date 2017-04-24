#ifndef BF_CHIGHSCORESSCREENPRESENTER_H
#define BF_CHIGHSCORESSCREENPRESENTER_H

namespace RunTheWorld {
	class CHighScoresScreenPresenter : public Vipper::IPresenter {
		std::shared_ptr<Vipper::IView> mView;
		public:
		explicit CHighScoresScreenPresenter( std::shared_ptr<Vipper::IView> view );
		
		void present() override;
		void onClickOnOkButton();
	};
}
#endif