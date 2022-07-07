//
// Created by tod on 2022-07-06.
//

#include "bl-tiger.h"
#include <stdlib.h>

#define ARRAY_SIZE(X) uint32_t(sizeof(X) / sizeof(X[0]))

static const double PI = 3.14159265359;


struct TigerPath {
  inline TigerPath()
      : fillColor(0),
        strokeColor(0),
        fillRule(BL_FILL_RULE_NON_ZERO),
        fill(false),
        stroke(false) {}
  inline ~TigerPath() {}

  BLPath blPath;
  BLPath blStrokedPath;
  BLStrokeOptions blStrokeOptions;

  BLRgba32 fillColor;
  BLRgba32 strokeColor;

  BLFillRule fillRule;
  bool fill;
  bool stroke;
};
struct Tiger {
  Tiger() {
    init(
        TigerData::commands, ARRAY_SIZE(TigerData::commands),
        TigerData::points, ARRAY_SIZE(TigerData::points));
  }

  ~Tiger() {
    destroy();
  }

  void init(const char* commands, int commandCount, const float* points, uint32_t pointCount) {
    size_t c = 0;
    size_t p = 0;

    float h = TigerData::height;

    while (c < commandCount) {
      TigerPath* tp = new TigerPath();

      // Fill params.
      switch (commands[c++]) {
        case 'N': tp->fill = false; break;
        case 'F': tp->fill = true; tp->fillRule = BL_FILL_RULE_NON_ZERO; break;
        case 'E': tp->fill = true; tp->fillRule = BL_FILL_RULE_EVEN_ODD; break;
      }

      // Stroke params.
      switch (commands[c++]) {
        case 'N': tp->stroke = false; break;
        case 'S': tp->stroke = true; break;
      }

      switch (commands[c++]) {
        case 'B': tp->blStrokeOptions.setCaps(BL_STROKE_CAP_BUTT); break;
        case 'R': tp->blStrokeOptions.setCaps(BL_STROKE_CAP_ROUND); break;
        case 'S': tp->blStrokeOptions.setCaps(BL_STROKE_CAP_SQUARE); break;
      }

      switch (commands[c++]) {
        case 'M': tp->blStrokeOptions.join = BL_STROKE_JOIN_MITER_BEVEL; break;
        case 'R': tp->blStrokeOptions.join = BL_STROKE_JOIN_ROUND; break;
        case 'B': tp->blStrokeOptions.join = BL_STROKE_JOIN_BEVEL; break;
      }

      tp->blStrokeOptions.miterLimit = points[p++];
      tp->blStrokeOptions.width = points[p++];

      // Stroke & Fill style.
      tp->strokeColor = BLRgba32(uint32_t(points[p + 0] * 255.0f), uint32_t(points[p + 1] * 255.0f), uint32_t(points[p + 2] * 255.0f), 255);
      tp->fillColor = BLRgba32(uint32_t(points[p + 3] * 255.0f), uint32_t(points[p + 4] * 255.0f), uint32_t(points[p + 5] * 255.0f), 255);
      p += 6;

      // Path.
      int i, count = int(points[p++]);
      for (i = 0 ; i < count; i++) {
        switch (commands[c++]) {
          case 'M':
            tp->blPath.moveTo(points[p], h - points[p + 1]);
            p += 2;
            break;
          case 'L':
            tp->blPath.lineTo(points[p], h - points[p + 1]);
            p += 2;
            break;
          case 'C':
            tp->blPath.cubicTo(points[p], h - points[p + 1], points[p + 2], h - points[p + 3], points[p + 4], h - points[p + 5]);
            p += 6;
            break;
          case 'E':
            tp->blPath.close();
            break;
        }
      }

      tp->blPath.shrink();

      if (tp->stroke) {
        tp->blStrokedPath.addStrokedPath(tp->blPath, tp->blStrokeOptions, blDefaultApproximationOptions);
        tp->blStrokedPath.shrink();
      }

      paths.append(tp);
    }
  }

  void destroy() {
    for (size_t i = 0, count = paths.size(); i < count; i++)
      delete paths[i];
    paths.reset();
  }

  BLArray<TigerPath*> paths;
};

static Tiger* _tiger = nullptr;

void onRenderB2D(BLContext& ctx, const int width, const int height, const double _rot, const double _scale, const bool _cacheStroke, const bool _renderStroke) noexcept {
  if (_tiger == nullptr) {
    _tiger = new Tiger;
  }

  bool renderStroke = _renderStroke;

  ctx.setFillStyle(BLRgba32(0xFF00007F));
  ctx.fillAll();

  double minX = 17;
  double minY = 53;
  double maxX = 562.0;
  double maxY = 613.0;
  double s = blMin(width / (maxX - minX), height / (maxY - minY)) * _scale;

  BLMatrix2D m;
  m.reset();
  m.rotate((_rot / 180.0) * PI, minX + maxX / 2.0, minY + maxY / 2.0);
  m.postTranslate(-maxX / 2, -maxY / 2);

  ctx.save();
  ctx.translate(width / 2, height / 2);
  ctx.scale(s);
  ctx.transform(m);

  for (size_t i = 0, count = _tiger->paths.size(); i < count; i++) {
    const TigerPath* tp = _tiger->paths[i];

    if (tp->fill) {
      ctx.setFillStyle(tp->fillColor);
      ctx.setFillRule(tp->fillRule);
      ctx.fillPath(tp->blPath);
    }

    if (tp->stroke && renderStroke) {
      if (_cacheStroke) {
        ctx.setFillStyle(tp->strokeColor);
        ctx.fillPath(tp->blStrokedPath);
      }
      else {
        ctx.setStrokeStyle(tp->strokeColor);
        ctx.setStrokeOptions(tp->blStrokeOptions);
        ctx.strokePath(tp->blPath);
      }
    }
  }

  ctx.restore();
}


