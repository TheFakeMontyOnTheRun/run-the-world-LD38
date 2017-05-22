#ifndef BF_CDOSRenderer_H
#define BF_CDOSRenderer_H

namespace RunTheWorld {

  class CDOSRenderer : public Vipper::IRenderer {
    std::array<unsigned int, 320 * 200 > mBuffer;
    std::array<char, 80 * 25 > mTextBuffer;
    std::map<IRenderer::BitmapId, std::shared_ptr<odb::NativeBitmap>> mSprites;
  public:
    explicit CDOSRenderer();
    void render() override;
    void shutdown() override;
    void update() override;        
    BitmapId loadBitmap( std::string path ) override;
    SoundId loadSound( std::string path ) override;
    FontId loadFont( std::string path, int sizeInPt ) override;
    void drawSquare( int x, int y, int x2, int y2, std::array<int,4> colour ) override;
    void drawTextAt( int x, int y, std::string text, std::array<int, 4> colour, FontId font ) override;
    void drawBitmapAt( int x, int y, int w, int h, const BitmapId id ) override;
    void playSound( const SoundId& id ) override;
    void fill(float x0, float x1, float y0, float x2, float x3, float y1, std::array<int,4> shade) override;
  };
}
#endif
