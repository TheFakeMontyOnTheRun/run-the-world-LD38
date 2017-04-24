#ifndef VIPPER_IVIEW_H
#define VIPPER_IVIEW_H

namespace Vipper {
	class IView {
		public:
		explicit IView(std::shared_ptr<IRenderer> renderer);
		
		virtual void show();
		
		std::shared_ptr<IPresenter> getPresenter();
		void setPresenter(std::shared_ptr<IPresenter> presenter);
		
		std::shared_ptr<IRenderer> getRenderer();
		void setRenderer(std::shared_ptr<IRenderer> renderer);
		
		private:
		std::shared_ptr<IRenderer> mRenderer;
		std::shared_ptr<IPresenter> mPresenter;
	};	
}
#endif