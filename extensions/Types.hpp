#ifndef _Types_
#define _Types_

#include <memory>
#include <stdexcept>
#include <queue>
#include <unordered_map>
#include "ns3/ndnSIM-module.h"


namespace tactic {

template< class T >
using SPtr = std::shared_ptr< T >;

template< class T >
using UPtr = std::unique_ptr< T >;

template< class T >
using NameMap = std::unordered_map< ndn::Name, T >;

template< class T >
using IntMap = std::unordered_map< int, T >;

template< class T >
using Queue = std::queue< T >;

template< class T >
using List = std::list< T >;

template< class T >
using Closure = std::function< T >;

typedef unsigned char uchar;
typedef unsigned int  uint;
typedef unsigned long ulong;

template< class T >
class Maybe {
    public:
        class Error : public std::runtime_error {
            public:
                Error( std::string const& what )
                    : std::runtime_error( what )
                {};
        };
        
        Maybe( T& thing )
            : m_thing( UPtr<T>( new T( thing ) ) )
            , m_has( true )
        {};
        
        Maybe( void )
            : m_thing( nullptr )
            , m_has( false )
        {};
        
        T const&
        operator *( void ) const {
            if( !m_has )
                throw Error( "Attempt to dereference empty Maybe" );
            return *m_thing;
        }
        
        T&
        operator *( void ) {
            if( !m_has )
                throw Error( "Attempt to dereference empty Maybe" );
            return *m_thing;
        }
        
        T const*
        operator ->( void ) const {
            return m_thing.get();
        }
        
        T*
        operator ->( void ) {
            return m_thing.get();
        } 
        
        operator bool( void ) const {
            return m_has;
        }
    
    private:
        UPtr<T> m_thing;
        bool    m_has;
};

}

#endif
