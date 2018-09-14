#include "AuthTag.hpp"

namespace tactic {

using namespace ::std;
using namespace ::ndn;
using namespace ::ns3;
using namespace ::boost;
using namespace ::ns3::ndn;
using namespace ::ndn::time;
using namespace ::ndn::encoding;

Block
AuthTag::Encode( void ) const {

    if( m_block.hasWire() )
        return m_block;
    
    EncodingBuffer encoder;
    size_t         totalSize = 0;
    
    // Signature
    totalSize += encoder.prependBlock( m_signature.getValue() );
    totalSize += encoder.prependBlock( m_signature.getInfo() );
    
    // NotAfter
    totalSize += prependNonNegativeIntegerBlock(
                    encoder,
                    TLV_NOT_AFTER,
                    toUnixTimestamp( m_notAfter ).count()
                 );
    
    
    // NotBefore
    totalSize += prependNonNegativeIntegerBlock(
                    encoder,
                    TLV_NOT_BEFORE,
                    toUnixTimestamp( m_notBefore ).count()
                 );
    
    // UserName
    totalSize += encoder.prependBlock(
                    Block(
                        TLV_USER_NAME,
                        m_userName.wireEncode()
                    )
                 );
    
    // OwnerName
    totalSize += encoder.prependBlock(
                    Block( 
                        TLV_OWNER_NAME,
                        m_ownerName.wireEncode()
                    )
                 );
    
    // AuthGroup
    totalSize += encoder.prependBlock( m_group.Encode() );
    
    // AuthTag
    encoder.prependVarNumber( totalSize );
    encoder.prependVarNumber( TLV_AUTH_TAG );
    
    m_block = encoder.block();
    return m_block;
}

void
AuthTag::Decode( Block const& blk ) {
    blk.parse();
    
    m_block = blk;
    
    m_group.Decode( blk.get( TLV_AUTH_GROUP ) );
        
    m_ownerName.wireDecode(
        blk.get( TLV_OWNER_NAME ).blockFromValue()
    );
    
    m_userName.wireDecode(
        blk.get( TLV_USER_NAME ).blockFromValue()
    );
    
    m_notBefore = fromUnixTimestamp(
                    chrono::milliseconds(
                        readNonNegativeInteger(
                            blk.get( TLV_NOT_BEFORE )
                        )
                    )
                  );
    
    m_notAfter = fromUnixTimestamp(
                    chrono::milliseconds(
                        readNonNegativeInteger(
                            blk.get( TLV_NOT_AFTER )
                        )
                    )
                 );
    m_signature.setInfo( blk.get( tlv::SignatureInfo ) );
    m_signature.setValue( blk.get( tlv::SignatureValue ) );
}
void
AuthTag::SetOwnerName( Name const& name ) {
    m_ownerName = name;
    m_block.reset();
}

void
AuthTag::SetUserName( Name const& name ) {
    m_userName = name;
    m_block.reset();
}

void
AuthTag::SetAuthGroup( AuthGroup const& grp ) {
    m_group = grp;
    m_block.reset();
}

void
AuthTag::SetNotBefore( system_clock::TimePoint t ) {
    m_notBefore = t;
    m_block.reset();
}

void
AuthTag::SetNotAfter( system_clock::TimePoint t ) {
    m_notAfter = t;
    m_block.reset();
}

void
AuthTag::SetSignature( Signature const& sig ) {
    m_signature = sig;
}

AuthGroup const&
AuthTag::GetAuthGroup( void ) const {
    return m_group;
}
        
Name const&
AuthTag::GetOwnerName( void ) const {
    return m_ownerName;
}
        
Name const&
AuthTag::GetUserName( void ) const {
    return m_userName;
}
        
::ndn::time::system_clock::TimePoint
AuthTag::GetNotBefore( void ) const {
    return m_notBefore;
}
        
::ndn::time::system_clock::TimePoint
AuthTag::GetNotAfter( void ) const {
    return m_notAfter;
}
        
Signature const&
AuthTag::GetSignature( void ) const {
    return m_signature;
}

bool
AuthTag::IsActive( void )  const {
    auto now = ::ndn::time::system_clock::now();
    return now >= m_notBefore && now <= m_notAfter;
}

bool
AuthTag::Covers( AuthGroup const& grp ) const {
    return m_group.m_level >= grp.m_level;
}

}
