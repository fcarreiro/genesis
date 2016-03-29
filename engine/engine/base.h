//////////////////////////////////////////////////////////////////////////
// Files frequently changed
//////////////////////////////////////////////////////////////////////////

#ifndef __INCBASE_H__
#define __INCBASE_H__

////////////////////////////////////
// Memory Manager

#ifdef _DEBUG
#include "../../common/utility/memmgr.h"
#endif

////////////////////////////////////
// Math

#include "../math/vectors.h"
#include "../math/3dmath.h"

////////////////////////////////////
// Utility Library

#include "../../common/utility/ext_etl.h"
#include "../../common/utility/timer.h"
#include "../../common/utility/manager.h"
#include "../../common/utility/options.h"

////////////////////////////////////
// File I/O

#include "../file_io/file_base.h"
#include "../file_io/package.h"
#include "../file_io/standard.h"
#include "../file_io/file.h"

////////////////////////////////////
// Language Packs

#include "../language/language.h"

////////////////////////////////////
// Digest

#include "../digest/digest.h"
#include "../digest/crc32.h"
#include "../digest/md5_base.h"
#include "../digest/md5.h"

////////////////////////////////////
// Perlin Noise

#include "../perlin/perlin.h"

////////////////////////////////////
// Texture

#include "../texture/image.h"
#include "../texture/texture.h"

////////////////////////////////////
// Sound

#include "../sound/sample.h"
#include "../sound/sound.h"
#include "../sound/snd_load_auto.h"
#include "../sound/snd_load_wav.h"

////////////////////////////////////
// Font

#ifdef WIN32
#define BITMAP_FONT_AVAILABLE 1
#else
#define BITMAP_FONT_AVAILABLE 0
#endif

#include "../font/font_base.h"
#include "../font/texture_font.h"
#if BITMAP_FONT_AVAILABLE == 1
#include "../font/bitmap_font.h"
#endif
#include "../font/font.h"

////////////////////////////////////
// Frustum

#include "../frustum/frustum.h"

////////////////////////////////////
// Camera

#include "../camera/camera.h"

////////////////////////////////////
// Models

#include "../model/model.h"

////////////////////////////////////
// Actor

#include "../actor/actor.h"

////////////////////////////////////
// Player

#include "../player/player.h"
#include "../player/human_player.h"
#include "../player/npc_player.h"

////////////////////////////////////
// Map

#include "../map/landscape/landscape.h"
#include "../map/landscape/quadnode.h"

#include "../map/sea/seanode.h"
#include "../map/sea/sea.h"

#include "../map/sky/sky.h"
#include "../map/map.h"

////////////////////////////////////
// Network System

#include "../network/network.h"

////////////////////////////////////
// Window System

#include "../winsys/windefs.h"
#include "../winsys/window.h"
#include "../winsys/static.h"
#include "../winsys/button.h"
#include "../winsys/textbox.h"
#include "../winsys/checkbox.h"
#include "../winsys/imagebox.h"
#include "../winsys/winsys.h"

////////////////////////////////////
// Modules

#include "../sound/sound.h"
#include "../input/input.h"
#include "../opengl/opengl.h"
#include "../console/console.h"

////////////////////////////////////
// Engine & Exports

#include "../engine/engine.h"

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
