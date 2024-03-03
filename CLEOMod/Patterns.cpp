#include "Patterns.h"
#include "Log.h"

std::vector<Pattern*> Patterns::m_Patterns;
int Patterns::m_TimeBetweenPatterns = 7000;

Pattern* Patterns::CreatePattern(std::string id)
{
	Pattern* pattern = new Pattern(id);
	m_Patterns.push_back(pattern);

	Log::file << "Patterns: Create pattern" << std::endl;

	return pattern;
}

bool Patterns::HasPattern(std::string id)
{
    return GetPatternById(id) != NULL;
}

Pattern* Patterns::GetPatternById(std::string id)
{
    for (auto pattern : m_Patterns)
    {
        if (pattern->id.compare(id) == 0) return pattern;
    }
    return NULL;
}

void Patterns::CreateDefaultPatterns()
{
    if (!Patterns::HasPattern("1_lights-1"))
    {
        auto PATTERN_1_lights_1 = Patterns::CreatePattern("1_lights-1");
        PATTERN_1_lights_1->AddStep({ 1 }, 300);
        PATTERN_1_lights_1->AddStep({ 0 }, 300);

        /*
        Pattern.setAmountOfLights(1);
        Pattern.addStep(300, [1]);
        Pattern.addStep(300, [0]);
        */
    }

    if (!Patterns::HasPattern("1_lights-2"))
    {
        auto PATTERN_1_lights_2 = Patterns::CreatePattern("1_lights-2");
        PATTERN_1_lights_2->AddStep({ 1 }, 80);
        PATTERN_1_lights_2->AddStep({ 0 }, 80);
        PATTERN_1_lights_2->AddStep({ 1 }, 80);
        PATTERN_1_lights_2->AddStep({ 0 }, 600);

        /*
        Pattern.setAmountOfLights(1);
        Pattern.addStep(80, [1]);
        Pattern.addStep(80, [0]);
        Pattern.addStep(80, [1]);
        Pattern.addStep(600, [0]);
        */
    }

    if (!Patterns::HasPattern("2_lights-1"))
    {
        auto PATTERN_2_lights_1 = Patterns::CreatePattern("2_lights-1");
        PATTERN_2_lights_1->AddStep({ 1, 0 }, 200);
        PATTERN_2_lights_1->AddStep({ 0, 0 }, 200);
        PATTERN_2_lights_1->AddStep({ 0, 1 }, 200);
        PATTERN_2_lights_1->AddStep({ 0, 0 }, 200);

        /*
        Pattern.setAmountOfLights(2);
        Pattern.addStep(200, [1, 0]);
        Pattern.addStep(200, [0, 0]);
        Pattern.addStep(200, [0, 1]);
        Pattern.addStep(200, [0, 0]);
        */
    }
    
    if (!Patterns::HasPattern("2_lights-2"))
    {
        auto PATTERN_2_lights_2 = Patterns::CreatePattern("2_lights-2");
        PATTERN_2_lights_2->AddStep({ 1, 0 }, 100);
        PATTERN_2_lights_2->AddStep({ 0, 0 }, 80);
        PATTERN_2_lights_2->AddStep({ 1, 0 }, 100);
        PATTERN_2_lights_2->AddStep({ 0, 0 }, 80);
        PATTERN_2_lights_2->AddStep({ 1, 0 }, 100);
        PATTERN_2_lights_2->AddStep({ 0, 0 }, 150);
        PATTERN_2_lights_2->AddStep({ 0, 1 }, 100);
        PATTERN_2_lights_2->AddStep({ 0, 0 }, 80);
        PATTERN_2_lights_2->AddStep({ 0, 1 }, 100);
        PATTERN_2_lights_2->AddStep({ 0, 0 }, 80);
        PATTERN_2_lights_2->AddStep({ 0, 1 }, 100);
        PATTERN_2_lights_2->AddStep({ 0, 0 }, 150);

        /*
        Pattern.setAmountOfLights(2);
        Pattern.addStep(100, [1, 0]);
        Pattern.addStep(80, [0, 0]);
        Pattern.addStep(100, [1, 0]);
        Pattern.addStep(80, [0, 0]);
        Pattern.addStep(100, [1, 0]);
        Pattern.addStep(150, [0, 0]);
        Pattern.addStep(100, [0, 1]);
        Pattern.addStep(80, [0, 0]);
        Pattern.addStep(100, [0, 1]);
        Pattern.addStep(80, [0, 0]);
        Pattern.addStep(100, [0, 1]);
        Pattern.addStep(150, [0, 0]);
        */
    }

    if (!Patterns::HasPattern("3_lights-1"))
    {
        auto PATTERN_3_lights_1 = Patterns::CreatePattern("3_lights-1");
        PATTERN_3_lights_1->AddStep({ 1, 0, 1 }, 200);
        PATTERN_3_lights_1->AddStep({ 0, 0, 0 }, 200);
        PATTERN_3_lights_1->AddStep({ 0, 1, 0 }, 200);
        PATTERN_3_lights_1->AddStep({ 0, 0, 0 }, 200);

        /*
        Pattern.setAmountOfLights(3);
        Pattern.addStep(200, [1, 0, 1]);
        Pattern.addStep(200, [0, 0, 0]);
        Pattern.addStep(200, [0, 1, 0]);
        Pattern.addStep(200, [0, 0, 0]);
        */
    }

    if (!Patterns::HasPattern("3_lights-2"))
    {
        auto PATTERN_3_lights_2 = Patterns::CreatePattern("3_lights-2");
        PATTERN_3_lights_2->AddStep({ 1, 0, 0 }, 200);
        PATTERN_3_lights_2->AddStep({ 0, 0, 0 }, 200);
        PATTERN_3_lights_2->AddStep({ 0, 1, 0 }, 200);
        PATTERN_3_lights_2->AddStep({ 0, 0, 0 }, 200);
        PATTERN_3_lights_2->AddStep({ 0, 0, 1 }, 200);
        PATTERN_3_lights_2->AddStep({ 0, 0, 0 }, 200);
        PATTERN_3_lights_2->AddStep({ 0, 1, 0 }, 200);
        PATTERN_3_lights_2->AddStep({ 0, 0, 0 }, 200);

        /*
        Pattern.setAmountOfLights(3);
        Pattern.addStep(200, [1, 0, 0]);
        Pattern.addStep(200, [0, 0, 0]);
        Pattern.addStep(200, [0, 1, 0]);
        Pattern.addStep(200, [0, 0, 0]);
        Pattern.addStep(200, [0, 0, 1]);
        Pattern.addStep(200, [0, 0, 0]);
        Pattern.addStep(200, [0, 1, 0]);
        Pattern.addStep(200, [0, 0, 0]);
        */
    }
   
    if (!Patterns::HasPattern("4_lights-1"))
    {
        auto PATTERN_4_lights_1 = Patterns::CreatePattern("4_lights-1");
        PATTERN_4_lights_1->AddStep({ 1, 1, 0, 0 }, 200);
        PATTERN_4_lights_1->AddStep({ 0, 0, 0, 0 }, 200);
        PATTERN_4_lights_1->AddStep({ 0, 0, 1, 1 }, 200);
        PATTERN_4_lights_1->AddStep({ 0, 0, 0, 0 }, 200);

        /*
        Pattern.setAmountOfLights(4);
        Pattern.addStep(200, [1, 1, 0, 0]);
        Pattern.addStep(200, [0, 0, 0, 0]);
        Pattern.addStep(200, [0, 0, 1, 1]);
        Pattern.addStep(200, [0, 0, 0, 0]);
        */
    }

    if (!Patterns::HasPattern("4_lights-2"))
    {
        auto PATTERN_4_lights_2 = Patterns::CreatePattern("4_lights-2");
        PATTERN_4_lights_2->AddStep({ 1, 1, 0, 0 }, 200);
        PATTERN_4_lights_2->AddStep({ 0, 0, 0, 0 }, 200);
        PATTERN_4_lights_2->AddStep({ 0, 0, 1, 1 }, 200);
        PATTERN_4_lights_2->AddStep({ 0, 0, 0, 0 }, 200);
        PATTERN_4_lights_2->AddStep({ 1, 1, 1, 1 }, 200);
        PATTERN_4_lights_2->AddStep({ 0, 0, 0, 0 }, 200);
        PATTERN_4_lights_2->AddStep({ 1, 1, 1, 1 }, 200);
        PATTERN_4_lights_2->AddStep({ 0, 0, 0, 0 }, 200);

        /*
        Pattern.setAmountOfLights(4);
        Pattern.addStep(200, [1, 1, 0, 0]);
        Pattern.addStep(200, [0, 0, 0, 0]);
        Pattern.addStep(200, [0, 0, 1, 1]);
        Pattern.addStep(200, [0, 0, 0, 0]);
        Pattern.addStep(200, [1, 1, 1, 1]);
        Pattern.addStep(200, [0, 0, 0, 0]);
        Pattern.addStep(200, [1, 1, 1, 1]);
        Pattern.addStep(200, [0, 0, 0, 0]);
        */
    }

    if (!Patterns::HasPattern("5_lights-1"))
    {
        auto PATTERN_5_lights_1 = Patterns::CreatePattern("5_lights-1");
        PATTERN_5_lights_1->AddStep({ 0, 0, 1, 0, 0 }, 100);
        PATTERN_5_lights_1->AddStep({ 0, 0, 0, 0, 0 }, 100);
        PATTERN_5_lights_1->AddStep({ 0, 0, 0, 1, 1 }, 100);
        PATTERN_5_lights_1->AddStep({ 0, 0, 0, 0, 0 }, 100);
        PATTERN_5_lights_1->AddStep({ 0, 0, 0, 1, 1 }, 100);
        PATTERN_5_lights_1->AddStep({ 0, 0, 0, 0, 0 }, 100);
        PATTERN_5_lights_1->AddStep({ 1, 1, 0, 0, 0 }, 100);
        PATTERN_5_lights_1->AddStep({ 0, 0, 0, 0, 0 }, 100);
        PATTERN_5_lights_1->AddStep({ 1, 1, 0, 0, 0 }, 100);
        PATTERN_5_lights_1->AddStep({ 0, 0, 0, 0, 0 }, 100);

        /*
        Pattern.setAmountOfLights(5);
        Pattern.addStep(100, [0, 0, 1, 0, 0]);
        Pattern.addStep(100, [0, 0, 0, 0, 0]);
        Pattern.addStep(100, [0, 0, 0, 1, 1]);
        Pattern.addStep(100, [0, 0, 0, 0, 0]);
        Pattern.addStep(100, [0, 0, 0, 1, 1]);
        Pattern.addStep(100, [0, 0, 0, 0, 0]);
        Pattern.addStep(100, [1, 1, 0, 0, 0]);
        Pattern.addStep(100, [0, 0, 0, 0, 0]);
        Pattern.addStep(100, [1, 1, 0, 0, 0]);
        Pattern.addStep(100, [0, 0, 0, 0, 0]);
        */
    }

    if (!Patterns::HasPattern("5_lights-2"))
    {
        auto PATTERN_5_lights_2 = Patterns::CreatePattern("5_lights-2");
        PATTERN_5_lights_2->AddStep({ 1, 1, 0, 1, 1 }, 200);
        PATTERN_5_lights_2->AddStep({ 0, 0, 0, 0, 0 }, 100);
        PATTERN_5_lights_2->AddStep({ 0, 0, 1, 0, 0 }, 200);
        PATTERN_5_lights_2->AddStep({ 0, 0, 0, 0, 0 }, 100);

        /*
        Pattern.setAmountOfLights(5);
        Pattern.addStep(200, [1, 1, 0, 1, 1]);
        Pattern.addStep(100, [0, 0, 0, 0, 0]);
        Pattern.addStep(200, [0, 0, 1, 0, 0]);
        Pattern.addStep(100, [0, 0, 0, 0, 0]);
        */
    }

    if (!Patterns::HasPattern("5_lights-3"))
    {
        auto PATTERN_5_lights_3 = Patterns::CreatePattern("5_lights-3");
        PATTERN_5_lights_3->AddStep({ 1, 1, 0, 0, 0 }, 200);
        PATTERN_5_lights_3->AddStep({ 0, 0, 0, 0, 0 }, 100);
        PATTERN_5_lights_3->AddStep({ 0, 0, 1, 0, 0 }, 200);
        PATTERN_5_lights_3->AddStep({ 0, 0, 0, 0, 0 }, 100);
        PATTERN_5_lights_3->AddStep({ 0, 0, 0, 1, 1 }, 200);
        PATTERN_5_lights_3->AddStep({ 0, 0, 0, 0, 0 }, 100);
        PATTERN_5_lights_3->AddStep({ 0, 0, 1, 0, 0 }, 200);
        PATTERN_5_lights_3->AddStep({ 0, 0, 0, 0, 0 }, 200);

        /*
        Pattern.setAmountOfLights(5);
        Pattern.addStep(200, [1, 1, 0, 0, 0]);
        Pattern.addStep(100, [0, 0, 0, 0, 0]);
        Pattern.addStep(200, [0, 0, 1, 0, 0]);
        Pattern.addStep(100, [0, 0, 0, 0, 0]);
        Pattern.addStep(200, [0, 0, 0, 1, 1]);
        Pattern.addStep(100, [0, 0, 0, 0, 0]);
        Pattern.addStep(200, [0, 0, 1, 0, 0]);
        Pattern.addStep(200, [0, 0, 0, 0, 0]);
        */
    }

    if (!Patterns::HasPattern("5_lights-4"))
    {
        auto pattern8 = Patterns::CreatePattern("5_lights-4");
        pattern8->AddStep({ 1, 0, 0, 0, 1 }, 100);
        pattern8->AddStep({ 0, 1, 0, 1, 0 }, 100);
        pattern8->AddStep({ 0, 0, 1, 0, 0 }, 100);
        pattern8->AddStep({ 0, 1, 0, 1, 0 }, 100);
        pattern8->AddStep({ 1, 0, 0, 0, 1 }, 100);
        pattern8->AddStep({ 0, 0, 0, 0, 0 }, 200);

        /*
        * 
        */
    }

    if (!Patterns::HasPattern("6_lights-1"))
    {
        auto PATTERN_6_lights_1 = Patterns::CreatePattern("6_lights-1");
        PATTERN_6_lights_1->AddStep({ 1, 1, 1, 0, 0, 0 }, 200);
        PATTERN_6_lights_1->AddStep({ 0, 0, 0, 0, 0, 0 }, 200);
        PATTERN_6_lights_1->AddStep({ 0, 0, 0, 1, 1, 1 }, 200);
        PATTERN_6_lights_1->AddStep({ 0, 0, 0, 0, 0, 0 }, 200);

        /*
        Pattern.setAmountOfLights(6);
        Pattern.addStep(200, [1, 1, 1, 0, 0, 0]);
        Pattern.addStep(200, [0, 0, 0, 0, 0, 0]);
        Pattern.addStep(200, [0, 0, 0, 1, 1, 1]);
        Pattern.addStep(200, [0, 0, 0, 0, 0, 0]);
        */
    }

    if (!Patterns::HasPattern("6_lights-2"))
    {
        auto PATTERN_6_lights_2 = Patterns::CreatePattern("6_lights-2");
        PATTERN_6_lights_2->AddStep({ 0, 0, 1, 1, 0, 0 }, 100);
        PATTERN_6_lights_2->AddStep({ 0, 0, 0, 0, 0, 0 }, 100);
        PATTERN_6_lights_2->AddStep({ 1, 1, 0, 0, 1, 1 }, 100);
        PATTERN_6_lights_2->AddStep({ 0, 0, 0, 0, 0, 0 }, 100);
        PATTERN_6_lights_2->AddStep({ 1, 1, 0, 0, 1, 1 }, 100);
        PATTERN_6_lights_2->AddStep({ 0, 0, 0, 0, 0, 0 }, 100);
        PATTERN_6_lights_2->AddStep({ 0, 0, 1, 1, 0, 0 }, 100);
        PATTERN_6_lights_2->AddStep({ 0, 0, 0, 0, 0, 0 }, 100);

        /*
        Pattern.setAmountOfLights(6);
        Pattern.addStep(100, [0, 0, 1, 1, 0, 0]);
        Pattern.addStep(100, [0, 0, 0, 0, 0, 0]);
        Pattern.addStep(100, [1, 1, 0, 0, 1, 1]);
        Pattern.addStep(100, [0, 0, 0, 0, 0, 0]);
        Pattern.addStep(100, [1, 1, 0, 0, 1, 1]);
        Pattern.addStep(100, [0, 0, 0, 0, 0, 0]);
        Pattern.addStep(100, [0, 0, 1, 1, 0, 0]);
        Pattern.addStep(100, [0, 0, 0, 0, 0, 0]);
        */
    }

    if (!Patterns::HasPattern("8_lights-1"))
    {
        auto pattern_8_lights_1 = Patterns::CreatePattern("8_lights-1");
        pattern_8_lights_1->AddStep({ 1, 1, 1, 1, 0, 0, 0, 0 }, 200);
        pattern_8_lights_1->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0 }, 200);
        pattern_8_lights_1->AddStep({ 0, 0, 0, 0, 1, 1, 1, 1 }, 200);
        pattern_8_lights_1->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0 }, 200);
        pattern_8_lights_1->AddStep({ 1, 1, 1, 1, 1, 1, 1, 1 }, 200);
        pattern_8_lights_1->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0 }, 200);

        /*
        [Pattern]
        11110000|200
        00000000|200
        00001111|200
        00000000|200
        11111111|200
        00000000|200
        */
    }

    if (!Patterns::HasPattern("8_lights-2"))
    {
        auto pattern_8_lights_2 = Patterns::CreatePattern("8_lights-2");
        pattern_8_lights_2->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0 }, 100);
        pattern_8_lights_2->AddStep({ 1, 0, 0, 0, 0, 0, 0, 0 }, 100);
        pattern_8_lights_2->AddStep({ 1, 1, 0, 0, 0, 0, 0, 0 }, 100);
        pattern_8_lights_2->AddStep({ 0, 1, 1, 0, 0, 0, 0, 0 }, 100);
        pattern_8_lights_2->AddStep({ 0, 0, 1, 1, 0, 0, 0, 0 }, 100);
        pattern_8_lights_2->AddStep({ 0, 1, 1, 0, 0, 0, 0, 0 }, 100);
        pattern_8_lights_2->AddStep({ 1, 1, 0, 0, 0, 0, 0, 0 }, 100);
        pattern_8_lights_2->AddStep({ 1, 0, 0, 0, 0, 0, 0, 0 }, 100);
        pattern_8_lights_2->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0 }, 50);
        pattern_8_lights_2->AddStep({ 0, 0, 0, 0, 0, 0, 0, 1 }, 100);
        pattern_8_lights_2->AddStep({ 0, 0, 0, 0, 0, 0, 1, 1 }, 100);
        pattern_8_lights_2->AddStep({ 0, 0, 0, 0, 0, 1, 1, 0 }, 100);
        pattern_8_lights_2->AddStep({ 0, 0, 0, 0, 1, 1, 0, 0 }, 100);
        pattern_8_lights_2->AddStep({ 0, 0, 0, 0, 0, 1, 1, 0 }, 100);
        pattern_8_lights_2->AddStep({ 0, 0, 0, 0, 0, 0, 1, 1 }, 100);
        pattern_8_lights_2->AddStep({ 0, 0, 0, 0, 0, 0, 0, 1 }, 100);

        /*
        [Pattern]
        00000000|100
        10000000|100
        11000000|100
        01100000|100
        00110000|100
        01100000|100
        11000000|100
        10000000|100
        00000000|50
        00000001|100
        00000011|100
        00000110|100
        00001100|100
        00000110|100
        00000011|100
        00000001|100
        */
    }

    if (!Patterns::HasPattern("10_lights-1"))
    {
        auto pattern_10lights_1 = Patterns::CreatePattern("10_lights-1");
        pattern_10lights_1->AddStep({ 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 }, 100);
        pattern_10lights_1->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 100);
        pattern_10lights_1->AddStep({ 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 }, 100);
        pattern_10lights_1->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 100);
        pattern_10lights_1->AddStep({ 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 }, 100);
        pattern_10lights_1->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 200);
        pattern_10lights_1->AddStep({ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1 }, 100);
        pattern_10lights_1->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 100);
        pattern_10lights_1->AddStep({ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1 }, 100);
        pattern_10lights_1->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 100);
        pattern_10lights_1->AddStep({ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1 }, 100);
        pattern_10lights_1->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 200);

        /*
        */
    }

    if (!Patterns::HasPattern("10_lights-2"))
    {
        auto pattern_10lights_2 = Patterns::CreatePattern("10_lights-2");
        pattern_10lights_2->AddStep({ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 100);
        pattern_10lights_2->AddStep({ 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 }, 100);
        pattern_10lights_2->AddStep({ 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 }, 100);
        pattern_10lights_2->AddStep({ 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 }, 100);
        pattern_10lights_2->AddStep({ 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 }, 100);
        pattern_10lights_2->AddStep({ 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 }, 100);
        pattern_10lights_2->AddStep({ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 100);
        pattern_10lights_2->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 60);
        pattern_10lights_2->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }, 100);
        pattern_10lights_2->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 }, 100);
        pattern_10lights_2->AddStep({ 0, 0, 0, 0, 0, 0, 0, 1, 1, 1 }, 100);
        pattern_10lights_2->AddStep({ 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 }, 100);
        pattern_10lights_2->AddStep({ 0, 0, 0, 0, 0, 0, 0, 1, 1, 1 }, 100);
        pattern_10lights_2->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 }, 100);
        pattern_10lights_2->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }, 100);
        pattern_10lights_2->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 100);

        /*
        */
    }

    if (!Patterns::HasPattern("10_lights-3"))
    {
        auto pattern_10lights_3 = Patterns::CreatePattern("10_lights-3");
        pattern_10lights_3->AddStep({ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, 150);
        pattern_10lights_3->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 150);

        /*
        */
    }
}