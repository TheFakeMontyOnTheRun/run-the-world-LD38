#ifndef BF_HIGHSCORESSCREENINTERACTOR_H
#define BF_HIGHSCORESSCREENINTERACTOR_H

namespace RunTheWorld {
	class CHighScoresScreenInteractor : public Vipper::IInteractor {
		std::shared_ptr<Vipper::IRouter> mRouter;
		public:
		explicit CHighScoresScreenInteractor(std::shared_ptr<Vipper::IRouter> router);
	
		void interact() override;
	};
}
#endif