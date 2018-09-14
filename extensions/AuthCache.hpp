#ifndef _AuthCache_
#define _AuthCache_
#include "AuthTag.hpp"
#include "Types.hpp"

#include <ns3/ndnSIM-module.h>
#include <ns3/core-module.h>

namespace tactic {

namespace bloom {
#include "bloom/bloom_filter.hpp"
}

class AuthCache : public ns3::ObjectBase {
    public:
        
        static ns3::TypeId
        GetTypeId( void );
        
        ns3::TypeId
        GetInstanceTypeId( void ) const override;
        
        AuthCache( void );
        
        AuthCache( size_t cap );
        
        void
        Insert( AuthTag const& authTag );
        
        bool
        Has( AuthTag const& authTag ) const;
        
        double
        FalsePositiveRate( void ) const;
    private:
        size_t const        m_cap;
        size_t              m_fill;
        bloom::bloom_filter m_bf;
        
        mutable ns3::TracedValue< uint > m_numResets;
        mutable ns3::TracedValue< uint > m_numInserts;
        mutable ns3::TracedValue< uint > m_numFailedLookups;
        mutable ns3::TracedValue< uint > m_numSuccessfulLookups;
};

}
#endif
