#include "AuthResponse.hpp"

namespace tactic {

AuthResponse::AuthResponse(
    ndn::Name const& producerName,
    ndn::Name const& consumerName,
    AuthTag const& auth
)
    : Response(
        ndn::Name( producerName )
        .append( KEYWORD_AUTH )
        .append( consumerName )
      )
    , m_auth( auth )
{}

Response::Type
AuthResponse::GetType( void ) const {
    return Response::TYPE_AUTH;
}

AuthTag const&
AuthResponse::GetAuthTag( void ) const {
    return m_auth;
}
    

SPtr< ndn::Data >
AuthResponse::ToData( ndn::EncodingBuffer& contentBuf ) const {
    contentBuf.prependBlock( m_auth.Encode() );
    return Response::ToData( contentBuf );
}

void
AuthResponse::FromData( ndn::Data const& data ) {
    ndn::Block const& content = data.getContent();
    m_auth.Decode( content.get( TLV_AUTH_TAG ) );
    Response::FromData( data );
}

bool
AuthResponse::IsAuthData( ndn::Data const& data ) {
    ndn::Name const& dname = data.getName();
    if( dname.get( 0 ) != KEYWORD_TACTIC )
        return false;
    
    auto it = std::find( dname.begin(), dname.end(), KEYWORD_AUTH );
    if( it == dname.end() )
        return false;
    
    return false;
}

}
