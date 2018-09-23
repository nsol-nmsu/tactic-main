#ifndef _Interface_
#define _Interface_

#include "Types.hpp"
#include "Request.hpp"
#include "Response.hpp"

#include <ns3/ndnSIM-module.h>
#include <ns3/core-module.h>

namespace tactic {

class Interface {
    public:
        typedef Closure< void( SPtr< Response > ) > ResponseCb;
        
        virtual void
        Submit( SPtr< Request > req, ResponseCb res ) = 0;
};

}

#endif
