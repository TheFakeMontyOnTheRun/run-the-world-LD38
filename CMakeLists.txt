cmake_minimum_required(VERSION 3.7)
project(run_the_world_LD38)

set(CMAKE_CXX_STANDARD 11)

find_package(SDL REQUIRED)
find_package(SDL_mixer REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_ttf REQUIRED)


include_directories(include fixed_point/include
        ${SDL_INCLUDE_DIRS} ${SDL_MIXER_INCLUDE_DIRS} ${SDL_IMAGE_INCLUDE_DIRS} ${SDL_TTF_INCLUDE_DIRS})

set(SOURCE_FILES
        include/Modules/Gameplay/Entities/CGameSession.h
        include/Modules/Gameplay/Interactor/CGameplayInteractor.h
        include/Modules/Gameplay/Presenter/CGameplayPresenter.h
        include/Modules/Gameplay/Presenter/CLevelPresenter.h
        include/Modules/Gameplay/Router/CGameplayRouter.h
        include/Modules/Gameplay/View/CGameplayView.h
        include/Modules/Gameplay/View/CGameSessionView.h
        include/Modules/Gameplay/View/CLevelView.h
        include/Modules/Gameplay/Gameplay.h
        include/Modules/HighScoresScreen/Interactor/CHighScoresScreenInteractor.h
        include/Modules/HighScoresScreen/Presenter/CHighScoresScreenPresenter.h
        include/Modules/HighScoresScreen/Router/CHighScoresScreenRouter.h
        include/Modules/HighScoresScreen/View/CHighScoresScreenView.h
        include/Modules/HighScoresScreen/HighScores.h
        include/Modules/TitleScreen/Interactor/CTitleScreenInteractor.h
        include/Modules/TitleScreen/Presenter/CTitleScreenPresenter.h
        include/Modules/TitleScreen/Router/CTitleScreenRouter.h
        include/Modules/TitleScreen/View/CPlayButton.h
        include/Modules/TitleScreen/View/CRect.h
        include/Modules/TitleScreen/View/CTitleScreenView.h
        include/Modules/TitleScreen/TitleScreen.h
        include/Vipper/IEntity.h
        include/Vipper/IInteractor.h
        include/Vipper/IPresenter.h
        include/Vipper/IRenderer.h
        include/Vipper/IRouter.h
        include/Vipper/IView.h
        include/Vipper/Vipper.h
        include/CGameSnapshot.cpp
        include/CGameSnapshot.h
        include/Common.h
        include/CRenderer.h
        SDLVersion/CSDLRenderer.cpp
        SDLVersion/main.cpp
        src/Modules/Gameplay/Entities/CGameSession.cpp
        src/Modules/Gameplay/Interactor/CGameplayInteractor.cpp
        src/Modules/Gameplay/Presenter/CGameplayPresenter.cpp
        src/Modules/Gameplay/Presenter/CLevelPresenter.cpp
        src/Modules/Gameplay/Router/CGameplayRouter.cpp
        src/Modules/Gameplay/View/CGameplayView.cpp
        src/Modules/HighScoresScreen/Interactor/CHighScoresScreenInteractor.cpp
        src/Modules/HighScoresScreen/Presenter/CHighScoresScreenPresenter.cpp
        src/Modules/HighScoresScreen/Router/CHighScoresScreenRouter.cpp
        src/Modules/HighScoresScreen/View/CHighScoresScreenView.cpp
        src/Modules/TitleScreen/Interactor/CTitleScreenInteractor.cpp
        src/Modules/TitleScreen/Presenter/CTitleScreenPresenter.cpp
        src/Modules/TitleScreen/Router/CTitleScreenRouter.cpp
        src/Modules/TitleScreen/View/CPlayButton.cpp
        src/Modules/TitleScreen/View/CRect.cpp
        src/Modules/TitleScreen/View/CTitleScreenView.cpp
        src/Vipper/IEntity.cpp
        src/Vipper/IInteractor.cpp
        src/Vipper/IPresenter.cpp
        src/Vipper/IRenderer.cpp
        src/Vipper/IRouter.cpp
        src/Vipper/IView.cpp
        src/Modules/Gameplay/Entities/CLevel.cpp)


find_package(OpenGL REQUIRED)

include (FindPkgConfig)


add_executable(run_the_world_LD38 ${SOURCE_FILES})
target_link_libraries(run_the_world_LD38 ${SDL_LIBRARY} ${SDL_MIXER_LIBRARY} ${SDL_IMAGE_LIBRARY} ${SDL_TTF_LIBRARY} ${OPENGL_LIBRARIES} )
