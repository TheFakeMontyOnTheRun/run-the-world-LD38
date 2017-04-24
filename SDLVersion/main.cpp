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

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>

#endif

#include "Vipper/Vipper.h"
#include "CSDLRenderer.h"

#include "Modules/Gameplay/Gameplay.h"
#include "Modules/TitleScreen/TitleScreen.h"
std::shared_ptr<Vipper::IRouter> nextRouter;
std::shared_ptr<Vipper::IPresenter> presenter;
std::shared_ptr<Vipper::IRouter> router;
std::vector<std::shared_ptr<Vipper::IRouter>> routers;
std::shared_ptr<RunTheWorld::CSDLRenderer> renderer;

void tick() {

    if (nextRouter != nullptr) {
        if (router != nullptr) {
            router->onRelinquishFocus();
            if (!router->isFinished()) {
                routers.push_back(router);
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


    nextRouter = router->route();

    if (router->isFinished() && nextRouter == nullptr) {

        nextRouter = routers.back();
        routers.pop_back();
    }


    if (nextRouter == router) {
        renderer->shutdown();
        exit(0);
    }

}

int main(int argc, char **argv) {

    renderer = std::make_shared<RunTheWorld::CSDLRenderer>();
    auto titleScreenRouter = std::make_shared<RunTheWorld::CTitleScreenRouter>(renderer);
    titleScreenRouter->initWithDefaults();

    nextRouter = titleScreenRouter;
    presenter = titleScreenRouter->getPresenter();


#ifdef __EMSCRIPTEN__
    //  emscripten_request_fullscreen(0, 1);
  emscripten_set_main_loop( tick, 30, 1 );
#else
    while (true) {
        tick();
        SDL_Delay(33);
    }

#endif



    return 0;
}
