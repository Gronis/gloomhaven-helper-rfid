#ifndef __MONSTER_ABILITY_DECK_H__
#define __MONSTER_ABILITY_DECK_H__

#include <vector>

#include "print.hpp"

namespace ghr
{
struct MonsterAbilityDeck
{
    int id;
    std::vector<int> abilities;
    bool shown;
    bool was_shown;
    MonsterAbilityDeck(int id) : id(id),
                                 abilities({}),
                                 shown(false),
                                 was_shown(false)
    {
    }
    MonsterAbilityDeck() : MonsterAbilityDeck(0)
    {
    }
};

void print(MonsterAbilityDeck &arg)
{
    print("id: ", arg.id, " { ");
    for (auto a : arg.abilities)
    {
        print(a, " ");
    }
    print("}\n");
}

} // namespace ghr

#endif // __MONSTER_ABILITY_DECK_H__