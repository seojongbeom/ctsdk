#include <blend2d.h>
#include <SDL.h>
#include <string>
#include <vector>

#define ARRAY_SIZE(X) uint32_t(sizeof(X) / sizeof(X[0]))

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

//      printf("FPS: %.2f\n", _fps);
    }
  }

  inline double fps() const noexcept { return _fps; }

  // --------------------------------------------------------------------------
  // [Interface]
  // --------------------------------------------------------------------------

  virtual void onEvent(SDL_Event& event) noexcept = 0;
  virtual void onFrame() noexcept = 0;
  virtual void onRender() noexcept = 0;

  // --------------------------------------------------------------------------
  // [Members]
  // --------------------------------------------------------------------------

  SDL_Window* _window;
  SDL_Renderer* _renderer;
  SDL_Texture* _texture;
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
  MyApplication() noexcept { onInit(); }

  void onEvent(SDL_Event& e) noexcept override {
    if (e.type == SDL_QUIT) {
      _quitting = true;
    }
    else if( e.type == SDL_KEYDOWN )
    {
      switch( e.key.keysym.sym )
      {
        case SDLK_f:
          is_fill = !is_fill;
          break;

        case SDLK_g:
          is_guide_line = !is_guide_line;
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

    is_fill = true;
    is_guide_line = false;
  }

  void onFrame() noexcept override {

  }

  void onRender() noexcept override {
    BLContext ctx(_blSurface);

    // Clear the image.
    ctx.setCompOp(BL_COMP_OP_SRC_COPY);
    ctx.fillAll();

    // Fill some path.
    BLPath path;
    path.moveTo(26, 31);
    path.cubicTo(642, 132, 587, -136, 25, 464);
    path.cubicTo(882, 404, 144, 267, 27, 31);

    ctx.setCompOp(BL_COMP_OP_SRC_OVER);

    ctx.save();

    auto ox = (1024 - (882 - 25)) / 2;
    auto oy = (1024 - (464 + 136))/ 2;

    ctx.translate(ox,oy);

    if (is_fill) {
      ctx.setFillStyle(BLRgba32(0xFFFFFFFF));
      ctx.fillPath(path);
    } else {
      ctx.setStrokeStyle(BLRgba32(0xFFFFFFFF));
      ctx.setStrokeWidth(10);
      ctx.setStrokeStartCap(BL_STROKE_CAP_BUTT);
      ctx.setStrokeEndCap(BL_STROKE_CAP_BUTT);
      ctx.strokePath(path);
    }

    path.clear();
    path.moveTo(100, 550);
    path.quadTo(150, 480, 200, 530);

    ctx.setStrokeStyle(BLRgba32(0xFFFF00FF));
    ctx.setStrokeWidth(6);
    ctx.setStrokeStartCap(BL_STROKE_CAP_ROUND);
    ctx.setStrokeEndCap(BL_STROKE_CAP_ROUND);
    ctx.strokePath(path);

    {
      BLPath path;
      path.moveTo(26, 31);
      path.cubicTo(642, 132, 587, -136, 25, 464);
      path.cubicTo(882, 404, 144, 267, 27, 31);
    }

    if (is_guide_line) {
      auto guid_a = std::vector<BLPoint>();
      auto guid_b = std::vector<BLPoint>();

      guid_a.emplace_back(BLPoint{26, 31});
      guid_a.emplace_back(BLPoint{642, 132});
      guid_a.emplace_back(BLPoint{587, -136});
      guid_a.emplace_back(BLPoint{25, 464});

      guid_b.emplace_back(BLPoint{25, 464});
      guid_b.emplace_back(BLPoint{882, 404});
      guid_b.emplace_back(BLPoint{144, 267});
      guid_b.emplace_back(BLPoint{26, 31});

#if 0 // not supported dash line.
      auto option = ctx.strokeOptions();
      option.dashOffset = 3;
      option.dashArray.append(10, 10);
      ctx.setStrokeOptions(option);
#endif
      auto dashArray = BLArray<double>();
      dashArray.append(10);
      dashArray.append(6);
      ctx.setStrokeDashArray(dashArray);
      ctx.setStrokeDashOffset(10);

      ctx.setStrokeWidth(4);
      ctx.setStrokeStartCap(BL_STROKE_CAP_BUTT);
      ctx.setStrokeEndCap(BL_STROKE_CAP_BUTT);

      ctx.setStrokeStyle(BLRgba32(0xFF00FFFF));

      ctx.strokeLine(26, 31, 642, 132);
      ctx.strokeLine(25, 464, 587, -136);

      ctx.setStrokeStyle(BLRgba32(0xFFFF00FF));

      ctx.strokeLine(25, 464, 882, 404);
      ctx.strokeLine(26, 31, 144, 267);

      auto DrawBox = [&ctx](const double x, const double y){
        ctx.fillBox(x-5,y-5,x+5,y+5);
      };

      ctx.setFillStyle(BLRgba32(0xFFFF0000));
      DrawBox(26,31);

      ctx.setFillStyle(BLRgba32(0xFF00FF00));
      DrawBox(642,132);
      DrawBox(587,-136);
      DrawBox(25,464);

      ctx.setFillStyle(BLRgba32(0xFF0000FF));
      DrawBox(882,404);
      DrawBox(144,267);
      DrawBox(27,31);
    }

    char buf[128];
    snprintf(buf, ARRAY_SIZE(buf), "FPS: %.2f", fps());


    ctx.restore();
    ctx.setFillStyle(BLRgba32(0xFFFF0000u));
    ctx.fillUtf8Text(BLPoint(5, 20), font, "Key f = fill toggle, g = show/hide bezier guide");
  }

  BLFont font;
  bool is_fill;
  bool is_guide_line;
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