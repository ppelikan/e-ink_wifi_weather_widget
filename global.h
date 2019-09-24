#pragma once

#define ESP8266 1

#include <c_types.h>

typedef struct
{
    const u8 *data;
    uint16 width;
    uint16 height;
    u8 dataSize;
} tImage;

#include "icons/icon_cloud.h"
#include "icons/icon_clouds.h"
#include "icons/icon_sun.h"
#include "icons/icon_suncloud.h"
#include "icons/icon_fogg.h"
#include "icons/icon_rain.h"
#include "icons/icon_snow.h"
#include "icons/icon_thunder.h"
#include "icons/icon_smallrain.h"
#include "icons/icon_smallrainmoon.h"
#include "icons/icon_moonstars.h"
#include "icons/icon_mooncloud.h"