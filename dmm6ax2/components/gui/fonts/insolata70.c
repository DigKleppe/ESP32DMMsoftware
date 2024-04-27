/*******************************************************************************
 * Size: 70 px
 * Bpp: 1
 * Opts: 
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef INSOLATA70
#define INSOLATA70 1
#endif

#if INSOLATA70

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+002C "," */
    0x39, 0xe7, 0xdf, 0x7c, 0xf1, 0xc7, 0x18, 0x63,
    0x8c, 0x31, 0x8e, 0x30, 0x0,

    /* U+002D "-" */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf8,

    /* U+002E "." */
    0x27, 0xff, 0xf7, 0x10,

    /* U+0030 "0" */
    0x0, 0xf8, 0x0, 0x1f, 0xf0, 0x1, 0xff, 0xc0,
    0x1e, 0xf, 0x1, 0xe0, 0x3c, 0xe, 0x0, 0xe0,
    0xf0, 0x7, 0x87, 0x0, 0x1c, 0x38, 0x0, 0xe3,
    0x80, 0x3, 0x9c, 0x0, 0x1c, 0xe0, 0x1, 0xe7,
    0x0, 0xf, 0x38, 0x0, 0xfb, 0x80, 0x6, 0xfc,
    0x0, 0x77, 0xe0, 0x7, 0x3f, 0x0, 0x31, 0xf8,
    0x3, 0x8f, 0xc0, 0x38, 0x7e, 0x1, 0x83, 0xf0,
    0x1c, 0x1f, 0x81, 0xc0, 0xfc, 0xc, 0x7, 0xe0,
    0xe0, 0x3f, 0x6, 0x1, 0xf8, 0x60, 0xf, 0xc7,
    0x0, 0x7e, 0x30, 0x3, 0xf3, 0x0, 0x1f, 0xb8,
    0x0, 0xef, 0x80, 0xe, 0x7c, 0x0, 0x73, 0xc0,
    0x3, 0x9c, 0x0, 0x1c, 0xe0, 0x0, 0xe3, 0x80,
    0xe, 0x1c, 0x0, 0x70, 0xf0, 0x7, 0x83, 0x80,
    0x38, 0xe, 0x7, 0x80, 0x7f, 0xfc, 0x1, 0xff,
    0xc0, 0x3, 0xf8, 0x0, 0x2, 0x0, 0x0,

    /* U+0031 "1" */
    0x0, 0xe0, 0x3c, 0x1f, 0x87, 0xf3, 0xff, 0xf9,
    0xfc, 0x3e, 0x7, 0x0, 0xe0, 0x1c, 0x3, 0x80,
    0x70, 0xe, 0x1, 0xc0, 0x38, 0x7, 0x0, 0xe0,
    0x1c, 0x3, 0x80, 0x70, 0xe, 0x1, 0xc0, 0x38,
    0x7, 0x0, 0xe0, 0x1c, 0x3, 0x80, 0x70, 0xe,
    0x1, 0xc0, 0x38, 0x7, 0x0, 0xe0, 0x1c, 0x3,
    0x80, 0x70, 0xe, 0x1, 0xc0, 0x38, 0x7, 0x0,
    0xe0, 0x1c, 0x3, 0x80, 0x70,

    /* U+0032 "2" */
    0x1, 0xfc, 0x0, 0x7f, 0xf0, 0xf, 0xff, 0x81,
    0xf0, 0x7c, 0x3c, 0x3, 0xc7, 0x80, 0x1e, 0x70,
    0x0, 0xee, 0x0, 0xe, 0xe0, 0x0, 0x76, 0x0,
    0x7, 0x0, 0x0, 0x70, 0x0, 0x7, 0x0, 0x0,
    0x70, 0x0, 0x7, 0x0, 0x0, 0x70, 0x0, 0x7,
    0x0, 0x0, 0x70, 0x0, 0xe, 0x0, 0x0, 0xe0,
    0x0, 0xe, 0x0, 0x1, 0xc0, 0x0, 0x1c, 0x0,
    0x3, 0x80, 0x0, 0x78, 0x0, 0x7, 0x0, 0x0,
    0xf0, 0x0, 0xe, 0x0, 0x1, 0xc0, 0x0, 0x3c,
    0x0, 0x7, 0x80, 0x0, 0x70, 0x0, 0xe, 0x0,
    0x1, 0xe0, 0x0, 0x3c, 0x0, 0x3, 0x80, 0x0,
    0x70, 0x0, 0xf, 0x0, 0x1, 0xe0, 0x0, 0x1c,
    0x0, 0x3, 0x80, 0x0, 0x78, 0x0, 0x1f, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff,

    /* U+0033 "3" */
    0x1, 0xf8, 0x0, 0x7f, 0xe0, 0xf, 0xff, 0x1,
    0xe0, 0xf8, 0x3c, 0x3, 0xc7, 0x0, 0x1c, 0x10,
    0x1, 0xc0, 0x0, 0xe, 0x0, 0x0, 0xe0, 0x0,
    0xe, 0x0, 0x0, 0xe0, 0x0, 0xe, 0x0, 0x0,
    0xe0, 0x0, 0xe, 0x0, 0x1, 0xc0, 0x0, 0x1c,
    0x0, 0x3, 0x80, 0x0, 0x78, 0x0, 0xf, 0x0,
    0xf, 0xe0, 0x3, 0xf8, 0x0, 0x3f, 0xc0, 0x0,
    0x7f, 0x0, 0x0, 0x78, 0x0, 0x3, 0xc0, 0x0,
    0x1e, 0x0, 0x0, 0xe0, 0x0, 0xe, 0x0, 0x0,
    0x70, 0x0, 0x7, 0x0, 0x0, 0x70, 0x0, 0x7,
    0x0, 0x0, 0x70, 0x0, 0x7, 0x0, 0x0, 0x70,
    0x0, 0x7, 0x0, 0x0, 0xe2, 0x0, 0xe, 0x70,
    0x1, 0xef, 0x0, 0x1c, 0x7c, 0x7, 0xc3, 0xff,
    0xf8, 0x1f, 0xff, 0x0, 0x7f, 0xc0, 0x0, 0x40,
    0x0,

    /* U+0034 "4" */
    0x0, 0x0, 0xe0, 0x0, 0x1, 0xc0, 0x0, 0x7,
    0x80, 0x0, 0xf, 0x0, 0x0, 0x3e, 0x0, 0x0,
    0x7c, 0x0, 0x1, 0xf8, 0x0, 0x3, 0xf0, 0x0,
    0xe, 0xe0, 0x0, 0x3d, 0xc0, 0x0, 0x73, 0x80,
    0x1, 0xe7, 0x0, 0x3, 0x8e, 0x0, 0xf, 0x1c,
    0x0, 0x1c, 0x38, 0x0, 0x78, 0x70, 0x0, 0xe0,
    0xe0, 0x3, 0xc1, 0xc0, 0x7, 0x3, 0x80, 0x1c,
    0x7, 0x0, 0x78, 0xe, 0x0, 0xe0, 0x1c, 0x3,
    0xc0, 0x38, 0x7, 0x0, 0x70, 0x1e, 0x0, 0xe0,
    0x38, 0x1, 0xc0, 0xf0, 0x3, 0x81, 0xc0, 0x7,
    0x7, 0x0, 0xe, 0xe, 0x0, 0x1c, 0x3f, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x0,
    0x1, 0xc0, 0x0, 0x3, 0x80, 0x0, 0x7, 0x0,
    0x0, 0xe, 0x0, 0x0, 0x1c, 0x0, 0x0, 0x38,
    0x0, 0x0, 0x70, 0x0, 0x0, 0xe0, 0x0, 0x1,
    0xc0, 0x0, 0x3, 0x80, 0x0, 0x7, 0x0,

    /* U+0035 "5" */
    0x1f, 0xff, 0xf0, 0xff, 0xff, 0x8f, 0xff, 0xfc,
    0x70, 0x0, 0x3, 0x80, 0x0, 0x1c, 0x0, 0x0,
    0xe0, 0x0, 0x7, 0x0, 0x0, 0x38, 0x0, 0x1,
    0xc0, 0x0, 0xe, 0x0, 0x0, 0x70, 0x0, 0x3,
    0x80, 0x0, 0x1c, 0x0, 0x0, 0xe0, 0x0, 0x7,
    0x0, 0x0, 0x38, 0x7c, 0x1, 0xcf, 0xf8, 0xf,
    0xff, 0xe0, 0x7e, 0xf, 0x83, 0xc0, 0x1e, 0x3c,
    0x0, 0x79, 0xc0, 0x3, 0xc0, 0x0, 0xe, 0x0,
    0x0, 0x70, 0x0, 0x1, 0xc0, 0x0, 0xe, 0x0,
    0x0, 0x70, 0x0, 0x3, 0x80, 0x0, 0x1c, 0x0,
    0x0, 0xe0, 0x0, 0x7, 0x0, 0x0, 0x38, 0x0,
    0x1, 0xc0, 0x0, 0xe, 0x0, 0x0, 0xe2, 0x0,
    0x7, 0x30, 0x0, 0x3b, 0xc0, 0x3, 0x8f, 0x0,
    0x3c, 0x3e, 0x3, 0xc0, 0xff, 0xfc, 0x3, 0xff,
    0xc0, 0x7, 0xfc, 0x0, 0x2, 0x0, 0x0,

    /* U+0036 "6" */
    0x0, 0x7e, 0x0, 0x1f, 0xf8, 0x3, 0xff, 0xc0,
    0x7c, 0x3e, 0xf, 0x0, 0xe0, 0xe0, 0x4, 0x1c,
    0x0, 0x3, 0xc0, 0x0, 0x38, 0x0, 0x3, 0x80,
    0x0, 0x70, 0x0, 0x7, 0x0, 0x0, 0x70, 0x0,
    0x7, 0x0, 0x0, 0x60, 0x0, 0xe, 0x0, 0x0,
    0xe0, 0xfc, 0xe, 0x3f, 0xe0, 0xe7, 0xff, 0xe,
    0xf0, 0x78, 0xee, 0x3, 0xcf, 0xc0, 0x1c, 0xf8,
    0x0, 0xef, 0x0, 0xe, 0xe0, 0x0, 0xee, 0x0,
    0x7, 0xe0, 0x0, 0x7e, 0x0, 0x7, 0xe0, 0x0,
    0x7e, 0x0, 0x7, 0xe0, 0x0, 0x7e, 0x0, 0x7,
    0x70, 0x0, 0x77, 0x0, 0x7, 0x70, 0x0, 0x77,
    0x0, 0xe, 0x38, 0x0, 0xe3, 0x80, 0xe, 0x3c,
    0x1, 0xc1, 0xe0, 0x3c, 0x1f, 0x7, 0x80, 0xff,
    0xf8, 0x7, 0xff, 0x0, 0x3f, 0xc0, 0x0, 0x20,
    0x0,

    /* U+0037 "7" */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80,
    0x0, 0xf0, 0x0, 0x1c, 0x0, 0x3, 0x80, 0x0,
    0xf0, 0x0, 0x1c, 0x0, 0x3, 0x80, 0x0, 0x70,
    0x0, 0x1c, 0x0, 0x3, 0x80, 0x0, 0x70, 0x0,
    0x1e, 0x0, 0x3, 0x80, 0x0, 0x70, 0x0, 0xe,
    0x0, 0x3, 0xc0, 0x0, 0x70, 0x0, 0xe, 0x0,
    0x1, 0xc0, 0x0, 0x70, 0x0, 0xe, 0x0, 0x1,
    0xc0, 0x0, 0x38, 0x0, 0xe, 0x0, 0x1, 0xc0,
    0x0, 0x38, 0x0, 0x7, 0x0, 0x1, 0xc0, 0x0,
    0x38, 0x0, 0x7, 0x0, 0x0, 0xe0, 0x0, 0x38,
    0x0, 0x7, 0x0, 0x0, 0xe0, 0x0, 0x1c, 0x0,
    0x7, 0x0, 0x0, 0xe0, 0x0, 0x1c, 0x0, 0x3,
    0x80, 0x0, 0xf0, 0x0, 0x1c, 0x0, 0x3, 0x80,
    0x0,

    /* U+0038 "8" */
    0x0, 0xfc, 0x0, 0x1f, 0xf8, 0x1, 0xff, 0xf0,
    0x1e, 0xf, 0x81, 0xe0, 0x1e, 0xe, 0x0, 0x70,
    0xf0, 0x3, 0xc7, 0x0, 0xe, 0x38, 0x0, 0x71,
    0xc0, 0x3, 0x8e, 0x0, 0x1c, 0x70, 0x0, 0xe3,
    0x80, 0x7, 0x1e, 0x0, 0x70, 0x70, 0x3, 0x83,
    0xc0, 0x3c, 0xf, 0x1, 0xc0, 0x3c, 0x1c, 0x0,
    0xf1, 0xc0, 0x3, 0xdc, 0x0, 0xf, 0xc0, 0x1,
    0xff, 0x0, 0x1f, 0x7c, 0x1, 0xe0, 0xf0, 0x1e,
    0x3, 0xc1, 0xe0, 0xf, 0xe, 0x0, 0x3c, 0xe0,
    0x0, 0xe7, 0x0, 0x7, 0x70, 0x0, 0x1f, 0x80,
    0x0, 0xfc, 0x0, 0x7, 0xe0, 0x0, 0x3f, 0x0,
    0x1, 0xf8, 0x0, 0xf, 0xc0, 0x0, 0x7f, 0x0,
    0x7, 0xb8, 0x0, 0x39, 0xe0, 0x3, 0xcf, 0x0,
    0x1c, 0x3e, 0x3, 0xe0, 0xff, 0xfe, 0x3, 0xff,
    0xe0, 0x7, 0xfc, 0x0, 0x2, 0x0, 0x0,

    /* U+0039 "9" */
    0x1, 0xf8, 0x0, 0x7f, 0xe0, 0xf, 0xff, 0x1,
    0xf0, 0xf8, 0x3c, 0x3, 0x83, 0x80, 0x3c, 0x78,
    0x1, 0xc7, 0x0, 0x1e, 0x70, 0x0, 0xee, 0x0,
    0xe, 0xe0, 0x0, 0xee, 0x0, 0xe, 0xe0, 0x0,
    0x7e, 0x0, 0x7, 0xe0, 0x0, 0x7e, 0x0, 0x7,
    0xe0, 0x0, 0x7e, 0x0, 0x7, 0xf0, 0x0, 0x77,
    0x0, 0x7, 0x70, 0x0, 0xf7, 0x80, 0xf, 0x3c,
    0x3, 0xf1, 0xe0, 0x7f, 0xf, 0xff, 0x70, 0x7f,
    0xc7, 0x1, 0xf8, 0x70, 0x0, 0x7, 0x0, 0x0,
    0x60, 0x0, 0x6, 0x0, 0x0, 0xe0, 0x0, 0xe,
    0x0, 0x0, 0xe0, 0x0, 0xe, 0x0, 0x0, 0xc0,
    0x0, 0x1c, 0x0, 0x1, 0xc0, 0x0, 0x38, 0x20,
    0x3, 0x87, 0x0, 0x78, 0x78, 0xf, 0x7, 0xff,
    0xe0, 0x3f, 0xfc, 0x0, 0xff, 0x80, 0x0, 0x80,
    0x0,

    /* U+0041 "A" */
    0x0, 0xc, 0x0, 0x0, 0x3, 0x0, 0x0, 0x0,
    0xc0, 0x0, 0x0, 0x30, 0x0, 0x0, 0x1e, 0x0,
    0x0, 0x7, 0x80, 0x0, 0x1, 0xe0, 0x0, 0x0,
    0x78, 0x0, 0x0, 0x3f, 0x0, 0x0, 0xf, 0xc0,
    0x0, 0x3, 0x30, 0x0, 0x1, 0xce, 0x0, 0x0,
    0x73, 0x80, 0x0, 0x1c, 0xe0, 0x0, 0x6, 0x18,
    0x0, 0x3, 0x87, 0x0, 0x0, 0xe1, 0xc0, 0x0,
    0x38, 0x70, 0x0, 0xc, 0xc, 0x0, 0x7, 0x3,
    0x80, 0x1, 0xc0, 0xe0, 0x0, 0x70, 0x38, 0x0,
    0x38, 0x7, 0x0, 0xe, 0x1, 0xc0, 0x3, 0x80,
    0x70, 0x0, 0xc0, 0xc, 0x0, 0x70, 0x3, 0x80,
    0x1c, 0x0, 0xe0, 0x7, 0xff, 0xf8, 0x1, 0xff,
    0xfe, 0x0, 0xff, 0xff, 0xc0, 0x38, 0x0, 0x70,
    0xe, 0x0, 0x1c, 0x7, 0x0, 0x3, 0x81, 0xc0,
    0x0, 0xe0, 0x70, 0x0, 0x38, 0x1c, 0x0, 0xe,
    0xe, 0x0, 0x1, 0xc3, 0x80, 0x0, 0x70, 0xe0,
    0x0, 0x1c, 0x38, 0x0, 0x7, 0x1c, 0x0, 0x0,
    0xe7, 0x0, 0x0, 0x39, 0xc0, 0x0, 0xe, 0xe0,
    0x0, 0x3, 0xc0,

    /* U+0048 "H" */
    0xe0, 0x0, 0x3f, 0x0, 0x1, 0xf8, 0x0, 0xf,
    0xc0, 0x0, 0x7e, 0x0, 0x3, 0xf0, 0x0, 0x1f,
    0x80, 0x0, 0xfc, 0x0, 0x7, 0xe0, 0x0, 0x3f,
    0x0, 0x1, 0xf8, 0x0, 0xf, 0xc0, 0x0, 0x7e,
    0x0, 0x3, 0xf0, 0x0, 0x1f, 0x80, 0x0, 0xfc,
    0x0, 0x7, 0xe0, 0x0, 0x3f, 0x0, 0x1, 0xf8,
    0x0, 0xf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x80, 0x0, 0xfc, 0x0, 0x7, 0xe0,
    0x0, 0x3f, 0x0, 0x1, 0xf8, 0x0, 0xf, 0xc0,
    0x0, 0x7e, 0x0, 0x3, 0xf0, 0x0, 0x1f, 0x80,
    0x0, 0xfc, 0x0, 0x7, 0xe0, 0x0, 0x3f, 0x0,
    0x1, 0xf8, 0x0, 0xf, 0xc0, 0x0, 0x7e, 0x0,
    0x3, 0xf0, 0x0, 0x1f, 0x80, 0x0, 0xfc, 0x0,
    0x7, 0xe0, 0x0, 0x3f, 0x0, 0x1, 0xf8, 0x0,
    0xf, 0xc0, 0x0, 0x70,

    /* U+004D "M" */
    0xe0, 0x0, 0xf, 0xc0, 0x0, 0x1f, 0xc0, 0x0,
    0x3f, 0x80, 0x0, 0xff, 0x0, 0x1, 0xff, 0x0,
    0x7, 0xfe, 0x0, 0xf, 0xfe, 0x0, 0x3f, 0xfc,
    0x0, 0x7f, 0xf8, 0x0, 0xff, 0xb8, 0x3, 0xbf,
    0x70, 0x7, 0x7e, 0x70, 0x1c, 0xfc, 0xe0, 0x39,
    0xf9, 0xe0, 0xe3, 0xf1, 0xc1, 0xc7, 0xe3, 0x83,
    0x8f, 0xc3, 0x8e, 0x1f, 0x87, 0x1c, 0x3f, 0x7,
    0x70, 0x7e, 0xe, 0xe0, 0xfc, 0x1f, 0x81, 0xf8,
    0x1f, 0x3, 0xf0, 0x3c, 0x7, 0xe0, 0x38, 0xf,
    0xc0, 0x70, 0x1f, 0x80, 0xc0, 0x3f, 0x0, 0x0,
    0x7e, 0x0, 0x0, 0xfc, 0x0, 0x1, 0xf8, 0x0,
    0x3, 0xf0, 0x0, 0x7, 0xe0, 0x0, 0xf, 0xc0,
    0x0, 0x1f, 0x80, 0x0, 0x3f, 0x0, 0x0, 0x7e,
    0x0, 0x0, 0xfc, 0x0, 0x1, 0xf8, 0x0, 0x3,
    0xf0, 0x0, 0x7, 0xe0, 0x0, 0xf, 0xc0, 0x0,
    0x1f, 0x80, 0x0, 0x3f, 0x0, 0x0, 0x70,

    /* U+0056 "V" */
    0xe0, 0x0, 0x7, 0xe0, 0x0, 0x7, 0xf0, 0x0,
    0x7, 0x70, 0x0, 0x7, 0x70, 0x0, 0xe, 0x70,
    0x0, 0xe, 0x38, 0x0, 0xe, 0x38, 0x0, 0xc,
    0x38, 0x0, 0x1c, 0x38, 0x0, 0x1c, 0x1c, 0x0,
    0x1c, 0x1c, 0x0, 0x18, 0x1c, 0x0, 0x38, 0x1c,
    0x0, 0x38, 0xe, 0x0, 0x38, 0xe, 0x0, 0x30,
    0xe, 0x0, 0x70, 0xf, 0x0, 0x70, 0x7, 0x0,
    0x70, 0x7, 0x0, 0x60, 0x7, 0x0, 0xe0, 0x7,
    0x80, 0xe0, 0x3, 0x80, 0xe0, 0x3, 0x80, 0xc0,
    0x3, 0x81, 0xc0, 0x1, 0xc1, 0xc0, 0x1, 0xc1,
    0xc0, 0x1, 0xc1, 0x80, 0x1, 0xc3, 0x80, 0x0,
    0xe3, 0x80, 0x0, 0xe3, 0x80, 0x0, 0xe3, 0x0,
    0x0, 0xe7, 0x0, 0x0, 0x77, 0x0, 0x0, 0x77,
    0x0, 0x0, 0x76, 0x0, 0x0, 0x7e, 0x0, 0x0,
    0x3e, 0x0, 0x0, 0x3c, 0x0, 0x0, 0x3c, 0x0,
    0x0, 0x3c, 0x0, 0x0, 0x1c, 0x0, 0x0, 0x18,
    0x0, 0x0, 0x18, 0x0,

    /* U+006B "k" */
    0xf0, 0x0, 0x3, 0x80, 0x0, 0xe, 0x0, 0x0,
    0x38, 0x0, 0x0, 0xe0, 0x0, 0x3, 0x80, 0x0,
    0xe, 0x0, 0x0, 0x38, 0x0, 0x0, 0xe0, 0x0,
    0x3, 0x80, 0x0, 0xe, 0x0, 0x0, 0x38, 0x0,
    0x0, 0xe0, 0x0, 0x3, 0x80, 0x0, 0xe, 0x0,
    0x0, 0x38, 0x0, 0x0, 0xe0, 0x0, 0xf3, 0x80,
    0x3, 0x8e, 0x0, 0x1c, 0x38, 0x0, 0xe0, 0xe0,
    0x7, 0x3, 0x80, 0x3c, 0xe, 0x1, 0xe0, 0x38,
    0xf, 0x0, 0xe0, 0x78, 0x3, 0x83, 0xc0, 0xe,
    0x1e, 0x0, 0x38, 0xf0, 0x0, 0xe3, 0x80, 0x3,
    0x9e, 0x0, 0xe, 0xfc, 0x0, 0x3f, 0x78, 0x0,
    0xf8, 0xe0, 0x3, 0xe3, 0xc0, 0xf, 0x7, 0x80,
    0x38, 0xe, 0x0, 0xe0, 0x3c, 0x3, 0x80, 0x78,
    0xe, 0x0, 0xf0, 0x38, 0x1, 0xc0, 0xe0, 0x7,
    0x83, 0x80, 0xf, 0xe, 0x0, 0x1c, 0x38, 0x0,
    0x78, 0xe0, 0x0, 0xf3, 0x80, 0x1, 0xce, 0x0,
    0x7, 0xb8, 0x0, 0xf,

    /* U+006D "m" */
    0x3, 0xc0, 0xf1, 0xcf, 0xc3, 0xf3, 0xbf, 0xcf,
    0xf7, 0x63, 0xb8, 0xef, 0x87, 0x61, 0xfe, 0x7,
    0x81, 0xfc, 0xf, 0x3, 0xf0, 0x1e, 0x7, 0xe0,
    0x38, 0xf, 0xc0, 0x70, 0x1f, 0x80, 0xe0, 0x3f,
    0x1, 0xc0, 0x7e, 0x3, 0x80, 0xfc, 0x7, 0x1,
    0xf8, 0xe, 0x3, 0xf0, 0x1c, 0x7, 0xe0, 0x38,
    0xf, 0xc0, 0x70, 0x1f, 0x80, 0xe0, 0x3f, 0x1,
    0xc0, 0x7e, 0x3, 0x80, 0xfc, 0x7, 0x1, 0xf8,
    0xe, 0x3, 0xf0, 0x1c, 0x7, 0xe0, 0x38, 0xf,
    0xc0, 0x70, 0x1f, 0x80, 0xe0, 0x3f, 0x1, 0xc0,
    0x7e, 0x3, 0x80, 0xfc, 0x7, 0x1, 0xf8, 0xe,
    0x3, 0xf0, 0x1c, 0x7, 0xe0, 0x38, 0xe,

    /* U+006E "n" */
    0x0, 0x7e, 0xe, 0x1f, 0xf8, 0xe3, 0xff, 0xce,
    0x78, 0x3c, 0xee, 0x1, 0xee, 0xc0, 0xe, 0xf8,
    0x0, 0xef, 0x80, 0xf, 0xf0, 0x0, 0x7f, 0x0,
    0x7, 0xe0, 0x0, 0x7e, 0x0, 0x7, 0xe0, 0x0,
    0x7e, 0x0, 0x7, 0xe0, 0x0, 0x7e, 0x0, 0x7,
    0xe0, 0x0, 0x7e, 0x0, 0x7, 0xe0, 0x0, 0x7e,
    0x0, 0x7, 0xe0, 0x0, 0x7e, 0x0, 0x7, 0xe0,
    0x0, 0x7e, 0x0, 0x7, 0xe0, 0x0, 0x7e, 0x0,
    0x7, 0xe0, 0x0, 0x7e, 0x0, 0x7, 0xe0, 0x0,
    0x7e, 0x0, 0x7, 0xe0, 0x0, 0x7e, 0x0, 0x7,
    0xe0, 0x0, 0x70,

    /* U+007A "z" */
    0x7f, 0xff, 0xf3, 0xff, 0xff, 0x9f, 0xff, 0xfc,
    0x0, 0x1, 0xc0, 0x0, 0x1e, 0x0, 0x0, 0xe0,
    0x0, 0xf, 0x0, 0x0, 0x70, 0x0, 0x7, 0x0,
    0x0, 0x78, 0x0, 0x3, 0x80, 0x0, 0x38, 0x0,
    0x1, 0xc0, 0x0, 0x1c, 0x0, 0x1, 0xe0, 0x0,
    0xe, 0x0, 0x0, 0xe0, 0x0, 0xf, 0x0, 0x0,
    0x70, 0x0, 0x7, 0x80, 0x0, 0x38, 0x0, 0x3,
    0x80, 0x0, 0x3c, 0x0, 0x1, 0xc0, 0x0, 0x1c,
    0x0, 0x0, 0xe0, 0x0, 0xe, 0x0, 0x0, 0xf0,
    0x0, 0x7, 0x0, 0x0, 0x7f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,

    /* U+03A9 "Ω" */
    0x0, 0xf, 0x80, 0x0, 0x1, 0xff, 0x0, 0x0,
    0x3f, 0xfe, 0x0, 0x7, 0xff, 0xf8, 0x0, 0x7e,
    0x7, 0xe0, 0x7, 0xc0, 0xf, 0x80, 0x3c, 0x0,
    0x3e, 0x3, 0xc0, 0x0, 0xf0, 0x3c, 0x0, 0x3,
    0xc1, 0xc0, 0x0, 0xe, 0x1e, 0x0, 0x0, 0x78,
    0xe0, 0x0, 0x1, 0xc7, 0x0, 0x0, 0xf, 0x38,
    0x0, 0x0, 0x7b, 0xc0, 0x0, 0x3, 0xcc, 0x0,
    0x0, 0x1e, 0x60, 0x0, 0x0, 0xf3, 0x0, 0x0,
    0x7, 0x98, 0x0, 0x0, 0x3c, 0xe0, 0x0, 0x1,
    0xe7, 0x0, 0x0, 0xf, 0x38, 0x0, 0x0, 0x79,
    0xc0, 0x0, 0x3, 0xce, 0x0, 0x0, 0x1c, 0x78,
    0x0, 0x0, 0xe1, 0xc0, 0x0, 0xf, 0xe, 0x0,
    0x0, 0x78, 0x70, 0x0, 0x3, 0x83, 0xc0, 0x0,
    0x1c, 0xe, 0x0, 0x1, 0xe0, 0x78, 0x0, 0xe,
    0x1, 0xc0, 0x0, 0x70, 0xe, 0x0, 0x7, 0x80,
    0x78, 0x0, 0x38, 0x1, 0xc0, 0x3, 0xc0, 0xf,
    0x0, 0x1c, 0x0, 0x38, 0x1, 0xe0, 0x1, 0xe0,
    0xe, 0x0, 0x7, 0x80, 0xf0, 0x0, 0x3c, 0x7,
    0x0, 0x0, 0xf0, 0x70, 0x0, 0x7f, 0x83, 0xfc,
    0x3, 0xfc, 0x1f, 0xe0,

    /* U+03BC "μ" */
    0xe0, 0x1, 0xc0, 0x70, 0x0, 0xe0, 0x38, 0x0,
    0x70, 0x1c, 0x0, 0x38, 0xe, 0x0, 0x1c, 0x7,
    0x0, 0xe, 0x3, 0x80, 0x7, 0x1, 0xc0, 0x3,
    0x80, 0xe0, 0x1, 0xc0, 0x70, 0x0, 0xe0, 0x38,
    0x0, 0x70, 0x1c, 0x0, 0x38, 0xe, 0x0, 0x1c,
    0x7, 0x0, 0xe, 0x3, 0x80, 0x7, 0x1, 0xc0,
    0x3, 0x80, 0xe0, 0x1, 0xc0, 0x70, 0x0, 0xe0,
    0x38, 0x0, 0x70, 0x1c, 0x0, 0x38, 0xe, 0x0,
    0x1c, 0x7, 0x0, 0xe, 0x3, 0x80, 0x7, 0x1,
    0xe0, 0x7, 0x80, 0xf0, 0x3, 0xc0, 0x78, 0x1,
    0xe0, 0x3e, 0x1, 0xb0, 0x5f, 0x1, 0xdc, 0x3d,
    0xe1, 0xce, 0x3e, 0x7f, 0xc3, 0xfb, 0xf, 0x80,
    0xf9, 0x80, 0x0, 0x0, 0xc0, 0x0, 0x0, 0x60,
    0x0, 0x0, 0x30, 0x0, 0x0, 0x18, 0x0, 0x0,
    0xc, 0x0, 0x0, 0x6, 0x0, 0x0, 0x3, 0x0,
    0x0, 0x1, 0x80, 0x0, 0x0, 0xc0, 0x0, 0x0,
    0x60, 0x0, 0x0, 0x30, 0x0, 0x0, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 448, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 448, .box_w = 6, .box_h = 17, .ofs_x = 11, .ofs_y = -12},
    {.bitmap_index = 14, .adv_w = 448, .box_w = 15, .box_h = 3, .ofs_x = 7, .ofs_y = 21},
    {.bitmap_index = 20, .adv_w = 448, .box_w = 5, .box_h = 6, .ofs_x = 11, .ofs_y = -1},
    {.bitmap_index = 24, .adv_w = 448, .box_w = 21, .box_h = 45, .ofs_x = 3, .ofs_y = -1},
    {.bitmap_index = 143, .adv_w = 448, .box_w = 11, .box_h = 44, .ofs_x = 5, .ofs_y = 0},
    {.bitmap_index = 204, .adv_w = 448, .box_w = 20, .box_h = 44, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 314, .adv_w = 448, .box_w = 20, .box_h = 45, .ofs_x = 4, .ofs_y = -1},
    {.bitmap_index = 427, .adv_w = 448, .box_w = 23, .box_h = 44, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 554, .adv_w = 448, .box_w = 21, .box_h = 45, .ofs_x = 4, .ofs_y = -1},
    {.bitmap_index = 673, .adv_w = 448, .box_w = 20, .box_h = 45, .ofs_x = 4, .ofs_y = -1},
    {.bitmap_index = 786, .adv_w = 448, .box_w = 19, .box_h = 44, .ofs_x = 5, .ofs_y = 0},
    {.bitmap_index = 891, .adv_w = 448, .box_w = 21, .box_h = 45, .ofs_x = 4, .ofs_y = -1},
    {.bitmap_index = 1010, .adv_w = 448, .box_w = 20, .box_h = 45, .ofs_x = 4, .ofs_y = -1},
    {.bitmap_index = 1123, .adv_w = 448, .box_w = 26, .box_h = 45, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1270, .adv_w = 448, .box_w = 21, .box_h = 44, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 1386, .adv_w = 448, .box_w = 23, .box_h = 44, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 1513, .adv_w = 448, .box_w = 24, .box_h = 44, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1645, .adv_w = 448, .box_w = 22, .box_h = 48, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 1777, .adv_w = 448, .box_w = 23, .box_h = 33, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1872, .adv_w = 448, .box_w = 20, .box_h = 33, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 1955, .adv_w = 448, .box_w = 21, .box_h = 32, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 2039, .adv_w = 460, .box_w = 29, .box_h = 43, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 2195, .adv_w = 460, .box_w = 25, .box_h = 43, .ofs_x = 2, .ofs_y = -12}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0xc, 0xd, 0xe, 0x10, 0x11, 0x12, 0x13,
    0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x21, 0x28,
    0x2d, 0x36, 0x4b, 0x4d, 0x4e, 0x5a, 0x389, 0x39c
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 925, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 24, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t insolata70 = {
#else
lv_font_t insolata70 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 60,          /*The maximum line height required by the font*/
    .base_line = 12,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0)
    .underline_position = -14,
    .underline_thickness = 4,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if INSOLATA70*/
