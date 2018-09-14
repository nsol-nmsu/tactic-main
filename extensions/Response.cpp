#include "Response.hpp"

namespace tactic {

Response::Response( ndn::Name const& name )
    : m_name( name )
    , m_sig( DUMMY_SIGNATURE )
{}

ndn::Name const&
Response::GetName( void ) const {
    return m_name;
}

ndn::Signature const&
Response::GetSignature( void ) const {
    return m_sig;
}

SPtr< ndn::Data >
Response::ToData( ndn::EncodingBuffer& contentBuf ) const {
    auto data = make_shared< ndn::Data >( m_name );
    
    contentBuf.prependVarNumber( contentBuf.size() );
    contentBuf.prependVarNumber( ndn::tlv::Content );
    
    data->setContent( contentBuf.block() );
    data->setSignature( m_sig );
    return data;
}

void
Response::FromData( ndn::Data const& data ) {
    if( data.getName().get( 0 ) != KEYWORD_TACTIC )
        throw Error( "Not a TACTIC Data" );
    
    m_name = data.getName();
    m_sig  = data.getSignature();
}

}
