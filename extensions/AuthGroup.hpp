#ifndef _AuthGroup_
#define _AuthGroup_

#include "ns3/ndnSIM-module.h"
#include "ns3/core-module.h"
#include "ns3/object.h"
#include "ns3/ndnSIM/apps/ndn-app.hpp"
#include "ns3/ndnSIM/utils/ndn-rtt-estimator.hpp"
#include "Tlv.hpp"

namespace tactic {

class AuthGroup {
    friend class AuthTag;
    public:
        AuthGroup( void );
        AuthGroup( unsigned level );
        
        ndn::Name const&
        GetName( void );
        
        ndn::Block
        Encode( void ) const;
        
        void
        Decode( ndn::Block const& blk );
    private:
        unsigned   m_level;
        ndn::Name  m_name;
        ndn::Block m_block;
};

}

#endif
