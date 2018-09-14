#ifndef _AuthTag_
#define _AuthTag_

#include "ns3/ndnSIM-module.h"
#include "ns3/core-module.h"
#include "ns3/object.h"
#include "ns3/ndnSIM/apps/ndn-app.hpp"
#include "ns3/ndnSIM/utils/ndn-rtt-estimator.hpp"
#include "Tlv.hpp"
#include "AuthGroup.hpp"

namespace tactic {

class AuthTag {
    public:
        
        ndn::Block
        Encode( void ) const;
        
        void
        Decode( ndn::Block const& blk );
        
        void
        SetAuthGroup( AuthGroup const& grp );
        
        void
        SetOwnerName( ndn::Name const& n );
        
        void
        SetUserName( ndn::Name const& n );
        
        void
        SetNotBefore( ndn::time::system_clock::TimePoint t );
        
        void
        SetNotAfter( ndn::time::system_clock::TimePoint t );
        
        void
        SetSignature( ndn::Signature const& sig );
        
        AuthGroup const&
        GetAuthGroup( void ) const;
        
        ndn::Name const&
        GetOwnerName( void ) const;
        
        ndn::Name const&
        GetUserName( void ) const;
        
        ndn::time::system_clock::TimePoint
        GetNotBefore( void ) const;
        
        ::ndn::time::system_clock::TimePoint
        GetNotAfter( void ) const;
        
        ndn::Signature const&
        GetSignature( void ) const;
        
        bool
        IsActive( void ) const;
        
        bool
        Covers( AuthGroup const& grp ) const;
        
    private:
        AuthGroup                          m_group;
        ndn::Name                          m_ownerName;
        ndn::Name                          m_userName;
        ndn::time::system_clock::TimePoint m_notBefore;
        ndn::time::system_clock::TimePoint m_notAfter;
        ndn::Signature                     m_signature;
        ndn::Block mutable                 m_block;
};

}
#endif
