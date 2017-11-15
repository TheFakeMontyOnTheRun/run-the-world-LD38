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
      int mWidth[3][3];
      int mHeight[3][3];

    Vipper::IRenderer::SoundId mHitSound;
      Vipper::IRenderer::SoundId mAccelerateSound;
    Vipper::IRenderer::SoundId mBrakeSound;
    
    Vipper::IRenderer::FontId mUIFont;

  public:
      bool mDrawBackdrop = true;
    explicit CGameplayView(std::shared_ptr<CGameSession> session, std::shared_ptr<Vipper::IRenderer> renderer);
    void drawTextAt( std::pair<int, int> position, std::string text );
    void drawGaugeAt( std::pair<int, int> position, int howFilled);
    std::pair<int, int> getLastClick();
    void show() override;
    void onClick( const std::pair< int, int>& position ) override;
    void onKeyUp( const Vipper::ECommand& keyCode ) override;
      void onKeyDown( const Vipper::ECommand& keyCode ) override;
      void drawBackdropForHeading(int modulus, int zone  );
      FixP mCameraHeight{ 10 };
      FixP mCameraHeightIncrements{ 0.1f };

      float getCurve(int completelyArbitraryCurveEasingFactor, int shapeDelta, int distanceToCurrentShape, int y) const;

      float getInitialSlope(int slopeDelta, int distanceToCurrentShape, int segmentLength) const;
  };
}
#endif
