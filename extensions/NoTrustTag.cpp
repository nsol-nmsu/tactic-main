#include "NoTrustTag.hpp"

namespace tactic {

ns3::TypeId
NoTrustTag::GetTypeId( void ) {
    static ns3::TypeId tid = ns3::TypeId( "tactic::NoTrustTag" )
        .SetParent<ns3::Object>()
        .AddConstructor<NoTrustTag>();
    return tid;
}

}
