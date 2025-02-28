//*****************************************************************************
//
// fontcm46.c - Font definition for the 46pt Cm font.
//
// Copyright (c) 2011 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 8028 of the Stellaris Graphics Library.
//
//*****************************************************************************

//*****************************************************************************
//
// This file is generated by ftrasterize; DO NOT EDIT BY HAND!
//
//*****************************************************************************

#include "grlib/grlib.h"

//*****************************************************************************
//
// Details of this font:
//     Characters: 32 to 126 inclusive
//     Style: cm
//     Size: 46 point
//     Bold: no
//     Italic: no
//     Memory usage: 6160 bytes
//
//*****************************************************************************

//*****************************************************************************
//
// The compressed data for the 46 point Cm font.
// Contains characters 32 to 126 inclusive.
//
//*****************************************************************************
static const unsigned char g_pucCm46Data[5959] =
{
      5,  18,   0, 105,  96,  36,   8, 240,  34,  84,  68,  68,
     68,  68,  68,  68,  68,  67,  98,  98,  98,  98,  98,  98,
     98,  98,  98,  98,  98,  98,  98,  98,   0,   5,  98,  84,
     68,  82,   0,  12,  80,  33,  17,   0,   6,  67,  83,  85,
     68,  69,  68,  84,  68, 129, 113, 129, 113, 129, 113, 129,
    113, 113, 113, 129, 113, 113, 113, 129, 113, 113, 113,   0,
     66,  96, 127,  35,   0,  15,   1, 145, 240, 130, 130, 240,
    130, 130, 240, 130, 130, 240, 129, 145, 240, 145, 130, 240,
    130, 130, 240, 130, 130, 240, 129, 145, 240, 145, 130, 240,
    130, 130, 240, 130, 130, 240, 129, 145, 240, 145, 130, 240,
    130, 130, 240, 130, 130, 207,  15,   1, 226, 130, 240, 130,
    130, 240, 130, 130, 240, 129, 145, 240, 130, 130, 240, 130,
    130, 240, 130, 130, 239,  15,   1, 194, 130, 240, 130, 130,
    240, 130, 129, 240, 145, 145, 240, 130, 130, 240, 130, 130,
    240, 130, 129, 240, 145, 145, 240, 130, 130, 240, 130, 130,
    240, 130, 129, 240, 145, 145, 240, 130, 130, 240, 130, 130,
    240, 130, 130, 240, 129, 145,   0,  15,  64,  93,  21, 129,
    240,  81, 240,  54, 210,  33,  50, 161,  65,  66, 130,  65,
     82,  98,  81,  97,  97,  97, 113,  66,  97, 113,  66,  97,
     83,  66,  97,  83,  66,  97,  83,  67,  81, 195,  81, 211,
     65, 213,  33, 231, 249, 232, 248, 225,  36, 225,  52, 209,
     67, 209,  83, 193,  98,  67,  81,  98,  67,  81,  98,  67,
     81,  98,  65, 113,  98,  65, 113,  97,  97,  97,  82,  98,
     81,  66, 130,  65,  65, 162,  49,  34, 214, 240,  49, 240,
     81, 240,  81,   0,  25,  16, 133,  35,  68, 240,  49, 178,
     49, 240,  18, 162,  81, 226, 162, 114, 179, 162, 113,  18,
    116, 177, 129,  55,  34, 162, 145, 162, 178, 145, 146, 194,
    145, 146, 194, 145, 130, 210, 145, 130, 210, 145, 114, 226,
    145,  98, 242, 145,  98, 240,  17, 129,  98, 240,  34, 113,
     82, 240,  65,  97,  98, 240,  66,  81,  82, 240,  98,  49,
     98, 240, 131,  98, 240, 240,  34, 118, 240,  82,  98,  81,
    240,  50, 113, 113, 240,  34,  98, 113, 240,  18, 113, 129,
    242, 114, 145, 226, 114, 145, 210, 130, 145, 195, 130, 145,
    194, 146, 145, 178, 162, 145, 178, 162, 145, 162, 193, 130,
    146, 210, 113, 162, 210, 113, 146, 242,  81, 162, 240,  18,
     49, 177, 240,  52,   0,  40,  48, 103,  33,   0,   9,  36,
    240, 210,  49, 240, 178,  65, 240, 177,  97, 240, 146,  97,
    240, 146,  97, 240, 146,  97, 240, 146,  97, 240, 146,  81,
    240, 162,  81, 240, 162,  65, 240, 179,  33, 240, 210,  33,
    240, 210,  17, 169, 163, 212, 211, 226, 227, 225, 225,  19,
    193, 225,  50, 193, 209,  67, 161, 225,  83, 145, 209,  99,
    129, 209, 131, 113, 209, 147,  81, 210, 147,  65, 226, 163,
     49, 226, 179,  17, 242, 180, 177,  67, 179, 177,  82, 180,
    145,  99, 145,  35, 129, 115,  98,  83,  66, 166, 165,   0,
     50,  64,  19,   9, 240, 211,  85,  69,  84, 129, 129, 129,
    129, 113, 129, 113, 129, 113,   0,  35,  96,  50,  14, 145,
    193, 193, 193, 194, 193, 194, 193, 194, 193, 194, 194, 193,
    194, 194, 194, 194, 178, 194, 194, 194, 194, 194, 194, 194,
    194, 194, 194, 194, 194, 195, 194, 194, 194, 194, 210, 194,
    194, 210, 194, 210, 194, 210, 209, 225, 225, 225,  64,  50,
     14,   1, 225, 225, 225, 210, 210, 194, 210, 194, 210, 194,
    194, 210, 194, 194, 194, 210, 194, 194, 194, 194, 194, 194,
    194, 194, 194, 194, 194, 194, 194, 193, 194, 194, 194, 194,
    193, 194, 193, 194, 194, 193, 193, 194, 193, 193, 193, 193,
    208,  48,  20, 114, 240,  50, 240,  50, 240,  50, 240,  50,
    179,  66,  67,  83,  50,  51, 130,  33,  50, 180,  18, 244,
    240,  20, 243,  19, 178,  49,  34, 131,  50,  51,  83,  66,
     67, 178, 240,  50, 240,  50, 240,  50, 240,  50,   0,  68,
    112,  91,  34,   0,  40,   2, 240, 240,  34, 240, 240,  34,
    240, 240,  34, 240, 240,  34, 240, 240,  34, 240, 240,  34,
    240, 240,  34, 240, 240,  34, 240, 240,  34, 240, 240,  34,
    240, 240,  34, 240, 240,  34, 240, 240,  34, 240,  63,  15,
    240,  50, 240, 240,  34, 240, 240,  34, 240, 240,  34, 240,
    240,  34, 240, 240,  34, 240, 240,  34, 240, 240,  34, 240,
    240,  34, 240, 240,  34, 240, 240,  34, 240, 240,  34, 240,
    240,  34, 240, 240,  34,   0,  40,  64,  20,   8,   0,  31,
      3,  84,  68,  68, 113, 113, 113, 113,  97, 113,  97, 113,
     97, 240, 240,  16,   9,  16,   0,  48,  12,  76,   0,  42,
     64,  11,   8,   0,  31,  18,  84,  68,  82,   0,  12,  80,
     97,  21, 242, 240,  66, 240,  65, 240,  66, 240,  66, 240,
     65, 240,  66, 240,  66, 240,  65, 240,  66, 240,  66, 240,
     65, 240,  66, 240,  66, 240,  65, 240,  66, 240,  66, 240,
     65, 240,  66, 240,  66, 240,  65, 240,  66, 240,  66, 240,
     65, 240,  66, 240,  66, 240,  65, 240,  66, 240,  66, 240,
     65, 240,  66, 240,  66, 240,  65, 240,  66, 240,  66, 240,
     65, 240,  66, 240,  66, 240,  65, 240,  66, 240,  66, 240,
     65, 240,  66, 240,  66, 240,  65, 240,  66, 240,  66, 240,
     64,  67,  20,   0,  10, 101, 210,  66, 177, 129, 146, 130,
    129, 161, 114, 162,  98, 177,  97, 193,  82, 194,  66, 194,
     66, 194,  66, 194,  66, 194,  66, 194,  66, 194,  66, 194,
     66, 194,  66, 194,  66, 194,  66, 194,  66, 194,  66, 194,
     66, 194,  81, 193,  98, 162,  98, 162, 113, 161, 130, 130,
    146, 113, 178,  66, 229,   0,  31,  16,  38,  17,   0,   9,
     49, 242, 212, 148,  34, 242, 242, 242, 242, 242, 242, 242,
    242, 242, 242, 242, 242, 242, 242, 242, 242, 242, 242, 242,
    242, 242, 242, 242, 242, 242, 228, 156,   0,  26,  68,  21,
      0,  11,  22, 225,  83, 177, 131, 129, 147, 113, 179,  97,
    180,  65, 211,  67, 179,  68, 163,  68, 163,  82, 179, 240,
     51, 240,  35, 240,  51, 240,  35, 240,  50, 240,  51, 240,
     50, 240,  50, 240,  50, 240,  50, 240,  50, 240,  50, 240,
     50, 240,  65, 161, 129, 177, 113, 193,  97, 193, 111,  95,
      1,  95,   1,   0,  32,  16,  67,  22,   0,  11, 102, 226,
     83, 162, 131, 145, 163, 115, 147, 116, 132, 100, 132, 114,
    147, 240,  67, 240,  67, 240,  51, 240,  66, 240,  66, 240,
     66, 240,  21, 240, 114, 240,  99, 240,  83, 240,  68, 240,
     67, 240,  68, 240,  52,  67, 180,  68, 164,  68, 164,  67,
    180,  65, 196,  97, 179, 129, 147, 163,  83, 215,   0,  34,
     32,  74,  23,   0,  13,  17, 240,  98, 240,  98, 240,  83,
    240,  83, 240,  65,  18, 240,  50,  18, 240,  49,  34, 240,
     33,  50, 240,  33,  50, 240,  17,  66, 241,  82, 241,  82,
    225,  98, 225,  98, 209, 114, 193, 130, 193, 130, 177, 146,
    177, 146, 161, 162, 145, 178, 159,   4, 240,  18, 240,  98,
    240,  98, 240,  98, 240,  98, 240,  98, 240,  98, 240,  42,
      0,  35,  16,  67,  20,   0,  10,  33, 177, 116,  98, 139,
    154, 161,  22, 193, 240,  65, 240,  65, 240,  65, 240,  65,
    240,  65, 240,  65, 240,  65,  53, 177,  18,  82, 146, 130,
    129, 162, 240,  50, 240,  65, 240,  66, 240,  50, 240,  50,
     67, 178,  67, 178,  67, 178,  67, 178,  65, 194,  81, 194,
     97, 162, 129, 145, 162,  82, 213,   0,  31,  32,  69,  21,
      0,  11,  69, 227,  66, 178, 114, 146, 145, 130, 131, 130,
    131, 114, 147, 114, 240,  51, 240,  50, 240,  66, 240,  51,
     69, 147,  34,  66, 131,  17, 114, 116, 146, 100, 147,  84,
    162,  83, 179,  67, 179,  67, 179,  67, 179,  67, 179,  82,
    179,  82, 179,  82, 179,  98, 162, 114, 147, 130, 130, 146,
    114, 179,  66, 229,   0,  32,  96,  71,  22,   0,   8,  49,
    240,  98, 240,  79,   3,  79,   2,  95,   2,  81, 225,  97,
    209, 113, 209, 240,  81, 240,  81, 240,  97, 240,  81, 240,
     81, 240,  97, 240,  81, 240,  97, 240,  81, 240,  97, 240,
     82, 240,  82, 240,  82, 240,  66, 240,  82, 240,  82, 240,
     82, 240,  67, 240,  67, 240,  67, 240,  67, 240,  67, 240,
     67, 240,  67,   0,  34,  96,  67,  23,   0,  12,  54, 242,
     83, 178, 146, 161, 178, 129, 210, 113, 210,  98, 210,  98,
    210, 114, 194, 115, 162, 133, 129, 165,  97, 198,  34, 231,
    240,  54, 240,  18,  21, 210,  70, 162, 117, 130, 149,  98,
    195,  98, 211,  66, 242,  66, 242,  66, 242,  66, 242,  66,
    242,  82, 225,  99, 193, 131, 162, 163,  98, 231,   0,  35,
     96,  69,  21,   0,  11,  37, 226,  82, 178, 114, 146, 146,
    130, 146, 114, 163,  98, 178,  83, 178,  83, 178,  83, 179,
     67, 179,  67, 179,  67, 179,  67, 179,  82, 179,  82, 164,
     83, 148,  98, 129,  19, 114, 113,  19, 130,  66,  35, 164,
     67, 240,  50, 240,  66, 240,  51, 240,  50, 240,  66, 115,
    130, 131, 114, 147, 113, 177,  98, 214,   0,  33,  17,   8,
      0,  15,  18,  84,  68,  82,   0,  12,  98,  84,  68,  82,
      0,  12,  80,  26,   8,   0,  15,  18,  84,  68,  82,   0,
     12,  98,  84,  68,  83, 113, 113, 113, 113,  97, 113,  97,
    113,  97, 240, 240,  16,  37,   8,   0,  12,  18,  84,  68,
     82, 240, 240, 145,  98,  98,  98,  98,  98,  98,  98,  98,
     98,  98,  98,  98,  98,  98,  99,  68,  68,  68,  68,  68,
     68,  68,  68,  82, 240,  96,  13,  33,   0,  78,  63,  14,
      0,  29,  63,  14,   0,  78, 112,  61,  19,   0,  29,  35,
    244, 244, 240,  19,   0,  11,  81, 240,  49, 240,  49, 240,
     49, 240,  49, 240,  49, 240,  49, 240,  33, 240,  49, 240,
     33, 240,  49, 240,  33, 240,  33, 240,  34, 240,  18, 240,
     33, 240,  34, 240,  34, 163,  66, 163,  66, 163,  66, 193,
     82, 161, 115,  98, 167,   0,   8,  16,  63,  19, 240, 240,
    214, 178,  83, 129, 146,  97, 177,  81, 194,  67, 162,  67,
    162,  67, 162, 240,  34, 240,  18, 240,  34, 240,  18, 240,
     18, 240,  18, 240,  33, 240,  34, 240,  33, 240,  49, 240,
     34, 240,  33, 240,  49, 240,  49, 240,  49, 240,  49, 240,
     49,   0,  11,  83, 244, 244, 240,  19,   0,  29,  96, 121,
     33,   0,   9,  87, 240, 146, 115, 240,  66, 178, 240,  33,
    241, 241, 240,  33, 209, 240,  65, 177, 240,  97, 161, 133,
    130, 129, 114,  82, 113, 129,  98, 129,  98,  97,  98, 145,
    113,  97,  82, 179,  65,  97,  82, 194,  66,  65,  98, 194,
     81,  65,  82, 210,  81,  65,  82, 210,  81,  65,  82, 210,
     81,  65,  82, 210,  81,  65,  82, 210,  81,  65,  82, 210,
     81,  65,  98, 194,  81,  81,  82, 194,  81,  81,  82, 179,
     81,  81,  98, 163,  65, 113,  98, 129,  18,  65, 113, 114,
     82,  50,  34, 129, 133, 100, 161, 240, 240,  34, 240, 240,
     34, 240,  82, 178, 243, 227, 148, 240,  89,   0,  51,  16,
    100,  34,   0,  10,  34, 240, 240,  34, 240, 240,  34, 240,
    240,  20, 240, 244, 240, 244, 240, 225,  20, 240, 209,  35,
    240, 193,  51, 240, 193,  67, 240, 177,  67, 240, 161,  83,
    240, 161,  99, 240, 145,  99, 240, 129, 115, 240, 129, 131,
    240, 113, 131, 240,  97, 148, 240,  81, 163, 240,  81, 163,
    240,  65, 180, 240,  63,   1, 240,  49, 195, 240,  33, 212,
    240,  17, 227, 240,  17, 227, 241, 244, 225, 240,  19, 225,
    240,  19, 209, 240,  36, 193, 240,  51, 164, 240,  36, 120,
    202,   0,  51,  64,  84,  30,   0,  11,  47,   4, 242, 180,
    210, 211, 194, 227, 178, 243, 162, 244, 146, 240,  19, 146,
    240,  19, 146, 240,  19, 146, 240,  19, 146, 243, 162, 243,
    162, 227, 178, 210, 210, 163, 255,   1, 226, 196, 194, 227,
    178, 243, 162, 240,  19, 146, 240,  19, 146, 240,  35, 130,
    240,  35, 130, 240,  35, 130, 240,  35, 130, 240,  35, 130,
    240,  19, 146, 240,  19, 146, 243, 162, 227, 178, 196, 143,
      5,   0,  46,  32,  89,  31,   0,  13,  23, 113, 212,  98,
     81, 180, 162,  34, 163, 210,  18, 147, 244, 131, 240,  35,
    115, 240,  51, 100, 240,  66,  99, 240,  82,  84, 240,  82,
     84, 240,  97,  83, 240, 113,  68, 240, 196, 240, 196, 240,
    196, 240, 196, 240, 196, 240, 196, 240, 196, 240, 212, 240,
     97,  84, 240,  97,  84, 240,  97,  99, 240,  97, 100, 240,
     65, 131, 240,  65, 147, 240,  33, 179, 240,  17, 195, 225,
    228, 162, 240,  36,  98, 240, 119,   0,  48,  96,  32,   0,
     12,  15,   3, 240,  50, 179, 240,  18, 211, 226, 227, 210,
    243, 194, 240,  19, 178, 240,  34, 178, 240,  35, 162, 240,
     50, 162, 240,  51, 146, 240,  51, 146, 240,  66, 146, 240,
     67, 130, 240,  67, 130, 240,  67, 130, 240,  67, 130, 240,
     67, 130, 240,  67, 130, 240,  67, 130, 240,  67, 130, 240,
     67, 130, 240,  66, 146, 240,  51, 146, 240,  51, 146, 240,
     50, 162, 240,  35, 162, 240,  19, 178, 240,  18, 194, 242,
    210, 211, 226, 179, 207,   3,   0,  49,  96,  97,  30,   0,
     11,  47,   9, 162, 213, 162, 243, 162, 240,  18, 162, 240,
     33, 162, 240,  33, 162, 240,  33, 162, 240,  33, 162, 240,
     49, 146, 161, 113, 146, 161, 240,  34, 161, 240,  34, 161,
    240,  34, 146, 240,  34, 131, 240,  45, 240,  34, 131, 240,
     34, 146, 240,  34, 161, 240,  34, 161, 240,  34, 161, 129,
    130, 161, 129, 130, 240,  65, 130, 240,  49, 146, 240,  49,
    146, 240,  49, 146, 240,  49, 146, 240,  34, 146, 240,  34,
    146, 240,  19, 146, 229,  95,   9,   0,  45,  96,  76,  28,
      0,  10,  79,   8, 146, 212, 146, 243, 130, 240,  18, 130,
    240,  18, 130, 240,  33, 130, 240,  33, 130, 240,  33, 130,
    240,  33, 130, 240,  33, 130, 161, 242, 161, 242, 161, 242,
    161, 242, 146, 242, 131, 253, 242, 131, 242, 146, 242, 161,
    242, 161, 242, 161, 242, 240, 178, 240, 178, 240, 178, 240,
    178, 240, 178, 240, 178, 240, 178, 240, 179, 240, 107,   0,
     44,  16,  94,  33,   0,  13, 119,  97, 240,  20,  98,  65,
    228, 146,  49, 211, 209,  18, 195, 228, 179, 240,  19, 163,
    240,  50, 163, 240,  50, 147, 240,  66, 147, 240,  81, 132,
    240,  81, 132, 240,  81, 131, 240,  97, 116, 240, 228, 240,
    228, 240, 228, 240, 228, 240, 228, 240, 228, 240, 243, 250,
     84, 240,  66, 132, 240,  66, 147, 240,  66, 148, 240,  50,
    163, 240,  50, 179, 240,  34, 195, 240,  18, 211, 227, 228,
    178,  17, 240,  20, 114,  49, 240,  72,  81,   0,  50,  48,
     70,  31,   0,  11,  90, 122, 130, 242, 194, 242, 194, 242,
    194, 242, 194, 242, 194, 242, 194, 242, 194, 242, 194, 242,
    194, 242, 194, 242, 194, 242, 194, 242, 194, 242, 207,   4,
    194, 242, 194, 242, 194, 242, 194, 242, 194, 242, 194, 242,
    194, 242, 194, 242, 194, 242, 194, 242, 194, 242, 194, 242,
    194, 242, 194, 242, 194, 242, 138, 122,   0,  47,  39,  14,
    240, 240, 202, 130, 194, 194, 194, 194, 194, 194, 194, 194,
    194, 194, 194, 194, 194, 194, 194, 194, 194, 194, 194, 194,
    194, 194, 194, 194, 194, 194, 194, 194, 194, 138,   0,  21,
     64,  67,  21,   0,   8, 106, 243, 240,  66, 240,  66, 240,
     66, 240,  66, 240,  66, 240,  66, 240,  66, 240,  66, 240,
     66, 240,  66, 240,  66, 240,  66, 240,  66, 240,  66, 240,
     66, 240,  66, 240,  66, 240,  66, 240,  66, 240,  66, 240,
     66, 240,  66, 240,  66, 130, 146, 116, 130, 116, 129, 131,
    130, 145, 130, 178,  82, 229,   0,  33,  94,  32,   0,  12,
     10, 168, 130, 244, 178, 240,  18, 194, 241, 226, 225, 242,
    209, 240,  18, 193, 240,  34, 177, 240,  50, 162, 240,  50,
    146, 240,  66, 130, 240,  82, 114, 240,  98,  98, 240, 114,
     99, 240,  98,  84, 240,  98,  65,  35, 240,  82,  49,  52,
    240,  66,  33,  83, 240,  66,  17, 100, 240,  51, 132, 240,
     34, 163, 240,  34, 164, 240,  18, 179, 240,  18, 195, 242,
    196, 226, 211, 226, 227, 210, 228, 194, 243, 194, 244, 178,
    245, 106, 153,   0,  48,  64,  71,  26,   0,   9, 107, 240,
     66, 240, 146, 240, 146, 240, 146, 240, 146, 240, 146, 240,
    146, 240, 146, 240, 146, 240, 146, 240, 146, 240, 146, 240,
    146, 240, 146, 240, 146, 240, 146, 240, 146, 240, 146, 240,
    146, 240, 146, 241, 130, 241, 130, 241, 130, 241, 130, 225,
    146, 225, 146, 225, 146, 210, 146, 210, 146, 195, 146, 165,
     95,   6,   0,  39,  80, 132,  38,   0,  14,  39, 240,  87,
    132, 240,  67, 193,  18, 240,  49,  18, 193,  19, 240,  33,
     18, 193,  19, 240,  33,  18, 193,  34, 240,  17,  34, 193,
     35, 241,  34, 193,  35, 241,  34, 193,  50, 225,  50, 193,
     51, 209,  50, 193,  51, 209,  50, 193,  66, 193,  66, 193,
     67, 177,  66, 193,  67, 161,  82, 193,  83, 145,  82, 193,
     83, 145,  82, 193,  98, 129,  98, 193,  99, 113,  98, 193,
     99, 113,  98, 193, 114,  97, 114, 193, 115,  81, 114, 193,
    115,  81, 114, 193, 130,  65, 130, 193, 131,  49, 130, 193,
    131,  49, 130, 193, 146,  33, 146, 193, 147,  17, 146, 193,
    148, 162, 193, 163, 162, 193, 163, 162, 179, 161, 178, 137,
    113, 122,   0,  57,  64,  96,  32,   0,  12,   7, 201, 132,
    227, 181, 225, 193,  19, 225, 193,  35, 209, 193,  35, 209,
    193,  51, 193, 193,  52, 177, 193,  67, 177, 193,  83, 161,
    193,  83, 161, 193,  99, 145, 193, 100, 129, 193, 115, 129,
    193, 131, 113, 193, 131, 113, 193, 147,  97, 193, 148,  81,
    193, 163,  81, 193, 179,  65, 193, 179,  65, 193, 195,  49,
    193, 196,  33, 193, 211,  33, 193, 227,  17, 193, 227,  17,
    193, 244, 193, 244, 193, 240,  19, 193, 240,  34, 179, 240,
     18, 137, 225,   0,  49,  95,  33,   0,  13, 103, 240, 147,
     84, 240,  67, 147, 240,  34, 211, 211, 243, 195, 243, 179,
    240,  35, 148, 240,  36, 131, 240,  67, 116, 240,  68, 100,
    240,  68,  99, 240,  99,  84, 240, 100,  68, 240, 100,  68,
    240, 100,  68, 240, 100,  68, 240, 100,  68, 240, 100,  68,
    240, 100,  68, 240, 100,  84, 240,  83, 100, 240,  68, 100,
    240,  68, 115, 240,  67, 132, 240,  36, 147, 240,  35, 179,
    243, 196, 212, 212, 180, 240,  19, 147, 240,  68,  84, 240,
    135,   0,  51,  48,  69,  29,   0,  10, 127,   3, 242, 164,
    210, 196, 178, 212, 162, 228, 146, 243, 146, 244, 130, 244,
    130, 244, 130, 244, 130, 244, 130, 243, 146, 228, 146, 227,
    162, 211, 178, 164, 222, 242, 240, 194, 240, 194, 240, 194,
    240, 194, 240, 194, 240, 194, 240, 194, 240, 194, 240, 194,
    240, 194, 240, 194, 240, 194, 240, 194, 240, 138,   0,  45,
    112, 129,  33,   0,  13, 103, 240, 147,  99, 240,  66, 178,
    240,  34, 211, 226, 243, 194, 240,  34, 178, 240,  66, 147,
    240,  67, 130, 240,  98, 115, 240,  99,  99, 240,  99,  99,
    240, 114,  83, 240, 131,  67, 240, 131,  67, 240, 131,  67,
    240, 131,  67, 240, 131,  67, 240, 131,  67, 240, 131,  67,
    240, 131,  82, 240, 130,  99, 240,  99,  99, 240,  99, 114,
    240,  98, 131, 240,  67, 146, 132, 114, 178,  97,  65,  83,
    179,  65,  97,  51, 211,  49,  97,  50, 240,  18,  33, 116,
    240,  51,  17,  83, 240, 135,  17, 240, 240,  34, 113, 240,
    130, 113, 240, 145, 113, 240, 146,  82, 240, 147,  51, 240,
    152, 240, 183, 240, 182, 240, 212,   0,  13,  32,  82,  33,
      0,  12,  63,   2, 240,  82, 164, 240,  34, 195, 240,  18,
    212, 226, 228, 210, 228, 210, 244, 194, 244, 194, 244, 194,
    244, 194, 244, 194, 243, 210, 227, 226, 211, 242, 195, 240,
     18, 163, 240,  60, 240,  98, 148, 240,  50, 179, 240,  34,
    195, 240,  18, 211, 242, 211, 242, 211, 242, 211, 242, 212,
    226, 212, 226, 212, 226, 212, 226, 227,  81, 130, 228,  65,
    130, 243,  49,  90, 213,   0,  50,  16,  76,  23,   0,   9,
     54,  81, 146,  83,  49, 114, 146,  18, 113, 180,  98, 195,
     82, 226,  82, 226,  82, 241,  82, 241,  83, 225,  83, 225,
     99, 240,  84, 240,  86, 240,  57, 251, 240,  24, 240,  54,
    240,  84, 240,  83, 240,  99,  65, 243,  65, 240,  18,  65,
    240,  18,  65, 240,  18,  66, 242,  66, 241,  83, 210,  84,
    193,  98,  18, 162,  97,  51,  98, 129, 102,   0,  35,  96,
     79,  31,   0,  11, 111,  11,  84, 130, 132,  82, 162, 162,
     81, 178, 162,  81, 178, 177,  81, 178, 177,  81, 178, 177,
     81, 178, 177,  65, 194, 177,  65, 194, 240, 226, 240, 226,
    240, 226, 240, 226, 240, 226, 240, 226, 240, 226, 240, 226,
    240, 226, 240, 226, 240, 226, 240, 226, 240, 226, 240, 226,
    240, 226, 240, 226, 240, 226, 240, 226, 240, 226, 240, 226,
    240, 227, 240, 140,   0,  47, 112,  98,  32,   0,  12,  10,
    153, 130, 240,  19, 178, 240,  33, 194, 240,  33, 194, 240,
     33, 194, 240,  33, 194, 240,  33, 194, 240,  33, 194, 240,
     33, 194, 240,  33, 194, 240,  33, 194, 240,  33, 194, 240,
     33, 194, 240,  33, 194, 240,  33, 194, 240,  33, 194, 240,
     33, 194, 240,  33, 194, 240,  33, 194, 240,  33, 194, 240,
     33, 194, 240,  33, 194, 240,  33, 194, 240,  33, 209, 240,
     17, 226, 241, 226, 241, 241, 225, 240,  33, 193, 240,  65,
    161, 240,  98,  98, 240, 150,   0,  49, 112, 101,  35,   0,
     13,  26, 231, 116, 240,  52, 163, 240,  66, 179, 240,  65,
    196, 240,  33, 227, 240,  33, 227, 240,  33, 243, 241, 240,
     19, 241, 240,  20, 225, 240,  35, 209, 240,  51, 209, 240,
     52, 177, 240,  83, 177, 240,  84, 161, 240,  99, 145, 240,
    115, 145, 240, 116, 129, 240, 131, 113, 240, 147, 113, 240,
    163,  81, 240, 179,  81, 240, 180,  65, 240, 195,  49, 240,
    211,  49, 240, 212,  17, 240, 243,  17, 240, 243,  17, 240,
    240,  19, 240, 240,  35, 240, 240,  35, 240, 240,  49,   0,
     54, 112, 137,  47,   0,  17,  89, 137, 167, 116, 212, 212,
    148, 211, 242, 179, 227, 225, 195, 227, 225, 195, 227, 225,
    211, 193,  19, 193, 227, 193,  19, 193, 227, 193,  19, 193,
    243, 161,  36, 161, 240,  19, 161,  51, 161, 240,  19, 161,
     51, 161, 240,  20, 145,  52, 145, 240,  35, 129,  83, 129,
    240,  51, 129,  83, 129, 240,  52, 113,  84, 113, 240,  67,
     97, 115,  97, 240,  83,  97, 115,  97, 240,  84,  81, 115,
     97, 240,  99,  65, 147,  65, 240, 115,  65, 147,  65, 240,
    115,  65, 147,  65, 240, 131,  33, 179,  33, 240, 147,  33,
    179,  33, 240, 147,  33, 179,  33, 240, 164, 197, 240, 180,
    212, 240, 180, 212, 240, 180, 211, 240, 210, 242, 240, 210,
    242, 240, 210, 242,   0,  72,  64,  97,  35,   0,  13,  42,
    168, 165, 212, 228, 211, 240,  19, 210, 240,  36, 193, 240,
     68, 161, 240,  99, 146, 240, 100, 129, 240, 132,  97, 240,
    163,  82, 240, 164,  65, 240, 196,  33, 240, 227,  18, 240,
    229, 240, 240,  20, 240, 240,  35, 240, 240,  36, 240, 241,
     20, 240, 210,  35, 240, 209,  52, 240, 177,  84, 240, 145,
    115, 240, 145, 116, 240, 113, 148, 240,  81, 179, 240,  81,
    180, 240,  49, 212, 240,  17, 243, 240,  17, 244, 226, 240,
     20, 180, 240,  21, 121, 202,   0,  53,  99,  34,   0,  12,
    105, 231, 116, 240,  36, 163, 240,  49, 196, 240,  18, 211,
    240,  17, 243, 225, 240,  20, 209, 240,  35, 193, 240,  52,
    161, 240,  83, 161, 240,  99, 129, 240, 115, 129, 240, 131,
     97, 240, 147,  81, 240, 179,  65, 240, 195,  33, 240, 211,
     33, 240, 228, 240, 240,  18, 240, 240,  34, 240, 240,  34,
    240, 240,  34, 240, 240,  34, 240, 240,  34, 240, 240,  34,
    240, 240,  34, 240, 240,  34, 240, 240,  34, 240, 240,  34,
    240, 240,  34, 240, 240,  34, 240, 218,   0,  52,  96,  72,
     26,   0,   9, 127,   6,  86, 180,  83, 212,  98, 227, 114,
    212, 113, 227, 129, 211, 145, 196, 145, 195, 161, 180, 240,
    115, 240, 115, 240, 116, 240, 115, 240, 116, 240, 115, 240,
    115, 240, 116, 240, 115, 240, 116, 240, 100, 177, 163, 193,
    148, 193, 147, 209, 131, 225, 116, 225, 115, 226, 100, 226,
     99, 227,  83, 228,  68, 198,  79,   7,   0,  39,  64,  49,
     10, 166,  66, 130, 130, 130, 130, 130, 130, 130, 130, 130,
    130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130,
    130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130,
    130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 134,  64,
     33,  17,   0,   6,  97, 113, 113, 113, 129, 113, 113, 113,
    129, 113, 113, 113, 129, 113, 129, 113, 129, 113, 132,  68,
     84,  69,  68,  84,  83,  83,   0,  66,  64,  49,  10, 166,
    130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130,
    130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130,
    130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130,
    130, 130, 130, 130, 130, 130, 130, 130,  70,  64,  19,  15,
      0,   6,  33, 211, 178,  18, 161,  50, 129,  82,  97, 114,
     65, 145,   0,  69, 112,  11,   8, 240, 240,  50,  84,  68,
     82,   0,  39,  80,  19,   9, 240, 241, 113, 129, 113, 129,
    113, 129, 129, 129, 132,  85,  84,  83,   0,  35,  64,  54,
     23,   0,  43,  86, 240,  17,  83, 209, 130, 177, 147, 162,
    146, 163, 130, 162, 146, 240,  98, 240,  23, 211,  82, 194,
    114, 163, 130, 162, 146, 146, 162,  65,  66, 162,  65,  66,
    162,  65,  66, 147,  65,  82, 113,  18,  65,  83,  81,  53,
    134,  83,   0,  35,  32,  75,  23,   0,   8, 100, 240,  53,
    240,  98, 240,  98, 240,  98, 240,  98, 240,  98, 240,  98,
    240,  98, 240,  98, 240,  98, 240,  98, 240,  98,  69, 194,
     33,  82, 178,  17, 129, 163, 146, 146, 178, 130, 178, 130,
    194, 114, 194, 114, 194, 114, 194, 114, 194, 114, 194, 114,
    194, 114, 194, 114, 178, 130, 178, 131, 146, 145,  33, 114,
    161,  49,  82, 177,  69,   0,  35,  96,  45,  19,   0,  36,
     53, 194,  82, 146, 129, 114, 161,  82, 147,  82, 147,  81,
    163,  66, 240,  34, 240,  34, 240,  34, 240,  34, 240,  34,
    240,  50, 240,  34, 177,  82, 177,  98, 145, 130, 129, 147,
     66, 197,   0,  29,  64,  74,  24,   0,  10, 114, 240,  69,
    240,  99, 240, 114, 240, 114, 240, 114, 240, 114, 240, 114,
    240, 114, 240, 114, 240, 114, 240, 114, 214,  50, 179,  81,
     34, 162, 129,  18, 146, 163, 145, 194, 130, 194, 129, 210,
    114, 210, 114, 210, 114, 210, 114, 210, 114, 210, 114, 210,
    129, 210, 130, 194, 130, 194, 146, 163, 162, 129,  18, 178,
     82,  37, 165,  69,   0,  36,  64,  45,  20,   0,  38,  37,
    210,  82, 161, 130, 129, 162,  98, 162,  97, 194,  81, 194,
     66, 194,  79,   1,  66, 240,  50, 240,  50, 240,  50, 240,
     65, 240,  66, 193,  97, 193,  98, 161, 130, 129, 163,  66,
    213,   0,  31,  16,  44,  17, 240, 240, 196, 178,  50, 146,
     51, 146,  51, 130,  67, 130, 242, 242, 242, 242, 242, 242,
    242, 187, 162, 242, 242, 242, 242, 242, 242, 242, 242, 242,
    242, 242, 242, 242, 242, 242, 242, 242, 201,   0,  26,  48,
     68,  24,   0,  42, 100,  84, 147,  35,  18,  35, 114,  98,
     66, 130,  98, 210, 130, 194, 130, 194, 130, 194, 130, 194,
    130, 194, 130, 210,  98, 226,  98, 227,  66, 225,  52, 240,
     17, 240, 129, 240, 129, 240, 130, 240, 139, 237, 146, 164,
    114, 210,  98, 242,  82, 242,  82, 242,  82, 242,  98, 210,
    130, 178, 163, 115, 216,   0,  10,  48,  73,  24,   0,   9,
     20, 240,  69, 240, 114, 240, 114, 240, 114, 240, 114, 240,
    114, 240, 114, 240, 114, 240, 114, 240, 114, 240, 114, 240,
    114,  69, 210,  34,  67, 178,  17, 114, 179, 146, 163, 146,
    162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
    162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
    162, 162, 162, 162, 120,  72,   0,  36,  64,  33,  12,   0,
      6,  35, 147, 147, 146,   0,  11,  84, 117, 162, 162, 162,
    162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
    162, 162, 120,   0,  18,  64,  46,  14,   0,   7, 114, 180,
    164, 178,   0,  13,  84, 134, 179, 194, 194, 194, 194, 194,
    194, 194, 194, 194, 194, 194, 194, 194, 194, 194, 194, 194,
    194, 194, 194, 194,  81,  98,  67,  81,  83,  66,  97,  66,
    132,   0,   6,  32,  76,  24,   0,   9,  20, 240,  69, 240,
    114, 240, 114, 240, 114, 240, 114, 240, 114, 240, 114, 240,
    114, 240, 114, 240, 114, 240, 114, 240, 114, 119, 130, 132,
    162, 130, 194, 114, 210, 113, 226,  82, 242,  66, 240,  18,
     50, 240,  34,  36, 240,  18,  17,  34, 240,  19,  51, 242,
     83, 226,  98, 226,  99, 210, 115, 194, 115, 194, 131, 178,
    147, 162, 148, 104,  72,   0,  36,  64,  39,  12, 240, 240,
    116, 117, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
    162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
    162, 162, 162, 162, 162, 162, 162, 120,   0,  18,  64,  71,
     36,   0,  67,  84,  69, 117, 165,  34,  67,  50,  67, 178,
     17, 114,  33, 114, 179, 147, 146, 163, 147, 146, 162, 162,
    162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
    162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
    162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
    162, 162, 162, 162, 120,  72,  72,   0,  54,  64,  49,  24,
      0,  45,  20,  69, 165,  34,  67, 178,  17, 114, 179, 146,
    163, 146, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
    162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
    162, 162, 162, 162, 162, 162, 120,  72,   0,  36,  64,  45,
     22,   0,  42,   6, 226,  98, 178, 130, 146, 162, 129, 194,
     98, 194,  97, 225,  82, 226,  66, 226,  66, 226,  66, 226,
     66, 226,  66, 226,  81, 225,  98, 194,  98, 194, 114, 162,
    146, 130, 178,  98, 230,   0,  34,  32,  68,  23,   0,  43,
     36,  54, 149,  34,  66, 178,  17, 114, 163, 146, 146, 178,
    130, 178, 130, 194, 114, 194, 114, 194, 114, 194, 114, 194,
    114, 194, 114, 194, 114, 179, 114, 178, 130, 178, 131, 146,
    146,  17, 114, 162,  33,  82, 178,  53, 210, 240,  98, 240,
     98, 240,  98, 240,  98, 240,  98, 240,  98, 240,  98, 240,
     56,   0,  10,  64,  69,  24,   0,  45, 101,  81, 179,  66,
     49, 162, 129,  33, 146, 163, 130, 179, 130, 194, 130, 194,
    114, 210, 114, 210, 114, 210, 114, 210, 114, 210, 114, 210,
    130, 194, 130, 194, 130, 179, 146, 163, 162, 129,  18, 179,
     66,  34, 213,  66, 240, 114, 240, 114, 240, 114, 240, 114,
    240, 114, 240, 114, 240, 114, 240, 114, 240,  72,   0,   9,
     64,  33,  17,   0,  32,   4,  52,  85,  33,  35, 114,  17,
     51, 115,  67, 115, 227, 226, 242, 242, 242, 242, 242, 242,
    242, 242, 242, 242, 242, 242, 201,   0,  26,  64,  44,  19,
      0,  36,  22,  49, 114, 100,  98, 146,  82, 162,  82, 177,
     82, 177,  83, 161, 100, 249, 201, 231, 240,  19,  81, 179,
     65, 194,  66, 178,  66, 178,  67, 162,  68, 130,  82,  18,
     98,  97,  70,   0,  29,  64,  48,  17,   0,  13,  49, 240,
     17, 240,  17, 240,  17, 240,  17, 242, 242, 227, 212, 188,
    146, 242, 242, 242, 242, 242, 242, 242, 242, 242, 242,  97,
    130,  97, 130,  97, 130,  97, 130,  97, 130,  97, 146,  65,
    163,  34, 196,   0,  26,  32,  49,  24,   0,  45,  20, 132,
    117, 117, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
    162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
    162, 162, 162, 162, 162, 147, 162, 147, 178, 113,  18, 179,
     81,  37, 165,  69,   0,  36,  64,  54,  25,   0,  46, 120,
    118, 115, 163, 147, 177, 178, 161, 195, 145, 195, 145, 210,
    129, 227, 113, 242,  97, 240,  19,  81, 240,  19,  81, 240,
     34,  65, 240,  51,  49, 240,  66,  33, 240,  82,  33, 240,
     83,  17, 240,  99, 240, 115, 240, 115, 240, 129,   0,  39,
     32,  80,  33,   0,  61, 119,  55,  87,  99, 130, 147, 131,
    130, 146, 162, 145, 145, 179, 130, 129, 179, 115, 129, 194,
    115, 113, 211,  81,  34,  97, 226,  81,  34,  97, 226,  81,
     34,  81, 243,  49,  66,  65, 240,  18,  49,  66,  65, 240,
     18,  49,  67,  33, 240,  35,  17,  98,  33, 240,  50,  17,
     98,  33, 240,  50,  17, 100, 240,  67, 131, 240,  82, 131,
    240,  82, 130, 240,  97, 161,   0,  51,  16,  56,  26,   0,
     48, 104, 103, 132, 132, 179, 130, 227, 113, 240,  18,  97,
    240,  35,  81, 240,  51,  49, 240,  83,  17, 240, 115, 240,
    131, 240, 147, 240, 113,  19, 240,  81,  50, 240,  65,  67,
    240,  49,  83, 240,  17, 115, 225, 146, 210, 147, 179, 148,
    119, 120,   0,  39,  64,  75,  24,   0,  45,   7, 118,  99,
    163, 146, 177, 162, 177, 178, 145, 194, 145, 194, 129, 226,
    113, 226, 113, 227,  81, 240,  18,  81, 240,  18,  65, 240,
     50,  49, 240,  50,  33, 240,  67,  17, 240,  82,  17, 240,
     83, 240,  99, 240, 113, 240, 129, 240, 129, 240, 113, 240,
    129, 240, 113, 240,  18,  81, 240,  18,  81, 240,  18,  65,
    240,  49,  49, 240,  68,   0,  11,  48,  47,  20,   0,  37,
     95,  83, 146,  98, 147,  97, 147, 113, 146, 129, 131, 240,
     19, 240,  19, 240,  34, 240,  35, 240,  19, 240,  34, 240,
     35, 113, 131, 129, 115, 145, 114, 161,  99, 146,  83, 162,
     67, 148,  79,   1,   0,  30,  64,   9,  26,   0,  71,  79,
      7,   0,  78,  64,  13,  47,   0, 127, 240,  63,  15,  13,
      0, 127,   0,  14,  64,  23,  17,   0,   6,  98,  98,  99,
     83,  98,  98, 114,  98,  98,  98, 113, 113, 114,  98, 113,
    113,   0,  77,  64,  15,  18,   0,   9,  51, 113, 102,  66,
     81,  86,  81, 115,   0,  88,  80,
};

//*****************************************************************************
//
// The font definition for the 46 point Cm font.
//
//*****************************************************************************
const tFont g_sFontCm46 =
{
    //
    // The format of the font.
    //
    FONT_FMT_PIXEL_RLE,

    //
    // The maximum width of the font.
    //
    42,

    //
    // The height of the font.
    //
    47,

    //
    // The baseline of the font.
    //
    35,

    //
    // The offset to each character in the font.
    //
    {
           0,    5,   41,   74,  201,  294,  427,  530,
         549,  599,  649,  697,  788,  808,  817,  828,
         925,  992, 1030, 1098, 1165, 1239, 1306, 1375,
        1446, 1513, 1582, 1599, 1625, 1662, 1675, 1736,
        1799, 1920, 2020, 2104, 2193, 2289, 2386, 2462,
        2556, 2626, 2665, 2732, 2826, 2897, 3029, 3125,
        3220, 3289, 3418, 3500, 3576, 3655, 3753, 3854,
        3991, 4088, 4187, 4259, 4308, 4341, 4390, 4409,
        4420, 4439, 4493, 4568, 4613, 4687, 4732, 4776,
        4844, 4917, 4950, 4996, 5072, 5111, 5182, 5231,
        5276, 5344, 5413, 5446, 5490, 5538, 5587, 5641,
        5721, 5777, 5852, 5899, 5908, 5921, 5944,
    },

    //
    // A pointer to the actual font data
    //
    g_pucCm46Data
};
