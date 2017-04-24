#ifndef VIPPER_IINTERACTOR_H
#define VIPPER_IINTERACTOR_H

namespace Vipper {
	class IInteractor {
		public:
		explicit IInteractor();	
		
		virtual void interact() = 0;
		
		std::shared_ptr<IPresenter> getPresenter();
		void setPresenter(std::shared_ptr<IPresenter> presenter);
		
		private:
		std::shared_ptr<IPresenter> mPresenter;		
	};
}
#endif