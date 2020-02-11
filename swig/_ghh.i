%module ghh
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

    using namespace ghh;
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
template_optional_object(ghh::MonsterActor, MonsterActor);
template_optional_object(ghh::PlayerActor, PlayerActor);
template_optional_integral(int, int);
template_optional_integral(ghh::AttackModifierNS::Value, AttackModifier);
template_optional_integral(ghh::CharacterClassNS::Value, CharacterClass);
template_optional_integral(ghh::ConditionNS::Value, Condition);
template_optional_integral(ghh::ElementStateNS::Value, ElementState);
template_optional_integral(ghh::MonsterTypeNS::Value, MonsterType);
template_optional_integral(ghh::PlayerInitNS::Value, PlayerInit);
template_optional_integral(ghh::SummonColorNS::Value, SummonColor);

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

// TODO: This typedef is required for writeByte to work. Include standard typedef instead
typedef unsigned char uint8_t;
%include "protocol/buffer.hpp"

// Used for abilities
%template(IntVector) std::vector<int>;

// We need to template every class that we want to iterate over in python.
// Define templates for enums
%template(CharacterClassVector) std::vector<ghh::CharacterClassNS::Value>;
%template(ConditionVector) std::vector<ghh::ConditionNS::Value>;
%template(ElementStateVector) std::vector<ghh::ElementStateNS::Value>;
%template(MonsterTypeVector) std::vector<ghh::MonsterTypeNS::Value>;
%template(PlayerInitVector) std::vector<ghh::PlayerInitNS::Value>;
%template(SummonColorVector) std::vector<ghh::SummonColorNS::Value>;

// Define templates for classes
%template(ActorVector) std::vector<ghh::Actor>;
%template(MonsterInstanceVector) std::vector<ghh::MonsterInstance>;
%template(MonsterAbilityDeckLookupMap) std::map<int, ghh::MonsterAbilityDeck>;

