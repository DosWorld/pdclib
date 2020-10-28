/* _PDCLIB_bigint_shl( _PDCLIB_bigint_t *, unsigned )

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#ifndef REGTEST

#include "pdclib/_PDCLIB_internal.h"

#include <stdint.h>
#include <string.h>

_PDCLIB_bigint_t * _PDCLIB_bigint_shl( _PDCLIB_bigint_t * lhs, unsigned rhs )
{
    unsigned word_shifts = rhs / 32;
    unsigned bit_shifts = rhs - ( word_shifts * 32 );
    uint_least64_t digit;
    uint_least32_t carry = 0;
    int i;

    if ( lhs->size == 0 )
    {
        return lhs;
    }

    if ( word_shifts > 0 )
    {
        memmove( lhs->data + word_shifts, lhs->data, sizeof( uint_least32_t ) * lhs->size );
        memset( lhs->data, 0, sizeof( uint_least32_t ) * word_shifts );
        lhs->size += word_shifts;
    }

    for ( i = 0 + word_shifts; i < lhs->size; ++i )
    {
        digit = ( (uint_least64_t)lhs->data[i] << bit_shifts ) | carry;
        carry = digit >> 32;
        lhs->data[i] = digit & UINT32_C( 0xFFFFFFFF );
    }

    if ( carry > 0 )
    {
        lhs->data[i] = carry;
        ++lhs->size;
    }

    return lhs;
}

#endif

#ifdef TEST

#include "_PDCLIB_test.h"

int main( void )
{
#ifndef REGTEST
    _PDCLIB_bigint_t lhs;
    _PDCLIB_bigint32( &lhs, UINT32_C( 0x00000012 ) );
    _PDCLIB_bigint_shl( &lhs, 2 );
    TESTCASE( lhs.size == 1 );
    TESTCASE( lhs.data[0] == 0x00000048 );
    _PDCLIB_bigint32( &lhs, UINT32_C( 0x80000001 ) );
    _PDCLIB_bigint_shl( &lhs, 1 );
    TESTCASE( lhs.size == 2 );
    TESTCASE( lhs.data[0] == 0x00000002 );
    TESTCASE( lhs.data[1] == 0x00000001 );
    _PDCLIB_bigint_shl( &lhs, 65 );
    TESTCASE( lhs.size == 4 );
    TESTCASE( lhs.data[0] == 0x00000000 );
    TESTCASE( lhs.data[1] == 0x00000000 );
    TESTCASE( lhs.data[2] == 0x00000004 );
    TESTCASE( lhs.data[3] == 0x00000002 );
#endif
    return TEST_RESULTS;
}

#endif
