#pragma once

#include "pch.h"

#include "Point.h"
#include "eLightGroupType.h"
#include "ModConfig.h"
#include "Patterns.h"
#include "iniconfig/INIsection.h"


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

	bool renderPointLight = true;
	float pointLightIntensity = 1.00f;
	float pointLightDistance = 60.0f;

	float nearClip = -0.30f;

	int patternOffset = 0;

	float distance = 0.25f;

	eLightGroupType type = eLightGroupType::SINGLE_LIGHT;

	bool usePointPositionInsteadOfIndex = false;

	bool useSmallWhiteCorona = false;

	bool freezeLights = false;

	bool useFlare = false;
	float flareIntensity = 1.00f;
	float flareDistance = 100.0f;

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
			if (index == 2) {
				return color3;
			}

			if (index > 2) {
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

		if (type == eLightGroupType::SINGLE_LIGHT)
		{
			AddPoint(CVector(0, 0, 0), ePointPosition::CENTER);
		}

		if (type == eLightGroupType::TWO_LIGHTS)
		{
			AddPoint(CVector(-distance, 0, 0), ePointPosition::LEFT);
			AddPoint(CVector(distance, 0, 0), ePointPosition::RIGHT);
		}

		if (type == eLightGroupType::FIVE_LIGHTS)
		{
			int num = 5;
			float dist = distance;

			for (int i = 0; i < num; i++)
			{
				float x = (i * dist) - ((num - 1) * dist / 2);

				ePointPosition pos = ePointPosition::LEFT;

				if (i == 2) {
					pos = ePointPosition::CENTER;
				}

				if (i > 2) {
					pos = ePointPosition::RIGHT;
				}

				AddPoint(CVector(x, 0, 0), pos);
			}
		}

		if (type == eLightGroupType::TEN_LIGHTS)
		{
			int num = 10;
			float dist = distance;

			for (int i = 0; i < num; i++)
			{
				float x = (i * dist) - ((num - 1) * dist / 2);

				ePointPosition pos = ePointPosition::LEFT;

				if (i >= 5) {
					pos = ePointPosition::RIGHT;
				}

				AddPoint(CVector(x, 0, 0), pos);
			}
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

		section->AddBool("renderPointLight", renderPointLight);
		section->AddFloat("pointLightIntensity", pointLightIntensity);
		section->AddFloat("pointLightDistance", pointLightDistance);

		section->AddFloat("nearClip", nearClip);
		section->AddInt("patternOffset", patternOffset);
		section->AddFloat("distance", distance);

		section->AddInt("type", (int)type);
		section->AddBool("usePointPositionInsteadOfIndex", usePointPositionInsteadOfIndex);
		section->AddBool("useSmallWhiteCorona", useSmallWhiteCorona);
		section->AddBool("freezeLights", freezeLights);

		section->AddBool("useFlare", useFlare);
		section->AddFloat("flareDistance", flareDistance);
		section->AddFloat("flareIntensity", flareIntensity);

		return section;
	}

	void FromINISection(INISection* section)
	{
		offset = section->GetCVector("offset");
		color1 = section->GetCRGBA("color1");
		color2 = section->GetCRGBA("color2");
		color3 = section->GetCRGBA("color3");

		radius = section->GetFloat("radius", radius);

		renderShadow = section->GetBool("renderShadow", renderShadow);
		shadowIntensity = section->GetFloat("shadowIntensity", shadowIntensity);
		shadowSize = section->GetFloat("shadowSize", shadowSize);

		renderPointLight = section->GetBool("renderPointLight", renderPointLight);
		pointLightIntensity = section->GetFloat("pointLightIntensity", pointLightIntensity);
		pointLightDistance = section->GetFloat("pointLightDistance", pointLightDistance);

		nearClip = section->GetFloat("nearClip", nearClip);

		patternOffset = section->GetInt("patternOffset", patternOffset);
		distance = section->GetFloat("distance", distance);
		type = (eLightGroupType)section->GetInt("type", (int)type);
		usePointPositionInsteadOfIndex = section->GetBool("usePointPositionInsteadOfIndex", usePointPositionInsteadOfIndex);
		useSmallWhiteCorona = section->GetBool("useSmallWhiteCorona", useSmallWhiteCorona);
		freezeLights = section->GetBool("freezeLights", freezeLights);

		useFlare = section->GetBool("useFlare", useFlare);
		flareDistance = section->GetFloat("flareDistance", flareDistance);
		flareIntensity = section->GetFloat("flareIntensity", flareIntensity);
	}
};