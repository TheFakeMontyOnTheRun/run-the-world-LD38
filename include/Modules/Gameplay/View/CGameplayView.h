#ifndef BF_CGAMEPLAYVIEW_H
#define BF_CGAMEPLAYVIEW_H

namespace RunTheWorld {

    struct Vec3 {
        Vec3(FixP aX, FixP aY, FixP aZ) : x(aX), y(aY), z(aZ) {
        }

        FixP x = 0;
        FixP y = 0;
        FixP z = 0;
    };

    struct Vec2 {
        Vec2(FixP aX, FixP aY) : x(aX), y(aY) {
        }

        FixP x = 0;
        FixP y = 0;
    };

  class CGameplayView : public Vipper::IView, public Vipper::IRenderer::IClickListener, public Vipper::IRenderer::IKeyListener {

    std::shared_ptr<CGameSession> mGameSession;

      Vipper::IRenderer::BitmapId mBackdrop[3];
      Vipper::IRenderer::BitmapId mSmoke;
      Vipper::IRenderer::BitmapId mCar[3][3];
      Vipper::IRenderer::BitmapId mOtherCar[3][3];

    Vipper::IRenderer::SoundId mHitSound;
      Vipper::IRenderer::SoundId mAccelerateSound;
    Vipper::IRenderer::SoundId mBrakeSound;
    
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
