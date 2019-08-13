#ifndef __MONSTER_ABILITY_DECK_H__
#define __MONSTER_ABILITY_DECK_H__

#include <vector>

#include "print.hpp"
#include "optional.hpp"

namespace ghr
{
struct MonsterAbilityDeck
{
    int id;
    bool shuffle;
    tl::optional<int> shown_ability;
    std::vector<int> abilities;
    std::vector<int> abilities_discard;
    bool shown;
    bool was_shown;
    MonsterAbilityDeck(int id) : id(id),
                                 shuffle(false),
                                 shown_ability(tl::nullopt),
                                 abilities({}),
                                 abilities_discard({}),
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
    print("id: ", arg.id, "\n");
    print("shuffle: ", arg.shuffle, "\n");
    print("shown ability: ", arg.shown_ability ? arg.shown_ability.value() : -1, "\n");
    print("abilties: { ");
    for (auto a : arg.abilities)
    {
        print(a, " ");
    }
    print("}\n");

    print("abilties discard: { ");
    for (auto a : arg.abilities_discard)
    {
        print(a, " ");
    }
    print("}\n");
}

} // namespace ghr

#endif // __MONSTER_ABILITY_DECK_H__