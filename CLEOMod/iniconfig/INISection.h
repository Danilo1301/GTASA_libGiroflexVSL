#pragma once

#include "../pch.h"

class INISection {
public:
    std::map<std::string, std::string> values;
    std::vector<std::string> rawLines;
    std::string key;
    int lineId = 0;

    INISection(std::string key);

    //std::vector<std::string> GetLines();

    std::string GetString(std::string key);
    void AddString(std::string key, std::string value);

    void AddLine(std::string value);

    int GetInt(std::string key, int defaultValue);
	void AddInt(std::string key, int value);

    float GetFloat(std::string key, float defaultValue);
	void AddFloat(std::string key, float value);

    bool GetBool(std::string key, bool defaultValue);
	void AddBool(std::string key, bool value);

    CVector GetCVector(std::string key);
	void AddCVector(std::string key, CVector value);

    CRGBA GetCRGBA(std::string key);
	void AddCRGBA(std::string key, CRGBA value);
};