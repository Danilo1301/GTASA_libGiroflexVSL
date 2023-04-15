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

// CLEO 2.0.1.2
#include "icleo.h"
static ICLEO* cleo2012 = NULL;

// CLEO 2.0.1.3
#include "cleo.h"
static cleo_ifs_t* cleo2013 = NULL;

// SAUtils
#include "isautils.h"
static ISAUtils* sautils = NULL;

static std::string to_upper(std::string data) {
    std::for_each(data.begin(), data.end(), [](char& c) {
        c = ::toupper(c);
        });
    return data;
}

static std::string to_lower(std::string data) {
    std::for_each(data.begin(), data.end(), [](char& c) {
        c = ::tolower(c);
        });
    return data;
}

static unsigned char ucharIntensity(unsigned char uc, float intensity) {
    return (unsigned char)std::clamp((int)round(((float)uc) * intensity), 0, 255);
}

static float lerp(float a, float b, float f)
{
    return a * (1.0 - f) + (b * f);
}

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

    CVector2D operator+(const CVector2D vec)
    {
        CVector2D result(this->x, this->y);
        result.x += vec.x;
        result.y += vec.y;
        return result;
    }

    CVector2D operator-(const CVector2D vec)
    {
        CVector2D result(this->x, this->y);
        result.x -= vec.x;
        result.y -= vec.y;
        return result;
    }
};

class CVehicle {
public:

};

static std::string printmem(uintptr_t ptr, int size)
{
    unsigned char* p = (unsigned char*)(ptr);
    std::string str;
    char buffer[512];
    for (int i = 0; i < size; i++)
    {
        sprintf(buffer, "%02hhX ", p[i]);
        str += buffer;
        if ((i + 1) % 4 == 0) str += "\n";
    }
    return str;
}

static double DistanceBetween(CVector vec1, CVector vec2)
{
    double a = ((double)vec1.x - (double)vec2.x);
    double b = ((double)vec1.y - (double)vec2.y);
    double c = ((double)vec1.z - (double)vec2.z);

    return sqrt(a * a + b * b + c * c);
}

static void (*RegisterCorona)(unsigned int id, void* attachTo, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, posStruct const& posn, float radius, float farClip, int coronaType, int flaretype, bool enableReflection, bool checkObstacles, int _param_not_used, float angle, bool longDistance, float nearClip, unsigned char fadeState, float fadeSpeed, bool onlyFromBelow, bool reflectionDelay);