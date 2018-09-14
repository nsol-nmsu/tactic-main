#include "AuthGroup.hpp"

namespace tactic {

using namespace ::std;
using namespace ::ndn;
using namespace ::ns3;
using namespace ::boost;
using namespace ::ns3::ndn;
using namespace ::ndn::time;
using namespace ::ndn::encoding;

AuthGroup::AuthGroup( void )
    : m_level( 0 )
    , m_name( "0" )
{}

AuthGroup::AuthGroup( unsigned level )
    : m_level( level )
    , m_name( std::to_string( level ) )
{}

Name const&
AuthGroup::GetName( void ) {
    return m_name;
}

Block
AuthGroup::Encode( void ) const {
    auto blk = makeNonNegativeIntegerBlock(
                    TLV_AUTH_GROUP ,
                    m_level
               );
    blk.encode();
    return blk;
}

void
AuthGroup::Decode( Block const& blk ) {
    m_level = readNonNegativeInteger( blk );
}

}
