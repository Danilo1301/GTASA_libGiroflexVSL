#pragma once

#define __AML

#include "mod/amlmod.h"
#include "mod/logger.h"
#include "mod/config.h"

/*
#include <stdlib.h>
#include <sys/stat.h>
#include <fstream>
#include <stdint.h>
#include <dlfcn.h>
*/

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <cmath>
#include <math.h>

//#define __print_to_log(__str)	cleo->PrintToCleoLog(__str); logger->Info(__str)
//#define __reg_opcode	cleo->RegisterOpcode
//#define __reg_func		cleo->RegisterOpcodeFunction

struct posStruct
{
    float x;
    float y;
    float z;
};

class CVector {
public:
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    CVector(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    CVector operator+(const CVector right)
    {
        CVector result(this->x, this->y, this->z);
        result.x += right.x;
        result.y += right.y;
        result.z += right.z;
        return result;
    }
};

class CRGBA {
public:
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
    unsigned char a = 0;

    CRGBA(unsigned char r, unsigned char g, unsigned char b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = 255;
    }

    CRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }
};

class CRect {
public:
    float left = 0;          // x1
    float bottom = 0;        // y1
    float right = 0;         // x2
    float top = 0;           // y2
};

class CVector2D {
public:
    float x = 0.0f;
    float y = 0.0f;

    CVector2D(float x, float y)
    {
        this->x = x;
        this->y = y;
    }
};

enum eDrawInfoType {
    AMOUNT_OF_DRAWITEMS = 0,
    TYPE,
    UPDATE_TOUCH_STATE,
    TOUCH_X,
    TOUCH_Y,
    POS_X,
    POS_Y,
    SIZE_X,
    SIZE_Y,
    R,
    G,
    B,
    A,
    GXT_ID,
    NUM_1,
    NUM_2,

    AMOUNT_OF_CORONAS,
    CORONA_CAR,
    CORONA_OFFSET_X,
    CORONA_OFFSET_Y,
    CORONA_OFFSET_Z,

    ALIGN
};