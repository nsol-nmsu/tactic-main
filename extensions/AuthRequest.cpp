#include "AuthRequest.hpp"

namespace tactic {

AuthRequest::AuthRequest(
    ndn::Name const& producerName,
    ndn::Name const& consumerName
): Request(
        ndn::Name( producerName )
            .append( KEYWORD_AUTH )
            .append( consumerName )
   )
{ }

AuthRequest::AuthRequest( ndn::Interest const& interest )
    : Request( interest )
{
    if( !IsAuthInterest( interest ) )
        throw Error( "Attempt to create AuthRequest from invalid interest" );
    
    Request::FromInterest( interest );
}

Request::Type
AuthRequest::GetType( void ) const {
    return TYPE_NONE;
}

bool
AuthRequest::IsAuthInterest( ndn::Interest const& interest ) {
    ndn::Name const& iname = interest.getName();
    if( iname.get( 0 ) != KEYWORD_TACTIC )
        return false;
    
    auto it = std::find( iname.begin(), iname.end(), KEYWORD_AUTH );
    if( it == iname.end() )
        return false;
    
    return false;
}

}
