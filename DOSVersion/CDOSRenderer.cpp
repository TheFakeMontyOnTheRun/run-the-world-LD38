#include <array>
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <conio.h>
#include <dpmi.h>
#include <go32.h>
#include <sys/movedata.h>
#include <pc.h>
#include <sys/farptr.h>
#include <bios.h>
#include <sys/nearptr.h>

#include <cstdlib>
#include <cstdio>
#include <functional>
#include <cmath>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstring>
#include <memory>
#include <iostream>
#include <map>
#include <array>
#include <iostream>

#include <conio.h>

#include "Vipper/Vipper.h"
#include "NativeBitmap.h"
#include "CDOSRenderer.h"

//
// Created by monty on 06/10/16.
//
#include <memory>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>

#include "NativeBitmap.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



namespace RunTheWorld {

  std::vector<char> readToBuffer(FILE *fileDescriptor) {
    const unsigned N = 1024;
    std::vector<char> total;
    while (true) {
      char buffer[N];
      size_t read = fread((void *) &buffer[0], 1, N, fileDescriptor);
      if (read) {
	for (int c = 0; c < read; ++c) {
	  total.push_back(buffer[c]);
	}
      }
      if (read < N) {
	break;
      }
    }

    return total;
  }
  
  
  std::vector<char> loadBinaryFileFromPath(const std::string &path) {
    FILE *fd;

    fd = fopen(path.c_str(), "rb");

    if (fd == nullptr) {
      exit(0);
    }

    std::vector<char> toReturn = readToBuffer(fd);
    fclose(fd);

    return toReturn;
  }
  
    Vipper::IRenderer::BitmapId CDOSRenderer::loadBitmap( std::string path ) {
      

      auto id = 1;

      for ( const auto& bitmap : mSprites ) {
	if (bitmap->getId() == path ) {
	  return id;
	}
	++id;
      }
      
      id = mSprites.size() + 1;
      
      std::cout << "loading " << path << std::endl;

      auto buffer = loadBinaryFileFromPath( path );
      int xSize;
      int ySize;
      int components;
      
      auto image = stbi_load_from_memory((const stbi_uc *) buffer.data(), buffer.size(), &xSize, &ySize, &components, 0 );
      auto rawData = new int[ xSize * ySize ];
      std::memcpy( rawData, image, xSize * ySize * 4 );
      stbi_image_free(image);
      mSprites.push_back( std::make_shared<odb::NativeBitmap>( path, xSize, ySize, rawData ) );
      return id;
    }
    
    Vipper::IRenderer::SoundId CDOSRenderer::loadSound( std::string path ) {
      return 0;
    }
    
    Vipper::IRenderer::FontId CDOSRenderer::loadFont( std::string path, int sizeInPt ) {
      return 0;
    }

  void CDOSRenderer::fill(float x0, float x1, float y0, float x2, float x3, float y1, std::array<int, 4> shade ) {
      
        float fromY = std::min( y0, y1 );
        float toY = std::max( y0, y1 );
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
            x -= ratiox0x2;
            fx -= ratiox1x3;
	    drawSquare( (int)x, line, (int)fx, line + 1, shade );
        }
    }

    void CDOSRenderer::drawSquare( int x1, int y1, int x2, int y2, std::array<int,4> colour ) {
      int _x0 = x1 / 2;
      int _x1 = x2 / 2;
      int _y0 = (y1 * 200) / 480;
      int _y1 = (y2 * 200 ) / 480;

      int pixel = colour[ 3 ];
      pixel += colour[ 2 ] << 8;
      pixel += colour[ 1 ] << 8;
      
      for ( int y = _y0; y < _y1; ++y ) {
	for ( int x = _x0; x < _x1; ++x ) {

	  if ( x < 0 || x >= 320 || y < 0 || y >= 200 ) {
	    continue;
	  }
	  
	  mBuffer[ (320 * y ) + x ] = pixel;
	}
      }
    };
    
    void CDOSRenderer::drawTextAt( int x, int y, std::string text, std::array<int, 4> colour, Vipper::IRenderer::FontId id ) {

      int offset = 0;
      int start = ( ((y * 25) / 200 ) * 80 ) + ((x * 80 ) / 320 );
      
      for ( char letter : text ) {
	mTextBuffer[ start + offset ] = letter;
	offset++;
      }
    };

  void CDOSRenderer::drawBitmapAt( int dx, int dy, int w, int h, const IRenderer::BitmapId id ) {
      auto bitmap = mSprites[ id ];
      auto data = bitmap->getPixelData();

      int x = 0;
      int y = 0;
      for ( int sy = 0; sy < bitmap->getHeight(); ++sy ) {
	for ( int sx = 0; sx < bitmap->getWidth(); ++sx ) {

	  x = (sx + dx) / 2;
	  y = ((sy + dy) * 200 ) / 480;
	  
	  if ( x < 0 || x >= 320 || y < 0 || y >= 200 ) {
	    continue;
	  }

	  auto pixel = data + ( sy * bitmap->getWidth() ) + sx;
	  auto colour = *pixel;

	  if ( (colour & 0xFF000000 ) < 254 ) {
	    continue;
	  }
	  
	  mBuffer[ (320 * y ) + x ] = colour;
	  sx++;
	}
	sy++;
      }
  };

  void CDOSRenderer::playSound( const IRenderer::SoundId& id ) {
  };    
  
  CDOSRenderer::CDOSRenderer() {
    __dpmi_regs reg;

    reg.x.ax = 0x13;
    __dpmi_int(0x10, &reg);

    outp(0x03c8, 0);

    for ( int r = 0; r < 4; ++r ) {
      for ( int g = 0; g < 4; ++g ) {
	for ( int b = 0; b < 4; ++b ) {
	  outp(0x03c9, (r * (85) ) );
	  outp(0x03c9, (g * (85) ) );
	  outp(0x03c9, (b * (85) ) );
	}
      }
    }
    
  }

  void CDOSRenderer::shutdown() {

  }

  void CDOSRenderer::update() {

    while ( kbhit() ) {
      switch ( getch() ) {
      case 'a':
	dispatchKeyDownToListeners(Vipper::ECommand::kLeft);
	dispatchKeyUpToListeners(Vipper::ECommand::kLeft);
	break;
      case 'd':
	dispatchKeyDownToListeners(Vipper::ECommand::kRight);
	dispatchKeyUpToListeners(Vipper::ECommand::kRight);
	break;
	
      case 'w':
	dispatchKeyDownToListeners(Vipper::ECommand::kUp);
	dispatchKeyUpToListeners(Vipper::ECommand::kUp);
	break;
	
      case 's':
	dispatchKeyDownToListeners(Vipper::ECommand::kDown);
	dispatchKeyUpToListeners(Vipper::ECommand::kDown);
	break;
      case 'z':
	dispatchKeyDownToListeners(Vipper::ECommand::kFire1);
	dispatchKeyUpToListeners(Vipper::ECommand::kFire1);
	break;
      default:
	break;
      }
    }
  }

  unsigned char getPaletteEntry( int origin ) {
    unsigned char shade = 0;
    shade += (((((origin & 0x0000FF)      ) * 4  ) / 255 ) ) << 4;
    shade += (((((origin & 0x00FF00) >> 8 ) * 4  ) / 255 ) ) << 2;
    shade += (((((origin & 0xFF0000) >> 16) * 4  ) / 255 ) ) << 0;
    return shade;
  }
  
  
  void CDOSRenderer::render() {

    int offset = 0;

    unsigned char buffer[ 320 * 200 ];
    int origin = 0;
    int lastOrigin = -1;
    unsigned char shade;
    
    for (int y = 0; y < 200; ++y) {
      for (int x = 0; x < 320; ++x) {

	offset = (y * 320) + x;
	origin = mBuffer[offset];

	if ( origin != lastOrigin ) {
	  shade = getPaletteEntry( origin );
	}

	lastOrigin = origin;

	buffer[ ( y * 320 ) + x ] = shade;
      }
    }

    dosmemput(buffer, 64000, 0xa0000);
    return;
    
    for ( int y = 0; y < 25; ++y ) {
      for ( int x = 0; x < 80; ++x ) {
	char c = ( mTextBuffer[ ( y * 80 ) + x ] );
	
	if ( c != ' ' ) {
	  gotoxy( x + 1, y + 1);
	  std::cout << c;
	}
      }
    }
    std::cout.flush();
    std::fill( std::begin( mTextBuffer), std::end( mTextBuffer ), ' ' );
    
  }
}
