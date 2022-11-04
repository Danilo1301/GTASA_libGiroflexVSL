#pragma once

#include "pch.h"

#include "Point.h"
#include "eLightGroupType.h"


class LightGroup {
public:
	CVector offset = { 0, 0, 0 };
	std::vector<Point*> points;

	CRGBA color1 = CRGBA(255, 0, 0);
	CRGBA color2 = CRGBA(255, 255, 255);
	CRGBA color3 = CRGBA(0, 0, 255);

	eLightGroupType type = eLightGroupType::SINGLE_LIGHT;

	bool usePointPositionInsteadOfIndex = false;

	void MakeLightGroup()
	{
		RemoveAllPoints();

		if (type == eLightGroupType::SINGLE_LIGHT)
		{
			AddPoint(CVector(0, 0, 0), color1, ePointPosition::CENTER);
		}

		if (type == eLightGroupType::TWO_LIGHTS)
		{
			AddPoint(CVector(-1, 0, 0), color1, ePointPosition::LEFT);
			AddPoint(CVector(1, 0, 0), color2, ePointPosition::RIGHT);
		}

		if (type == eLightGroupType::FIVE_LIGHTS)
		{
			int num = 5;
			float dist = 0.5f;

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

				AddPoint(CVector(x, 0, 0), color, pos);
			}
		}
	}

	Point* AddPoint(CVector offset, CRGBA color, ePointPosition pointPos)
	{
		Point* point = new Point();
		point->offset = offset;
		point->color = color;
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