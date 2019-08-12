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

    Actor actor(player);
    __parseCommonActor(msg, actor);
    print("returning after parsing player actor\n");
    return actor;
}

Actor __parseMonsterActor(Message &msg)
{
    MonsterActor monster = MonsterActor();
    monster.id = msg.readInt(true);
    monster.level = msg.readInt(true);
    monster.is_normal = msg.readBoolean();
    monster.is_elite = msg.readBoolean();
    Actor actor(monster);
    __parseCommonActor(msg, actor);
    return actor;
}

void readGameState(Message &msg, GameState &state)
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

    for (int i = 0, n = msg.readInt(true); i < n; i++)
    {
        auto deck = MonsterAbilityDeck(msg.readInt(true));
        deck.shown = msg.readBoolean();
        deck.was_shown = msg.readBoolean();
        for (int ii = 0, nn = msg.readInt(true); ii < nn; ii++)
            deck.abilities.push_back(msg.readInt(true));
        state.ability_decks[deck.id] = deck;
    }

    int n = msg.readInt(true);
    state.actors.reserve(n);
    print("nr actors", n, "\n");
    for (int i = 0; i < n; i++)
    {

        if (msg.readBoolean())
        {
            Actor actor = __parsePlayerActor(msg);
            print(actor);
            print("parsed player actor retured. pushing to gamestate\n");
            state.actors.push_back(actor);
            print("parsing player end\n");
        }
        else
        {
            Actor actor = __parseMonsterActor(msg);
            print(actor);
            state.actors.push_back(actor);
            print("parsing monster end\n");
        }
        print("deleted temp actor successfully\n");
    }

    print(state);
}

void writeGameState(const GameState &state, Message &msg)
{
    msg.writeInt(state.round, true);
    msg.writeInt(state.scenario_number, true);
    msg.writeInt(state.scenario_level, true);
    msg.writeBoolean(state.track_standees);
    msg.writeBoolean(state.random_standees);
    msg.writeBoolean(state.elites_first);
    msg.writeBoolean(state.expire_conditions);
    msg.writeBoolean(state.solo);
    msg.writeBoolean(state.hide_stats);
    msg.writeBoolean(state.calculate_stats);
    msg.writeBoolean(state.can_draw);
    msg.writeBoolean(state.needs_shuffle);
    msg.writeEnum(state.player_init);
    msg.writeEnumArray(state.attack_modifiers);
    msg.writeEnumOrNull(state.attack_modifier1);
    msg.writeEnumOrNull(state.attack_modifier2);
    msg.writeEnum(state.fire);
    msg.writeEnum(state.ice);
    msg.writeEnum(state.air);
    msg.writeEnum(state.earth);
    msg.writeEnum(state.light);
    msg.writeEnum(state.dark);

    msg.writeInt(state.ability_decks.size(), true);
    for (auto &&deck_pair : state.ability_decks)
    {
        auto deck = deck_pair.second;
        msg.writeInt(deck.id, true);
        msg.writeBoolean(deck.shown);
        msg.writeBoolean(deck.was_shown);
        for (int i = 0, n = msg.writeArrayStart(deck.abilities); i < n; i++)
        {
            msg.writeInt(deck.abilities[i], true);
        }
    }

    // SnapshotArray<Actor> children = gloom.rows.getChildren();
    // for (int i = 0, n = writeArrayStart(msg, children); i < n; i++)
    // {
    //     Actor actor = children.get(i);
    //     if (actor instanceof PlayerRow)
    //     {
    //         msg.writeBoolean(true);
    //         writePlayerRow(msg, (PlayerRow)actor);
    //     }
    //     else
    //     {
    //         msg.writeBoolean(false);
    //         writeMonsterRow(msg, (MonsterRow)actor);
    //     }
    // }
}

} // namespace ghr

#endif // __PROTOCOL_H__