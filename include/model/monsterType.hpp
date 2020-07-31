#ifndef __MONSTER_TYPE_H__
#define __MONSTER_TYPE_H__

#include <vector>

#include "print.hpp"

namespace ghh {
struct MonsterTypeNS {
  enum Value {
    Normal,
    Elite,
    Boss,
    Summon,
    value5,
    value6,
    value7,
    value8,
    value9,
    value10,
    value11,
    value12,
    value13,
    value14,
    value15,
    value16
  };
};

using MonsterType = MonsterTypeNS::Value;

inline std::vector<MonsterType> getMonsterTypeValues() {
  return {MonsterType::Normal,  MonsterType::Elite,   MonsterType::Boss,
          MonsterType::Summon,  MonsterType::value5,  MonsterType::value6,
          MonsterType::value7,  MonsterType::value8,  MonsterType::value9,
          MonsterType::value10, MonsterType::value11, MonsterType::value12,
          MonsterType::value13, MonsterType::value14, MonsterType::value15,
          MonsterType::value16};
}

inline void print(const MonsterType &arg) {
  switch (arg) {
    case MonsterType::Normal:
      ghh::print("Normal");
      break;
    case MonsterType::Elite:
      ghh::print("Elite");
      break;
    case MonsterType::Boss:
      ghh::print("Boss");
      break;
    case MonsterType::Summon:
      ghh::print("Summon");
      break;
    case MonsterType::value5:
      ghh::print("value5");
      break;
    case MonsterType::value6:
      ghh::print("value6");
      break;
    case MonsterType::value7:
      ghh::print("value7");
      break;
    case MonsterType::value8:
      ghh::print("value8");
      break;
    case MonsterType::value9:
      ghh::print("value9");
      break;
    case MonsterType::value10:
      ghh::print("value10");
      break;
    case MonsterType::value11:
      ghh::print("value11");
      break;
    case MonsterType::value12:
      ghh::print("value12");
      break;
    case MonsterType::value13:
      ghh::print("value13");
      break;
    case MonsterType::value14:
      ghh::print("value14");
      break;
    case MonsterType::value15:
      ghh::print("value15");
      break;
    case MonsterType::value16:
      ghh::print("value16");
      break;
  }
}

}  // namespace ghh
#endif  // __MONSTER_TYPE_H__