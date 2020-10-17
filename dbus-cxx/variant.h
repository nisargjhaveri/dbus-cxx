/***************************************************************************
 *   Copyright (C) 2020 by Robert Middleton                                *
 *   robert.middleton@rm5248.com                                           *
 *                                                                         *
 *   This file is part of the dbus-cxx library.                            *
 *                                                                         *
 *   The dbus-cxx library is free software; you can redistribute it and/or *
 *   modify it under the terms of the GNU General Public License           *
 *   version 3 as published by the Free Software Foundation.               *
 *                                                                         *
 *   The dbus-cxx library is distributed in the hope that it will be       *
 *   useful, but WITHOUT ANY WARRANTY; without even the implied warranty   *
 *   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   *
 *   General Public License for more details.                              *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this software. If not see <http://www.gnu.org/licenses/>.  *
 ***************************************************************************/
#ifndef DBUSCXX_VARIANT_H
#define DBUSCXX_VARIANT_H

#include <dbus-cxx/enums.h>
#include <dbus-cxx/path.h>
#include <dbus-cxx/signature.h>
#include <dbus-cxx/dbus-cxx-config.h>
#include <dbus-cxx/variantappenditerator.h>
#include <dbus-cxx/variantiterator.h>
#include <dbus-cxx/marshaling.h>
#include <dbus-cxx/types.h>
#include <dbus-cxx/error.h>
#include <string>
#include <any>
#include <stdint.h>
#include <ostream>
#include <vector>
#include <map>
#include <tuple>

namespace DBus {

class MessageIterator;
class FileDescriptor;

/**
 * A Variant is a type-safe union for DBus operations.
 *
 * Variants are immutable - once you create them, you cannot change them.
 *
 * To get the value out of the variant, use one of the `to_XXX` methods.  If
 * the type you requested is not possible to get out, an `ErrorBadVariantCast`
 * will be thrown.
 */
class Variant {
public:
    Variant();
    Variant( uint8_t byte );
    Variant( bool b );
    Variant( int16_t i );
    Variant( uint16_t i );
    Variant( int32_t i );
    Variant( uint32_t i );
    Variant( int64_t i );
    Variant( uint64_t i );
    Variant( double i );
    Variant( const char* cstr );
    Variant( std::string str );
    explicit Variant( Signature sig );
    explicit Variant( Path path );

    template<typename T>
    Variant( const std::vector<T>& vec ) :
        m_currentType( DataType::ARRAY ),
        m_signature( DBus::signature( vec ) ),
        m_dataAlignment( 4 ) {
        priv::VariantAppendIterator it( this );

        it << vec;
    }

    template<typename Key, typename Value>
    Variant( const std::map<Key, Value>& map ) :
        m_currentType( DataType::ARRAY ),
        m_signature( DBus::signature( map ) ),
        m_dataAlignment( 4 ) {
        priv::VariantAppendIterator it( this );

        it << map;
    }

    template<typename ...T>
    Variant( const std::tuple<T...>& tup ) :
        m_currentType( DataType::STRUCT ),
        m_signature( DBus::signature( tup ) ),
        m_dataAlignment( 8 ) {
        priv::VariantAppendIterator it( this );
        it << tup;
    }

    Variant( const Variant& other );
    Variant( Variant&& other );
    ~Variant();

    Signature signature() const;

    DataType currentType() const;

    const std::vector<uint8_t>* marshaled() const;

    int data_alignment() const;

    bool operator==( const Variant& other ) const;

    Variant& operator=( const Variant& other );

    template <typename T>
    std::vector<T> to_vector() {
        priv::VariantIterator vi( this );

        std::vector<T> retval = vi;

        return retval;
    }

    template <typename Key, typename Value>
    std::map<Key, Value> to_map() {
        priv::VariantIterator vi( this );

        std::map<Key, Value> retval = vi;

        return retval;
    }

    template <typename ...T>
    std::tuple<T...> to_tuple() {
        std::tuple<T...> tup;
        priv::VariantIterator vi( this );

        tup = vi;

        return tup;
    }

    bool        to_bool() const;
    uint8_t     to_uint8() const;
    uint16_t    to_uint16() const;
    int16_t     to_int16() const;
    uint32_t    to_uint32() const;
    int32_t     to_int32() const;
    uint64_t    to_uint64() const;
    int64_t     to_int64() const;
    double      to_double() const;
    std::string to_string() const;
    DBus::Path  to_path() const;
    DBus::Signature to_signature() const;

    operator bool();
    operator uint8_t();
    operator uint16_t();
    operator int16_t();
    operator uint32_t();
    operator int32_t();
    operator uint64_t();
    operator int64_t();
    operator double();
    operator std::string();
    operator DBus::Path();
    operator DBus::Signature();

    template <typename T>
    operator std::vector<T>() {
        return to_vector<T>();
    }

    template <typename Key, typename Data>
    operator std::map<Key, Data>() {
        return to_map<Key, Data>();
    }

    template <typename... T>
    operator std::tuple<T...>() {
        return to_tuple<T...>();
    }

    static Variant createFromMessage( MessageIterator iter );

private:
    void recurseArray( MessageIterator iter, Marshaling* marshal );
    void recurseDictEntry( MessageIterator iter, Marshaling* marshal );
    void recurseStruct( MessageIterator iter, Marshaling* marshal );

private:
    DataType m_currentType;
    Signature m_signature;
    std::vector<uint8_t> m_marshaled;
    int m_dataAlignment;

    friend std::ostream& operator<<( std::ostream& os, const Variant& var );
    friend class priv::VariantAppendIterator;
    friend class priv::VariantIterator;
};

} /* namespace DBus */

#endif /* DBUSCXX_VARIANT_H */
