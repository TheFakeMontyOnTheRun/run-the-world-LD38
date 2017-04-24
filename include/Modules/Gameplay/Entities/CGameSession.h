#ifndef BF_CGAMESESSION_H
#define BF_CGAMESESSION_H

namespace RunTheWorld {

  class CGameSession : public Vipper::IEntity {
    int mTimeLeft;
    std::shared_ptr<CLevel> mLevel;
    int mScore;
  public:
    explicit CGameSession( int initialTimeLeft, int pointsNeeded, int levelOfDificulty);
    std::shared_ptr<CLevel> getLevel();
    int getTimeLeft();
    void update();
    bool isOver();
    int getScore();
    void addToScore( int score );
  };
}
#endif
