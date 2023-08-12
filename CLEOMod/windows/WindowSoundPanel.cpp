#include "WindowSoundPanel.h"

#include "WindowSoundPanelSettings.h"
#include "../menu/Menu.h"
#include "../Input.h"
#include "../ModConfig.h"
#include "../Log.h"
#include "../Mod.h"

SoundPanelButton::SoundPanelButton()
{

}

void SoundPanelButton::Draw()
{
	CRGBA bgcolor = outlineColor;
	if (blink)
	{
		bgcolor = CRGBA(
			ucharIntensity(bgcolor.r, blinkState),
			ucharIntensity(bgcolor.g, blinkState),
			ucharIntensity(bgcolor.b, blinkState)
		);
	}

	Draw::DrawBox(position, size, bgcolor);

	//

	float outlineSize = 3.0f;

	Draw::DrawBoxWithText(
		gxtId,
		gxtNum1,
		gxtNum2,
		position + CVector2D(outlineSize, outlineSize),
		size - CVector2D(outlineSize * 2, outlineSize * 2),
		color,
		CRGBA(0, 0, 0, 255)
	);
}

void SoundPanelButton::Update(int dt)
{
	//audioStream
	if (audioStream)
	{
		audioStream->SetVolume(volume);
	}

	//input
	isPointerOver = Input::IsPointInsideRect(Input::GetTouchPos(), position, size);

	if (isPointerOver)
	{
		if (Input::hasTouchBeenReleasedThisFrame)
		{
			if (onClick) onClick();
		}

		if (Input::isTouchPressed)
		{
			if (!isPressed)
			{
				isPressed = true;
				if (onPressBegin) onPressBegin();
			}
		}
	}

	if (!isPointerOver || !Input::isTouchPressed)
	{
		if (isPressed)
		{
			isPressed = false;
			if (onPressEnd) onPressEnd();
		}
	}

	//blink
	blinkState += 0.004f * dt;
	if (blinkState >= 1.0f) blinkState = 0.0f;
}

//

std::vector<SoundPanelButton*> WindowSoundPanel::Buttons;
std::vector<SoundPanelButton*> WindowSoundPanel::AudioButtons;
bool WindowSoundPanel::Visible = false;
int WindowSoundPanel::PrevActiveButtonIndex = -1;

bool WindowSoundPanel::AllowMultipleSounds = false;
CRGBA WindowSoundPanel::ButtonColor = CRGBA(255, 80, 80, 150);
CRGBA WindowSoundPanel::ButtonOutlineColor = CRGBA(255, 0, 0, 150);
CVector2D WindowSoundPanel::OffsetPosition = CVector2D(0, 0);
float WindowSoundPanel::ButtonSize = 30.0f;

void WindowSoundPanel::Toggle(bool state)
{
	Visible = state;

	if (Visible)
	{
		Create();
	}
}

void WindowSoundPanel::Create()
{

	if (Buttons.size() == 0)
	{
		int numOfButtons = 8;
		float margin = 5.0f;
		CVector2D buttonSize = CVector2D(ButtonSize, ButtonSize);

		CVector2D screenSize = Input::GetGTAScreenSize();
		CVector2D startPos = {
			(screenSize.x / 2) - ((numOfButtons+2) * (buttonSize.x + margin))/2,
			screenSize.y - 45.0f
		};

		startPos = startPos + OffsetPosition;

		CVector2D currentPos = startPos;

		//Log::file << "WindowSoundPanel: startPos: " << startPos.x << ", " << startPos.y << std::endl;

		for (int i = 0; i < numOfButtons; i++)
		{
			auto button = AddButton();

			AudioButtons.push_back(button);

			button->color = ButtonColor;
			button->outlineColor = ButtonOutlineColor;
			button->size = buttonSize;
			button->position = currentPos;
			button->gxtNum1 = i + 1;

			/*
			* so.. yes. it loads a new audiostream every time I change any settings value
			* oh god
			* I hate this, but it should work from now
			*/
			std::string audiosPath = ModConfig::GetConfigFolder() + "/audios/";
			button->audioStream = new AudioStream(audiosPath + "/siren" + std::to_string(i+1) + ".wav");
			button->audioStream->Loop(true);

			if (i < 2)
			{
				button->onPressBegin = [button]() {
					button->blink = true;
					button->audioStream->Play();
				};

				button->onPressEnd = [button]() {
					button->blink = false;
					button->audioStream->Stop();
				};
			}
			else {
				button->onClick = [i]() {
					ToggleButton(i);
				};
			}
			
			currentPos = currentPos + CVector2D(buttonSize.x + margin, 0);
		}

		auto buttonOptions = AddButton();
		buttonOptions->size = buttonSize;
		buttonOptions->position = currentPos;
		buttonOptions->gxtId = 79;
		buttonOptions->color = CRGBA(110, 110, 110);
		buttonOptions->outlineColor = CRGBA(80, 80, 80);
		buttonOptions->onClick = []() {
			//Menu::ShowPopup(1, 1, 0, 1000.0f);

			WindowSoundPanelSettings::Create();
		};

		currentPos = currentPos + CVector2D(buttonSize.x + margin, 0);

		auto buttonClose = AddButton();
		buttonClose->size = buttonSize;
		buttonClose->position = currentPos;
		buttonClose->gxtId = 78;
		buttonClose->color = CRGBA(255, 100, 100);
		buttonClose->outlineColor = CRGBA(178, 70, 70);
		buttonClose->onClick = []() {
			//Menu::ShowPopup(1, 2, 0, 1000.0f);

			StopAllSounds();
			Toggle(false);
		};

	}
}

void WindowSoundPanel::DestroyButtons()
{
	AudioButtons.clear();

	while (Buttons.size() > 0)
	{
		auto button = Buttons[0];

		if (button->audioStream) button->audioStream->Destroy();

		Buttons.erase(std::find(Buttons.begin(), Buttons.end(), button));

		delete button;
	}
}

void WindowSoundPanel::RecreateButtons()
{
	StopAllSounds();
	DestroyButtons();
	Create();
}

void WindowSoundPanel::Update(int dt)
{
	/*
	//enable if testing
	if (!Visible)
	{
		Toggle(true);
	}
	*/

	if (!Visible) return;

	std::for_each(Buttons.begin(), Buttons.end(), [dt](SoundPanelButton* button) { button->Update(dt); });

	//disable if testing
	if (!Mod::IsPlayerInAnyVehicle())
	{
		StopAllSounds();
		Toggle(false);
	}
}

void WindowSoundPanel::Draw()
{
	if (!Visible) return;

	std::for_each(Buttons.begin(), Buttons.end(), [](SoundPanelButton* button) { button->Draw(); });
}

void WindowSoundPanel::StopAllSounds()
{
	PrevActiveButtonIndex = -1;

	std::for_each(AudioButtons.begin(), AudioButtons.end(), [](SoundPanelButton* button) {
		button->audioStream->Stop();
		button->blink = false;
	});
}

SoundPanelButton* WindowSoundPanel::AddButton()
{
	SoundPanelButton* button = new SoundPanelButton();
	Buttons.push_back(button);
	return button;
}

void WindowSoundPanel::ToggleButton(int index)
{
	auto button = AudioButtons[index];

	if (!button->audioStream->streamInternal) return;

	if (button->audioStream->GetState() == 1) //if playing
	{
		button->audioStream->Stop();
		button->blink = false;
	}
	else {
		button->audioStream->Play();
		button->blink = true;

		if (AllowMultipleSounds == false)
		{
			if (PrevActiveButtonIndex > 0 && PrevActiveButtonIndex != index)
			{
				auto prevButton = AudioButtons[PrevActiveButtonIndex];
				prevButton->audioStream->Stop();
				prevButton->blink = false;
			}
		}

	}

	PrevActiveButtonIndex = index;

	/*
	if (CurrentActiveButtonIndex != -1)
	{
		auto prevButton = AudioButtons[CurrentActiveButtonIndex];
		prevButton->audioStream->Stop();
		prevButton->blink = false;
	}

	if (CurrentActiveButtonIndex == index)
	{
		CurrentActiveButtonIndex = -1;
		return;
	}

	CurrentActiveButtonIndex = index;

	auto button = AudioButtons[CurrentActiveButtonIndex];
	button->audioStream->Play();
	button->blink = true;
	*/
}