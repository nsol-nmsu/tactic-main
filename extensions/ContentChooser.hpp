#ifndef _ContentChooser_
#define _ContentChooser_

#include <ns3/ndnSIM-module.h>
#include <ns3/core-module.h>
#include <ns3/rtt-estimator.h>

namespace tactic {

class ContentChooser : public ns3::Object {
    public:
        virtual ndn::Name
        NextName( void ) = 0;
        
        virtual ndn::Time
        NextDelay( void ) = 0;
};

}
#endif
