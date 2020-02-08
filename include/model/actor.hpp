#ifndef __ACTOR_H__
#define __ACTOR_H__

#include <vector>

#include "optional.hpp"
#include "print.hpp"

#include "model/monsterInstance.hpp"
#include "model/monsterActor.hpp"
#include "model/playerActor.hpp"

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
    std::vector<ghr::MonsterInstance> instances;

    Actor(const ghr::MonsterActor & monster) : __is_player(false), __monster(monster)
    {
    }
    Actor(const ghr::PlayerActor & player) : __is_player(true), __player(player)
    {
    }

    Actor(ghr::MonsterActor * monster) : __is_player(false), __monster(*monster)
    {
    }
    Actor(ghr::PlayerActor * player) : __is_player(true), __player(*player)
    {
    }

    Actor() : __is_player(false)
    {
    }

    tl::optional<ghr::MonsterActor &> getMonster()
    {
        return __is_player ? tl::nullopt : tl::optional<MonsterActor &>(__monster);
    }
    tl::optional<ghr::PlayerActor &> getPlayer()
    {
        return __is_player ? tl::optional<PlayerActor &>(__player) : tl::nullopt;
    }

    tl::optional<const ghr::MonsterActor &> getMonster() const
    {
        return __is_player ? tl::nullopt : tl::optional<const MonsterActor &>(__monster);
    }
    tl::optional<const ghr::PlayerActor &> getPlayer() const
    {
        return __is_player ? tl::optional<const PlayerActor &>(__player) : tl::nullopt;
    }
};

void print(const Actor &arg)
{
    const auto monster = arg.getMonster();
    const auto player = arg.getPlayer();
    if (monster)
        print("monster\n", monster.value());
    if (player)
        print("player\n", player.value());
    print("turn_completed: ", arg.turn_completed, "\n");
    for (const auto m : arg.instances)
    {
        print("instance: ", m, "\n");
    }
}

} // namespace ghr

#endif // __ACTOR_H__