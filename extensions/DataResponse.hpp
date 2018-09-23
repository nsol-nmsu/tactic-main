#ifndef _DataResponse_
#define _DataResponse_
#include "Response.hpp"
#include "AuthGroup.hpp"

namespace tactic {

class DataResponse : public Response {
    public:
        
        DataResponse(
            ndn::Name const& producerName,
            ndn::Name const& contentName,
            size_t           contentSize,
            AuthGroup const& authGroup,
            uint             lastSeg,
            bool             wasDenied
        );
        
        Response::Type
        GetType( void ) const;
        
        size_t
        GetSize( void ) const;
        
        AuthGroup const&
        GetAuthGroup( void ) const;
        
        uint
        GetLastSeg( void ) const;
        
        
        SPtr< ndn::Data >
        ToData( ndn::EncodingBuffer& contentBuf ) const override;
        
        void
        FromData( ndn::Data const& data ) override;
        
        static bool
        IsDataData( ndn::Data const& data );
    protected:
        size_t      m_contentSize;
        AuthGroup   m_authGroup;
        uint        m_lastSeg;
        bool        m_wasDenied;
};

}
#endif
