CXX = clang++

SDL_LIB = `sdl-config --libs ` -lSDL_gfx -lSDL_image -lSDL_mixer -lSDL_ttf
SDL_INCLUDE = -I/usr/local/include `sdl-config --cflags `

CXXFLAGS = -std=c++1y -g  $(SDL_INCLUDE) -Iinclude -Ifixed_point/include
OBJS = SDLVersion/CSDLRenderer.o SDLVersion/main.o src/Modules/Gameplay/Entities/CGameSession.o src/Modules/Gameplay/Interactor/CGameplayInteractor.o src/Modules/Gameplay/Presenter/CGameplayPresenter.o src/Modules/Gameplay/Presenter/CLevelPresenter.o src/Modules/Gameplay/Router/CGameplayRouter.o src/Modules/Gameplay/View/CGameplayView.o src/Modules/HighScoresScreen/Interactor/CHighScoresScreenInteractor.o src/Modules/HighScoresScreen/Presenter/CHighScoresScreenPresenter.o src/Modules/HighScoresScreen/Router/CHighScoresScreenRouter.o src/Modules/HighScoresScreen/View/CHighScoresScreenView.o src/Modules/TitleScreen/Interactor/CTitleScreenInteractor.o src/Modules/TitleScreen/Presenter/CTitleScreenPresenter.o src/Modules/TitleScreen/Router/CTitleScreenRouter.o src/Modules/TitleScreen/View/CPlayButton.o src/Modules/TitleScreen/View/CRect.o src/Modules/TitleScreen/View/CTitleScreenView.o src/Vipper/IEntity.o src/Vipper/IInteractor.o src/Vipper/IPresenter.o src/Vipper/IRenderer.o src/Vipper/IRouter.o src/Vipper/IView.o src/Modules/Gameplay/Entities/CLevel.o

LIBS = $(SDL_LIB)
TARGET = game

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:   $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
