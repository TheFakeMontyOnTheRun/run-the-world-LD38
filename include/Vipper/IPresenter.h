#ifndef VIPPER_IPRESENTER_H
#define VIPPER_IPRESENTER_H

namespace Vipper {
	class IPresenter {
		std::shared_ptr<IRouter> mRouter;
		std::shared_ptr<IView> mView;
		std::shared_ptr<IInteractor> mInteractor;
		public:
		explicit IPresenter();	
		
		virtual void present() = 0;
		
		std::shared_ptr<IInteractor> getInteractor();
		void setInteractor( std::shared_ptr<IInteractor> interactor);
		
		void setRouter( std::shared_ptr<IRouter> router);

		std::shared_ptr<IView> getView();
		void setView( std::shared_ptr<IView> view);
	};	
}
#endif