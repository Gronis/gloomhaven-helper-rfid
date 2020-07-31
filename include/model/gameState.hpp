#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include <map>
#include <vector>

#include "optional.hpp"
#include "print.hpp"

// Model
#include "model/actor.hpp"
#include "model/attackModifier.hpp"
#include "model/elementState.hpp"
#include "model/monsterAbilityDeck.hpp"
#include "model/playerInit.hpp"

namespace ghh {
struct GameState {
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
  bool jotl;
  std::map<int, MonsterAbilityDeck> ability_decks;
  std::vector<Actor> actors;

  GameState()
      : round(0),
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
        jotl(false),
        ability_decks(),
        actors() {}
  inline void clear() {
    attack_modifiers_discard.clear();
    attack_modifiers.clear();
    removed_abilities.clear();
    actors.clear();
    ability_decks.clear();
  }
};

inline void print(const GameState &arg) {
  ghh::print("round: ", arg.round, "\n");
  ghh::print("scen nr: ", arg.scenario_number, "\n");
  ghh::print("scen lvl: ", arg.scenario_level, "\n");
  ghh::print("track standees: ", arg.track_standees, "\n");
  ghh::print("ability cards: ", arg.ability_cards, "\n");
  ghh::print("rand standees: ", arg.random_standees, "\n");
  ghh::print("elite first: ", arg.elites_first, "\n");
  ghh::print("expire cond: ", arg.expire_conditions, "\n");
  ghh::print("solo: ", arg.solo, "\n");
  ghh::print("hide stats: ", arg.hide_stats, "\n");
  ghh::print("calc stats: ", arg.calculate_stats, "\n");
  ghh::print("can draw: ", arg.can_draw, "\n");
  ghh::print("needs shuffle: ", arg.needs_shuffle, "\n");
  ghh::print("player init: ", arg.player_init, "\n");
  for (const auto am : arg.attack_modifiers) {
    ghh::print("attack modifier: ", am, "\n");
  }
  for (const auto am : arg.attack_modifiers_discard) {
    ghh::print("attack modifier discard: ", am, "\n");
  }
  if (arg.attack_modifier1) {
    ghh::print("attack modifier 1: ", arg.attack_modifier1.value(), "\n");
  }
  if (arg.attack_modifier2) {
    ghh::print("attack modifier 2: ", arg.attack_modifier2.value(), "\n");
  }
  ghh::print("fire state:  ", arg.fire, "\n");
  ghh::print("ics state:   ", arg.ice, "\n");
  ghh::print("air state:   ", arg.air, "\n");
  ghh::print("earth state: ", arg.earth, "\n");
  ghh::print("light state: ", arg.light, "\n");
  ghh::print("dark state:  ", arg.dark, "\n");
  ghh::print("removed abilities: {");
  for (const auto ab : arg.removed_abilities) {
    ghh::print(ab, " ");
  }
  ghh::print("}\n");
  ghh::print("bad omen: ", arg.bad_omen, "\n");
  ghh::print("jotl: ", arg.jotl, "\n");
  for (const auto ab : arg.ability_decks) {
    const MonsterAbilityDeck deck = ab.second;
    ghh::print("monster ability deck: ", deck);
  }
  for (const auto a : arg.actors) {
    ghh::print("actor: ", a);
  }
}

}  // namespace ghh

#endif  // __GAME_STATE_H__