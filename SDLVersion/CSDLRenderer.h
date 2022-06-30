#ifndef BF_CSDLRenderer_H
#define BF_CSDLRenderer_H

namespace RunTheWorld {
    class CSDLRenderer : public Vipper::IRenderer {
        SDL_Surface *video;
        std::map<IRenderer::BitmapId, SDL_Surface *> mSprites;
        std::map<IRenderer::SoundId, Mix_Chunk *> mSounds;
        std::map<IRenderer::FontId, TTF_Font *> mFonts;
    public:
        explicit CSDLRenderer();

        void render() override;

        void shutdown() override;

        void update() override;

        BitmapId loadBitmap(const std::string& path) override;

        std::pair<int, int> getBitmapSize( const BitmapId& id ) override;

        SoundId loadSound(const std::string& path) override;

        FontId loadFont(const std::string& path, int sizeInPt) override;

        void drawSquare(int x, int y, int x2, int y2, const std::array<int, 4>& colour) override;

        void drawTextAt(int x, int y, const std::string& text, const std::array<int, 4>& colour, const FontId& font) override;

        void drawBitmapAt(int x, int y, int w, int h, const BitmapId& id) override;

        void playSound(const SoundId &id) override;

        void fill(int x0, int x1, int y0, int x2, int x3, int y1, const std::array<int, 4>& shade) override;
    };
}
#endif
