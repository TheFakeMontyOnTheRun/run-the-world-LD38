#ifndef BF_CLEVELPRESENTER_H
#define BF_CLEVELPRESENTER_H

namespace RunTheWorld {

  class CLevelPresenter : public Vipper::IPresenter {
  
  public:
    explicit CLevelPresenter();
    
    void drawScoreGain(int gain);
    void playVanishSound();
    void playFallSound();
    void playPullSound();
  };
}
#endif
