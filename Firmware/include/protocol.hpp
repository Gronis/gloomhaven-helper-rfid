#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "summonColor.hpp"
#include "message.hpp"
#include "condition.hpp"
#include "characterClass.hpp"
#include "monsterActor.hpp"
#include "monsterType.hpp"
#include "monsterInstance.hpp"
#include "playerActor.hpp"
#include "gameState.hpp"

namespace ghr
{

void __parseCommonActor(Message &msg, Actor &actor)
{
    print("parsing common actor\n");
    actor.turn_completed = msg.readBoolean();
    int nn = msg.readInt(true);
    print("nr monsters:", nn, "\n");
    for (int ii = 0; ii < nn; ii++)
    {
        MonsterInstance monster;

        monster.number = msg.readInt(true);
        monster.type = msg.readEnum(getMonsterTypeValues());
        print("Monster type: ", monster.type, "\n");
        if (monster.type == MonsterType::Summon)
        {
            monster.summon_color = msg.readEnum(getSummonColorValues());
            monster.summon_move = msg.readInt(true);
            monster.summon_attack = msg.readInt(true);
            monster.summon_range = msg.readInt(true);
        }
        monster.is_new = msg.readBoolean();
        monster.hp = msg.readInt(true);
        monster.hp_max = msg.readInt(true);
        msg.readEnumArray(monster.conditions, getConditionValues());
        msg.readEnumArray(monster.conditions_expired, getConditionValues());
        msg.readEnumArray(monster.conditions_current_turn, getConditionValues());

        print("monster:\n");
        print(monster);
        actor.instances.push_back(monster);

        // auto monsterActor = actor.getMonster();
        // if (monsterActor)
        // {
        //     monster.data = monsterActor.value().data;
        //     monster.stats = monsterActor.value().data.stats[monster.type][monsterActor.value().level];
        // }
        // else
        // {
        //     monster.data = summonData;
        //     monster.stats = summonStats;
        // }

        // MonsterBox box = new MonsterBox(row, monster, row.monstersGroup.getScaleX());
        // row.boxes.add(box);
        // row.monstersGroup.addActor(box);
    }
    print("parding common actor done\n");
}

Actor __parsePlayerActor(Message &msg)
{
    PlayerActor player;
    auto name = msg.readString();
    player.character_class = msg.readEnum(getCharacterClassValues());
    if (name)
    {
        player.name = name.value();
    }
    else
    {
        player.name = "nameless";
    }
    player.xp = msg.readInt(true);
    player.hp = msg.readInt(true);
    player.hp_max = msg.readInt(true);
    player.level = msg.readInt(true);
    player.loot = msg.readInt(true);
    player.initiative = msg.readInt(true);
    msg.readEnumArray(player.conditions, getConditionValues());
    msg.readEnumArray(player.conditions_expired, getConditionValues());
    msg.readEnumArray(player.conditions_current_turn, getConditionValues());
    player.exhausted = msg.readBoolean();

    print("parsing player\n");
    print(player);

    Actor actor(player);
    __parseCommonActor(msg, actor);
    return actor;
}

Actor __parseMonsterActor(Message &msg)
{
    MonsterActor monster = MonsterActor();
    Actor actor(monster);
    monster.data = msg.readInt(true);
    monster.id = msg.readInt(true);
    monster.is_normal = msg.readBoolean();
    monster.is_elite = msg.readBoolean();
    __parseCommonActor(msg, actor);
    return actor;
}

void parseGameState(Message &msg, GameState &state)
{
    state.round = msg.readInt(true);
    state.scenario_number = msg.readInt(true);
    state.scenario_level = msg.readInt(true);
    state.track_standees = msg.readBoolean();
    state.random_standees = msg.readBoolean();
    state.elites_first = msg.readBoolean();
    state.expire_conditions = msg.readBoolean();
    state.solo = msg.readBoolean();
    state.hide_stats = msg.readBoolean();
    state.calculate_stats = msg.readBoolean();
    state.can_draw = msg.readBoolean();
    state.needs_shuffle = msg.readBoolean();
    state.player_init = msg.readEnum(ghr::getPlayerInitValues());
    msg.readEnumArray(state.attack_modifiers, ghr::getAttackModifierValues());
    state.attack_modifier1 = msg.readEnumOrNull(ghr::getAttackModifierValues());
    state.attack_modifier2 = msg.readEnumOrNull(ghr::getAttackModifierValues());
    state.fire = msg.readEnum(ghr::getElementStateValues());
    state.ice = msg.readEnum(ghr::getElementStateValues());
    state.air = msg.readEnum(ghr::getElementStateValues());
    state.earth = msg.readEnum(ghr::getElementStateValues());
    state.light = msg.readEnum(ghr::getElementStateValues());
    state.dark = msg.readEnum(ghr::getElementStateValues());

    print("Size of actor: ", sizeof(Actor), "\n");

    for (int i = 0, n = msg.readInt(true); i < n; i++)
    {
        auto deck = MonsterAbilityDeck(msg.readInt(true));
        state.ability_decks[deck.id] = deck;
        deck.shown = msg.readBoolean();
        deck.was_shown = msg.readBoolean();
        for (int ii = 0, nn = msg.readInt(true); ii < nn; ii++)
            deck.abilities.push_back(msg.readInt(true));
    }

    int n = msg.readInt(true);
    state.actors.reserve(n);
    print("nr actors", n, "\n");
    for (int i = 0; i < n; i++)
    {

        if (msg.readBoolean())
        {
            Actor actor = __parsePlayerActor(msg);
            state.actors.push_back(std::move(actor));
            print("parsing player end\n");
        }
        else
        {
            Actor &&actor = __parseMonsterActor(msg);
            state.actors.push_back(std::move(actor));
            print("parsing monster end\n");
        }
    }

    print(state);
}

} // namespace ghr

#endif // __PROTOCOL_H__