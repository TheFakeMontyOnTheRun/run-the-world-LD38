#ifndef BF_IRENDERER_H
#define BF_IRENDERER_H

namespace Vipper {

    enum class ECommand {
        kUp,
        kRight,
        kDown,
        kLeft,
        kFire1,
        kFire2
    };

    class IRenderer {


    public:
        class IClickListener {
        public:
            virtual void onClick(const std::pair<int, int>& pointerPosition) = 0;
        };

        class IKeyListener {
        public:
            virtual void onKeyUp(const ECommand& keyCode) = 0;

            virtual void onKeyDown(const ECommand& keyCode) = 0;
        };

        void dispatchClickToListeners(const std::pair<int, int>& pointerPosition);

        void dispatchKeyDownToListeners(const ECommand& keyCode);

        void dispatchKeyUpToListeners(const ECommand& keyCode);

        std::vector<std::shared_ptr<IClickListener>> mClickListeners;
        std::vector<std::shared_ptr<IKeyListener>> mKeyListeners;

        using BitmapId = long;
        using SoundId = long;
        using FontId = long;

        virtual void render() = 0;

        virtual std::pair<int, int> getBitmapSize( const BitmapId& id ) = 0;

        virtual BitmapId loadBitmap(const std::string& path) = 0;

        virtual SoundId loadSound(const std::string& path) = 0;

        virtual FontId loadFont(const std::string& path, int sizeInPt) = 0;

        virtual void drawSquare(int x, int y, int x2, int y2, const std::array<int, 4>& colour) = 0;

        virtual void drawTextAt(int x, int y, const std::string& text, const std::array<int, 4>& colour, const FontId& font) = 0;

        virtual void drawBitmapAt(int x, int y, int w, int h, const BitmapId& id) = 0;

        virtual void playSound(const SoundId &id) = 0;

        virtual void update() = 0;

        virtual void shutdown() = 0;

        virtual void fill(int x0, int x1, int y0, int x2, int x3, int y1, const std::array<int, 4>& color) = 0;

        void registerClickListener(std::shared_ptr<IClickListener> listener);

        void unregisterClickListener(std::shared_ptr<IClickListener> listener);

        void registerKeyListener(std::shared_ptr<IKeyListener> listener);

        void unregisterKeyListener(std::shared_ptr<IKeyListener> listener);
    };
}
#endif
