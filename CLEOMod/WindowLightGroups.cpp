#include "WindowLightGroups.h"

#include "WindowMain.h"

#include "ModelInfos.h"
#include "LightGroupDatas.h"

void WindowLightGroups::Create(Window* parent)
{
    auto modelId = WindowMain::m_ModelId;

    auto window = Menu::AddWindow(15, parent);
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

        window->RemoveThisWindow();
        Create(parent);
    };

    if (ModelInfos::HasModelInfo(modelId))
    {
        int i = 0;
        for (auto lightGroup : ModelInfos::GetModelInfo(modelId)->lightGroups)
        {
            auto button_add = window->AddButton(12, { 0, 255, 0, 255 });
            button_add->text->num1 = i;
            button_add->onClick = [window, lightGroup]() {
                CreateEditLightGroup(window, lightGroup);
            };

            i++;
        }
    }


}




void WindowLightGroups::CreateEditLightGroup(Window* parent, LightGroup* lightGroup)
{
    auto modelId = WindowMain::m_ModelId;

    auto window = Menu::AddWindow(15, parent);
    window->showPageControls = true;
    window->btnBack->onClick = [window]()
    {
        window->GoToPrevWindow();
    };

    auto option_giroflex = window->AddOptions(8);
    option_giroflex->optionsValue = (int)lightGroup->type;
    option_giroflex->AddOption(1, 0, 0);
    option_giroflex->AddOption(1, 1, 0);
    option_giroflex->AddOption(1, 2, 0);
    option_giroflex->onValueChange = [option_giroflex, lightGroup]() {
        lightGroup->type = (eLightGroupType)option_giroflex->optionsValue;
        LightGroupDatas::DeleteLightGroupRerefences(lightGroup);
        lightGroup->MakeLightGroup();
    };

    auto distance = window->AddFloatRange(27, &lightGroup->distance, 0.0f, 10.0f, 0.01f);
    distance->onValueChange = [lightGroup]() {
        lightGroup->MakeLightGroup();
    };

    auto button_position = window->AddButton(26, { 0, 20, 120, 255 });
    button_position->onClick = [lightGroup, window]() {
        Menu::AddPositionWindow(window, &lightGroup->offset);
    };

    auto button_color1 = window->AddButton(9, { 0, 20, 120, 255 });
    button_color1->onClick = [lightGroup, window]() {
        Menu::AddColorMenu(window, &lightGroup->color1);
    };

    auto button_color2 = window->AddButton(10, { 0, 20, 120, 255 });
    button_color2->onClick = [lightGroup, window]() {
        Menu::AddColorMenu(window, &lightGroup->color2);
    };

    auto button_color3 = window->AddButton(11, { 0, 20, 120, 255 });
    button_color3->onClick = [lightGroup, window]() {
        Menu::AddColorMenu(window, &lightGroup->color3);
    };

    /*
    option_pos_z->optionMin = -1000;
    option_pos_z->optionCurrent = (int)round(lightGroup->offset.z * 100);
    option_pos_z->optionMax = 1000;
    option_pos_z->holdToChange = true;
    option_pos_z->onValueChange = [option_pos_z, lightGroup]()
    {
        lightGroup->offset.z = ((float)option_pos_z->optionCurrent) / 100.0f;

        Menu::ShowPopup(1, option_pos_z->optionCurrent, 0, 500);
    };
    */
}