#pragma once

#include "pch.h"

#include "Point.h"
#include "eLightGroupType.h"
#include "ModConfig.h"
#include "Patterns.h"
#include "iniconfig/INIsection.h"

static double arch_fn_parabola(float x, float curve, float len)
{
	return -(pow(x, 2) * (double)curve) + (((double)len * (double)curve) * (double)x);
}

class LightGroup {
public:
	CVector offset = CVector(0, 0, 0);
	std::vector<Point*> points;

	CRGBA color1 = CRGBA(255, 0, 0);
	CRGBA color2 = CRGBA(0, 0, 255);
	CRGBA color3 = CRGBA(255, 255, 255);

	float radius = 1.0f;

	bool renderShadow = false;
	float shadowIntensity = 0.80f;
	float shadowSize = 5.0f;
	float shadowPositionX = 0.0f;
	float shadowPositionY = 0.0f;
	int shadowTexture = 3;
	float shadowRotation = 0.0f;
	bool shadowFlipTextures = false;

	bool renderPointLight = true;
	float pointLightIntensity = 1.00f;
	float pointLightDistance = 60.0f;

	float nearClip = -0.30f;

	int patternOffset = 0;

	float distance = 0.25f;

	eLightGroupType type = eLightGroupType::SINGLE_LIGHT;

	bool usePointPositionInsteadOfIndex = false;

	bool useSmallWhiteCorona = true;
	float smallWhiteCoronaScale = 0.3f;
	int smallWhiteCoronaTexture = 0;
	CRGBA smallWhiteCoronaColor = CRGBA(255, 255, 255);

	bool freezeLights = false;

	bool useFlare = false;
	float flareIntensity = 1.00f;
	float flareDistance = 100.0f;
	int flareTexture = 1;

	float curve = 0.0f;

	int coronaTexture = 0;

	int lightSlotId = 0;

	void ChangeDistance(float distance)
	{
		this->distance = distance;
		MakeLightGroup();
	}

	CRGBA GetPointColor(Point* point, int index)
	{
		//CRGBA color = color1;
		
		if (type == eLightGroupType::SINGLE_LIGHT)
		{
			return color1;
		}

		if (type == eLightGroupType::TWO_LIGHTS)
		{
			if (point->pointPosition == ePointPosition::LEFT) return color1;
			if (point->pointPosition == ePointPosition::RIGHT) return color2;
		}

		if (type == eLightGroupType::FIVE_LIGHTS)
		{
			if (index == 2) return color3;
			if (index > 2) return color2;

			return color1;
		}

		if (type == eLightGroupType::SIX_LIGHTS)
		{
			if (index >= 3) {
				return color2;
			}
			return color1;
		}

		if (type == eLightGroupType::EIGHT_LIGHTS)
		{
			if (index >= 4) {
				return color2;
			}
			return color1;
		}

		if (type == eLightGroupType::TEN_LIGHTS)
		{
			if (index >= 5) {
				return color2;
			}
			return color1;
		}

		return color1;
	}

	void MakeLightGroup()
	{
		RemoveAllPoints();



		int amountOfLights = 0;

		if (type == eLightGroupType::SINGLE_LIGHT) amountOfLights = 1;
		if (type == eLightGroupType::TWO_LIGHTS) amountOfLights = 2;
		if (type == eLightGroupType::FIVE_LIGHTS) amountOfLights = 5;
		if (type == eLightGroupType::SIX_LIGHTS) amountOfLights = 6;
		if (type == eLightGroupType::EIGHT_LIGHTS) amountOfLights = 8;
		if (type == eLightGroupType::TEN_LIGHTS) amountOfLights = 10;

		for (int i = 0; i < amountOfLights; i++)
		{
			float x = (i * distance) - ((amountOfLights - 1) * distance / 2);
			float y = (float)arch_fn_parabola((float)i, curve, (float)(amountOfLights - 1));

			ePointPosition pos = ePointPosition::LEFT;

			if (type == eLightGroupType::TWO_LIGHTS)
			{
				if (i == 0) pos = ePointPosition::LEFT;
				if (i == 1) pos = ePointPosition::RIGHT;
			}

			if (type == eLightGroupType::FIVE_LIGHTS)
			{
				if (i == 2) pos = ePointPosition::CENTER;
				if (i > 2) pos = ePointPosition::RIGHT;
			}

			if (type == eLightGroupType::SIX_LIGHTS)
			{
				if (i >= 3) pos = ePointPosition::RIGHT;
			}

			if (type == eLightGroupType::EIGHT_LIGHTS)
			{
				if (i >= 4) pos = ePointPosition::RIGHT;
			}

			if (type == eLightGroupType::TEN_LIGHTS)
			{
				if (i >= 5) pos = ePointPosition::RIGHT;
			}
			
			AddPoint(CVector(x, y, 0), pos);
		}
	}

	Point* AddPoint(CVector offset, ePointPosition pointPos)
	{
		Point* point = new Point();
		point->offset = offset;
		//point->color = color;
		point->pointPosition = pointPos;

		points.push_back(point);
		return point;
	}

	void RemovePoint(Point* point)
	{
		auto it = std::find(points.begin(), points.end(), point);
		if (it == points.end()) return;
		points.erase(it);
		delete point;
	}

	void RemoveAllPoints()
	{
		while (points.size() > 0)
			RemovePoint(points[0]);
	}

	void Destroy()
	{
		RemoveAllPoints();
	}

	INISection* ToINISection()
	{
		INISection* section = new INISection("Lightgroup");

		//section->AddLine("[LightGroup]");

		section->AddCVector("offset", offset);
		section->AddCRGBA("color1", color1);
		section->AddCRGBA("color2", color2);
		section->AddCRGBA("color3", color3);
		section->AddFloat("radius", radius);

		section->AddBool("renderShadow", renderShadow);
		section->AddFloat("shadowIntensity", shadowIntensity);
		section->AddFloat("shadowSize", shadowSize);
		section->AddFloat("shadowPositionX", shadowPositionX);
		section->AddFloat("shadowPositionY", shadowPositionY);
		section->AddInt("shadowTexture", shadowTexture);
		section->AddFloat("shadowRotation", shadowRotation);
		section->AddBool("shadowFlipTextures", shadowFlipTextures);

		section->AddBool("renderPointLight", renderPointLight);
		section->AddFloat("pointLightIntensity", pointLightIntensity);
		section->AddFloat("pointLightDistance", pointLightDistance);

		section->AddFloat("nearClip", nearClip);
		section->AddInt("patternOffset", patternOffset);
		section->AddFloat("distance", distance);

		section->AddInt("type", (int)type);
		section->AddBool("usePointPositionInsteadOfIndex", usePointPositionInsteadOfIndex);

		section->AddBool("useSmallWhiteCorona", useSmallWhiteCorona);
		section->AddFloat("smallWhiteCoronaScale", smallWhiteCoronaScale);
		section->AddInt("smallWhiteCoronaTexture", smallWhiteCoronaTexture);
		section->AddCRGBA("smallWhiteCoronaColor", smallWhiteCoronaColor);

		section->AddBool("freezeLights", freezeLights);

		section->AddBool("useFlare", useFlare);
		section->AddFloat("flareDistance", flareDistance);
		section->AddFloat("flareIntensity", flareIntensity);
		section->AddFloat("flareTexture", flareTexture);

		section->AddFloat("curve", curve);

		section->AddInt("coronaTexture", coronaTexture);

		section->AddInt("lightSlotId", lightSlotId);

		return section;
	}

	void FromINISection(INISection* section)
	{
		offset = section->GetCVector("offset", offset);
		color1 = section->GetCRGBA("color1", color1);
		color2 = section->GetCRGBA("color2", color2);
		color3 = section->GetCRGBA("color3", color3);

		radius = section->GetFloat("radius", radius);

		renderShadow = section->GetBool("renderShadow", renderShadow);
		shadowIntensity = section->GetFloat("shadowIntensity", shadowIntensity);
		shadowSize = section->GetFloat("shadowSize", shadowSize);
		shadowPositionX = section->GetFloat("shadowPositionX", shadowPositionX);
		shadowPositionY = section->GetFloat("shadowPositionY", shadowPositionY);
		shadowTexture = section->GetInt("shadowTexture", shadowTexture);
		shadowRotation = section->GetFloat("shadowRotation", shadowRotation);
		shadowFlipTextures = section->GetBool("shadowFlipTextures", shadowFlipTextures);

		renderPointLight = section->GetBool("renderPointLight", renderPointLight);
		pointLightIntensity = section->GetFloat("pointLightIntensity", pointLightIntensity);
		pointLightDistance = section->GetFloat("pointLightDistance", pointLightDistance);

		nearClip = section->GetFloat("nearClip", nearClip);

		patternOffset = section->GetInt("patternOffset", patternOffset);
		distance = section->GetFloat("distance", distance);
		type = (eLightGroupType)section->GetInt("type", (int)type);
		usePointPositionInsteadOfIndex = section->GetBool("usePointPositionInsteadOfIndex", usePointPositionInsteadOfIndex);
		
		useSmallWhiteCorona = section->GetBool("useSmallWhiteCorona", useSmallWhiteCorona);
		smallWhiteCoronaScale = section->GetFloat("smallWhiteCoronaScale", smallWhiteCoronaScale);
		smallWhiteCoronaTexture = section->GetInt("smallWhiteCoronaTexture", smallWhiteCoronaTexture);
		smallWhiteCoronaColor = section->GetCRGBA("smallWhiteCoronaColor", smallWhiteCoronaColor);
		
		freezeLights = section->GetBool("freezeLights", freezeLights);

		useFlare = section->GetBool("useFlare", useFlare);
		flareDistance = section->GetFloat("flareDistance", flareDistance);
		flareIntensity = section->GetFloat("flareIntensity", flareIntensity);
		flareTexture = section->GetInt("flareTexture", flareTexture);

		curve = section->GetFloat("curve", curve);

		coronaTexture = section->GetInt("coronaTexture", coronaTexture);

		lightSlotId = section->GetInt("lightSlotId", lightSlotId);
	}
};