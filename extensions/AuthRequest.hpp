#ifndef _AuthRequest_
#define _AuthRequest_
#include "Request.hpp"

namespace tactic {

class AuthRequest : public Request {
    public:
        
        AuthRequest(
            ndn::Name const& producerName,
            ndn::Name const& consumerName
        );
        
        AuthRequest( ndn::Interest const& interest );
        
        Request::Type
        GetType( void ) const override;
        
        static bool
        IsAuthInterest( ndn::Interest const& interest );
};

}
#endif
