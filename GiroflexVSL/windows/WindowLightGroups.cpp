#include "WindowLightGroups.h"

#include "WindowMain.h"
#include "WindowEditing.h"
#include "WindowWhiteCorona.h"
#include "WindowShadow.h"
#include "WindowPointLight.h"
#include "WindowFlare.h"

#include "ModelInfos.h"
#include "LightGroupDatas.h"
#include "Globals.h"
#include "Vehicles.h"

void WindowLightGroups::Create(Window* parent)
{
    auto modelId = WindowMain::m_ModelId;

    auto window = Menu::AddWindow(8, parent);
    window->showPageControls = true;
    window->btnBack->onClick = [window]()
    {
        window->GoToPrevWindow();
    };

    int numLightgroups = 0;
    if(ModelInfos::HasModelInfo(modelId))
    {
        numLightgroups = ModelInfos::GetModelInfo(modelId)->lightGroups.size();
    }

    auto text_lightgroups = window->AddText(18, CRGBA(255, 255, 255));
    text_lightgroups->text->num1 = numLightgroups;

    auto button_add = window->AddButton(19, { 0, 0, 50, 255 });
    button_add->onClick = [modelId, window, parent]() {
        if (!ModelInfos::HasModelInfo(modelId))
        {
            ModelInfos::CreateModelInfo(modelId);
        }

        auto lightGroup = new LightGroup();
        lightGroup->AddPoint();
        lightGroup->AddPoint();
        //lightGroup->type = eLightGroupType::TWO_LIGHTS;
        //lightGroup3->usePointPositionInsteadOfIndex = true;
        lightGroup->offset.z = 1.0f;
        //lightGroup->MakeLightGroup();
        ModelInfos::GetModelInfo(modelId)->AddLightGroup(lightGroup);

        //enable all lights
        auto vehicle = Globals::GetPlayerVehicle();
        if (vehicle)
        {
            vehicle->Update(0);
            vehicle->SetGiroflexEnabled(true);
        }

        window->RemoveThisWindow();
        Create(parent);
    };

    if (ModelInfos::HasModelInfo(modelId))
    {
        int i = 0;
        for (auto lightGroup : ModelInfos::GetModelInfo(modelId)->lightGroups)
        {
            auto button_edit = window->AddButton(20, CRGBA(96, 125, 219));
            button_edit->text->num1 = i+1;

            button_edit->AddColorIndicator(&lightGroup->color3);
            button_edit->AddColorIndicator(&lightGroup->color2);
            button_edit->AddColorIndicator(&lightGroup->color1);

            button_edit->onClick = [window, lightGroup]() {
                CreateEditLightGroup(window, lightGroup);
            };

            i++;
        }
    }
}


void WindowLightGroups::CreateEditLightGroup(Window* parent, LightGroup* lightGroup)
{
    auto modelId = WindowMain::m_ModelId;

    auto window = Menu::AddWindow(8, parent);
    window->showPageControls = true;
    window->btnBack->onClick = [window]()
    {
        window->GoToPrevWindow();
    };

    auto option_giroflex = window->AddOptions(11);
    option_giroflex->optionsValue = (int)lightGroup->points.size();
    for(int i = 0; i <= 11; i++)
    {
        option_giroflex->AddOption(12, i, 0);
    }
    option_giroflex->onValueChange = [option_giroflex, lightGroup]() {
        //LightGroupDatas::DeleteLightGroupRerefences(lightGroup);

        lightGroup->SetNumberOfPoints(option_giroflex->optionsValue);

        //reset lightGroupData
        for (auto pair : Vehicles::m_Vehicles)
        {
            if (pair.second->modelId != WindowMain::m_ModelId) continue;
            LightGroupDatas::RemoveLightGroupDataFromVehicle(pair.second->hVehicle);
        }

        //enable again
        auto vehicle = Globals::GetPlayerVehicle();
        if (vehicle)
        {
            vehicle->Update(0);
            vehicle->SetGiroflexEnabled(true);
        }
    };

    auto edit_poits = window->AddButton(23);
    edit_poits->onClick = [window, lightGroup]()
    {
        CreateEditPoints(window, lightGroup);
    };

    window->AddFloatRange(41, &lightGroup->radius, 0.0f, 5.0f, 0.01f);

    auto button_position = window->AddButton(28);
    button_position->onClick = [lightGroup, window]() {
        /*
        MoveLightsSpeed from 
        * 0.01
        0.0f  0.00
        1.0f  0.01
        2.0f  0.02
        */
        float moveSpeed = WindowEditing::MoveLightsSpeed * 0.01f;

        auto posWindow = Menu::AddPositionWindow(window, &lightGroup->offset, -1000.0f, 1000.0f, moveSpeed, []() {});

        posWindow->width += 40.0f;

        auto distance = posWindow->AddFloatRange(43, &lightGroup->distance, -10.0f, 10.0f, moveSpeed);
        distance->onValueChange = [lightGroup]() {
            /*
            lightGroup->MakeLightGroup();

            auto vehicle = Mod::GetPlayerVehicle();
            if (vehicle)
            {
                vehicle->Update(0);
                vehicle->SetGiroflexEnabled(true);
            }
            */
        };

        auto curve = posWindow->AddFloatRange(44, &lightGroup->curve, -1.0f, 1.0f, 0.0005f);
        curve->onValueChange = [lightGroup]() {
            /*
            lightGroup->MakeLightGroup();

            auto vehicle = Mod::GetPlayerVehicle();
            if (vehicle)
            {
                vehicle->Update(0);
                vehicle->SetGiroflexEnabled(true);
            }
            */
        };
    };

    auto button_editing_options = window->AddButton(24);
    button_editing_options->onClick = [lightGroup, window]() {
        WindowEditing::Create(window, lightGroup);
    };

    auto button_white_corona = window->AddButton(42);
    button_white_corona->onClick = [lightGroup, window]() {
        WindowWhiteCorona::Create(window, lightGroup);
    };

    auto patternOffset = window->AddIntRange(43, &lightGroup->patternOffset, 0, 10, 1);
    patternOffset->holdToChange = false;
    patternOffset->onValueChange = [lightGroup]() {
        LightGroupDatas::DeleteLightGroupRerefences(lightGroup);

        auto vehicle = Globals::GetPlayerVehicle();
        if (vehicle)
        {
            vehicle->Update(0);
            vehicle->SetGiroflexEnabled(true);
        }
    };

    window->AddFloatRange(44, &lightGroup->nearClip, -2.0f, 5.0f, 0.01f);

    auto coronaTexture = window->AddIntRange(45, &lightGroup->coronaTexture, 0, 9, 1);
    coronaTexture->holdToChange = false;

    auto lightSlotId = window->AddIntRange(56, &lightGroup->lightSlotId, 0, 3, 1);
    lightSlotId->holdToChange = false;

    auto button_shadow = window->AddButton(46);
    button_shadow->onClick = [lightGroup, window]() {
        WindowShadow::Create(window, lightGroup);
    };

    auto button_pointLight = window->AddButton(49);
    button_pointLight->onClick = [lightGroup, window]() {
        WindowPointLight::Create(window, lightGroup);
    };

    auto button_color1 = window->AddButton(31);
    button_color1->AddColorIndicator(&lightGroup->color1);
    button_color1->onClick = [lightGroup, window]() {
        Menu::AddColorMenu(window, &lightGroup->color1);
    };

    auto button_color2 = window->AddButton(32);
    button_color2->AddColorIndicator(&lightGroup->color2);
    button_color2->onClick = [lightGroup, window]() {
        Menu::AddColorMenu(window, &lightGroup->color2);
    };

    auto button_color3 = window->AddButton(33);
    button_color3->AddColorIndicator(&lightGroup->color3);
    button_color3->onClick = [lightGroup, window]() {
        Menu::AddColorMenu(window, &lightGroup->color3);
    };

    auto button_flare = window->AddButton(53);
    button_flare->onClick = [lightGroup, window]() {
        WindowFlare::Create(window, lightGroup);
    };

    window->AddCheckbox(57, &lightGroup->alwaysEnabled);

    window->AddCheckbox(52, &lightGroup->freezeLights);

    auto button_duplicate = window->AddButton(58);
    button_duplicate->onClick = [window, lightGroup, modelId]()
    {
        Menu::AddConfirmWindow(window, 59, [lightGroup, modelId]() {
            ModelInfos::GetModelInfo(WindowMain::m_ModelId)->DuplicateLightGroup(lightGroup);

            //reset lightGroupData
            for (auto pair : Vehicles::m_Vehicles)
            {
                if (pair.second->modelId != WindowMain::m_ModelId) continue;

                LightGroupDatas::RemoveLightGroupDataFromVehicle(pair.second->hVehicle);
            }

            //enable again
            auto vehicle = Globals::GetPlayerVehicle();
            if (vehicle)
            {
                vehicle->Update(0);
                vehicle->SetGiroflexEnabled(true);
            }

            Menu::RemoveAllWindows();

            WindowMain::Remove();
            WindowMain::Create(modelId);
        }, []() {

        });
    };

    auto button_delete = window->AddButton(60, CRGBA(170, 70, 70));
    button_delete->onClick = [window, lightGroup, modelId]()
    {
        Menu::AddConfirmWindow(window, 61, [lightGroup, modelId]() {
            ModelInfos::GetModelInfo(WindowMain::m_ModelId)->RemoveLightGroup(lightGroup);

            //idk if this works
            if (ModelInfos::GetModelInfo(WindowMain::m_ModelId)->lightGroups.size() == 0)
            {
                ModelInfos::RemoveModelInfo(WindowMain::m_ModelId);
            }

            //enable lights again
            auto vehicle = Globals::GetPlayerVehicle();
            if (vehicle)
            {
                vehicle->Update(0);
                vehicle->SetGiroflexEnabled(true);
            }

            Menu::RemoveAllWindows();

            WindowMain::Remove();
            WindowMain::Create(modelId);
        }, []() {

        });
    };
}

void WindowLightGroups::CreateEditPoints(Window* parent, LightGroup* lightGroup)
{
    auto window = Menu::AddWindow(23, parent);
    window->showPageControls = true;
    window->btnBack->onClick = [window]()
    {
        window->GoToPrevWindow();
    };

    for(int i = 0; i < lightGroup->points.size(); i++)
    {
        auto point = lightGroup->points[i];

        auto button_edit = window->AddButton(20, CRGBA(96, 125, 219));
        button_edit->text->num1 = i+1;

        if(point->useCustomColor)
            button_edit->AddColorIndicator(&point->customColor);

        button_edit->onClick = [window, lightGroup, point]() {
            CreateEditPoint(window, lightGroup, point);
        };
    }
}

void WindowLightGroups::CreateEditPoint(Window* parent, LightGroup* lightGroup, Point* point)
{
    auto window = Menu::AddWindow(23, parent);
    window->showPageControls = true;
    window->btnBack->onClick = [window]()
    {
        window->GoToPrevWindow();
    };

    auto button_position = window->AddButton(28);
    button_position->onClick = [point, window]() {
        float moveSpeed = WindowEditing::MoveLightsSpeed * 0.01f;

        auto posWindow = Menu::AddPositionWindow(window, &point->customOffset, -1000.0f, 1000.0f, moveSpeed, []() {});

        posWindow->width += 40.0f;
    };

    window->AddCheckbox(29, &point->useCustomColor);

    auto button_color = window->AddButton(30);
    button_color->AddColorIndicator(&point->customColor);
    button_color->onClick = [point, window]() {
        Menu::AddColorMenu(window, &point->customColor);
    };
}