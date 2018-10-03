#ifndef _Consumer_
#define _Consumer_

#include "Types.hpp"
#include "AuthTag.hpp"
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

class Consumer : public ns3::ObjectBase {
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
        
        Consumer(
            ndn::Name const& consumerName,
            ndn::Name const& contentName
        );
        
        void
        Start( SPtr<Interface> iface );
        
        void
        Stop( void );
        
        typedef Closure< void( void ) > OnFinishCb;
        void
        DoOnFinish( OnFinishCb const& cb );
        
        typedef Closure< void( SPtr< Response > ) > OnResponseCb;
        void
        DoOnResponse( OnResponseCb const& cb );
        
        typedef Closure< void( SPtr< Request > ) > OnRequestCb;
        void
        DoOnRequest( OnRequestCb const& cb );
        
        typedef Closure< void( void ) > OnFailCb;
        void
        DoOnFail( OnFailCb const& cb );
        
    private:
        
        struct PendingRequest {
            SPtr< Request >  request;
            uint             retxNum;
            ns3::Time        txTime;
            ns3::Time        rxTime;
            ns3::EventId     toEvent;
        };
        
        void
        RequestNext( void );
        
        void
        RequestAuth( void );
        
        void
        FillWindow( void );
        
        void
        RetransmitNext( void );
        
        void
        IncWindow( void );
        
        void
        DecWindow( void );
        
        void
        OnResponse( SPtr< PendingRequest > p, SPtr< Response > r );
        
        void
        OnTimeout( SPtr< PendingRequest > p );
        
        void
        OnFailure( void );
    private:
        
        // The consumer uses a windowing strategy for
        // congestion control.  This indicates how large
        // the current window is.
        uint m_windowSize;
        
        // This keeps track of how many pending requests
        // there are (i.e how many window slots are filled)
        uint m_windowFill;
        
        // These indicate the min and max window sizes,
        // the initial window size will be the min size.
        uint m_windowMin;
        uint m_windowMax;
        
        // This is the maximum number of retransmits allowed
        // for an interest before the consuemr will give up.
        uint m_retxMax;
        
        // This determines the lifetime assigned to each interest.
        ns3::Time m_interestLifetime;
        
        // This indicates the current segment number, since
        // a consumer is disposeable (only consumed one content)
        // this also doubles as a sequence number; since the two
        // will always be the same.
        uint m_nextSeg;
        
        // This indicates the number of segments in the content,
        // it'll be set from the FINAL_BLOCK_ID of segment 0, but
        // until then will be set to some very large number.
        uint m_lastSeg;
        
        // These are important portions of the content name,
        // separated into their own portions for convenience.
        // A TACTIC content name always begins with the prefix
        // /TACTIC/ to distinquish TACTIC traffic from normal
        // NDN traffic.  This also allows nodes that don't
        // support the protocol to drop or redirect such packets.
        // Everything after the prefix and before the next
        // /GET/ component is considered to be the producer
        // name; it's important to distinguish the producer's
        // prefix from the content name because we need to
        // know the prefix to know where to request an AuthTag
        // from.  This prefix is also used by the routers to
        // figure out where to send reports of suspision.
        // Anything after the /GET/ is the content name, though
        // for actual interests this'll be followed by a segment
        // number.
        ndn::Name m_producerName;
        ndn::Name m_consumerName;
        ndn::Name m_contentName;
        
        
        // The current auth tag if it's already been retrieved.
        Maybe<AuthTag> m_authTag;
        
        // Random number generator for interest nonce values.
        ns3::Ptr<ns3::UniformRandomVariable> m_nonceRng;
        
        // RTT estimator.
        ns3::Ptr< ns3::RttMeanDeviation > m_rttEst;
        
        // Face to use for requests.
        SPtr< Interface > m_iface;
        
        // Retransmit queue.
        Queue< SPtr< PendingRequest > > m_retx;
        
        // Callbacks.
        List< OnFinishCb >   m_onFinish;
        List< OnResponseCb > m_onResponse;
        List< OnRequestCb >  m_onRequest;
        List< OnFailCb >     m_onFail;
        
        // Traced values.
        ns3::TracedValue< uint >      m_numSegmentsRequested;
        ns3::TracedValue< uint >      m_numSegmentsReceived;
        ns3::TracedValue< uint >      m_numTimeoutsOccured;
        ns3::TracedValue< ns3::Time > m_sumResponseDelay;
};

}
#endif
