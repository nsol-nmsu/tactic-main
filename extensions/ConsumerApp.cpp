#include "ConsumerApp.hpp"

namespace tactic {

NS_OBJECT_ENSURE_REGISTERED( ConsumerApp );

ns3::TypeId
ConsumerApp::GetTypeId( void ) {
    static ns3::TypeId tid =
        ns3::TypeId( "tactic::ConsumerApp" )
        .SetParent< ns3::ndn::App >()
        .AddConstructor<ConsumerApp>()
        .AddAttribute(
            "ContentChooser",
            "Object of class implementing ContentChooser interface "
            "which is a generic type for controlling the order and "
            "names of requested contents.",
            ns3::PointerValue( nullptr ),
            ns3::MakePointerAccessor( ns3::m_contentChooser ),
            ns3::MakePointerChecker< ConsumerApp >()
        )
        .AddAttribute(
            "ConsumerName",
            "Name of the consumer, used for authentication and such",
            ns3::ndn::NameValue( "/" ),
            ns3::ndn::MakeNameAccessor( ns3::consumerName ),
            ns3::ndn::MakeNameChecker()
        )
        .AddTraceSource(
            "NumContentsReceived",
            "Number of full contents received by the app",
            ns3::MakeTraceSourceAccessor(
                &Consumer::m_numContentsReceived
            ),
            "ns3::TracedValueCallback::UInt"
        )
        .AddTraceSource(
            "NumContentsFailed",
            "Number of contents that couldn't be retrieved",
            ns3::MakeTraceSourceAccessor(
                &Consumer::m_numContentsFailed
            ),
            "ns3::TracedValueCallback::UInt"
        );
    return tid;
}

ConsumerApp::ConsumerApp( void )
    : m_numContentsReceived( 0 )
    , m_numContentsFailed( 0 )
{}

void
ConsumerApp::StartApplication( void ) {
    ScheduleNext();
}

void
ConsumerApp::StopApplication( void ) {
    m_nextEvent.Cancel();
}

void
ConsumerApp::ScheduleNext( void ) {
    m_nextEvent =
        ns3::Simulator::Schedule(
            m_contentChooser->NextDelay(),
            &ConsumerApp::ConsumeNext,
            this
        );
}

class NfdInterface : public Interface {
    public:
        NfdInterface( ndf::face::Face& face );
        
        void
        Submit( SPtr< Request > req, ResponseCb rCb ) override;
        
    private:
        ndn::Face m_face;
}

NfdInterface::NfdInterface( nfd::face::Face& face )
    : m_face( face.getTransport() )
{}

void
NfdInterface::Submit( SPtr< Request > req, ResponseCb res ) {
    using namespace ::ndn;
    
    auto onData =
        [&]( Interest const& interest, Data const& data ) {
            auto response = make_shared< DataResponse >( data );
            res( std::dynamic_pointer_cast< Response >( response ) );
        };
    auto onNack =
        [-]( Interest const& interest, ip::Nack const& nack ) {};
    auto onTimeout =
        [-]( Interest const& interest ){};
    
    m_face.expressInterest(
        req->ToInterest(),
        onData,
        onNack,
        onTimeout
    );
}

void
ConsumerApp::ConsumeNext( void ) {
    m_consumer = Consumer( m_consumerName, m_contentChooser->NextName() );
    
    auto onFinish =
        [this]( void ) {
            ScheduleNext();
            m_numContentsReceived++;
        };
    m_consumer.DoOnFinish( onFinish );
    
    auto onFail =
        [this]( void ) {
            ScheduleNext();
            m_numContentsFailed++;
        };
    m_consumer.DoOnFail( onFail );
    
    auto iface = std::make_shared<NfdInterface)( *m_face );
    m_consumer.Start( std::dynamic_pointer_cast< Interface >( iface ) );
}

}
