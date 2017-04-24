#ifndef BF_CGAMEPLAYPRESENTER_H
#define BF_CGAMEPLAYPRESENTER_H

namespace RunTheWorld {

  class CGameplayPresenter : public Vipper::IPresenter {
    std::shared_ptr<CGameSession> mGameSession;
    std::shared_ptr<CGameplayView> mView;
  public:
    explicit CGameplayPresenter(std::shared_ptr<CGameSession> gameSession, std::shared_ptr<CGameplayView> view ); 
    
    void setScore( int score );
    void setPullTime( int timeToPull );
    void setPointsUntilNextLevel( int points );
    
    void present() override;
  };
}
#endif
