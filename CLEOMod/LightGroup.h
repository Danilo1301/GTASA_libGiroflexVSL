#pragma once

#include "pch.h"

#include "Point.h"
#include "eLightGroupType.h"


class LightGroup {
public:
	CVector offset = CVector(0, 0, 0);
	std::vector<Point*> points;

	CRGBA color1 = CRGBA(255, 0, 0);
	CRGBA color2 = CRGBA(0, 0, 255);
	CRGBA color3 = CRGBA(255, 255, 255);

	bool renderShadow = true;
	float shadowIntensity = 1.0f;
	float shadowSize = 5.0f;

	bool renderPointLight = true;
	float pointLightIntensity = 1.0f;
	float pointLightDistance = 60.0f;

	int patternOffset = 0;

	float distance = 0.2f;

	eLightGroupType type = eLightGroupType::SINGLE_LIGHT;

	bool usePointPositionInsteadOfIndex = false;

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

				CRGBA color = color1;
				ePointPosition pos = ePointPosition::LEFT;

				if (i == 2) {
					color = color3;
					pos = ePointPosition::CENTER;
				}

				if (i > 2) {
					color = color2;
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
};