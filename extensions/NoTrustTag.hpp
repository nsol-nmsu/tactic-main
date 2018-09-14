#ifndef _NoTrustTag_
#define _NoTrustTag_
#include "ns3/core-module.h"

namespace tactic {

class NoTrustTag : public ns3::Object {
public:
    static ns3::TypeId
    GetTypeId( void );
};

}
#endif
