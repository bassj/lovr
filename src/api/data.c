#include "api.h"
#include "data/audioStream.h"
#include "data/modelData.h"
#include "data/rasterizer.h"
#include "data/textureData.h"

int l_lovrDataInit(lua_State* L) {
  lua_newtable(L);
  luaL_register(L, NULL, lovrData);
  luax_registertype(L, "Blob", lovrBlob);
  luax_registertype(L, "AudioStream", lovrAudioStream);
  luax_registertype(L, "ModelData", lovrModelData);
  luax_registertype(L, "Rasterizer", lovrRasterizer);
  luax_extendtype(L, "Blob", "TextureData", lovrBlob, lovrTextureData);
  luax_extendtype(L, "Blob", "VertexData", lovrBlob, lovrVertexData);
  return 1;
}

int l_lovrDataNewBlob(lua_State* L) {
  size_t size;
  uint8_t* data = NULL;
  int type = lua_type(L, 1);
  if (type == LUA_TNUMBER) {
    size = lua_tonumber(L, 1);
    data = calloc(1, size);
  } else if (type == LUA_TSTRING) {
    const char* str = luaL_checklstring(L, 1, &size);
    data = malloc(size + 1);
    memcpy(data, str, size);
    data[size] = '\0';
  } else {
    Blob* blob = luax_checktypeof(L, 1, Blob);
    size = blob->size;
    data = malloc(size);
  }
  const char* name = luaL_optstring(L, 2, "");
  Blob* blob = lovrBlobCreate(data, size, name);
  luax_pushtype(L, Blob, blob);
  lovrRelease(blob);
  return 1;
}

int l_lovrDataNewAudioStream(lua_State* L) {
  Blob* blob = luax_readblob(L, 1, "Sound");
  size_t bufferSize = luaL_optinteger(L, 2, 4096);
  AudioStream* stream = lovrAudioStreamCreate(blob, bufferSize);
  luax_pushtype(L, AudioStream, stream);
  lovrRelease(blob);
  lovrRelease(stream);
  return 1;
}

int l_lovrDataNewModelData(lua_State* L) {
  Blob* blob = luax_readblob(L, 1, "Model");
  ModelData* modelData = lovrModelDataCreate(blob);
  luax_pushtype(L, ModelData, modelData);
  lovrRelease(blob);
  lovrRelease(modelData);
  return 1;
}

int l_lovrDataNewRasterizer(lua_State* L) {
  Blob* blob = NULL;
  float size;

  if (lua_type(L, 1) == LUA_TNUMBER || lua_isnoneornil(L, 1)) {
    size = luaL_optnumber(L, 1, 32);
  } else {
    blob = luax_readblob(L, 1, "Font");
    size = luaL_optnumber(L, 2, 32);
  }

  Rasterizer* rasterizer = lovrRasterizerCreate(blob, size);
  luax_pushtype(L, Rasterizer, rasterizer);
  lovrRelease(blob);
  lovrRelease(rasterizer);
  return 1;
}

int l_lovrDataNewTextureData(lua_State* L) {
  TextureData* textureData = NULL;
  if (lua_type(L, 1) == LUA_TNUMBER) {
    int width = luaL_checknumber(L, 1);
    int height = luaL_checknumber(L, 2);
    textureData = lovrTextureDataGetBlank(width, height, 0x0, FORMAT_RGBA);
  } else {
    Blob* blob = luax_readblob(L, 1, "Texture");
    textureData = lovrTextureDataFromBlob(blob);
    lovrRelease(blob);
  }

  luax_pushtype(L, TextureData, textureData);
  lovrRelease(textureData);
  return 1;
}

int l_lovrDataNewVertexData(lua_State* L) {
  uint32_t count;
  int dataIndex = 0;
  bool hasFormat = false;
  VertexFormat format;
  vertexFormatInit(&format);

  if (lua_isnumber(L, 1)) {
    count = lua_tointeger(L, 1);
  } else if (lua_istable(L, 1)) {
    if (lua_isnumber(L, 2)) {
      hasFormat = luax_checkvertexformat(L, 1, &format);
      count = lua_tointeger(L, 2);
      dataIndex = 0;
    } else if (lua_istable(L, 2)) {
      hasFormat = luax_checkvertexformat(L, 1, &format);
      count = lua_objlen(L, 2);
      dataIndex = 2;
    } else {
      count = lua_objlen(L, 1);
      dataIndex = 1;
    }
  } else {
    return luaL_argerror(L, 1, "table or number expected");
  }

  VertexData* vertexData = lovrVertexDataCreate(count, hasFormat ? &format : NULL);

  if (dataIndex) {
    luax_loadvertices(L, dataIndex, &vertexData->format, (VertexPointer) { .raw = vertexData->blob.data });
  }
  luax_pushtype(L, VertexData, vertexData);
  lovrRelease(vertexData);
  return 1;
}

const luaL_Reg lovrData[] = {
  { "newBlob", l_lovrDataNewBlob },
  { "newAudioStream", l_lovrDataNewAudioStream },
  { "newModelData", l_lovrDataNewModelData },
  { "newRasterizer", l_lovrDataNewRasterizer },
  { "newTextureData", l_lovrDataNewTextureData },
  { "newVertexData", l_lovrDataNewVertexData },
  { NULL, NULL }
};
