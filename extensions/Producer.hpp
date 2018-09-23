#ifndef _Producer_
#define _Producer_

#include "Types.hpp"
#include "AuthTag.hpp"
#include "AuthGroup.hpp"
#include "AuthRequest.hpp"
#include "DataRequest.hpp"
#include "AuthResponse.hpp"
#include "DataResponse.hpp"
#include "Interface.hpp"

#include <ns3/ndnSIM-module.h>
#include <ns3/core-module.h>
#include <ns3/rtt-estimator.h>

#include <algorithm>
#include <stdexcept>

namespace tactic {

class Producer : public ns3::ObjectBase {
    public:
    
        class Error : public std::runtime_error {
            public:
                Error( std::string const& what )
                    : std::runtime_error( what )
                {};
        };
        
        static ns3::TypeId
        GetTypeId( void );
        
        ns3::TypeId
        GetInstanceTypeId( void ) const override;
        
        Producer(
            ndn::Name const& producerName,
            size_t           segmentSize,
            AuthGroup const& authGroup
        );
        
        void
        Add( ndn::Name const& name, size_t size );
        
        SPtr<Response>
        Get( Request const& request ) const;
        
    private:
    
        ndn::Name m_producerName;
        size_t    m_segmentSize;
        AuthGroup m_authGroup;
        
        NameMap< size_t > m_contentSizes;
        
        mutable ns3::TracedValue< uint > m_numRequestsReceived;
        mutable ns3::TracedValue< uint > m_numRequestsSatisfied;
        mutable ns3::TracedValue< uint > m_numRequestsDenied;
};

}

#endif
