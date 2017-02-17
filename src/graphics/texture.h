#include "glfw.h"
#include "util.h"

struct CanvasState;

#pragma once

typedef enum {
  FORMAT_RED,
  FORMAT_RG,
  FORMAT_RGB,
  FORMAT_RGBA
} TextureFormat;

typedef struct {
  void* data;
  int width;
  int height;
  int channels;
  TextureFormat format;
} TextureData;

typedef enum {
  FILTER_NEAREST = GL_NEAREST,
  FILTER_LINEAR = GL_LINEAR
} FilterMode;

typedef enum {
  WRAP_CLAMP = GL_CLAMP_TO_EDGE,
  WRAP_REPEAT = GL_REPEAT,
  WRAP_MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
  WRAP_CLAMP_ZERO = GL_CLAMP_TO_BORDER
} WrapMode;

typedef enum {
  PROJECTION_ORTHOGRAPHIC,
  PROJECTION_PERSPECTIVE
} TextureProjection;

typedef struct {
  Ref ref;
  TextureData* textureData;
  GLuint id;
  GLuint msaaId;
  GLuint framebuffer;
  GLuint resolveFramebuffer;
  GLuint depthBuffer;
  TextureProjection projection;
  FilterMode filterMin;
  FilterMode filterMag;
  WrapMode wrapHorizontal;
  WrapMode wrapVertical;
  int msaa;
} Texture;

Texture* lovrTextureCreate(TextureData* textureData);
Texture* lovrTextureCreateWithFramebuffer(TextureData* textureData, TextureProjection projection, int msaa);
void lovrTextureDestroy(const Ref* ref);
void lovrTextureDataDestroy(TextureData* textureData);
void lovrTextureBindFramebuffer(Texture* texture);
void lovrTextureResolveMSAA(Texture* texture);
void lovrTextureRefresh(Texture* texture);
int lovrTextureGetHeight(Texture* texture);
int lovrTextureGetWidth(Texture* texture);
void lovrTextureGetFilter(Texture* texture, FilterMode* min, FilterMode* mag);
void lovrTextureSetFilter(Texture* texture, FilterMode min, FilterMode mag);
void lovrTextureGetWrap(Texture* texture, WrapMode* horizontal, WrapMode* vertical);
void lovrTextureSetWrap(Texture* texture, WrapMode horizontal, WrapMode vertical);
