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

    #include "protocol/v7_6/protocol.hpp"
    #include "protocol/v8_0/protocol.hpp"

    using namespace ghh;
    using namespace ghh::protocol;
    using namespace tl;
%}
%rename(readGameState_v7_6) ghh::protocol::v7_6::readGameState;
%rename(readGameState_v8_0) ghh::protocol::v8_0::readGameState;

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

// Include classes used for serializing and deserializing the model
%include "protocol/buffer.hpp"

namespace ghh
{
namespace protocol
{
namespace v8_0
{
void readGameState(GameState &state, Buffer &buffer);
}
}
}
namespace ghh
{
namespace protocol
{
namespace v7_6
{
void readGameState(GameState &state, Buffer &buffer);
}
}
}


%template(IntVector) std::vector<int>;

// We need to template every class that we want to iterate over in python.
// Define templates for enums
%template(AttackModifierVector) std::vector<ghh::AttackModifierNS::Value>;
%template(CharacterClassVector) std::vector<ghh::CharacterClassNS::Value>;
%template(ConditionVector) std::vector<ghh::ConditionNS::Value>;
%template(ElementStateVector) std::vector<ghh::ElementStateNS::Value>;
%template(MonsterTypeVector) std::vector<ghh::MonsterTypeNS::Value>;
%template(PlayerInitVector) std::vector<ghh::PlayerInitNS::Value>;
%template(SummonColorVector) std::vector<ghh::SummonColorNS::Value>;

// Define templates for classes
%template(ActorVector) std::vector<ghh::Actor>;
%template(GameStateVector) std::vector<ghh::GameState>;
%template(MonsterActorVector) std::vector<ghh::MonsterActor>;
%template(MonsterInstanceVector) std::vector<ghh::MonsterInstance>;
%template(PlayerActorVector) std::vector<ghh::PlayerActor>;

%template(MonsterAbilityDeckLookupMap) std::map<int, ghh::MonsterAbilityDeck>;

