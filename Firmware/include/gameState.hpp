#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include <vector>

#include "optional.hpp"
#include "print.hpp"

// Model
#include "attackModifier.hpp"
#include "playerInit.hpp"
#include "elementState.hpp"

namespace ghr
{
struct GameState
{
    int round;
    int scenario_number;
    int scenario_level;
    bool track_standees;
    bool random_standees;
    bool elites_first;
    bool expire_conditions;
    bool solo;
    bool hide_stats;
    bool calculate_stats;
    bool can_draw;
    bool needs_shuffle;
    PlayerInit player_init;
    std::vector<AttackModifier> attackModifiers;
    tl::optional<AttackModifier> attackModifier1;
    tl::optional<AttackModifier> attackModifier2;
    ElementState fire;
    ElementState ice;
    ElementState air;
    ElementState earth;
    ElementState light;
    ElementState dark;

    GameState() : round(0),
                  scenario_number(0),
                  scenario_level(0),
                  track_standees(false),
                  random_standees(false),
                  elites_first(false),
                  expire_conditions(false),
                  solo(false),
                  hide_stats(false),
                  calculate_stats(false),
                  can_draw(false),
                  needs_shuffle(false),
                  player_init(PlayerInit::value1),
                  attackModifiers({}),
                  attackModifier1(tl::nullopt),
                  attackModifier2(tl::nullopt),
                  fire(ElementState::INERT),
                  ice(ElementState::INERT),
                  air(ElementState::INERT),
                  earth(ElementState::INERT),
                  light(ElementState::INERT),
                  dark(ElementState::INERT)
    {
    }
};

void print(GameState &arg)
{
    ghr::print("round: ", arg.round, "\n");
    ghr::print("scen nr: ", arg.scenario_number, "\n");
    ghr::print("scen lvl: ", arg.scenario_level, "\n");
    ghr::print("track standees: ", arg.track_standees, "\n");
    ghr::print("rand standees: ", arg.random_standees, "\n");
    ghr::print("elite first: ", arg.elites_first, "\n");
    ghr::print("expire cond: ", arg.expire_conditions, "\n");
    ghr::print("solo: ", arg.solo, "\n");
    ghr::print("hide stats: ", arg.hide_stats, "\n");
    ghr::print("calc stats: ", arg.calculate_stats, "\n");
    ghr::print("can draw: ", arg.can_draw, "\n");
    ghr::print("needs shuffle: ", arg.needs_shuffle, "\n");
    ghr::print("player init: ", arg.player_init, "\n");
    for (auto &&am : arg.attackModifiers)
    {
        ghr::print("attack modifier: ", am, "\n");
    }
    if (arg.attackModifier1)
    {
        ghr::print("attack modifier 1: ", arg.attackModifier1.value(), "\n");
    }
    if (arg.attackModifier2)
    {
        ghr::print("attack modifier 2: ", arg.attackModifier2.value(), "\n");
    }
    ghr::print("fire state:  ", arg.fire, "\n");
    ghr::print("ics state:   ", arg.ice, "\n");
    ghr::print("air state:   ", arg.air, "\n");
    ghr::print("earth state: ", arg.earth, "\n");
    ghr::print("light state: ", arg.light, "\n");
    ghr::print("dark state:  ", arg.dark, "\n");
}

} // namespace ghr

#endif // __GAME_STATE_H__