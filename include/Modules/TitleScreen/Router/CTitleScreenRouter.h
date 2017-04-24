#ifndef BF_CTITLESCREENROUTER_H
#define BF_CTITLESCREENROUTER_H

namespace RunTheWorld {
	class CTitleScreenRouter : public Vipper::IRouter {
		public:

		explicit CTitleScreenRouter( std::shared_ptr<Vipper::IRenderer> renderer );

		void goToGameplay();
		void initWithDefaults() override;
		void onFocus() override;
    	void onRelinquishFocus() override;
	};	
}
#endif