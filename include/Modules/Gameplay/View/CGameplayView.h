#ifndef BF_CGAMEPLAYVIEW_H
#define BF_CGAMEPLAYVIEW_H

namespace RunTheWorld {

    struct Vec3 {
        Vec3(float aX, float aY, float aZ) : x(aX), y(aY), z(aZ) {
        }

        float x = 0;
        float y = 0;
        float z = 0;
    };

    struct Vec2 {
        Vec2(float aX, float aY) : x(aX), y(aY) {
        }

        float x = 0;
        float y = 0;
    };

  class CGameplayView : public Vipper::IView, public Vipper::IRenderer::IClickListener, public Vipper::IRenderer::IKeyListener {

    std::shared_ptr<CGameSession> mGameSession;

      Vipper::IRenderer::BitmapId mBackdrop[3];
      Vipper::IRenderer::BitmapId mSmoke;
      Vipper::IRenderer::BitmapId mCar[3][3];

    Vipper::IRenderer::SoundId mFallSound;
    Vipper::IRenderer::SoundId mCollapseSound;
    Vipper::IRenderer::SoundId mBustedSound;
    Vipper::IRenderer::SoundId mPullSound;
    
    Vipper::IRenderer::FontId mUIFont;

  public:
    explicit CGameplayView(std::shared_ptr<CGameSession> session, std::shared_ptr<Vipper::IRenderer> renderer);
    void drawTextAt( std::pair<int, int> position, std::string text );
    void drawGaugeAt( std::pair<int, int> position, int howFilled);
    std::pair<int, int> getLastClick();
    void show() override;
    void onClick( std::pair< int, int> position ) override;
    void onKeyUp( Vipper::ECommand keyCode ) override;
      void onKeyDown( Vipper::ECommand keyCode ) override;
      void drawBackdropForHeading(int modulus, int zone  );
  };
}
#endif
