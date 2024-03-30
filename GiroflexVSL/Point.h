#pragma once

#include "pch.h"

class Point {
public:
	CVector customOffset = { 0, 0, 0 };

    bool useCustomColor = false;
	CRGBA customColor = { 255, 255, 0 };

	Json::Value ToJSON()
	{
		Json::Value value = Json::objectValue;

		value["customOffset"] = CVectorToJSON(customOffset);
		value["useCustomColor"] = useCustomColor;
		value["customColor"] = ColorToJSON(customColor);

		return value;
	}

	void FromJSON(Json::Value value)
	{
		customOffset = ValidateCVector(value["customOffset"], customOffset);
		useCustomColor = ValidateValue(value["useCustomColor"], useCustomColor).asBool();
		customColor = ValidateColor(value["customColor"], customColor);
	}
};