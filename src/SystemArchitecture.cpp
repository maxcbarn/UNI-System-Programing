#include "SystemArchitecture.hpp"

int TwoComplementViwer( Word data ) {
    if ( data & 0x80 ) {
        return -( int )( ( ~data & 0xFF ) + 1 );
    }
    return (int)data;
}

int TwoComplementViwer( DoubleWord data ) {
    if ( data & 0x8000 ) {
        return -( int )(( ~data & 0xFFFF ) + 1 );
    }
    return ( int )data;
}

bool EvenParity( Word value ) {
    int count = 0;
    while ( value ) {
        count += value & 1;
        value >>= 1;
    }
    return ( count % 2 ) == 0;
}