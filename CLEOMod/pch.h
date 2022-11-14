#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <cmath>
#include <math.h>

#include <dirent.h>

#include <sys/stat.h> // stat
#include <errno.h>    // errno, ENOENT, EEXIST

enum eGameIdent
{
    GTA3,
    GTAVC,
    GTASA,
    GTALCS,
    GTAVCS
};

enum eGameVerInternal
{
    VER_NONE,
    VER_GTA3_1_4,
    VER_GTAVC_1_03,
    VER_GTASA_1_00,
    VER_GTASA_1_01,
    VER_GTASA_1_02,
    VER_GTASA_1_03,
    VER_GTASA_1_05,
    VER_GTASA_1_06,
    VER_GTASA_1_05_GER,
    VER_GTASA_1_07,
    VER_GTA3_1_6,
    VER_GTAVC_1_06,
    VER_GTASA_1_08,
    VER_GTALCS_2_2,
    VER_GTA3_1_8_OR_HIGHER,
    VER_GTAVC_1_09_OR_HIGHER,
    VER_GTASA_2_00_OR_HIGHER,
    VER_GTALCS_2_4_OR_HIGHER,
    VER_GTALCS_PSP_1_05_OR_HIGHER,
    VER_GTAVCS_PSP_1_02_OR_HIGHER
};


/*
#define __AML

#include "mod/amlmod.h"
#include "mod/logger.h"
#include "mod/config.h"


#include <stdlib.h>
#include <sys/stat.h>
#include <fstream>
#include <stdint.h>
#include <dlfcn.h>


*/

//#define __print_to_log(__str)	cleo->PrintToCleoLog(__str); logger->Info(__str)
//#define __reg_opcode	cleo->RegisterOpcode
//#define __reg_func		cleo->RegisterOpcodeFunction

/*
static std::string to_upper(std::string data) {
    std::for_each(data.begin(), data.end(), [](char& c) {
        c = ::toupper(c);
        });
    return data;
}
*/



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