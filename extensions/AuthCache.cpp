#include "AuthCache.hpp"

namespace tactic {

NS_OBJECT_ENSURE_REGISTERED( AuthCache );

ns3::TypeId
AuthCache::GetTypeId( void ) {
    static ns3::TypeId tid = 
        ns3::TypeId( "tactic::AuthCache" )
        .AddTraceSource(
            "NumResets",
            "Number of cache resets triggered by overfills",
            ns3::MakeTraceSourceAccessor( &AuthCache::m_numResets ),
            "ns3::TracedValueCallback::UInt"
        )
        .AddTraceSource(
            "NumInserts",
            "Number of cache inserts that have occured, this "
            "does not reset when the cache resets.",
            ns3::MakeTraceSourceAccessor( &AuthCache::m_numInserts ),
            "ns3::TracedValueCallback::UInt"
        )
        .AddTraceSource(
            "NumFailedLookups",
            "Number of lookup failures",
            ns3::MakeTraceSourceAccessor( &AuthCache::m_numFailedLookups ),
            "ns3::TracedValueCallback::UInt"
        )
        .AddTraceSource(
            "NumSuccessfulLookups",
            "Number of lookup successes",
            ns3::MakeTraceSourceAccessor( &AuthCache::m_numSuccessfulLookups ),
            "ns3::TracedValueCallback::UInt"
        );
        
        return tid;
}

ns3::TypeId
AuthCache::GetInstanceTypeId( void ) const {
    return GetTypeId();
}

AuthCache::AuthCache( void )
    : m_cap( 1000 )
    , m_fill( 0 )
    , m_numResets( 0 )
    , m_numInserts( 0 )
    , m_numFailedLookups( 0 )
    , m_numSuccessfulLookups( 0 )
{
    bloom::bloom_parameters params;
    params.projected_element_count = m_cap;
    params.false_positive_probability = 0.0001;
    params.random_seed = 123456;
    params.compute_optimal_parameters();
    m_bf = bloom::bloom_filter( params );
}

AuthCache::AuthCache( size_t cap )
    : m_cap( cap )
    , m_fill( 0 )
    , m_numResets( 0 )
    , m_numInserts( 0 )
    , m_numFailedLookups( 0 )
    , m_numSuccessfulLookups( 0 )
{
    bloom::bloom_parameters params;
    params.projected_element_count = m_cap;
    params.false_positive_probability = 0.0001;
    params.random_seed = 123456;
    params.compute_optimal_parameters();
    m_bf = bloom::bloom_filter( params );
}

void
AuthCache::Insert( AuthTag const& authTag ) {
    if( m_fill > m_cap ) {
        m_bf.clear();
        m_fill = 0;
        m_numResets++;
    }
    
    m_fill++;
    ndn::Block const& blk = authTag.Encode();
    m_bf.insert( (char*)blk.wire(), blk.size() );
}

bool
AuthCache::Has( AuthTag const& authTag )  const {
    ndn::Block const& blk = authTag.Encode();
    bool has = m_bf.contains( (char*)blk.wire(), blk.size() );
    if( has )
        m_numSuccessfulLookups++;
    else
        m_numFailedLookups++;
    return has;
}

double
AuthCache::FalsePositiveRate( void ) const {
    return m_bf.effective_fpp();
}



}
