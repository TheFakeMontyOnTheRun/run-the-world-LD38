
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
#include "Common.h"
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

        fseek(fileDescriptor, 0, SEEK_END);
        auto endPos = ftell(fileDescriptor);
        rewind(fileDescriptor);
        std::vector<char> total(endPos);
        auto writeHead = std::begin(total);

        for (int c = 0; c < endPos; ++c) {
            char buffer[N];
            size_t read = fread((void *) &buffer[0], 1, N, fileDescriptor);
            if (read) {
                for (int c = 0; c < read; ++c) {
                    *writeHead = (buffer[c]);
                    writeHead = std::next(writeHead);
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
            std::cout << "deu ruim: " << path << std::endl;
            exit(0);
        }

        std::vector<char> toReturn = readToBuffer(fd);
        fclose(fd);

        return toReturn;
    }

    Vipper::IRenderer::BitmapId CDOSRenderer::loadBitmap(const std::string& path) {


        auto id = 1;

        for (const auto &bitmap : mSprites) {
            if (bitmap->getId() == path) {
                return id;
            }
            ++id;
        }

        id = mSprites.size() + 1;

        auto buffer = loadBinaryFileFromPath("resDOS/" + path);
        int xSize;
        int ySize;
        int components;

        auto image = stbi_load_from_memory((const stbi_uc *) buffer.data(), buffer.size(), &xSize, &ySize, &components,
                                           0);
        auto rawData = new int[xSize * ySize];
        std::memcpy(rawData, image, xSize * ySize * (components));
        stbi_image_free(image);

        for (int c = 0; c < xSize * ySize; ++c) {
            int origin = rawData[c];

            int r = (origin & 0xFF000000) >> 24;
            int g = (origin & 0x00FF0000) >> 16;
            int b = (origin & 0x0000FF00) >> 8;
            int a = (origin & 0x000000FF);

            int r20 = ((r / 21) * 21);
            int g20 = ((g / 21) * 21);
            int b20 = ((b / 21) * 21);

            int pixel = a;
            pixel += (r20 << 24);
            pixel += (g20 << 16);
            pixel += (b20 << 8);

            rawData[c] = pixel;
        }

        mSprites.push_back(std::make_shared<odb::NativeBitmap>(path, xSize, ySize, rawData));

        return id;
    }

    Vipper::IRenderer::SoundId CDOSRenderer::loadSound(const std::string& path) {
        return 0;
    }

    Vipper::IRenderer::FontId CDOSRenderer::loadFont(const std::string& path, int sizeInPt) {
        return 0;
    }

    void CDOSRenderer::fill(int x0, int x1, int y0, int x2, int x3, int y1, const std::array<int, 4>& colour) {

        int fromY = std::min(y0, y1);
        int toY = std::max(y0, y1);
        auto deltaY = FixP{toY - fromY};

        auto ratiox0x2 = FixP{1};
        auto ratiox1x3 = FixP{1};

        if (toY - fromY > 0) {
            ratiox0x2 = (x0 - x2) / deltaY;
            ratiox1x3 = (x1 - x3) / deltaY;
        }

        auto x = FixP{x0};
        auto fx = FixP{x1};

        int pixel = 0;
        pixel += colour[0];
        pixel += colour[1] << 8;
        pixel += colour[2] << 16;
        pixel += colour[3] << 24;

        auto data = std::begin(mBuffer);

        for (int y = toY; y >= fromY; y--) {
            x -= ratiox0x2;
            fx -= ratiox1x3;

            int _x0 = std::min(319, std::max(0, static_cast<int>(x) ));
            int _x1 = std::min(319, std::max(0, static_cast<int>(fx)));
            int _y0 = std::min(199, std::max(0, y ));
            auto line = data + (320 * _y0);

            std::fill(line + _x0, line + _x1, pixel);
        }
    }

    void CDOSRenderer::drawSquare(int x1, int y1, int x2, int y2, const std::array<int, 4>& colour) {
        int _x0 = std::min(319, std::max(0, x1 ));
        int _x1 = std::min(319, std::max(0, x2 ));
        int _y0 = std::min(199, std::max(0, y1 ));
        int _y1 = std::min(199, std::max(0, y2 ));

        int pixel = 0;
        pixel += colour[0];
        pixel += colour[1] << 8;
        pixel += colour[2] << 16;
        pixel += colour[3] << 24;

        auto data = std::begin(mBuffer);

        for (int y = _y0; y < _y1; ++y) {
            auto line = data + (320 * y);
            std::fill(line + _x0, line + _x1, pixel);
        }
    };

    std::pair<int, int> CDOSRenderer::getBitmapSize( const BitmapId& id ) {
        auto sprite = mSprites[ id - 1];

        return { sprite->getWidth(), sprite->getHeight() };
    };

    void
    CDOSRenderer::drawTextAt(int x, int y, const std::string& text, const std::array<int, 4>& colour, const Vipper::IRenderer::FontId& id) {
    };

    void CDOSRenderer::drawBitmapAt(int dx, int dy, int w, int h, const IRenderer::BitmapId& id) {
        auto bitmap = mSprites[id - 1];
        auto data = bitmap->getPixelData();

        int x = 0;
        int y = 0;
        int width = bitmap->getWidth();
        int height = bitmap->getHeight();

        for (int sy = 0; sy < height; ++sy) {

            auto line = std::begin( mBuffer ) + (320 * y);
            auto sourceLine = data + (sy * width);

            y = (sy + dy);

            if ( y < 0 || y >= 200) {
                continue;
            }

            for (int sx = 0; sx < width; ++sx) {

                x = (sx + dx);

                if (x < 0 || x >= 320 ) {
                    continue;
                }

                auto pixel = sourceLine + sx;
                auto colour = *pixel;

                if (((colour & 0xFF000000) ) ) {
                    *( line + x ) = colour;
                }
            }
        }
    };

    void CDOSRenderer::playSound(const IRenderer::SoundId &id) {
    };

    CDOSRenderer::CDOSRenderer() {
        __dpmi_regs reg;

        reg.x.ax = 0x13;
        __dpmi_int(0x10, &reg);

        outp(0x03c8, 0);

        for (int r = 0; r < 4; ++r) {
            for (int g = 0; g < 8; ++g) {
                for (int b = 0; b < 8; ++b) {
                    outp(0x03c9, (r * (21)));
                    outp(0x03c9, (g * (8)));
                    outp(0x03c9, (b * (8)));
                }
            }
        }

    }

    void CDOSRenderer::shutdown() {

    }

    void CDOSRenderer::update() {
        auto lastKey = bioskey(0x11);

        bdos(0xC, 0, 0);

        switch (lastKey) {
            case 18656:
                //up
                dispatchKeyDownToListeners(Vipper::ECommand::kUp);
                dispatchKeyUpToListeners(Vipper::ECommand::kUp);
                break;

            case 8051:
            case 20704:
                //down
                dispatchKeyDownToListeners(Vipper::ECommand::kDown);
                dispatchKeyUpToListeners(Vipper::ECommand::kDown);
                break;

            case 19424: //right arrow
            case 4209: //q
                //left
                dispatchKeyDownToListeners(Vipper::ECommand::kLeft);
                dispatchKeyUpToListeners(Vipper::ECommand::kLeft);
                break;

            case 4709: //e
            case 19936: //right arrow
                //right
                dispatchKeyDownToListeners(Vipper::ECommand::kRight);
                dispatchKeyUpToListeners(Vipper::ECommand::kRight);
                break;

            case 3849:
            case 14624:
            case 11785: //c
            case 5236: //t
                //space
                dispatchKeyDownToListeners(Vipper::ECommand::kFire1);
                dispatchKeyUpToListeners(Vipper::ECommand::kFire1);
                break;

        }
    }

    unsigned char getPaletteEntry(int origin) {
        unsigned char shade = 0;
        shade += (((((origin & 0x0000FF)) << 2) >> 8)) << 6;
        shade += (((((origin & 0x00FF00) >> 8) << 3) >> 8)) << 3;
        shade += (((((origin & 0xFF0000) >> 16) << 3) >> 8)) << 0;
        return shade;
    }

    void CDOSRenderer::render() {

        int offset = 0;

        int origin = 0;
        int lastOrigin = -1;
        unsigned char shade;

        auto pixel = std::begin(mFinalBuffer);
        for (const auto &origin : mBuffer) {

            if (origin != lastOrigin) {
                shade = getPaletteEntry(origin);
            }

            lastOrigin = origin;

            *pixel = shade;

            pixel = std::next(pixel);
        }

        mFrame++;
        dosmemput(mFinalBuffer, 64000, 0xa0000);
    }
}
