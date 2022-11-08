#pragma once

#include "pch.h"

static std::string to_lower(std::string data) {
	std::for_each(data.begin(), data.end(), [](char& c) {
		c = ::tolower(c);
		});
	return data;
}

class INIFile {
	class Section {
	public:
		std::string key;
		std::map<std::string, std::string> values;

		std::string GetString(std::string key)
		{
			for (auto v : values)
			{
				if (to_lower(v.first).compare(to_lower(key)) == 0)
				{
					return v.second;
				}
			}

			return "";
		}

		int GetInt(std::string key, int defaultValue)
		{
			auto valueStr = GetString(key);

			if (valueStr.empty()) return defaultValue;

			return std::atoi(valueStr.c_str());
		}

		float GetFloat(std::string key, float defaultValue)
		{
			auto valueStr = GetString(key);

			if (valueStr.empty()) return defaultValue;

			return (float)std::atof(valueStr.c_str());
		}

		bool GetBool(std::string key, bool defaultValue)
		{
			auto valueStr = to_lower(GetString(key));

			if (valueStr.empty()) return defaultValue;

			return valueStr.compare("true") == 0;
		}

		CVector GetCVector(std::string key)
		{
			return CVector(
				GetFloat(key + ".x", 0),
				GetFloat(key + ".y", 0),
				GetFloat(key + ".z", 0)
			);
		}

		CRGBA GetCRGBA(std::string key)
		{
			return CRGBA(
				GetInt(key + ".r", 0),
				GetInt(key + ".g", 0),
				GetInt(key + ".b", 0),
				GetInt(key + ".a", 0)
			);
		}
	};
	std::vector<Section> sections;
	Section currentReadSection;

	std::fstream file;
	std::string path;
public:

	INIFile(std::string path)
	{
		this->path = path;
	}

	//

	std::vector<Section> GetSections(std::string name)
	{
		std::vector<Section> result;

		for (auto session : sections)
		{
			if (to_lower(session.key).compare(to_lower(name)) == 0)
			{
				result.push_back(session);
			}
		}

		return result;
	}

	void PushSection()
	{
		if (currentReadSection.values.size() > 0)
		{
			std::cout << currentReadSection.key << ", " << currentReadSection.values.size() << std::endl;

			sections.push_back(currentReadSection);
		}

		currentReadSection.key = "";
		currentReadSection.values.clear();
	}

	void Read()
	{
		sections.clear();

		PushSection();

		std::ifstream infile(path);

		std::string line;
		while (std::getline(infile, line))
		{
			//std::cout << "Line: " << line << std::endl;

			if (line.size() == 0)
			{
				PushSection();
				continue;
			}

			char szSection[100];
			if (sscanf(line.c_str(), "[%99[^]]]", szSection) == 1)
			{
				PushSection();

				currentReadSection.key = szSection;
			}

			if (line.find("=") != std::string::npos)
			{

				std::string key = line.substr(0, line.find("="));
				std::string value = line.substr(line.find("=") + 1);

				key.erase(std::remove_if(key.begin(), key.end(), ::isspace), key.end());
				value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());

				std::cout << "(" << key << ")(" << value << ")" << std::endl;

				currentReadSection.values[key] = value;
			}
		}

		PushSection();

		infile.close();
	}

	//

	void Open()
	{
		file.open(path.c_str(), std::fstream::out | std::fstream::trunc);
	}

	void AddLine(std::string value)
	{
		file << value << std::endl;
	}

	void AddString(std::string key, std::string value)
	{
		file << key << " = " << value << std::endl;
	}

	void AddInt(std::string key, int value)
	{
		file << key << " = " << std::to_string(value) << std::endl;
	}

	void AddFloat(std::string key, float value)
	{
		file << key << " = " << value << std::endl;
	}

	void AddBool(std::string key, bool value)
	{
		file << key << " = " << (value ? "true" : "false") << std::endl;
	}

	void AddCVector(std::string key, CVector value)
	{
		AddFloat(key + ".x", value.x);
		AddFloat(key + ".y", value.y);
		AddFloat(key + ".z", value.z);
	}

	void AddCRGBA(std::string key, CRGBA value)
	{
		AddInt(key + ".r", value.r);
		AddInt(key + ".g", value.g);
		AddInt(key + ".b", value.b);
		AddInt(key + ".a", value.a);
	}

	void Close()
	{
		file.close();
	}
};

class INIConfig {
public:
	static void MakePaths();

	static std::string GetConfigFolder();
	static void CreateFolder(std::string path);
	static void Save();
	static void Load();
};