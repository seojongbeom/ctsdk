# How to build packages on msvc2019

mkdir libSDL
cd libSDL
git clone https://github.com/libsdl-org/SDL.git
git clone https://github.com/libsdl-org/SDL_image.git
git clone https://github.com/libsdl-org/SDL_ttf.git

---
download zlib, libpng, freetype, bzip2, harfbuzz
---
mkdir -p build/zlib build/libpng build/freetype build/harfbuzz 
mkdir -p build/sdl build/sdl_image build/sdl_ttf

---
Build Debug
---
cmake ..\..\zlib-1.2.12 -DCMAKE_INSTALL_PREFIX=C:\TOOLS\SDLSDK\Debug
cmake --build . --parallel --config Debug
cmake --install . --config Debug

cmake ..\..\libpng-1.6.37 -DCMAKE_INSTALL_PREFIX=C:\TOOLS\SDLSDK\Debug -DCMAKE_PREFIX_PATH=C:\TOOLS\SDLSDK\Debug
cmake --build . --parallel --config Debug
cmake --install . --config Debug

cmake ..\..\freetype-2.12.1 -DCMAKE_INSTALL_PREFIX=C:\TOOLS\SDLSDK\Debug -DCMAKE_PREFIX_PATH=C:\TOOLS\SDLSDK\Debug
cmake --build . --parallel --config Debug
cmake --install . --config Debug

cmake ..\..\harfbuzz-4.3.0 -DCMAKE_INSTALL_PREFIX=C:\TOOLS\SDLSDK\Debug -DCMAKE_PREFIX_PATH=C:\TOOLS\SDLSDK\Debug
cmake --build . --parallel --config Debug
cmake --install . --config Debug

cmake ..\..\SDL -DSDL_STATIC_PIC=ON -DCMAKE_INSTALL_PREFIX=C:\TOOLS\SDLSDK\Debug -DCMAKE_PREFIX_PATH=C:\TOOLS\SDLSDK\Debug
cmake --build . --parallel --config Debug
cmake --install . --config Debug

cmake ..\..\sdl_image -DCMAKE_INSTALL_PREFIX=C:\TOOLS\SDLSDK\Debug -DCMAKE_PREFIX_PATH=C:\TOOLS\SDLSDK\Debug
cmake --build . --parallel --config Debug
cmake --install . --config Debug

cmake ..\..\SDL_ttf -DCMAKE_INSTALL_PREFIX=C:\TOOLS\SDLSDK\Debug -DCMAKE_PREFIX_PATH=C:\TOOLS\SDLSDK\Debug
cmake --build . --parallel --config Debug
cmake --install . --config Debug

---
Build Release
---
cmake ..\..\zlib-1.2.12 -DCMAKE_INSTALL_PREFIX=C:\TOOLS\SDLSDK\Release
cmake --build . --parallel --config Release
cmake --install . --config Release

cmake ..\..\libpng-1.6.37 -DCMAKE_INSTALL_PREFIX=C:\TOOLS\SDLSDK\Release -DCMAKE_PREFIX_PATH=C:\TOOLS\SDLSDK\Release
cmake --build . --parallel --config Release
cmake --install . --config Release

cmake ..\..\freetype-2.12.1 -DCMAKE_INSTALL_PREFIX=C:\TOOLS\SDLSDK\Release -DCMAKE_PREFIX_PATH=C:\TOOLS\SDLSDK\Release
cmake --build . --parallel --config Release
cmake --install . --config Release

cmake ..\..\harfbuzz-4.3.0 -DCMAKE_INSTALL_PREFIX=C:\TOOLS\SDLSDK\Release -DCMAKE_PREFIX_PATH=C:\TOOLS\SDLSDK\Release -DBUILD_SHARED_LIBS=OFF
cmake --build . --parallel --config Release
cmake --install . --config Release

cmake ..\..\SDL -DSDL_STATIC_PIC=ON -DCMAKE_INSTALL_PREFIX=C:\TOOLS\SDLSDK\Release -DCMAKE_PREFIX_PATH=C:\TOOLS\SDLSDK\Release
cmake --build . --parallel --config Release
cmake --install . --config Release

cmake ..\..\sdl_image -DCMAKE_INSTALL_PREFIX=C:\TOOLS\SDLSDK\Release -DCMAKE_PREFIX_PATH=C:\TOOLS\SDLSDK\Release
cmake --build . --parallel --config Release
cmake --install . --config Release

cmake ..\..\SDL_ttf -DCMAKE_INSTALL_PREFIX=C:\TOOLS\SDLSDK\Release -DCMAKE_PREFIX_PATH=C:\TOOLS\SDLSDK\Release
cmake --build . --parallel --config Release
cmake --install . --config Release


cmake ..\..\jsoncpp -DCMAKE_INSTALL_PREFIX=C:\TOOLS\SDLSDK\Release -DCMAKE_PREFIX_PATH=C:\TOOLS\SDLSDK\Release -DJSONCPP_STATIC_WINDOWS_RUNTIME=OFF -DBUILD_SHARED_LIBS=OFF -DBUILD_OBJECT_LIBS=OFF

cmake ..\..\curl -DCMAKE_INSTALL_PREFIX=C:\TOOLS\SDLSDK\Release -DCMAKE_PREFIX_PATH=C:\TOOLS\SDLSDK\Release -DCURL_DISABLE_HTTP=OFF -DSSL_ENABLED=ON -DCURL_USE_OPENSSL=ON



## MINGW


cmake ../../packages/jsoncpp/ -DCMAKE_INSTALL_PREFIX=$UIXSDK_INSTALL_PREFIX -DCMAKE_PREFIX_PATH=$UIXSDK_PREFIX_PATH -G "MinGW Makefiles" -DBUILD_SHARED_LIBS=OFF


cmake ../../packages/SDL_image/external/libpng/ -DCMAKE_INSTALL_PREFIX=$UIXSDK_INSTALL_PREFIX -DCMAKE_PREFIX_PATH=$UIXSDK_PREFIX_PATH -G "MinGW Makefiles" -DBUILD_SHARED_LIBS=OFF -DPNG_SHARED=OFF

cmake ../../packages/SDL_image/external/jpeg/ -DCMAKE_INSTALL_PREFIX=$UIXSDK_INSTALL_PREFIX -DCMAKE_PREFIX_PATH=$UIXSDK_PREFIX_PATH -G "MinGW Makefiles" -DBUILD_SHARED_LIBS=OFF

cmake ../../packages/SDL -DSDL_STATIC_PIC=ON -DCMAKE_INSTALL_PREFIX=$UIXSDK_INSTALL_PREFIX -DCMAKE_PREFIX_PATH=$UIXSDK_PREFIX_PATH -G "MinGW Makefiles" -DBUILD_SHARED_LIBS=OFF



