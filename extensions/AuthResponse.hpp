#ifndef _AuthResponse_
#define _AuthResponse_
#include "Response.hpp"
#include "AuthTag.hpp"

namespace tactic {

class AuthResponse : public Response {
    public:
        
        AuthResponse(
            ndn::Name const& producerName,
            ndn::Name const& consumerName,
            AuthTag const& auth
        );
        
        AuthTag const&
        GetAuthTag( void ) const;
        
        Response::Type
        GetType( void ) const;
        
        SPtr< ndn::Data >
        ToData( ndn::EncodingBuffer& contentBuf ) const override;
        
        void
        FromData( ndn::Data const& data ) override;
        
        static bool
        IsAuthData( ndn::Data const& data );
    protected:
        AuthTag m_auth;
};

}
#endif
