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

    // Include enums first. Somehow swig will intruduce memory leaks if included in this order
    #include "model/attackModifier.hpp"
    #include "model/characterClass.hpp"
    #include "model/condition.hpp"
    #include "model/elementState.hpp"
    #include "model/monsterType.hpp"
    #include "model/playerInit.hpp"
    #include "model/summonColor.hpp"

    // Include classes
    #include "model/actor.hpp"
    #include "model/monsterAbilityDeck.hpp"
    #include "model/monsterActor.hpp"
    #include "model/monsterInstance.hpp"
    #include "model/playerActor.hpp"
    #include "model/gameState.hpp"

    #include "protocol/buffer.hpp"

    using namespace ghr;
    using namespace ghr::protocol;
    using namespace tl;
%}

// Rename all enums
%rename(AttackModifier) AttackModifierNS;
%rename(CharacterClass) CharacterClassNS;
%rename(Condition) ConditionNS;
%rename(ElementState) ElementStateNS;
%rename(MonsterType) MonsterTypeNS;
%rename(PlayerInit) PlayerInitNS;
%rename(SummonColor) SummonColorNS;

// Create tl::optional templates for some classes. Enums are represented as an integer in python
template_optional_object(ghr::MonsterActor, MonsterActor);
template_optional_object(ghr::PlayerActor, PlayerActor);
template_optional_integral(int, int);
template_optional_integral(ghr::AttackModifierNS::Value, AttackModifier);
template_optional_integral(ghr::CharacterClassNS::Value, CharacterClass);
template_optional_integral(ghr::ConditionNS::Value, Condition);
template_optional_integral(ghr::ElementStateNS::Value, ElementState);
template_optional_integral(ghr::MonsterTypeNS::Value, MonsterType);
template_optional_integral(ghr::PlayerInitNS::Value, PlayerInit);
template_optional_integral(ghr::SummonColorNS::Value, SummonColor);

// Include enums first. Somehow swig will intruduce memory leaks if included in this order
%include "model/attackModifier.hpp"
%include "model/characterClass.hpp"
%include "model/condition.hpp"
%include "model/elementState.hpp"
%include "model/monsterType.hpp"
%include "model/playerInit.hpp"
%include "model/summonColor.hpp"

// Include classes
%include "model/actor.hpp"
%include "model/monsterAbilityDeck.hpp"
%include "model/monsterActor.hpp"
%include "model/monsterInstance.hpp"
%include "model/playerActor.hpp"
%include "model/gameState.hpp"

// Include classes used for serializing and deserializing the model
%include "protocol/buffer.hpp"

%template(IntVector) std::vector<int>;

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
%template(MonsterActorVector) std::vector<ghr::MonsterActor>;
%template(MonsterInstanceVector) std::vector<ghr::MonsterInstance>;
%template(PlayerActorVector) std::vector<ghr::PlayerActor>;

%template(MonsterAbilityDeckLookupMap) std::map<int, ghr::MonsterAbilityDeck>;

