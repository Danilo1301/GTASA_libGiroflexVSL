#pragma once

#include "../pch.h"

class INISection {
public:
    std::vector<std::pair<std::string, std::string>> values;
    std::string key;

    //int lineId = 0;
    //std::vector<std::string> rawLines;
    //bool tmpSaveFix = false;

    INISection(std::string key);

    //std::vector<std::string> GetLines();

    std::string GetString(std::string key);
    void AddString(std::string key, std::string value);

    void AddLine(std::string value);

    int GetIntWithDefaultValue(std::string key, int defaultValue);
    void GetInt(std::string key, int* pValue);
	void AddInt(std::string key, int value);
    void AddIntFromBool(std::string key, bool value);

    float GetFloatWithDefaultValue(std::string key, float defaultValue);
    void GetFloat(std::string key, float* pValue);
	void AddFloat(std::string key, float value);

    bool GetBoolWithDefaultValue(std::string key, bool defaultValue);
    void GetBool(std::string key, bool* pValue);
    void GetBoolFromInt(std::string key, bool* pValue);
	void AddBool(std::string key, bool value);

    CVector GetCVectorWithDefaultValue(std::string key, CVector defaultValue);
    void GetCVector(std::string key, CVector* pValue);
	void AddCVector(std::string key, CVector value);

    CVector2D GetCVector2DWithDefaultValue(std::string key, CVector2D defaultValue);
    void GetCVector2D(std::string key, CVector2D* pValue);
    void AddCVector2D(std::string key, CVector2D value);

    CRGBA GetCRGBAWithDefaultValue(std::string key, CRGBA defaultValue);
	void GetCRGBA(std::string key, CRGBA* pValue);
	void AddCRGBA(std::string key, CRGBA value);

    //std::vector<int> GetIntVectorList(std::string key);
};