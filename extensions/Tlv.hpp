#ifndef _Tlv_
#define _Tlv_

namespace tactic {

enum Tlv {
    TLV_AUTH_GROUP,
    TLV_AUTH_TAG,
    TLV_OWNER_NAME,
    TLV_USER_NAME,
    TLV_NOT_BEFORE,
    TLV_NOT_AFTER,
    TLV_ROUTE_HASH,
    TLV_AUTH_PROB,
    TLV_AUTH_LEVEL,
    TLV_DATA,
    TLV_DATA_CONTENT,
    TLV_INTEREST_CONTENT,
    TLV_WAS_DENIED
};

}

#endif
