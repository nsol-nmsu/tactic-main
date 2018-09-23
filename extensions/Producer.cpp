#include "Producer.hpp"

namespace tactic {

NS_OBJECT_ENSURE_REGISTERED( Producer );

ns3::TypeId
Producer::GetTypeId( void ) {
    static ns3::TypeId tid =
        ns3::TypeId( "tactic::Producer" )
        .AddTraceSource(
            "NumRequestsReceived",
            "Number of requests received so far",
            ns3::MakeTraceSourceAccessor(
                &Producer::m_numRequestsReceived
            ),
            "ns3::TracedValueCallback::UInt"
        )
        .AddTraceSource(
            "NumRequestsSatisfied",
            "Number of requests satisfied successfully",
            ns3::MakeTraceSourceAccessor(
                &Producer::m_numRequestsSatisfied
            ),
            "ns3::TracedValueCallback::UInt"
        )
        .AddTraceSource(
            "NumRequestsDenied",
            "Number of requests denied",
            ns3::MakeTraceSourceAccessor(
                &Producer::m_numRequestsSatisfied
            ),
            "ns3::TracedValueCallback::UInt"
        );
    return tid;
}

ns3::TypeId
Producer::GetInstanceTypeId( void ) const {
    return GetTypeId();
}

Producer::Producer(
    ndn::Name const& producerName,
    size_t           segmentSize,
    AuthGroup const& authGroup
)
    : m_producerName( producerName )
    , m_segmentSize( segmentSize )
    , m_authGroup( authGroup )
{}

void
Producer::Add( ndn::Name const& name, size_t size ) {
    m_contentSizes[name] = size;
}

SPtr<Response>
Producer::Get( Request const& request ) const {
    m_numRequestsReceived++;
    
    uint      seg  = request.GetName().get( -1 ).toSegment();
    ndn::Name name = request.GetName();
    
    // Remove producer prefix from name.
    name = name.getSubName( m_producerName.size(), ndn::Name::npos );
    
    // Remove segment number from name.
    name = name.getPrefix( -1 );
    
    // Find the content size, if content doesn't exists
    // then return nullptr.
    auto sizeIt = m_contentSizes.find( name );
    if( sizeIt == m_contentSizes.end() )
        return nullptr;
    
    // Size of the segment.  If segment is out of content's
    // range then return nullptr.
    size_t start = seg*m_segmentSize;
    if( start >= sizeIt->second )
        return nullptr;
    size_t size = sizeIt->second - start;
    if( size > m_segmentSize )
        size = m_segmentSize;
    
    // Validate the request.  We don't Nack or drop the
    // request on failure since there may be valid
    // requests in the downstream PITs, instead we just
    // set the wasDenied flag if the request is invalid.
    auto& dreq = std::dynamic_cast<DataRequest const&>( request );
    bool wasDenied = false;
    if( !dreq.GetAuthTag().IsActive() )
        wasDenied = true;
    if( !dreq.GetAuthTag().Covers( m_authGroup ) )
        wasDenied = true;
    if( dreq.GetAuthTag().GetOwnerName() != m_producerName )
        wasDenied = true;
    if( dreq.GetAuthTag().GetSignature() != DUMMY_SIGNATURE )
        wasDenied = true;
    if( dreq.GetSignature() != DUMMY_SIGNATURE )
        wasDenied = true;
    
    if( wasDenied )
        m_numRequestsDenied++;
    else
        m_numRequestsSatisfied++;
    
    return
        std::make_shared<DataResponse>(
            m_producerName,
            name,
            size,
            m_authGroup,
            sizeIt->second / m_segmentSize,
            wasDenied
        );
};

}
