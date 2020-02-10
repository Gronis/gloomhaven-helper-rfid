

from . import ghh as __ghh

Actor = __ghh.Actor
AttackModifier = __ghh.AttackModifier
CharacterClass = __ghh.CharacterClass
Condition = __ghh.Condition
ElementState = __ghh.ElementState
GameState = __ghh.GameState
MonsterAbilityDeck = __ghh.MonsterAbilityDeck
MonsterActor = __ghh.MonsterActor
MonsterInstance = __ghh.MonsterInstance
MonsterType = __ghh.MonsterType
PlayerActor = __ghh.PlayerActor
PlayerInit = __ghh.PlayerInit
SummonColor = __ghh.SummonColor

readGameState_v7_6 = __ghh.readGameState_v7_6
readGameState_v8_0 = __ghh.readGameState_v8_0


def __init__():
    import inspect

    def __remove_optional_from_property(old_property, make_optional):
        def new_set(self, v):
            old_property.fset(self, make_optional(v))

        def new_get(self):
            v = old_property.fget(self)
            if v.has_value():
                return v.value()
            else:
                return None
        return property(new_get, new_set)

    # Find every property which is using tl::optional and replace them
    __classes = [__c for __c in __ghh.__dict__.values() if inspect.isclass(__c)]
    for __c in __classes:
        __properties = {(__n, __p)
                        for (__n, __p) in __c.__dict__.items() if isinstance(__p, property)}
        for __n, __p in __properties:
            if __p.__doc__.find("tl::optional") > 0:
                __opt_name = "Optional" + __p.__doc__.split("tl::optional")[-1].replace(
                    "(", "").replace(")", "").replace("<", "").replace(">", "").split("::")[-1]
                __opt_class = __ghh.__dict__.get(__opt_name, None)
                if __opt_class:
                    print("self {}, replaced {}, {}".format(__c, __n, __p.__doc__))
                    setattr(__c, __n, __remove_optional_from_property(__p, __opt_class))
                else:
                    print("Warning, {} does not have a make_optional function.".format(__p.__doc__))


__init__()
print = __ghh._print
