#include <set>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <functional>
#include <utility>
#include <tuple>
#include <vector>
#include <array>
#include <iostream>
#include <random>
#include <algorithm>
#include <map>
#include <vector>
#include <memory>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
 
#include "Vipper/Vipper.h"
#include "CSDLRenderer.h"

#include "Modules/Gameplay/Gameplay.h"
#include "Modules/TitleScreen/TitleScreen.h"

int main ( int argc, char **argv ) {
  std::shared_ptr<Vipper::IRouter> nextRouter;
  std::shared_ptr<Vipper::IPresenter> presenter;
  std::shared_ptr<Vipper::IRouter> router;
  
  auto renderer = std::make_shared<RunTheWorld::CSDLRenderer>();
  
  auto titleScreenRouter = std::make_shared<RunTheWorld::CTitleScreenRouter>(renderer);
  
  titleScreenRouter->initWithDefaults();
  
  nextRouter = titleScreenRouter;
  presenter = titleScreenRouter->getPresenter();

  std::vector<std::shared_ptr<Vipper::IRouter>> routers;
  
  do {  
    if ( nextRouter != nullptr ) {
      if ( router != nullptr ) {
        router->onRelinquishFocus();
        if ( !router->isFinished() ) {
          routers.push_back( router );
        }
        
      }      
      router = nextRouter;
      router->onFocus();
      nextRouter = nullptr;
    }
    
    presenter = router->getPresenter();
    presenter->present();
    
    renderer->update();
    renderer->render();
    SDL_Delay(33);
    
    nextRouter = router->route();

    if ( router->isFinished() && nextRouter == nullptr ) {
      
      nextRouter = routers.back();
      routers.pop_back();
    }

  } while ( nextRouter != router );

  renderer->shutdown();
  return 0;
}
