#include "protocol/v8_0/protocol.hpp"

#include "model/summonColor.hpp"
#include "model/condition.hpp"
#include "model/characterClass.hpp"
#include "model/monsterActor.hpp"
#include "model/monsterType.hpp"
#include "model/monsterInstance.hpp"
#include "model/playerActor.hpp"

using namespace ghh;
using namespace ghh::protocol;
using namespace ghh::protocol::v8_0;

static void __readCommonActor(Buffer &buffer, Actor &actor)
{
    actor.turn_completed = buffer.readBoolean();
    int nn = buffer.readInt(true);
    for (int ii = 0; ii < nn; ii++)
    {
        MonsterInstance monster;

        monster.number = buffer.readInt(true);
        monster.type = buffer.readEnum(getMonsterTypeValues());
        print("Monster type: ", monster.type, "\n");
        if (monster.type == MonsterType::Summon)
        {
            monster.summon_color = buffer.readEnum(getSummonColorValues());
            monster.summon_move = buffer.readInt(true);
            monster.summon_attack = buffer.readInt(true);
            monster.summon_range = buffer.readInt(true);
        }
        monster.is_new = buffer.readBoolean();
        monster.hp = buffer.readInt(true);
        monster.hp_max = buffer.readInt(true);
        buffer.readEnumArray(monster.conditions, getConditionValues());
        buffer.readEnumArray(monster.conditions_expired, getConditionValues());
        buffer.readEnumArray(monster.conditions_current_turn, getConditionValues());

        print(monster);
        actor.instances.push_back(monster);
    }
}

static Actor __readPlayerActor(Buffer &buffer)
{
    print("Reading player actor");
    PlayerActor player;
    auto name = buffer.readString();
    player.character_class = buffer.readEnum(getCharacterClassValues());
    if (name)
    {
        player.name = name.value();
    }
    else
    {
        player.name = "nameless";
    }
    player.xp = buffer.readInt(true);
    player.hp = buffer.readInt(true);
    player.hp_max = buffer.readInt(true);
    player.level = buffer.readInt(true);
    player.loot = buffer.readInt(true);
    player.initiative = buffer.readInt(true);
    buffer.readEnumArray(player.conditions, getConditionValues());
    buffer.readEnumArray(player.conditions_expired, getConditionValues());
    buffer.readEnumArray(player.conditions_current_turn, getConditionValues());
    player.exhausted = buffer.readBoolean();

    Actor actor(player);
    __readCommonActor(buffer, actor);
    return actor;
}

static tl::optional<int> __readMonsterAbility(Buffer &buffer)
{
    int ability_id = buffer.readInt(true);
    return ability_id == 0 ? tl::nullopt : tl::make_optional(ability_id - 1);
}

static Actor __readMonsterActor(Buffer &buffer)
{
    print("Reading monster actor");
    MonsterActor monster = MonsterActor();
    monster.id = buffer.readInt(true);
    monster.level = buffer.readInt(true);
    monster.is_normal = buffer.readBoolean();
    monster.is_elite = buffer.readBoolean();
    monster.ability = __readMonsterAbility(buffer);
    Actor actor(monster);
    __readCommonActor(buffer, actor);
    return actor;
}

void ghh::protocol::v8_0::readGameState(GameState &state, Buffer &buffer)
{
    state.round = buffer.readInt(true);
    state.scenario_number = buffer.readInt(true);
    state.scenario_level = buffer.readInt(true);
    state.track_standees = buffer.readBoolean();
    state.ability_cards = buffer.readBoolean();
    state.random_standees = buffer.readBoolean();
    state.elites_first = buffer.readBoolean();
    state.expire_conditions = buffer.readBoolean();
    state.solo = buffer.readBoolean();
    state.hide_stats = buffer.readBoolean();
    state.calculate_stats = buffer.readBoolean();
    state.can_draw = buffer.readBoolean();
    state.needs_shuffle = buffer.readBoolean();
    state.player_init = buffer.readEnum(ghh::getPlayerInitValues());
    buffer.readEnumArray(state.attack_modifiers, ghh::getAttackModifierValues());
    buffer.readEnumArray(state.attack_modifiers_discard, ghh::getAttackModifierValues());
    state.fire = buffer.readEnum(ghh::getElementStateValues());
    state.ice = buffer.readEnum(ghh::getElementStateValues());
    state.air = buffer.readEnum(ghh::getElementStateValues());
    state.earth = buffer.readEnum(ghh::getElementStateValues());
    state.light = buffer.readEnum(ghh::getElementStateValues());
    state.dark = buffer.readEnum(ghh::getElementStateValues());
    buffer.readIntArray(state.removed_abilities, true);
    state.bad_omen = buffer.readInt(true);

    int nrMonsterAbilityDecks = buffer.readInt(true);
    print("Nr monster ability decks: ", nrMonsterAbilityDecks, "\n");
    for (int i = 0, n = nrMonsterAbilityDecks; i < n; i++)
    {
        auto deck = MonsterAbilityDeck(buffer.readInt(true));
        deck.shuffle = buffer.readBoolean();
        deck.shown_ability = __readMonsterAbility(buffer);
        for (int ii = 0, nn = buffer.readInt(true); ii < nn; ii++)
        {
            deck.abilities.push_back(buffer.readInt(true));
        }
        for (int ii = 0, nn = buffer.readInt(true); ii < nn; ii++)
        {
            deck.abilities_discard.push_back(buffer.readInt(true));
        }
        state.ability_decks[deck.id] = deck;
    }

    int nrActors = buffer.readInt(true);
    print("Nr actors: ", nrActors, "\n");
    for (int i = 0, n = nrActors; i < n; i++)
    {
        if (buffer.readBoolean())
        {
            Actor actor = __readPlayerActor(buffer);
            print(actor);
            state.actors.push_back(actor);
        }
        else
        {
            Actor actor = __readMonsterActor(buffer);
            print(actor);
            state.actors.push_back(actor);
        }
    }
    print(state);
}

static void __writeCommonActor(Buffer &buffer, const Actor &actor)
{
    // Note: Same as v7_6
    buffer.writeBoolean(actor.turn_completed);
    for (int i = 0, n = buffer.writeArrayStart(actor.instances); i < n; i++)
    {
        const MonsterInstance &monster = actor.instances[i];
        buffer.writeInt(monster.number, true);
        buffer.writeEnum(monster.type);
        if (monster.type == MonsterType::Summon)
        {
            buffer.writeEnum(monster.summon_color);
            buffer.writeInt(monster.summon_move, true);
            buffer.writeInt(monster.summon_attack, true);
            buffer.writeInt(monster.summon_range, true);
        }
        buffer.writeBoolean(monster.is_new);
        buffer.writeInt(monster.hp, true);
        buffer.writeInt(monster.hp_max, true);
        buffer.writeEnumArray(monster.conditions);
        buffer.writeEnumArray(monster.conditions_expired);
        buffer.writeEnumArray(monster.conditions_current_turn);
    }
}
static void __writePlayerActor(Buffer &buffer, const Actor &actor)
{
    // Note: Same as v7_6
    const auto &player = actor.getPlayer().value();
    buffer.writeString(player.name == "nameless" ? tl::nullopt : tl::make_optional(player.name));
    buffer.writeEnum(player.character_class);
    buffer.writeInt(player.xp, true);
    buffer.writeInt(player.hp, true);
    buffer.writeInt(player.hp_max, true);
    buffer.writeInt(player.level, true);
    buffer.writeInt(player.loot, true);
    // TODO Remove and replace with acctual initiative
    buffer.writeInt(78, true); //buffer.writeInt(player.initiative, true);
    buffer.writeEnumArray(player.conditions);
    buffer.writeEnumArray(player.conditions_expired);
    buffer.writeEnumArray(player.conditions_current_turn);
    buffer.writeBoolean(player.exhausted);

    __writeCommonActor(buffer, actor);
}

static void __writeMonsterAbility(Buffer &buffer, tl::optional<int> ability)
{
    buffer.writeInt(ability ? (ability.value() + 1) : 0, true);
}

static void __writeMonsterActor(Buffer &buffer, const Actor &actor)
{
    const auto &monster = actor.getMonster().value();
    buffer.writeInt(monster.id, true);
    buffer.writeInt(monster.level, true);
    buffer.writeBoolean(monster.is_normal);
    buffer.writeBoolean(monster.is_elite);
    __writeMonsterAbility(buffer, monster.ability);
    __writeCommonActor(buffer, actor);
}

void ghh::protocol::v8_0::writeGameState(const GameState &state, Buffer &buffer)
{
    buffer.writeInt(state.round, true);
    buffer.writeInt(state.scenario_number, true);
    buffer.writeInt(state.scenario_level, true);
    buffer.writeBoolean(state.track_standees);
    buffer.writeBoolean(state.ability_cards);
    buffer.writeBoolean(state.random_standees);
    buffer.writeBoolean(state.elites_first);
    buffer.writeBoolean(state.expire_conditions);
    buffer.writeBoolean(state.solo);
    buffer.writeBoolean(state.hide_stats);
    buffer.writeBoolean(state.calculate_stats);
    buffer.writeBoolean(state.can_draw);
    buffer.writeBoolean(state.needs_shuffle);
    buffer.writeEnum(state.player_init);
    buffer.writeEnumArray(state.attack_modifiers);
    buffer.writeEnumArray(state.attack_modifiers_discard);
    buffer.writeEnum(state.fire);
    buffer.writeEnum(state.ice);
    buffer.writeEnum(state.air);
    buffer.writeEnum(state.earth);
    buffer.writeEnum(state.light);
    buffer.writeEnum(state.dark);
    buffer.writeIntArray(state.removed_abilities, true);
    buffer.writeInt(state.bad_omen, true);

    buffer.writeInt(state.ability_decks.size(), true);
    for (const auto deck_pair : state.ability_decks)
    {
        const auto deck = deck_pair.second;
        buffer.writeInt(deck.id, true);
        buffer.writeBoolean(deck.shuffle);
        __writeMonsterAbility(buffer, deck.shown_ability);
        for (int i = 0, n = buffer.writeArrayStart(deck.abilities); i < n; i++)
        {
            buffer.writeInt(deck.abilities[i], true);
        }
        for (int i = 0, n = buffer.writeArrayStart(deck.abilities_discard); i < n; i++)
        {
            buffer.writeInt(deck.abilities_discard[i], true);
        }
    }

    for (int i = 0, n = buffer.writeArrayStart(state.actors); i < n; i++)
    {
        const Actor actor = state.actors[i];
        if (actor.getPlayer())
        {
            buffer.writeBoolean(true);
            __writePlayerActor(buffer, actor);
        }
        else if (actor.getMonster())
        {
            buffer.writeBoolean(false);
            __writeMonsterActor(buffer, actor);
        }
    }
}
