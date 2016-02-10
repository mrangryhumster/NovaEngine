#ifndef SENGINECONF_H
#define SENGINECONF_H

#include "NTypes.h"
#include "types/dim2.h"
#include "ERendererEnums.h"
#include "ILogger.h"

namespace novaengine
{


struct SEngineConf
{

  //---------------
  SEngineConf() :
    Renderer(renderer::ERT_OPENGL),
    WindowSize(640,480),
    Bits(32),
    FullScreen(false),
    DoubleBuf(true),
    VSync(false),
    AntiAlias(0),
    ZBits(32),
    IgnoreWindowEvents(false),
    ExternalWindowPointer(nullptr),
    StartWindowHidden(false),
    LogLevel(log::ELL_INFO)
  {

  }
  //---------------
  renderer::E_RENDERER_TYPE Renderer;

  core::dim2<u32>       WindowSize;
  u32                   Bits;

  bool                  FullScreen;
  bool                  DoubleBuf;
  bool                  VSync;
  u32                   AntiAlias;

  u32                   ZBits;
  bool                  IgnoreWindowEvents;
  void*                 ExternalWindowPointer;
  bool                  StartWindowHidden;
  u32                   LogLevel;
};
}
#endif // SENGINECONF_H
