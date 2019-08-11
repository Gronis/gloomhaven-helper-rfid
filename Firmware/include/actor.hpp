#ifndef __ACTOR_H__
#define __ACTOR_H__

#include <vector>

#include "optional.hpp"
#include "print.hpp"

#include "monsterInstance.hpp"
#include "monsterActor.hpp"
#include "playerActor.hpp"

namespace ghr
{
struct Actor
{
    //private:
    bool __is_player;
    // TODO: put in a union. For this to work, copy constructor, destructor and assignment operator
    // must be implemented (See further down). However, arduino crashes with the current
    // implementation. This solusion is less memory efficient, but it works.
    MonsterActor __monster;
    PlayerActor __player;
    // union {
    // };

    //public:
    bool turn_completed = false;
    std::vector<MonsterInstance> instances;

    Actor(MonsterActor monster) : __is_player(false), __monster(monster)
    {
    }
    Actor(PlayerActor player) : __is_player(true), __player(player)
    {
    }

    // TODO: Use copy constructor, destructor, assignment operator when union is used (see top).
    //
    // Actor(const Actor &other)
    // {
    //     __is_player = other.__is_player;
    //     if (__is_player)
    //     {
    //         __player = other.__player;
    //     }
    //     else
    //     {
    //         __monster = other.__monster;
    //     }
    //     turn_completed = other.turn_completed;
    //     instances.reserve(other.instances.size());
    //     for (auto &&instance : other.instances)
    //     {
    //         instances.push_back(instance);
    //     }
    // }

    // ~Actor()
    // {
    //     if (__is_player)
    //     {
    //         (&__player)->PlayerActor::~PlayerActor();
    //     }
    //     else
    //     {
    //         (&__monster)->MonsterActor::~MonsterActor();
    //     }
    //     (&instances)->vector<MonsterInstance>::~vector<MonsterInstance>();
    // }

    // Actor &operator=(Actor &other)
    // {
    //     __is_player = other.__is_player;
    //     if (other.__is_player)
    //     {
    //         __player = other.__player;
    //     }
    //     else
    //     {
    //         __monster = other.__monster;
    //     }
    //     turn_completed = other.turn_completed;
    //     instances.reserve(other.instances.size());
    //     for (auto &&instance : other.instances)
    //     {
    //         instances.push_back(instance);
    //     }
    //     return *this;
    // }

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