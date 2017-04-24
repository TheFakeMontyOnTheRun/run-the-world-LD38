#include "Vipper/Vipper.h"
#include "Modules/HighScoresScreen/HighScores.h"


namespace RunTheWorld {

		CHighScoresScreenRouter::CHighScoresScreenRouter(std::shared_ptr<Vipper::IRenderer> renderer): Vipper::IRouter( renderer,"game over" ){	
		}

		void CHighScoresScreenRouter::goToTitle() {
			finish();
		}
		
		void CHighScoresScreenRouter::initWithDefaults() {
			auto view = std::make_shared<RunTheWorld::CHighScoresScreenView>(getRenderer());
			auto interactor = std::make_shared<RunTheWorld::CHighScoresScreenInteractor>(shared_from_this());
			auto presenter = std::make_shared<RunTheWorld::CHighScoresScreenPresenter>(view);
				
			setPresenter( presenter );
			presenter->setView( view );
			presenter->setInteractor( interactor );
			view->setPresenter( presenter );
		}
		
		void CHighScoresScreenRouter::onFocus(){
			IRouter::onFocus();
			getRenderer()->registerClickListener( std::dynamic_pointer_cast<CHighScoresScreenView>(getPresenter()->getView()) );
		}
		
    	void CHighScoresScreenRouter::onRelinquishFocus(){
			IRouter::onRelinquishFocus();
			setNextRoute( nullptr );
			getRenderer()->unregisterClickListener( std::dynamic_pointer_cast<CHighScoresScreenView>(getPresenter()->getView()) );
		}		
}
