#ifndef __ACTOR_H__
#define __ACTOR_H__

#include <vector>

#include "optional.hpp"
#include "print.hpp"

#include "monsterActor.hpp"
#include "playerActor.hpp"

namespace ghr
{
struct Actor
{
    //private:
    bool __is_player;
    union {
        MonsterActor __monster;
        PlayerActor __player;
    };

    //public:
    bool turn_completed = false;
    std::vector<MonsterInstance> instances;

    Actor(MonsterActor monster) : __is_player(false), __monster(monster)
    {
    }
    Actor(PlayerActor player) : __is_player(true), __player(player)
    {
    }

    Actor(const Actor &actor)
    {
        __is_player = actor.__is_player;
        if (__is_player)
        {
            __player = actor.__player;
        }
        else
        {
            __monster = actor.__monster;
        }
        turn_completed = actor.turn_completed;
        instances = actor.instances;
    }

    ~Actor()
    {
        if (__is_player)
        {
            __player.~PlayerActor();
        }
        else
        {
            __monster.~MonsterActor();
        }
    }

    tl::optional<MonsterActor &> getMonster()
    {
        return __is_player ? tl::nullopt : tl::optional<MonsterActor &>(__monster);
    }
    tl::optional<PlayerActor &> getPlayer()
    {
        return __is_player ? tl::optional<PlayerActor &>(__player) : tl::nullopt;
    }
};

void print(Actor &arg)
{
    auto monster = arg.getMonster();
    auto player = arg.getPlayer();
    if (monster)
        print("monster\n", monster.value());
    if (player)
        print("player\n", player.value());
    print("turn_completed: ", arg.turn_completed, "\n");
    for (auto &&m : arg.instances)
    {
        print("instance: ", m, "\n");
    }
}

} // namespace ghr

#endif // __ACTOR_H__