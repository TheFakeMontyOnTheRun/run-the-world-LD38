#include <array>
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "Vipper/Vipper.h"
#include "CSDLRenderer.h"


namespace RunTheWorld {
        
    Vipper::IRenderer::BitmapId CSDLRenderer::loadBitmap( std::string path ) {
      auto id = mSprites.size() + 1;
      mSprites[ id ] = IMG_Load( path.c_str() );
      return id;
    }
    
    Vipper::IRenderer::SoundId CSDLRenderer::loadSound( std::string path ) {
      auto id = mSounds.size() + 1;
      mSounds[ id ] = Mix_LoadWAV(path.c_str() );
      return id;
    }
    
    Vipper::IRenderer::FontId CSDLRenderer::loadFont( std::string path, int sizeInPt ) {
      auto id = mFonts.size() + 1;
      mFonts[ id ] = TTF_OpenFont(path.c_str(), sizeInPt );

      return id;
    }

    void CSDLRenderer::fill(float x0, float x1, float y0, float x2, float x3, float y1, int shade[3]) {
        float fromY = std::min( y0, y1 );
        float toY = std::max( y0, y1 );
        SDL_Rect rect;
        float deltaY = toY - fromY;

        float ratiox0x2 = 1.0f;
        float ratiox1x3 = 1.0f;

        if ( toY - fromY > 0.0f  ) {
            ratiox0x2 = ( x0 - x2 ) / deltaY;
            ratiox1x3 = ( x1 - x3 ) / deltaY;
        }

        float x = x0;
        float fx = x1;

        for ( int line = toY; line >= fromY; line--) {

            rect.x = x;
            rect.y = line;
            rect.w = ( fx - x);
            rect.h = 1;

            x -= ratiox0x2;
            fx -= ratiox1x3;

            SDL_FillRect(video, &rect, SDL_MapRGB(video->format, shade[0], shade[1],  shade[2] ));
        }
    }

    void CSDLRenderer::drawSquare( int x, int y, int x2, int y2, std::array<int,4> colour ) {
      SDL_Rect rect;
      rect.x = x;
      rect.y = y;
      rect.w = ( x2 - x );
      rect.h = ( y2 - y );
      int colourMerged = SDL_MapRGBA( video->format, colour[ 0 ], colour[ 1 ], colour[ 2 ], colour[ 3 ] );
      SDL_FillRect( video, &rect, colourMerged );
    };
    
    void CSDLRenderer::drawTextAt( int x, int y, std::string text, std::array<int, 4> colour, Vipper::IRenderer::FontId id ) {
      if ( id == 0 ) {
        return;
      }

      SDL_Color color = { (Uint8)colour[ 0 ], (Uint8)colour[ 1 ], (Uint8)colour[ 2 ], (Uint8)colour[ 3 ] };
      auto font = mFonts[ id ];
      auto result = TTF_RenderText_Solid( font, text.c_str(), color );
      SDL_Rect rect;
      rect.x = x;
      rect.y = y;
      rect.w = result->w;
      rect.h = result->h;
      SDL_BlitSurface( result, nullptr, video, &rect );
      SDL_FreeSurface( result );
    };

    void CSDLRenderer::drawBitmapAt( int x, int y, int w, int h, const IRenderer::BitmapId id ) {

      if ( id == 0 ) {
        return;
      }

      auto bitmap = mSprites[ id ];
      SDL_Rect rect;
      rect.x = x;
      rect.y = y;
      rect.w = w;
      rect.h = h;
      SDL_BlitSurface( bitmap, nullptr, video, &rect );
    };

    void CSDLRenderer::playSound( const IRenderer::SoundId& id ) {
      if ( id == 0 ) {
        return;
      }

      auto sound = mSounds[ id ];

      Mix_PlayChannel( -1, sound, 0 );
    };    
  
  CSDLRenderer::CSDLRenderer() {
  
    //REFACTOR!  
    SDL_Init(  SDL_INIT_EVERYTHING );
    TTF_Init();
    video = SDL_SetVideoMode( 640, 480, 32, 0 );

    if ( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1 ) {
      std::cout << "coudlnt init mixer" << std::endl;
    }
  }

  void CSDLRenderer::shutdown() {
    SDL_Quit();
  }

  void CSDLRenderer::update() {
    SDL_Event event;


    if ( SDL_PollEvent( &event ) ) {

      if( event.type == SDL_QUIT ) {
        exit(0);
      }
      
      if ( event.type == SDL_MOUSEBUTTONDOWN ) {
        dispatchClickToListeners( std::pair<int, int>( event.button.x, event.button.y ) );
      }

        if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
                case SDLK_q:
#ifndef __EMSCRIPTEN__
                    exit(0);
#endif
                case SDLK_LEFT:
                    dispatchKeyUpToListeners(Vipper::ECommand::kLeft);
                    break;

                case SDLK_RIGHT:
                    dispatchKeyUpToListeners(Vipper::ECommand::kRight);
                    break;

                case SDLK_UP:
                    dispatchKeyUpToListeners(Vipper::ECommand::kUp);
                    break;

                case SDLK_DOWN:
                    dispatchKeyUpToListeners(Vipper::ECommand::kDown);
                    break;
                case SDLK_SPACE:
                    dispatchKeyUpToListeners(Vipper::ECommand::kFire1);
                    break;
                default:
                    break;
            }
        }

        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                    dispatchKeyDownToListeners(Vipper::ECommand::kLeft);
                    break;
                case SDLK_RIGHT:
                    dispatchKeyDownToListeners(Vipper::ECommand::kRight);
                    break;

                case SDLK_UP:
                    dispatchKeyDownToListeners(Vipper::ECommand::kUp);
                    break;

                case SDLK_DOWN:
                    dispatchKeyDownToListeners(Vipper::ECommand::kDown);
                    break;
                case SDLK_SPACE:
                    dispatchKeyDownToListeners(Vipper::ECommand::kFire1);
                    break;
                default:
                    break;
            }
        }
    }
  }

  void CSDLRenderer::render() {
    SDL_Flip(video);
  }
}
