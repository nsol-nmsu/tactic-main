#include "DataRequest.hpp"

namespace tactic {

DataRequest::DataRequest(
    ndn::Name const& producerPrefix,
    ndn::Name const& contentPostfix,
    AuthTag const&   authTag,
    uint             seg
) : Request(
        ndn::Name( producerPrefix )
        .append( KEYWORD_DATA )
        .append( contentPostfix )
        .appendSegment( seg )
    )
  , m_authProb( 0 )
  , m_authTag( authTag )
{ }

DataRequest::DataRequest( ndn::Interest const& interest )
    : Request( interest )
{
    FromInterest( interest );
}

AuthTag const&
DataRequest::GetAuthTag( void ) const {
    return m_authTag;
}

void
DataRequest::SetAuthProb( double prob ) {
    m_authProb = prob;
}

double
DataRequest::GetAuthProb( void ) const {
    return m_authProb;
}

Request::Type
DataRequest::GetType( void ) const {
    return TYPE_DATA;
}

SPtr< ndn::Interest >
DataRequest::ToInterest( ndn::EncodingBuffer& contentBuf ) const {
    ndn::encoding::prependNonNegativeIntegerBlock(
        contentBuf,
        m_authProb*UINT32_MAX,
        TLV_AUTH_PROB
    );
    
    contentBuf.prependBlock( m_authTag.Encode() );
    
    return Request::ToInterest( contentBuf );
}

void
DataRequest::FromInterest( ndn::Interest const& interest ) {
    ndn::Block const& blk = interest.getContent();
    m_authProb =
        ndn::encoding::readNonNegativeInteger(
            blk.get( TLV_AUTH_PROB )
        );
    
    m_authTag.Decode( blk.get( TLV_AUTH_TAG ) );
    
    Request::FromInterest( interest );
}

bool
DataRequest::IsDataInterest( ndn::Interest const& interest ) {
    ndn::Name const& iname = interest.getName();
    if( iname.get( 0 ) != KEYWORD_TACTIC )
        return false;
    
    auto it = std::find( iname.begin(), iname.end(), KEYWORD_DATA );
    if( it == iname.end() )
        return false;
    
    return false;
}

}
