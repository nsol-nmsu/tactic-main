#ifndef _DataRequest_
#define _DataRequest_
#include "Request.hpp"
#include "AuthTag.hpp"

namespace tactic {

class DataRequest : public Request {
    public:
        
        DataRequest(
            ndn::Name const& producerPrefix,
            ndn::Name const& contentPostfix,
            AuthTag const&   authTag,
            uint             seg
        );
        
        DataRequest( ndn::Interest const& interest );
        
        AuthTag const&
        GetAuthTag( void ) const;
        
        void
        SetAuthProb( double prob );
        
        double
        GetAuthProb( void ) const;
        
        Request::Type
        GetType( void ) const override;
        
        SPtr< ndn::Interest >
        ToInterest( ndn::EncodingBuffer& contentBuf ) const override;
        
        void
        FromInterest( ndn::Interest const& interest ) override;
        
        static bool
        IsDataInterest( ndn::Interest const& interest );
    protected:
        double   m_authProb;
        AuthTag  m_authTag;
};

}
#endif
