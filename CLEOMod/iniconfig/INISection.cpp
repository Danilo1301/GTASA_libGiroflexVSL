#include "INISection.h"

INISection::INISection(std::string key)
{
    this->key = key;
}

/*
* doesn't work
std::vector<std::string> INISection::GetLines()
{
    std::vector<std::string> lines;
    for (std::map<std::string, std::string>::iterator it = values.begin(); it != values.end(); ++it) {
        lines.push_back(it->first);
    }
    return lines;
}
*/

std::string INISection::GetString(std::string key)
{
    for (auto v : values)
    {
        if (to_lower(v.first).compare(to_lower(key)) == 0) return v.second;
    }
    return "";
}

void INISection::AddString(std::string key, std::string value)
{
    std::cout << "AddString key=" << key << ", value= " << value << std::endl;

    values.push_back(std::pair<std::string, std::string>(key, value));

    //file << key << " = " << value << std::endl;
}

void INISection::AddLine(std::string value)
{
    /*
    if (tmpSaveFix)
    {
        rawLines.push_back(value);
        return;
    }
    */

    AddString("##LINE##", value);
    //lineId++;
    //file << value << std::endl;
}

int INISection::GetInt(std::string key, int defaultValue)
{
    auto valueStr = GetString(key);
    if (valueStr.empty()) return defaultValue;
    return std::atoi(valueStr.c_str());
}

void INISection::AddInt(std::string key, int value)
{
    AddString(key, std::to_string(value));
    //file << key << " = " << std::to_string(value) << std::endl;
}

float INISection::GetFloat(std::string key, float defaultValue)
{
    auto valueStr = GetString(key);
    if (valueStr.empty()) return defaultValue;
    return (float)std::atof(valueStr.c_str());
}

void INISection::AddFloat(std::string key, float value)
{
    AddString(key, std::to_string(value));
    //file << key << " = " << value << std::endl;
}

bool INISection::GetBool(std::string key, bool defaultValue)
{
    auto valueStr = to_lower(GetString(key));
    if (valueStr.empty()) return defaultValue;
    return valueStr.compare("true") == 0;
}

void INISection::AddBool(std::string key, bool value)
{
    AddString(key, std::string((value ? "true" : "false")));
    //file << key << " = " << (value ? "true" : "false") << std::endl;
}

CVector INISection::GetCVector(std::string key, CVector defaultValue)
{
    return CVector(
        GetFloat(key + ".x", defaultValue.x),
        GetFloat(key + ".y", defaultValue.y),
        GetFloat(key + ".z", defaultValue.z)
    );
}

void INISection::AddCVector(std::string key, CVector value)
{
    AddFloat(key + ".x", value.x);
    AddFloat(key + ".y", value.y);
    AddFloat(key + ".z", value.z);
}

CRGBA INISection::GetCRGBA(std::string key, CRGBA defaultValue)
{
    return CRGBA(
        GetInt(key + ".r", defaultValue.r),
        GetInt(key + ".g", defaultValue.g),
        GetInt(key + ".b", defaultValue.b),
        GetInt(key + ".a", defaultValue.a)
    );
}

void INISection::AddCRGBA(std::string key, CRGBA value)
{
    AddInt(key + ".r", value.r);
    AddInt(key + ".g", value.g);
    AddInt(key + ".b", value.b);
    AddInt(key + ".a", value.a);
}