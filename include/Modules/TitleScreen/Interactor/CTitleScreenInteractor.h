#ifndef BF_CCLICKINGPLAYSTARTSGAMEINTERACTOR_H
#define BF_CCLICKINGPLAYSTARTSGAMEINTERACTOR_H

namespace RunTheWorld {
	class CTitleScreenInteractor : public Vipper::IInteractor {
		std::shared_ptr<Vipper::IRouter> mRouter;
		public:
		explicit CTitleScreenInteractor(std::shared_ptr<Vipper::IRouter> router);
	
		void interact() override;
	};
}
#endif