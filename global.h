#pragma once

#define ESP8266 1
// #define WIDGETS_SHOW_DEBUG_FRAMES 1

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

// ******** SELECT LANGUAGE ********

#define LAN_ENG
//#define LAN_PL
//#define LAN_YOUR_LANGUAGE

// ******** TRANSLATIONS ********

#ifdef LAN_PL

#define TR_CCODE "pl"
#define TR_DOW_1 "Niedziela"
#define TR_DOW_2 "Poniedziałek"
#define TR_DOW_3 "Wtorek"
#define TR_DOW_4 "Środa"
#define TR_DOW_5 "Czwartek"
#define TR_DOW_6 "Piątek"
#define TR_DOW_7 "Sobota"
#define TR_NOW "teraz"

/*
#elif LAN_YOUR_LANGUAGE
#define TR_CCODE "YOUR TRANSLATION"
#define TR_DOW_1 "YOUR TRANSLATION"
#define TR_DOW_2 "YOUR TRANSLATION"
#define TR_DOW_3 "YOUR TRANSLATION"
#define TR_DOW_4 "YOUR TRANSLATION"
#define TR_DOW_5 "YOUR TRANSLATION"
#define TR_DOW_6 "YOUR TRANSLATION"
#define TR_DOW_7 "YOUR TRANSLATION"
#define TR_NOW "YOUR TRANSLATION"
*/

#else // LAN_ENG

#define TR_CCODE "eng"
#define TR_DOW_1 "Sunday"
#define TR_DOW_2 "Monday"
#define TR_DOW_3 "Tuesday"
#define TR_DOW_4 "Wednesday"
#define TR_DOW_5 "Thursday"
#define TR_DOW_6 "Friday"
#define TR_DOW_7 "Saturday"
#define TR_NOW "now"

#endif