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
#include "Vipper/Vipper.h"
#include "NativeBitmap.h"
#include "CDOSRenderer.h"
#include "Modules/Gameplay/Gameplay.h"
#include "Modules/TitleScreen/TitleScreen.h"

std::shared_ptr<Vipper::IRouter> nextRouter;
std::shared_ptr<Vipper::IPresenter> presenter;
std::shared_ptr<Vipper::IRouter> router;
std::vector<std::shared_ptr<Vipper::IRouter>> routers;
std::shared_ptr<RunTheWorld::CDOSRenderer> renderer;

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

    renderer = std::make_shared<RunTheWorld::CDOSRenderer>();
    auto titleScreenRouter = std::make_shared<RunTheWorld::CTitleScreenRouter>(renderer);
    titleScreenRouter->initWithDefaults();

    nextRouter = titleScreenRouter;
    presenter = titleScreenRouter->getPresenter();

    while (true) {
        tick();
    }

    return 0;
}
