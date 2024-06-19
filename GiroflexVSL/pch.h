#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <filesystem>
#include <map>
//#include <windows.h>
#include <sys/stat.h>

#define _USE_MATH_DEFINES
#include <cmath>

#include <SimpleGTA.h>

#include "json/json.h"

#include "ibass.h"

static unsigned char ucharIntensity(unsigned char uc, float intensity) {
    return (unsigned char)std::clamp((int)round(((float)uc) * intensity), 0, 255);
}

static double DistanceBetween(CVector vec1, CVector vec2)
{
    double a = ((double)vec1.x - (double)vec2.x);
    double b = ((double)vec1.y - (double)vec2.y);
    double c = ((double)vec1.z - (double)vec2.z);

    return sqrt(a * a + b * b + c * c);
}

static double arch_fn_parabola(float x, float curve, float len)
{
	return -(pow(x, 2) * (double)curve) + (((double)len * (double)curve) * (double)x);
}

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

static Json::Value ColorToJSON(CRGBA color) {
	Json::Value json;
	json["r"] = color.r;
	json["g"] = color.g;
	json["b"] = color.b;
	json["a"] = color.a;
	return json;
}

static CRGBA ColorFromJSON(Json::Value json) {
	CRGBA color(0, 0, 0, 0);
	color.r = json["r"].asInt();
	color.g = json["g"].asInt();
	color.b = json["b"].asInt();
	color.a = json["a"].asInt();
	return color;
}

static Json::Value CVectorToJSON(CVector vec) {
	Json::Value json;
	json["x"] = vec.x;
	json["y"] = vec.y;
	json["z"] = vec.z;
	return json;
}

static CVector CVectorFromJSON(Json::Value json) {
	CVector vec(0, 0, 0);
	vec.x = json["x"].asFloat();
	vec.y = json["y"].asFloat();
	vec.z = json["z"].asFloat();
	return vec;
}

template<class T>
static Json::Value ValidateValue(Json::Value value, T defaultValue)
{
	if (value.empty()) return defaultValue;
	return value;
}

static CVector ValidateCVector(Json::Value value, CVector defaultValue)
{
	if (value.empty()) return defaultValue;
	return CVectorFromJSON(value);
}

static CRGBA ValidateColor(Json::Value value, CRGBA defaultValue)
{
	if (value.empty()) return defaultValue;
	return ColorFromJSON(value);
}