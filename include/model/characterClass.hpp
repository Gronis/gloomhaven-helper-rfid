#ifndef __CHARACTER_CLASS_H__
#define __CHARACTER_CLASS_H__

#include "print.hpp"

namespace ghh {

struct CharacterClassNS {
  enum Value {
    Escort,
    Objective,
    Brute,
    Cragheart,
    Mindtheif,
    Scoundrel,
    Spellweaver,
    Tinkerer,
    Diviner,
    // Diviner was added on version 8, so these are incorrect, for lower
    // versions
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
    value20,
    Demolitionist,
    RedGuard,
    Voidwarden,
    Hatchet,
    value25,
  };
};

using CharacterClass = CharacterClassNS::Value;

inline std::vector<CharacterClass> getCharacterClassValues() {
  return {
      CharacterClass::Escort,      CharacterClass::Objective,
      CharacterClass::Brute,       CharacterClass::Cragheart,
      CharacterClass::Mindtheif,   CharacterClass::Scoundrel,
      CharacterClass::Spellweaver, CharacterClass::Tinkerer,
      CharacterClass::Diviner,     CharacterClass::TwoMinis,
      CharacterClass::Lightning,   CharacterClass::AngryFace,
      CharacterClass::Triangles,   CharacterClass::Moon,
      CharacterClass::ChuluFace,   CharacterClass::TrippleArrow,
      CharacterClass::Saw,         CharacterClass::MusicNote,
      CharacterClass::Circles,     CharacterClass::Sun,
      CharacterClass::value20,     CharacterClass::Demolitionist,
      CharacterClass::RedGuard,    CharacterClass::Voidwarden,
      CharacterClass::Hatchet,     CharacterClass::value25,
  };
}

inline void print(const CharacterClass& arg) {
  switch (arg) {
    case CharacterClass::Escort:
      ghh::print("Escort");
      break;
    case CharacterClass::Objective:
      ghh::print("Objective");
      break;
    case CharacterClass::Brute:
      ghh::print("Brute");
      break;
    case CharacterClass::Cragheart:
      ghh::print("Cragheart");
      break;
    case CharacterClass::Mindtheif:
      ghh::print("Mindtheif");
      break;
    case CharacterClass::Scoundrel:
      ghh::print("Scoundrel");
      break;
    case CharacterClass::Spellweaver:
      ghh::print("Spellweaver");
      break;
    case CharacterClass::Tinkerer:
      ghh::print("Tinkerer");
      break;
    case CharacterClass::Diviner:
      ghh::print("Diviner");
      break;
    case CharacterClass::TwoMinis:
      ghh::print("TwoMinis");
      break;
    case CharacterClass::Lightning:
      ghh::print("Lightning");
      break;
    case CharacterClass::AngryFace:
      ghh::print("AngryFace");
      break;
    case CharacterClass::Triangles:
      ghh::print("Triangles");
      break;
    case CharacterClass::Moon:
      ghh::print("Moon");
      break;
    case CharacterClass::ChuluFace:
      ghh::print("ChuluFace");
      break;
    case CharacterClass::TrippleArrow:
      ghh::print("TrippleArrow");
      break;
    case CharacterClass::Saw:
      ghh::print("Saw");
      break;
    case CharacterClass::MusicNote:
      ghh::print("MusicNote");
      break;
    case CharacterClass::Circles:
      ghh::print("Circles");
      break;
    case CharacterClass::Sun:
      ghh::print("Sun");
      break;
    case CharacterClass::Demolitionist:
      ghh::print("Demolitionist");
      break;
    case CharacterClass::RedGuard:
      ghh::print("RedGuard");
      break;
    case CharacterClass::Voidwarden:
      ghh::print("Voidwarden");
      break;
    case CharacterClass::Hatchet:
      ghh::print("Hatchet");
      break;
    default:
      ghh::print("Unknown (", (int)arg, ")");
      break;
  }
}

}  // namespace ghh

#endif  // __CHARACTER_CLASS_H__