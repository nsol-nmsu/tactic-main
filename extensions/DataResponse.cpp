#include "DataResponse.hpp"

namespace tactic {

DataResponse::DataResponse(
    ndn::Name const& producerName,
    ndn::Name const& contentName,
    size_t           contentSize,
    AuthGroup const& authGroup,
    uint             lastSeg,
    bool             wasDenied
)
    : Response(
        ndn::Name( producerName )
        .append( KEYWORD_DATA )
        .append( contentName )
      )
    , m_contentSize( contentSize )
    , m_authGroup( authGroup )
    , m_lastSeg( lastSeg )
    , m_wasDenied( wasDenied )
{}

DataResponse::DataResponse( ndn::Data const& data )
    : Response( "" )
{
    FromData( data );
}

Response::Type
DataResponse::GetType( void ) const {
    return Response::TYPE_DATA;
}

size_t
DataResponse::GetSize( void ) const {
    return m_contentSize;
}

AuthGroup const&
DataResponse::GetAuthGroup( void ) const {
    return m_authGroup;
}

uint
DataResponse::GetLastSeg( void ) const {
    return m_lastSeg;
}
 

SPtr< ndn::Data >
DataResponse::ToData( ndn::EncodingBuffer& contentBuf ) const {
    // Prepend random block of data of size m_contentSize.
    uint8_t content[m_contentSize];
    contentBuf.prependByteArrayBlock( TLV_DATA, content, m_contentSize );
    
    // Flag to indicate if the request was valid or not.
    ndn::encoding::prependNonNegativeIntegerBlock(
        contentBuf,
        TLV_WAS_DENIED,
        m_wasDenied
    );
    
    // The group this response belongs to, used by the routers
    // to validate an AuthTag's coverage of the content.
    contentBuf.prependBlock( m_authGroup.Encode() );
    
    auto data = Response::ToData( contentBuf );
    data->setFinalBlockId( ndn::Name::Component::fromSegment( m_lastSeg ) );
    return data;
}

void
DataResponse::FromData( ndn::Data const& data ) {
    ndn::Block const& content = data.getContent();
    m_contentSize = content.value_size();
    m_authGroup.Decode( content.get( TLV_AUTH_GROUP ) );
    m_wasDenied =
        ndn::encoding::readNonNegativeInteger(
            content.get( TLV_WAS_DENIED )
        );
    m_lastSeg = data.getFinalBlockId().toSegment();
    Response::FromData( data );
}

bool
DataResponse::IsDataData( ndn::Data const& data ) {
    ndn::Name const& dname = data.getName();
    if( dname.get( 0 ) != KEYWORD_TACTIC )
        return false;
    
    auto it = std::find( dname.begin(), dname.end(), KEYWORD_DATA );
    if( it == dname.end() )
        return false;
    
    return false;
}

}
