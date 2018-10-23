#ifndef _Request_
#define _Request_

#include "Types.hpp"
#include "Keywords.hpp"
#include "DummySignature.hpp"
#include "Tlv.hpp"

#include <ns3/ndnSIM-module.h>
#include <ns3/core-module.h>

#include <stdexcept>

namespace tactic {

class Request {
    public:
        class Error : public std::runtime_error {
            public:
                Error( std::string const& what )
                    : std::runtime_error( what )
                {};
        };
        
        enum Type {
            TYPE_NONE,
            TYPE_DATA,
            TYPE_AUTH
        };
        
        Request( void ){};
        
        Request( ndn::Name const& name );
        
        Request( ndn::Interest const& interest );
        
        ndn::Name const&
        GetName( void ) const;
        
        void
        UpdateRouteHash( uint64_t node );
        
        uint64_t
        GetRouteHash( void ) const;
        
        ndn::Signature const&
        GetSignature( void ) const;
        
        virtual Type
        GetType( void ) const = 0;
        
        SPtr< ndn::Interest >
        ToInterest( void ) const;
        
        virtual SPtr< ndn::Interest >
        ToInterest( ndn::EncodingBuffer& contentBuf ) const;
        
        virtual void
        FromInterest( ndn::Interest const& interest );
        
    protected:
        ndn::Name                   m_name;
        uint32_t                    m_nonce;
        ndn::time::milliseconds     m_lifetime;
        uint64_t                    m_route;
        ndn::Signature              m_sig;
    private:
        static ns3::Ptr< ns3::UniformRandomVariable > s_nonceRng;
        static ndn::time::milliseconds                s_lifetime;
};

}
#endif
