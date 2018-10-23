#include "Request.hpp"

namespace tactic {

ns3::Ptr< ns3::UniformRandomVariable > Request::s_nonceRng =
    ns3::CreateObject< ns3::UniformRandomVariable >();

ndn::time::milliseconds Request::s_lifetime =
    ndn::time::milliseconds( 2000 );

Request::Request( ndn::Name const& name )
    : m_name( name )
    , m_nonce( s_nonceRng->GetInteger() )
    , m_lifetime( s_lifetime )
    , m_route( 0 )
    , m_sig( DUMMY_SIGNATURE )
{}

Request::Request( ndn::Interest const& interest ) {
    FromInterest( interest );
}

ndn::Name const&
Request::GetName( void ) const {
    return m_name;
}

uint64_t
Request::GetRouteHash( void ) const {
    return m_route;
}

void
Request::UpdateRouteHash( uint64_t node ) {
    m_route ^= node;
}

ndn::Signature const&
Request::GetSignature( void ) const {
    return m_sig;
}

SPtr< ndn::Interest >
Request::ToInterest( void ) const {
    ndn::EncodingBuffer contentBuf;
    return ToInterest( contentBuf );
}

SPtr< ndn::Interest >
Request::ToInterest( ndn::EncodingBuffer& contentBuf ) const {
    size_t size = contentBuf.size();
    
    size +=
        ndn::encoding::prependNonNegativeIntegerBlock(
            contentBuf,
            TLV_ROUTE_HASH,
            m_route
        );
    
    contentBuf.prependVarNumber( size );
    contentBuf.prependVarNumber( ndn::tlv::Content );
    
    auto interest = std::make_shared<ndn::Interest>( m_name );
    interest->setNonce( m_nonce );
    interest->setInterestLifetime( m_lifetime );
    interest->setSignature( m_sig );
    interest->setContent( contentBuf.block() );
    
    return interest;
}

void
Request::FromInterest( ndn::Interest const& interest ) {
    if( interest.getName().get( 0 ) != KEYWORD_TACTIC )
        throw Error( "Not a TACTIC Interest" );
    
    m_name      = interest.getName();
    m_nonce     = interest.getNonce();
    m_lifetime  = interest.getInterestLifetime();
    m_sig       = interest.getSignature();
    
    ndn::Block const& blk = interest.getContent();
    m_route = ndn::readNonNegativeInteger( blk.get( TLV_ROUTE_HASH ) );
}


}
