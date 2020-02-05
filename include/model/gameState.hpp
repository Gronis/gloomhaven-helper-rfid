#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include <vector>
#include <map>

#include "optional.hpp"
#include "print.hpp"

// Model
#include "model/actor.hpp"
#include "model/monsterAbilityDeck.hpp"
#include "model/attackModifier.hpp"
#include "model/playerInit.hpp"
#include "model/elementState.hpp"

namespace ghr
{
struct GameState
{
    int round;
    int scenario_number;
    int scenario_level;
    bool track_standees;
    bool ability_cards;
    bool random_standees;
    bool elites_first;
    bool expire_conditions;
    bool solo;
    bool hide_stats;
    bool calculate_stats;
    bool can_draw;
    bool needs_shuffle;
    PlayerInit player_init;
    std::vector<AttackModifier> attack_modifiers;
    std::vector<AttackModifier> attack_modifiers_discard;
    tl::optional<AttackModifier> attack_modifier1;
    tl::optional<AttackModifier> attack_modifier2;
    ElementState fire;
    ElementState ice;
    ElementState air;
    ElementState earth;
    ElementState light;
    ElementState dark;
    std::vector<int> removed_abilities;
    int bad_omen;
    std::map<int, MonsterAbilityDeck> ability_decks;
    std::vector<Actor> actors;

    GameState() : round(0),
                  scenario_number(0),
                  scenario_level(0),
                  track_standees(false),
                  ability_cards(false),
                  random_standees(false),
                  elites_first(false),
                  expire_conditions(false),
                  solo(false),
                  hide_stats(false),
                  calculate_stats(false),
                  can_draw(false),
                  needs_shuffle(false),
                  player_init(PlayerInit::value1),
                  attack_modifiers({}),
                  attack_modifiers_discard({}),
                  attack_modifier1(tl::nullopt),
                  attack_modifier2(tl::nullopt),
                  fire(ElementState::Inert),
                  ice(ElementState::Inert),
                  air(ElementState::Inert),
                  earth(ElementState::Inert),
                  light(ElementState::Inert),
                  dark(ElementState::Inert),
                  removed_abilities({}),
                  bad_omen(0),
                  ability_decks(),
                  actors()
    {
    }
    void clear()
    {
        attack_modifiers_discard.clear();
        attack_modifiers.clear();
        removed_abilities.clear();
        actors.clear();
        ability_decks.clear();
    }
};

void print(const GameState &arg)
{
    ghr::print("round: ", arg.round, "\n");
    ghr::print("scen nr: ", arg.scenario_number, "\n");
    ghr::print("scen lvl: ", arg.scenario_level, "\n");
    ghr::print("track standees: ", arg.track_standees, "\n");
    ghr::print("ability cards: ", arg.ability_cards, "\n");
    ghr::print("rand standees: ", arg.random_standees, "\n");
    ghr::print("elite first: ", arg.elites_first, "\n");
    ghr::print("expire cond: ", arg.expire_conditions, "\n");
    ghr::print("solo: ", arg.solo, "\n");
    ghr::print("hide stats: ", arg.hide_stats, "\n");
    ghr::print("calc stats: ", arg.calculate_stats, "\n");
    ghr::print("can draw: ", arg.can_draw, "\n");
    ghr::print("needs shuffle: ", arg.needs_shuffle, "\n");
    ghr::print("player init: ", arg.player_init, "\n");
    for (const auto am : arg.attack_modifiers)
    {
        ghr::print("attack modifier: ", am, "\n");
    }
    for (const auto am : arg.attack_modifiers_discard)
    {
        ghr::print("attack modifier discard: ", am, "\n");
    }
    if (arg.attack_modifier1)
    {
        ghr::print("attack modifier 1: ", arg.attack_modifier1.value(), "\n");
    }
    if (arg.attack_modifier2)
    {
        ghr::print("attack modifier 2: ", arg.attack_modifier2.value(), "\n");
    }
    ghr::print("fire state:  ", arg.fire, "\n");
    ghr::print("ics state:   ", arg.ice, "\n");
    ghr::print("air state:   ", arg.air, "\n");
    ghr::print("earth state: ", arg.earth, "\n");
    ghr::print("light state: ", arg.light, "\n");
    ghr::print("dark state:  ", arg.dark, "\n");
    ghr::print("removed abilities: {");
    for (const auto ab : arg.removed_abilities)
    {
        ghr::print(ab, " ");
    }
    ghr::print("}\n");
    ghr::print("bad omen: ", arg.bad_omen, "\n");
    for (const auto ab : arg.ability_decks)
    {
        const MonsterAbilityDeck deck = ab.second;
        ghr::print("monster ability deck: ", deck);
    }
    for (const auto a : arg.actors)
    {
        ghr::print("actor: ", a);
    }
}

} // namespace ghr

#endif // __GAME_STATE_H__