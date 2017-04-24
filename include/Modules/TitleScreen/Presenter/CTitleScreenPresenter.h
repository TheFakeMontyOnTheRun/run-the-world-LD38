#ifndef BF_CTITLESCREENPRESENTER_H
#define BF_CTITLESCREENPRESENTER_H

namespace RunTheWorld {
	class CTitleScreenPresenter : public Vipper::IPresenter {
		std::shared_ptr<Vipper::IView> mView;
		public:
		explicit CTitleScreenPresenter( std::shared_ptr<Vipper::IView> view );
		
		void present() override;
		void onClickOnPlayButton();
	};
}
#endif