#include "WindowSoundPanel.h"

#include "WindowSoundPanelSettings.h"
#include "../menu/Menu.h"
#include "../Input.h"
#include "../ModConfig.h"
#include "../Log.h"
#include "../Mod.h"

std::vector<SoundPanelButton*> WindowSoundPanel::m_buttons;
std::vector<AudioStreamData> WindowSoundPanel::m_audioStreamData;
int WindowSoundPanel::m_prevActiveIndex = -1;
bool WindowSoundPanel::m_allowMultipleSounds = false;
CRGBA WindowSoundPanel::m_buttonDefaultColor = { 109, 167, 100, 255 };
CRGBA WindowSoundPanel::m_buttonActiveColor = { 255, 131, 131, 255 };
CVector2D WindowSoundPanel::m_position = CVector2D(260, 340);
bool WindowSoundPanel::m_visible = false;

void WindowSoundPanel::Toggle(bool state)
{
	if (m_visible == state) return;

	m_visible = state;

	if (m_visible)
	{
		Create();
	}
	else {
		StopAllSounds();
		DestroyButtons();
	}
}

void WindowSoundPanel::Create()
{
	CVector2D bgPosition = m_position;
	CVector2D bgSize = { 235.0f, 103.0f };

	Draw::DrawBox(bgPosition, bgSize, CRGBA(32, 32, 32));

	//

	CRGBA buttonDefaultColor = m_buttonDefaultColor;
	CRGBA buttonActiveColor = m_buttonActiveColor;
	CRGBA textDefaultColor = { 0, 0, 0, 255 };

	auto buttonSettings = AddButton();
	buttonSettings->position = bgPosition + CVector2D(3, 3);
	buttonSettings->size = CVector2D(38, 25);
	buttonSettings->sprite.spriteId = 3;
	buttonSettings->text.gxtId = 79;
	buttonSettings->text.color = textDefaultColor;
	buttonSettings->color = CRGBA(128, 128, 128);
	buttonSettings->canBeActivated = false;
	buttonSettings->onClick = []() {
		WindowSoundPanelSettings::Create();
	};

	auto buttonSiren1 = AddButton();
	buttonSiren1->position = bgPosition + CVector2D(3, 29);
	buttonSiren1->size = CVector2D(50, 35);
	buttonSiren1->sprite.spriteId = 1;
	buttonSiren1->text.gxtId = 1;
	buttonSiren1->text.num1 = 1;
	buttonSiren1->text.color = textDefaultColor;
	buttonSiren1->color = buttonDefaultColor;
	buttonSiren1->activeColor = buttonActiveColor;
	AddButtonToAudioList(buttonSiren1, 1);

	auto buttonSiren2 = AddButton();
	buttonSiren2->position = bgPosition + CVector2D(3, 65);
	buttonSiren2->size = CVector2D(63, 35);
	buttonSiren2->sprite.spriteId = 1;
	buttonSiren2->text.gxtId = 1;
	buttonSiren2->text.num1 = 2;
	buttonSiren2->text.color = textDefaultColor;
	buttonSiren2->color = buttonDefaultColor;
	buttonSiren2->activeColor = buttonActiveColor;
	AddButtonToAudioList(buttonSiren2, 2);

	//

	auto buttonSiren3 = AddButton();
	buttonSiren3->position = bgPosition + CVector2D(67, 29);
	buttonSiren3->size = CVector2D(50, 35);
	buttonSiren3->sprite.spriteId = 3;
	buttonSiren3->text.gxtId = 1;
	buttonSiren3->text.num1 = 3;
	buttonSiren3->text.color = textDefaultColor;
	buttonSiren3->color = buttonDefaultColor;
	buttonSiren3->activeColor = buttonActiveColor;
	AddButtonToAudioList(buttonSiren3, 3);

	auto buttonSiren4 = AddButton();
	buttonSiren4->position = bgPosition + CVector2D(67, 65);
	buttonSiren4->size = CVector2D(50, 35);
	buttonSiren4->sprite.spriteId = 3;
	buttonSiren4->text.gxtId = 1;
	buttonSiren4->text.num1 = 4;
	buttonSiren4->text.color = textDefaultColor;
	buttonSiren4->color = buttonDefaultColor;
	buttonSiren4->activeColor = buttonActiveColor;
	AddButtonToAudioList(buttonSiren4, 4);

	//

	auto buttonSiren5 = AddButton();
	buttonSiren5->position = bgPosition + CVector2D(118, 29);
	buttonSiren5->size = CVector2D(50, 35);
	buttonSiren5->sprite.spriteId = 3;
	buttonSiren5->text.gxtId = 1;
	buttonSiren5->text.num1 = 5;
	buttonSiren5->text.color = textDefaultColor;
	buttonSiren5->color = buttonDefaultColor;
	buttonSiren5->activeColor = buttonActiveColor;
	AddButtonToAudioList(buttonSiren5, 5);

	auto buttonSiren6 = AddButton();
	buttonSiren6->position = bgPosition + CVector2D(118, 65);
	buttonSiren6->size = CVector2D(50, 35);
	buttonSiren6->sprite.spriteId = 3;
	buttonSiren6->text.gxtId = 1;
	buttonSiren6->text.num1 = 6;
	buttonSiren6->text.color = textDefaultColor;
	buttonSiren6->color = buttonDefaultColor;
	buttonSiren6->activeColor = buttonActiveColor;
	AddButtonToAudioList(buttonSiren6, 6);

	//

	auto buttonClose = AddButton();
	buttonClose->position = bgPosition + CVector2D(194, 3);
	buttonClose->size = CVector2D(38, 25);
	buttonClose->sprite.spriteId = 3;
	buttonClose->text.gxtId = 78;
	buttonClose->text.color = textDefaultColor;
	buttonClose->color = CRGBA(220, 145, 145);;
	buttonClose->canBeActivated = false;
	buttonClose->onClick = []() {
		WindowSoundPanel::Toggle(false);
	};

	auto buttonSiren7 = AddButton();
	buttonSiren7->position = bgPosition + CVector2D(182, 29);
	buttonSiren7->size = CVector2D(50, 35);
	buttonSiren7->sprite.spriteId = 2;
	buttonSiren7->text.gxtId = 1;
	buttonSiren7->text.num1 = 7;
	buttonSiren7->text.color = textDefaultColor;
	buttonSiren7->color = buttonDefaultColor;
	buttonSiren7->activeColor = buttonActiveColor;
	AddButtonToAudioList(buttonSiren7, 7);

	auto buttonSiren8 = AddButton();
	buttonSiren8->position = bgPosition + CVector2D(169, 65);
	buttonSiren8->size = CVector2D(63, 35);
	buttonSiren8->sprite.spriteId = 2;
	buttonSiren8->text.gxtId = 1;
	buttonSiren8->text.num1 = 8;
	buttonSiren8->text.color = textDefaultColor;
	buttonSiren8->color = buttonDefaultColor;
	buttonSiren8->activeColor = buttonActiveColor;
	AddButtonToAudioList(buttonSiren8, 8);

}


void WindowSoundPanel::Update(int dt)
{
	//enable if testing
	/*
	if (!Visible)
	{
		Toggle(true);
	}
	*/

	if (!m_visible) return;

	std::for_each(m_buttons.begin(), m_buttons.end(), [dt](SoundPanelButton* button) { button->Update(dt); });

	//disable if testing
	if (!Mod::IsPlayerInAnyVehicle())
	{
		Toggle(false);
	}
}

void WindowSoundPanel::Draw()
{
	if (!m_visible) return;

	CVector2D bgPosition = m_position;
	CVector2D bgSize = { 235.0f, 103.0f };

	Draw::DrawBox(bgPosition, bgSize, CRGBA(32, 32, 32));

	std::for_each(m_buttons.begin(), m_buttons.end(), [](SoundPanelButton* button) { button->Draw(); });
}

void WindowSoundPanel::DestroyButtons()
{
	while (m_buttons.size() > 0)
	{
		auto button = m_buttons[0];
		button->Destroy();
		m_buttons.erase(std::find(m_buttons.begin(), m_buttons.end(), button));
		delete button;
	}

	m_audioStreamData.clear();
}

SoundPanelButton* WindowSoundPanel::AddButton()
{
	SoundPanelButton* button = new SoundPanelButton();
	m_buttons.push_back(button);
	return button;
}

void WindowSoundPanel::RecreateButtons()
{
	StopAllSounds();
	DestroyButtons();
	Create();
}

void WindowSoundPanel::AddButtonToAudioList(SoundPanelButton* button, int audioId)
{
	std::string audiosPath = ModConfig::GetConfigFolder() + "/audios/";

	AudioStreamData audioStreamData;
	audioStreamData.button = button;
	audioStreamData.audioStream = new AudioStream(audiosPath + "/siren" + std::to_string(audioId) + ".wav");
	audioStreamData.audioStream->Loop(true);

	m_audioStreamData.push_back(audioStreamData);

	button->onClick = [audioId]() {
		ToggleAudioButton(audioId - 1);
	};
}


void WindowSoundPanel::ToggleAudioButton(int index)
{
	//Menu::ShowPopup(1, index, 0, 1000.0f);

	AudioStreamData* audioStreamData = &m_audioStreamData[index];

	if (!audioStreamData->audioStream->streamInternal) return;

	if (audioStreamData->audioStream->GetState() == 1) //if playing
	{
		audioStreamData->audioStream->Stop();
	}
	else {
		audioStreamData->audioStream->Play();
		
		if (m_allowMultipleSounds == false)
		{
			if (m_prevActiveIndex != -1 && m_prevActiveIndex != index)
			{
				AudioStreamData* prevAudioStreamData = &m_audioStreamData[m_prevActiveIndex];
				prevAudioStreamData->audioStream->Stop();
				prevAudioStreamData->button->isActive = prevAudioStreamData->audioStream->GetState() == 1;
			}
		}
	}

	audioStreamData->button->isActive = audioStreamData->audioStream->GetState() == 1;

	m_prevActiveIndex = index;
}

void WindowSoundPanel::StopAllSounds()
{
	m_prevActiveIndex = -1;

	std::for_each(m_audioStreamData.begin(), m_audioStreamData.end(), [](AudioStreamData& audioStreamData) {
		audioStreamData.audioStream->Stop();
		audioStreamData.button->isActive = false;
	});
}