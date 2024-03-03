#include "WindowLightGroups.h"

#include "WindowMain.h"
#include "WindowEditing.h"
#include "WindowFlare.h"
#include "WindowWhiteCorona.h"
#include "WindowShadow.h"
#include "WindowPointLight.h"

#include "../ModelInfos.h"
#include "../LightGroupDatas.h"
#include "../Vehicles.h"
#include "../Mod.h"

void WindowLightGroups::Create(Window* parent)
{
    auto modelId = WindowMain::m_ModelId;

    auto window = Menu::AddWindow(29, parent);
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

    auto text_lightgroups = window->AddText(15, CRGBA(255, 255, 255));
    text_lightgroups->text->num1 = numLightgroups;

    auto button_add = window->AddButton(13, { 0, 0, 50, 255 });
    button_add->onClick = [modelId, window, parent]() {


        if (!ModelInfos::HasModelInfo(modelId))
        {
            ModelInfos::CreateModelInfo(modelId);
        }

        auto lightGroup = new LightGroup();
        lightGroup->type = eLightGroupType::TWO_LIGHTS;
        //lightGroup3->usePointPositionInsteadOfIndex = true;
        lightGroup->offset.z = 1.0f;
        lightGroup->MakeLightGroup();
        ModelInfos::GetModelInfo(modelId)->AddLightGroup(lightGroup);

        //enable all lights
        auto vehicle = Mod::GetPlayerVehicle();
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
            auto button_edit = window->AddButton(12, CRGBA(96, 125, 219));
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

    auto window = Menu::AddWindow(29, parent);
    window->showPageControls = true;
    window->btnBack->onClick = [window]()
    {
        window->GoToPrevWindow();
    };

    //==================

    //1
    auto option_giroflex = window->AddOptions(8);
    option_giroflex->optionsValue = (int)lightGroup->type;
    option_giroflex->AddOption(33, 0, 0); //1
    option_giroflex->AddOption(34, 0, 0); //2
    option_giroflex->AddOption(93, 0, 0); //3
    option_giroflex->AddOption(94, 0, 0); //4
    option_giroflex->AddOption(35, 0, 0); //5
    option_giroflex->AddOption(76, 0, 0); //6
    option_giroflex->AddOption(85, 0, 0); //8
    option_giroflex->AddOption(65, 0, 0); //10
    option_giroflex->onValueChange = [option_giroflex, lightGroup]() {
        lightGroup->type = (eLightGroupType)option_giroflex->optionsValue;
        LightGroupDatas::DeleteLightGroupRerefences(lightGroup);
        lightGroup->MakeLightGroup();

        auto vehicle = Mod::GetPlayerVehicle();
        if (vehicle)
        {
            vehicle->Update(0);
            vehicle->SetGiroflexEnabled(true);
        }
    };

    //2
    window->AddFloatRange(44, &lightGroup->radius, 0.0f, 5.0f, 0.01f);

    //3
    auto button_editing_options = window->AddButton(54);
    button_editing_options->onClick = [lightGroup, window]() {
        WindowEditing::Create(window, lightGroup);
    };
    
    //4
    auto button_white_corona = window->AddButton(53);
    button_white_corona->onClick = [lightGroup, window]() {
        WindowWhiteCorona::Create(window, lightGroup);
    };

    //5
    auto button_position = window->AddButton(26);
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

        auto distance = posWindow->AddFloatRange(27, &lightGroup->distance, -10.0f, 10.0f, moveSpeed);
        distance->onValueChange = [lightGroup]() {
            lightGroup->MakeLightGroup();

            auto vehicle = Mod::GetPlayerVehicle();
            if (vehicle)
            {
                vehicle->Update(0);
                vehicle->SetGiroflexEnabled(true);
            }
        };

        auto curve = posWindow->AddFloatRange(68, &lightGroup->curve, -1.0f, 1.0f, 0.0005f);
        curve->onValueChange = [lightGroup]() {
            lightGroup->MakeLightGroup();

            auto vehicle = Mod::GetPlayerVehicle();
            if (vehicle)
            {
                vehicle->Update(0);
                vehicle->SetGiroflexEnabled(true);
            }
        };
    };
    
    //====================
    
    //1
    auto patternOffset = window->AddIntRange(52, &lightGroup->patternOffset, 0, 10, 1);
    patternOffset->holdToChange = false;
    patternOffset->onValueChange = [lightGroup]() {
        LightGroupDatas::DeleteLightGroupRerefences(lightGroup);

        auto vehicle = Mod::GetPlayerVehicle();
        if (vehicle)
        {
            vehicle->Update(0);
            vehicle->SetGiroflexEnabled(true);
        }
    };
    
    //2
    window->AddFloatRange(43, &lightGroup->nearClip, -2.0f, 5.0f, 0.01f);

    //3
    auto coronaTexture = window->AddIntRange(72, &lightGroup->coronaTexture, 0, 9, 1);
    coronaTexture->holdToChange = false;

    //4
    auto button_shadow = window->AddButton(36);
    button_shadow->onClick = [lightGroup, window]() {
        WindowShadow::Create(window, lightGroup);
    };

    //5
    auto button_pointLight = window->AddButton(39);
    button_pointLight->onClick = [lightGroup, window]() {
        WindowPointLight::Create(window, lightGroup);
    };

    //==============

    //1
    auto button_color1 = window->AddButton(9);
    button_color1->AddColorIndicator(&lightGroup->color1);
    button_color1->onClick = [lightGroup, window]() {
        Menu::AddColorMenu(window, &lightGroup->color1);
    };

    //2
    auto button_color2 = window->AddButton(10);
    button_color2->AddColorIndicator(&lightGroup->color2);
    button_color2->onClick = [lightGroup, window]() {
        Menu::AddColorMenu(window, &lightGroup->color2);
    };

    //3
    auto button_color3 = window->AddButton(11);
    button_color3->AddColorIndicator(&lightGroup->color3);
    button_color3->onClick = [lightGroup, window]() {
        Menu::AddColorMenu(window, &lightGroup->color3);
    };

    //4
    window->AddCheckbox(55, &lightGroup->freezeLights);

    //5
    auto button_flare = window->AddButton(62);
    button_flare->onClick = [lightGroup, window]() {
        WindowFlare::Create(window, lightGroup);
    };

    //=====================

    //1
    auto lightSlotId = window->AddIntRange(75, &lightGroup->lightSlotId, 0, 3, 1);
    lightSlotId->holdToChange = false;

    //2
    auto button_duplicate = window->AddButton(59);
    button_duplicate->onClick = [window, lightGroup, modelId]()
    {
        Menu::AddConfirmWindow(window, 60, [lightGroup, modelId]() {
            ModelInfos::GetModelInfo(WindowMain::m_ModelId)->DuplicateLightGroup(lightGroup);

            //reset lightGroupData
            for (auto pair : Vehicles::m_Vehicles)
            {
                if (pair.second->modelId != WindowMain::m_ModelId) continue;

                LightGroupDatas::RemoveLightGroupDataFromVehicle(pair.second->hVehicle);
            }

            //enable again
            auto vehicle = Mod::GetPlayerVehicle();
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

    //3
    auto button_delete = window->AddButton(42, CRGBA(170, 70, 70));
    button_delete->onClick = [window, lightGroup, modelId]()
    {
        Menu::AddConfirmWindow(window, 47, [lightGroup, modelId]() {
            ModelInfos::GetModelInfo(WindowMain::m_ModelId)->RemoveLightGroup(lightGroup);

            //idk if this works
            if (ModelInfos::GetModelInfo(WindowMain::m_ModelId)->lightGroups.size() == 0)
            {
                ModelInfos::RemoveModelInfo(WindowMain::m_ModelId);
            }

            //enable lights again
            auto vehicle = Mod::GetPlayerVehicle();
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