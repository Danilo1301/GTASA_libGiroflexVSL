#include "WindowPanel.h"
#include "WindowMain.h"
#include "../Mod.h"
#include "../Input.h"
#include "../Vehicles.h"
#include "../ModelInfos.h"
#include "../LightGroupDatas.h"
#include "../menu/Menu.h"
#include "../Log.h"

/*

top 10 spaghetti code

I DONT WANT TO CHANGE THIS ANYMORE '-

*/

bool WindowPanel::Visible = false;
std::vector<PanelButton*> WindowPanel::PanelButtons;

void WindowPanel::Toggle(bool state)
{
	Visible = state;

	if (Visible)
	{
		Create();
	}
}

void WindowPanel::Create()
{
	if (PanelButtons.size() > 0) return;

	for (int i = 0; i <= 3; i++)
	{
		PanelButton* panelButton = new PanelButton();
		PanelButtons.push_back(panelButton);
	}
}

void WindowPanel::Update()
{
	//if (!Visible) Toggle(true);
}

void WindowPanel::Draw()
{
	if (!Visible) return;

	if (!Mod::IsPlayerInAnyVehicle()) return; //test

	auto veh = Mod::GetPlayerVehicle();

	if (!ModelInfos::HasModelInfo(veh->modelId)) return; //test

	auto modelInfo = ModelInfos::GetModelInfo(veh->modelId);

	//


	CRGBA COLOR_PANEL_BACKGROUND = CRGBA(45, 45, 45);
	CRGBA COLOR_PANEL_BTN_OFF = CRGBA(70, 70, 70);
	CRGBA COLOR_PANEL_BTN_LIGHT = CRGBA(109, 132, 91);
	CRGBA COLOR_PANEL_BTN_DARK = CRGBA(127, 127, 127);

	CVector2D panelPosition = CVector2D(10, 200);

	CVector2D pos = panelPosition;
	int lightSlot = 0;
	for (auto panelButton : PanelButtons)
	{
		//test

		std::vector<LightGroup*> lightGroups;
		for (auto lg : modelInfo->lightGroups)
		{
			if (lg->lightSlotId != lightSlot) continue;
			lightGroups.push_back(lg);
		}
		LightGroupData* demoLightGroupData = NULL;
		if (lightGroups.size() > 0)
		{
			demoLightGroupData = LightGroupDatas::GetLightGroupData(lightGroups[0], veh->hVehicle);
		}

		//

		CVector2D btnBigSize = CVector2D(60, 50);
		CVector2D btnSmallSize = CVector2D(60, 30);

		float margin = 4.0f;

		//background
		Draw::DrawBox(
			pos,
			CVector2D(
				margin + btnBigSize.x + margin,
				margin + btnSmallSize.y + margin + btnBigSize.y + margin + btnBigSize.y + margin + btnSmallSize.y + margin
			),
			COLOR_PANEL_BACKGROUND
		);

		// ON / OFF
		CVector2D btnToggle_Pos = pos + CVector2D(margin, margin);
		
		bool lightsOn = demoLightGroupData ? demoLightGroupData->lightsOn : false;
		//if (!veh->lightsOn) lightsOn = false;

		if (DrawButton(
			demoLightGroupData ? (lightsOn ? 17 : 18) : 67,
			0,
			0,
			btnToggle_Pos,
			btnSmallSize,
			false,
			demoLightGroupData ? (lightsOn ? COLOR_PANEL_BTN_LIGHT : COLOR_PANEL_BTN_OFF) : COLOR_PANEL_BTN_OFF
		))
		{
			bool newState = !demoLightGroupData->lightsOn;
			for (auto lightGroup : lightGroups)
			{
				auto lightGroupData = LightGroupDatas::GetLightGroupData(lightGroup, veh->hVehicle);
				lightGroupData->lightsOn = newState;
			}

		}


		// 2  [pattern]
		CVector2D btnPattern_Pos = btnToggle_Pos + CVector2D(0, margin) + CVector2D(0, btnSmallSize.y);

		if (DrawButton(
			1,
			demoLightGroupData ? demoLightGroupData->patternLoop->StepIndex : 0,
			0,
			btnPattern_Pos,
			btnBigSize,
			false,
			COLOR_PANEL_BTN_LIGHT
		))
		{
			for (auto lightGroup : lightGroups)
			{
				auto newStep = demoLightGroupData->patternLoop->StepIndex + 1;

				auto lightGroupData = LightGroupDatas::GetLightGroupData(lightGroup, veh->hVehicle);
				lightGroupData->patternLoop->SetStep(newStep);
				lightGroupData->stepLoop->Clear();

			}
		}


		// 2  [step]
		CVector2D btnStep_Pos = btnPattern_Pos + CVector2D(0, margin) + CVector2D(0, btnBigSize.y);

		if (DrawButton(
			1,
			demoLightGroupData ? demoLightGroupData->stepLoop->StepIndex : 0,
			0,
			btnStep_Pos,
			btnBigSize,
			false,
			COLOR_PANEL_BTN_LIGHT
		))
		{
			bool newState = !demoLightGroupData->stepLoop->DontChangeSteps;
			for (auto lightGroup : lightGroups)
			{
				auto lightGroupData = LightGroupDatas::GetLightGroupData(lightGroup, veh->hVehicle);
				lightGroupData->stepLoop->DontChangeSteps = newState;
				lightGroupData->patternLoop->DontChangeSteps = newState;
			}
		}

		// Manual / Auto
		CVector2D btnMode_Pos = btnStep_Pos + CVector2D(0, margin) + CVector2D(0, btnBigSize.y);

		if (DrawButton(
			demoLightGroupData ? (demoLightGroupData->patternLoop->DontChangeSteps ? 74 : 70) : 67,
			0,
			0,
			btnMode_Pos,
			btnSmallSize,
			false,
			COLOR_PANEL_BTN_DARK
		))
		{
			bool newState = !demoLightGroupData->patternLoop->DontChangeSteps;
			for (auto lightGroup : lightGroups)
			{
				auto lightGroupData = LightGroupDatas::GetLightGroupData(lightGroup, veh->hVehicle);
				lightGroupData->patternLoop->DontChangeSteps = newState;
			}
		}

		pos.x += 58;
		lightSlot++;
	}

	pos.x -= 58;

	if (DrawButton(
		31,
		0,
		0,
		pos - CVector2D(0, 34),
		CVector2D(60, 30),
		false,
		CRGBA(191, 94, 87)
	))
	{
		WindowPanel::Toggle(false);
	}
}

bool WindowPanel::DrawButton(int gxtId, int num1, int num2, CVector2D position, CVector2D size, bool statePressedDown, CRGBA btnColor)
{
	bool mouseOver = Input::IsPointInsideRect(Input::GetTouchPos(), position, size);
	bool pressed = mouseOver && Input::hasTouchBeenReleasedThisFrame;;

	CRGBA boxColor = btnColor;
	if (statePressedDown || pressed) boxColor = CRGBA(46, 55, 68);

	Draw::DrawBoxWithText(
		gxtId, //on off
		num1,
		num2,
		position,
		size,
		boxColor,
		CRGBA(255, 255, 255)
	);

	return mouseOver && Input::hasTouchBeenReleasedThisFrame;
}