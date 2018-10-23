#ifndef _Response_
#define _Response_

#include "Types.hpp"
#include "Keywords.hpp"
#include "DummySignature.hpp"
#include "Tlv.hpp"

#include <ns3/ndnSIM-module.h>
#include <ns3/core-module.h>

#include <stdexcept>

namespace tactic {

class Response {
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
        
        Response( void ) {};
        
        Response( ndn::Name const& name );
        
        ndn::Name const&
        GetName( void ) const;
        
        ndn::Signature const&
        GetSignature( void ) const;
        
        virtual Type
        GetType( void ) const = 0;
        
        SPtr< ndn::Data >
        ToData( void ) const;
        
        virtual SPtr< ndn::Data >
        ToData( ndn::EncodingBuffer& contentBuf ) const;
        
        virtual void
        FromData( ndn::Data const& data );
        
    protected:
        ndn::Name       m_name;
        ndn::Signature  m_sig;
};

}
#endif
