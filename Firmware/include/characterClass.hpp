#ifndef __CHARACTER_CLASS_H__
#define __CHARACTER_CLASS_H__

#include "print.hpp"

namespace ghr
{
enum class CharacterClass
{
    Escort,
    Objective,
    Brute,
    Cragheart,
    Mindtheif,
    Scoundrel,
    Spellweaver,
    Tinkerer,
    TwoMinis,
    Lightning,
    AngryFace,
    Triangles,
    Moon,
    ChuluFace,
    TrippleArrow,
    Saw,
    MusicNote,
    Circles,
    Sun,
    value20
};
std::vector<CharacterClass> getCharacterClassValues()
{
    return {
        CharacterClass::Escort,
        CharacterClass::Objective,
        CharacterClass::Brute,
        CharacterClass::Cragheart,
        CharacterClass::Mindtheif,
        CharacterClass::Scoundrel,
        CharacterClass::Spellweaver,
        CharacterClass::Tinkerer,
        CharacterClass::TwoMinis,
        CharacterClass::Lightning,
        CharacterClass::AngryFace,
        CharacterClass::Triangles,
        CharacterClass::Moon,
        CharacterClass::ChuluFace,
        CharacterClass::TrippleArrow,
        CharacterClass::Saw,
        CharacterClass::MusicNote,
        CharacterClass::Circles,
        CharacterClass::Sun,
        CharacterClass::value20};
}

void print(CharacterClass &arg)
{
    switch (arg)
    {
    case CharacterClass::Escort:
        ghr::print("Escort");
        break;
    case CharacterClass::Objective:
        ghr::print("Objective");
        break;
    case CharacterClass::Brute:
        ghr::print("Brute");
        break;
    case CharacterClass::Cragheart:
        ghr::print("Cragheart");
        break;
    case CharacterClass::Mindtheif:
        ghr::print("Mindtheif");
        break;
    case CharacterClass::Scoundrel:
        ghr::print("Scoundrel");
        break;
    case CharacterClass::Spellweaver:
        ghr::print("Spellweaver");
        break;
    case CharacterClass::Tinkerer:
        ghr::print("Tinkerer");
        break;
    case CharacterClass::TwoMinis:
        ghr::print("TwoMinis");
        break;
    case CharacterClass::Lightning:
        ghr::print("Lightning");
        break;
    case CharacterClass::AngryFace:
        ghr::print("AngryFace");
        break;
    case CharacterClass::Triangles:
        ghr::print("Triangles");
        break;
    case CharacterClass::Moon:
        ghr::print("Moon");
        break;
    case CharacterClass::ChuluFace:
        ghr::print("ChuluFace");
        break;
    case CharacterClass::TrippleArrow:
        ghr::print("TrippleArrow");
        break;
    case CharacterClass::Saw:
        ghr::print("Saw");
        break;
    case CharacterClass::MusicNote:
        ghr::print("MusicNote");
        break;
    case CharacterClass::Circles:
        ghr::print("Circles");
        break;
    case CharacterClass::Sun:
        ghr::print("Sun");
        break;
    case CharacterClass::value20:
        ghr::print("value20");
        break;
    }
}

} // namespace ghr

#endif // __CHARACTER_CLASS_H__