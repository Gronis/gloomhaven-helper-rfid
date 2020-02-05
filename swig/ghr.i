%module ghr
%feature("autodoc", "3");

// Include swig stuff that is needed
%include "typemaps.i"
%include "std_vector.i"
%include "std_map.i"
%include "std_string.i"
%include "optional.i"

%{
    #define SWIG_FILE_WITH_INIT
    #define SWIG_PYTHON_EXTRA_NATIVE_CONTAINERS

    // #include "optional.hpp"

    // Include enums first. Somehow swig will intruduce memory leaks if included in this order
    #include "attackModifier.hpp"
    #include "characterClass.hpp"
    #include "condition.hpp"
    #include "elementState.hpp"
    #include "monsterType.hpp"
    #include "playerInit.hpp"
    #include "summonColor.hpp"

    // Include classes
    #include "actor.hpp"
    #include "decoding.hpp"
    #include "encoding.hpp"
    #include "protocol/message.hpp"
    #include "monsterAbilityDeck.hpp"
    #include "monsterActor.hpp"
    #include "monsterInstance.hpp"
    #include "playerActor.hpp"
    #include "gameState.hpp"

    using namespace ghr;
    using namespace ghr::protocol;
%}

// Rename all enums
%rename(AttackModifier) AttackModifierNS;
%rename(CharacterClass) CharacterClassNS;
%rename(Condition) ConditionNS;
%rename(ElementState) ElementStateNS;
%rename(MonsterType) MonsterTypeNS;
%rename(PlayerInit) PlayerInitNS;
%rename(SummonColor) SummonColorNS;

// Include enums first. Somehow swig will intruduce memory leaks if included in this order
%include "attackModifier.hpp"
%include "characterClass.hpp"
%include "condition.hpp"
%include "elementState.hpp"
%include "monsterType.hpp"
%include "playerInit.hpp"
%include "summonColor.hpp"

// Include classes
%include "actor.hpp"
%include "decoding.hpp"
%include "encoding.hpp"
%include "protocol/message.hpp"
%include "monsterAbilityDeck.hpp"
%include "monsterActor.hpp"
%include "monsterInstance.hpp"
%include "playerActor.hpp"
%include "gameState.hpp"

// namespace ghr{
//     %extend PlayerActor {
//         char *__str__() {
//             static char tmp[1024];
//             sprintf(tmp, "PlayerActor(%s)", $self->name);
//         }
//     };
// }

// We need to template every class that we want to iterate over in python.
// Define templates for enums
%template(AttackModifierVector) std::vector<ghr::AttackModifierNS::Value>;
%template(CharacterClassVector) std::vector<ghr::CharacterClassNS::Value>;
%template(ConditionVector) std::vector<ghr::ConditionNS::Value>;
%template(ElementStateVector) std::vector<ghr::ElementStateNS::Value>;
%template(MonsterTypeVector) std::vector<ghr::MonsterTypeNS::Value>;
%template(PlayerInitVector) std::vector<ghr::PlayerInitNS::Value>;
%template(SummonColorVector) std::vector<ghr::SummonColorNS::Value>;

// Define templates for classes
%template(ActorVector) std::vector<ghr::Actor>;
%template(GameStateVector) std::vector<ghr::GameState>;
%template(MonsterAbilityDeckVector) std::vector<ghr::MonsterAbilityDeck>;
%template(MonsterActorVector) std::vector<ghr::MonsterActor>;
%template(MonsterInstanceVector) std::vector<ghr::MonsterInstance>;
%template(PlayerActorVector) std::vector<ghr::PlayerActor>;

// %template(MonsterAbilityDeckLookupMap) std::map<int, MonsterAbilityDeck>;

// %typemap(in) tl::optional<ghr::MonsterActor &> %{
//     if($input == Py_None)
//         $1 = tl::optional<ghr::MonsterActor &>();
//     else
//         $1 = tl::optional<ghr::MonsterActor &>(PyObject($input));
// %}


%template(OptionalAttackModifier) tl::optional<ghr::AttackModifierNS::Value>;
%template(OptionalInt) tl::optional<int>;
// %typemap(out) tl::optional<ghr::AttackModifierNS::Value> %{
//     if(!!$1)
//         $result = PyObject($1.value());
//     else
//     {
//         $result = Py_None;
//         Py_INCREF(Py_None);
//     }
// %}