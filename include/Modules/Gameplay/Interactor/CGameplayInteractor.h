#ifndef BF_CGAMEPLAYINTERACTOR_H
#define BF_CGAMEPLAYINTERACTOR_H

namespace RunTheWorld {
	class CGameplayInteractor : public Vipper::IInteractor {
		std::shared_ptr<Vipper::IRouter> mRouter;
		std::shared_ptr<CGameSession> mSession;
		public:
		explicit CGameplayInteractor(std::shared_ptr<CGameSession> session, std::shared_ptr<Vipper::IRouter> router);
	
		void interact() override;
	};
}
#endif