#include "bl-tiger.h"

#include <SDL.h>
#include <blend2d.h>
#include <string>
#include <cmath>

#define ARRAY_SIZE(X) uint32_t(sizeof(X) / sizeof(X[0]))
constexpr double pi = 3.1415926535897932;

const double ToRadian(const double degree) {
  if (degree > 360) {
    return std::fmod(degree, 360.0) * pi / 180;
  } else if (degree < 0) {
    return (360 + degree) * pi / 180;
  } else {
    return degree * pi / 180;
  }
}

// ============================================================================
// [SDL_Application]
// ============================================================================

class SDL_Application {
 public:
  SDL_Application() noexcept
      : _window(nullptr),
        _renderer(nullptr),
        _texture(nullptr),
        _blSurface(),
        _quitting(false),
        _exitCode(0),
        _frameCounter(0),
        _frameTicks(0) {}
  ~SDL_Application() noexcept { destroyWindow(); }

  // --------------------------------------------------------------------------
  // [Core]
  // --------------------------------------------------------------------------

  bool createWindow(int w, int h) noexcept {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
      printf("Failed to initialize SDL: %s", SDL_GetError());
      return false;
    }

    _window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
    if (!_window)
      return false;

    _renderer = SDL_CreateRenderer(_window, -1, 0);
    if (!_renderer) {
      printf("FAILED to create SDL_Renderer: %s\n", SDL_GetError());
      destroyWindow();
      return false;
    }
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_NONE);

    if (!createSurface(w, h)) {
      destroyWindow();
      return false;
    }

    return true;
  }

  void destroyWindow() noexcept {
    destroySurface();

    if (_renderer) {
      SDL_DestroyRenderer(_renderer);
      _renderer = nullptr;
    }

    if (!_window) {
      SDL_DestroyWindow(_window);
      _window = nullptr;
    }
  }

  bool createSurface(int w, int h) noexcept {
    destroySurface();

    _texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, w, h);
    if (!_texture)
      return false;
    SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_NONE);

    if (_blSurface.create(w, h, BL_FORMAT_PRGB32) != BL_SUCCESS)
      return false;

    return true;
  }

  void destroySurface() noexcept {
    if (_texture) {
      SDL_DestroyTexture(_texture);
      _texture = nullptr;
    }

    _blSurface.reset();
  }

  int run() noexcept {
    SDL_Event event;

    for (;;) {
      while (!_quitting && SDL_PollEvent(&event))
        onEvent(event);

      if (_quitting)
        break;

      onFrame();
      onRender();
      blit();
      updateFrameCounter();

      //SDL_Delay(20);
    }

    return _exitCode;
  }

  void blit() noexcept {
    BLImageData data;
    _blSurface.getData(&data);

    SDL_UpdateTexture(_texture, nullptr, data.pixelData, int(data.stride));
    SDL_RenderCopy(_renderer, _texture, nullptr, nullptr);
    SDL_RenderPresent(_renderer);
  }

  void updateFrameCounter() noexcept {
    uint32_t ticks = SDL_GetTicks();
    if (++_frameCounter >= 100) {
      _fps = (1000.0 / double(ticks - _frameTicks)) * double(_frameCounter);
      _frameCounter = 0;
      _frameTicks = ticks;
    }
  }

  inline double fps() const noexcept { return _fps; }

  // --------------------------------------------------------------------------
  // [Interface]
  // --------------------------------------------------------------------------

  virtual void onEvent(SDL_Event &event) noexcept = 0;
  virtual void onFrame() noexcept = 0;
  virtual void onRender() noexcept = 0;

  // --------------------------------------------------------------------------
  // [Members]
  // --------------------------------------------------------------------------

  SDL_Window *_window;
  SDL_Renderer *_renderer;
  SDL_Texture *_texture;
  BLImage _blSurface;

  bool _quitting;
  int _exitCode;
  int _frameCounter;
  int _frameTicks;
  double _fps;
};

// ============================================================================
// [MyApplication]
// ============================================================================

class MyApplication : public SDL_Application {
 public:
  enum : uint32_t { kNumVertices = 100 };

  MyApplication() noexcept { onInit(); }

  void onEvent(SDL_Event &e) noexcept override {
    if (e.type == SDL_QUIT) {
      _quitting = true;
    }
    else if( e.type == SDL_KEYDOWN )
    {
      switch( e.key.keysym.sym )
      {
        case SDLK_1:
        case SDLK_2:
        case SDLK_3:
        case SDLK_4:
        case SDLK_5:

          break;
      }
    }
  }

  void onInit() noexcept {
    auto font_path = std::string(SDK_ROOT_PATH) + std::string("/assets/fonts/Roboto-Regular.ttf");
    printf("%s\n", font_path.c_str());
    BLFontFace face;
    face.createFromFile(font_path.c_str());
    font.createFromFace(face, 20.f);

  }

  void onFrame() noexcept override {

  }

  void onRender() noexcept override {
    BLContext ctx(_blSurface);
    onRenderB2D(ctx, _blSurface.width(), _blSurface.height(), 0, 1.0, false, true);
  }

  void initPoints() noexcept {
  }

  BLFont font;


};

// ============================================================================
// [Main]
// ============================================================================

int main(int argc, char *argv[]) {
  MyApplication app;

  if (!app.createWindow(1024, 1024))
    return 1;

  return app.run();
}