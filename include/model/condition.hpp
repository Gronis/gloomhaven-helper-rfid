#ifndef __CONDITION_H__
#define __CONDITION_H__

#include <vector>

#include "print.hpp"

namespace ghh {
struct ConditionNS {
  enum Value {
    Star,
    Summon,
    NotSummon,
    Stuned,
    Immobilized,
    Disarmed,
    Wounded,
    Muddle,
    Poisoned,
    Strengthen,
    Invisible,
    Regenerate,
    value13,
    value14,
    value15,
    value16,
    value17,
    value18,
    value19
  };
};

using Condition = ConditionNS::Value;

inline std::vector<Condition> getConditionValues() {
  return {Condition::Star,       Condition::Summon,      Condition::NotSummon,
          Condition::Stuned,     Condition::Immobilized, Condition::Disarmed,
          Condition::Wounded,    Condition::Muddle,      Condition::Poisoned,
          Condition::Strengthen, Condition::Invisible,   Condition::Regenerate,
          Condition::value13,    Condition::value14,     Condition::value15,
          Condition::value16,    Condition::value17,     Condition::value18,
          Condition::value19};
}

inline void print(const Condition &arg) {
  switch (arg) {
    case Condition::Star:
      ghh::print("Star");
      break;
    case Condition::Summon:
      ghh::print("Summon");
      break;
    case Condition::NotSummon:
      ghh::print("NotSummon");
      break;
    case Condition::Stuned:
      ghh::print("Stuned");
      break;
    case Condition::Immobilized:
      ghh::print("Immobilized");
      break;
    case Condition::Disarmed:
      ghh::print("Disarmed");
      break;
    case Condition::Wounded:
      ghh::print("Wounded");
      break;
    case Condition::Muddle:
      ghh::print("Muddle");
      break;
    case Condition::Poisoned:
      ghh::print("Poisoned");
      break;
    case Condition::Strengthen:
      ghh::print("Strengthen");
      break;
    case Condition::Invisible:
      ghh::print("Invisible");
      break;
    case Condition::Regenerate:
      ghh::print("Regenerate");
      break;
    case Condition::value13:
      ghh::print("value13");
      break;
    case Condition::value14:
      ghh::print("value14");
      break;
    case Condition::value15:
      ghh::print("value15");
      break;
    case Condition::value16:
      ghh::print("value16");
      break;
    case Condition::value17:
      ghh::print("value17");
      break;
    case Condition::value18:
      ghh::print("value18");
      break;
    case Condition::value19:
      ghh::print("value19");
      break;
  }
}

}  // namespace ghh

#endif  // __CONDITION_H__