#include "DataResponse.hpp"

namespace tactic {

DataResponse::DataResponse(
    ndn::Name const& producerName,
    ndn::Name const& contentName,
    size_t           contentSize,
    uint             lastSeg
)
    : Response(
        ndn::Name( producerName )
        .append( KEYWORD_DATA )
        .append( contentName )
      )
    , m_size( contentSize )
    , m_lastSeg( lastSeg )
{}

Response::Type
DataResponse::GetType( void ) const {
    return Response::TYPE_DATA;
}

size_t
DataResponse::GetSize( void ) const {
    return m_size;
}

uint
DataResponse::GetLastSeg( void ) const {
    return m_lastSeg;
}
 

SPtr< ndn::Data >
DataResponse::ToData( ndn::EncodingBuffer& contentBuf ) const {
    // Prepend random block of data of size m_size.
    uint8_t content[m_size];
    contentBuf.prependByteArray( content, m_size );
    
    auto data = Response::ToData( contentBuf );
    data->setFinalBlockId( ndn::Name::Component::fromSegment( m_lastSeg ) );
    return data;
}

void
DataResponse::FromData( ndn::Data const& data ) {
    ndn::Block const& content = data.getContent();
    m_size    = content.value_size();
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
