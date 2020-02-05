#ifndef __PROTOCOL_V7_6_PROTOCOL_H__
#define __PROTOCOL_V7_6_PROTOCOL_H__

#include "protocol/message.hpp"

#include "model/summonColor.hpp"
#include "model/condition.hpp"
#include "model/characterClass.hpp"
#include "model/monsterActor.hpp"
#include "model/monsterType.hpp"
#include "model/monsterInstance.hpp"
#include "model/playerActor.hpp"
#include "model/gameState.hpp"

namespace ghr
{
namespace protocol
{
namespace v7_6
{
void __readCommonActor(Message &msg, Actor &actor)
{
    actor.turn_completed = msg.readBoolean();
    int nn = msg.readInt(true);
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

        print(monster);
        actor.instances.push_back(monster);
    }
}

Actor __readPlayerActor(Message &msg)
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
    __readCommonActor(msg, actor);
    return actor;
}

Actor __readMonsterActor(Message &msg)
{
    MonsterActor monster = MonsterActor();
    monster.id = msg.readInt(true);
    monster.level = msg.readInt(true);
    monster.is_normal = msg.readBoolean();
    monster.is_elite = msg.readBoolean();
    Actor actor(monster);
    __readCommonActor(msg, actor);
    return actor;
}

void readGameState(GameState &state, Message &msg)
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
    for (int i = 0; i < n; i++)
    {

        if (msg.readBoolean())
        {
            Actor actor = __readPlayerActor(msg);
            print(actor);
            state.actors.push_back(actor);
        }
        else
        {
            Actor actor = __readMonsterActor(msg);
            print(actor);
            state.actors.push_back(actor);
        }
    }
    print(state);
}

void __writeCommonActor(Message &msg, const Actor &actor)
{
    msg.writeBoolean(actor.turn_completed);
    for (int i = 0, n = msg.writeArrayStart(actor.instances); i < n; i++)
    {
        const MonsterInstance &monster = actor.instances[i];
        msg.writeInt(monster.number, true);
        msg.writeEnum(monster.type);
        if (monster.type == MonsterType::Summon)
        {
            msg.writeEnum(monster.summon_color);
            msg.writeInt(monster.summon_move, true);
            msg.writeInt(monster.summon_attack, true);
            msg.writeInt(monster.summon_range, true);
        }
        msg.writeBoolean(monster.is_new);
        msg.writeInt(monster.hp, true);
        msg.writeInt(monster.hp_max, true);
        msg.writeEnumArray(monster.conditions);
        msg.writeEnumArray(monster.conditions_expired);
        msg.writeEnumArray(monster.conditions_current_turn);
    }
}
void __writePlayerActor(Message &msg, const Actor &actor)
{
    const auto &player = actor.getPlayer().value();
    msg.writeString(player.name == "nameless" ? tl::nullopt : tl::make_optional(player.name));
    msg.writeEnum(player.character_class);
    msg.writeInt(player.xp, true);
    msg.writeInt(player.hp, true);
    msg.writeInt(player.hp_max, true);
    msg.writeInt(player.level, true);
    msg.writeInt(player.loot, true);
    msg.writeInt(player.initiative, true);
    msg.writeEnumArray(player.conditions);
    msg.writeEnumArray(player.conditions_expired);
    msg.writeEnumArray(player.conditions_current_turn);
    msg.writeBoolean(player.exhausted);

    __writeCommonActor(msg, actor);
}
void __writeMonsterActor(Message &msg, const Actor &actor)
{
    const auto &monster = actor.getMonster().value();
    msg.writeInt(monster.id, true);
    msg.writeInt(monster.level, true);
    msg.writeBoolean(monster.is_normal);
    msg.writeBoolean(monster.is_elite);
    __writeCommonActor(msg, actor);
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
    for (const auto deck_pair : state.ability_decks)
    {
        const auto deck = deck_pair.second;
        msg.writeInt(deck.id, true);
        msg.writeBoolean(deck.shown);
        msg.writeBoolean(deck.was_shown);
        for (int i = 0, n = msg.writeArrayStart(deck.abilities); i < n; i++)
        {
            msg.writeInt(deck.abilities[i], true);
        }
    }

    for (int i = 0, n = msg.writeArrayStart(state.actors); i < n; i++)
    {
        const Actor actor = state.actors[i];
        if (actor.getPlayer())
        {
            msg.writeBoolean(true);
            __writePlayerActor(msg, actor);
        }
        else if (actor.getMonster())
        {
            msg.writeBoolean(false);
            __writeMonsterActor(msg, actor);
        }
    }
}

} // namespace v7_6
} // namespace protocol
} // namespace ghr

#endif // __PROTOCOL_V7_6_PROTOCOL_H__