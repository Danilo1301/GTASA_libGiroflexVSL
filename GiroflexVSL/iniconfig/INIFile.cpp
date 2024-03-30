#include "INIFile.h"

#include "../Log.h"

INISection* INIFile::AddSection(std::string key)
{
	std::cout << "AddSection " << key << std::endl;

	INISection* section = new INISection(key);
	sections.push_back(section);
	return section;
}

void INIFile::Save(std::string path)
{
	std::cout << "Save to " << path << std::endl;

	std::fstream file;
	file.open(path.c_str(), std::fstream::out | std::fstream::trunc);

	for (auto section : sections)
	{
		file << "[" << section->key << "]" << std::endl;

		/*
		* TEMPORARY FIX
		*/

		/*
		if (section->tmpSaveFix)
		{
		*/
			/*
			for (auto line : section->rawLines)
			{
				file << line << std::endl;
			}
			*/
		/*
		}
		else {
		*/
			for (auto value : section->values)
			{
				if (value.first.find("##LINE##") != std::string::npos)
				{
					file << value.second << std::endl;
					continue;
				}

				file << value.first << " = " << value.second << std::endl;
			}
		//}

		file << std::endl;
	}

	file.close();
}

void INIFile::Destroy()
{
	for (auto sec : sections)
	{
		delete sec;
	}
	sections.clear();
}

bool INIFile::Read(std::string path)
{
	std::cout << "Read from " << path << std::endl;

	std::vector<std::string> lines;
	std::string secname = "";
	auto PushSection = [&lines, &secname, this]() {
		if (lines.size() == 0)
		{
			return;
		}

		std::cout << secname << ", " << lines.size() << " lines" << std::endl;

		auto section = AddSection(secname);

		for (auto line : lines)
		{
			if (line == lines[0]) continue;

			std::string key = line.substr(0, line.find("="));
			key.erase(std::remove_if(key.begin(), key.end(), ::isspace), key.end());

			//
			//section->values.push_back(line);
			//

			if (line.find("=") == std::string::npos)
			{
				//Log::file << "INIFile: AddLine " << line << std::endl;
				section->AddLine(line);
				continue;
			}

			std::string value = line.substr(line.find("=") + 1);
			value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());
			
			//Log::file << "INIFile: AddString " << key << " | " << value << std::endl;
			section->AddString(key, value);
		}

		secname = "";
		lines.clear();
	};

	PushSection();

	std::ifstream infile(path);

	if (!infile.good()) return false;

	std::string line;
	while (std::getline(infile, line))
	{
		std::cout << "Line: " << line << std::endl;

		if (line.size() == 0)
		{
			PushSection();
			continue;
		}

		char szSection[100];
		if (sscanf(line.c_str(), "[%99[^]]]", szSection) == 1)
		//if (sscanf_s(line.c_str(), "[%99[^]]]", szSection, sizeof(szSection)) == 1)
		{
			PushSection();

			secname = szSection;
		}

		lines.push_back(line);
	}

	PushSection();

	return true;
}

std::vector<INISection*> INIFile::GetSections(std::string name)
{
	std::vector<INISection*> result;

	for (auto session : sections)
	{
		if (to_lower(session->key).compare(to_lower(name)) == 0)
		{
			result.push_back(session);
		}
	}

	return result;
}