#ifndef _ConsumerApp_
#define _ConsumerApp_

#include "Types.hpp"
#include "AuthTag.hpp"
#include "AuthRequest.hpp"
#include "DataRequest.hpp"
#include "AuthResponse.hpp"
#include "DataResponse.hpp"
#include "Interface.hpp"
#include "ContentChooser.hpp"
#include "Consumer.hpp"

#include <ns3/ndnSIM-module.h>
#include <ns3/core-module.h>
#include <ns3/rtt-estimator.h>

namespace tactic {

class ConsumerApp : public ns3::Application {
    public:
    
        static ns3::TypeId
        GetTypeId( void );
        
        ConsumerApp( void );
        
        void
        StartApplication( void ) override;
        
        void
        StopApplication( void ) override;
    
    private:
    
        void
        ConsumeNext( void );
        
        void
        ScheduleNext( void );
        
    private:
    
        ndn::Name                  m_consumerName;
        SPtr< Interface >          m_iface;
        UPtr< Consumer >           m_consumer;
        ns3::Ptr< ContentChooser > m_contentChooser;
        ns3::EventId               m_nextEvent;
        ns3::TracedValue< uint >   m_numContentsReceived;
        ns3::TracedValue< uint >   m_numContentsFailed;
};

}
#endif
