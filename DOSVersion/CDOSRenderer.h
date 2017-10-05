#ifndef BF_CDOSRenderer_H
#define BF_CDOSRenderer_H

namespace RunTheWorld {

  class CDOSRenderer : public Vipper::IRenderer {
    std::array<unsigned int, 320 * 200 > mBuffer;
    std::array<char, 80 * 25 > mTextBuffer;
    std::vector<std::shared_ptr<odb::NativeBitmap>> mSprites;
    long mFrame = 0;
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
    void fill(int x0, int x1, int y0, int x2, int x3, int y1, std::array<int,4> shade) override;
  };
}
#endif
