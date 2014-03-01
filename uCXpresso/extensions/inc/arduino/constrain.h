/*
||
|| @file Constrain.h
|| @version 1.2
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Provide an easy way of making constrained variables
|| #
||
|| @license
|| | Copyright (c) 2009 Alexander Brevig. All rights reserved.
|| | This code is subject to AlphaLicence.txt
|| | alphabeta.alexanderbrevig.com/AlphaLicense.txt
|| #
||
*/

#ifndef CONSTRAIN_H
#define CONSTRAIN_H

//#include "WProgram.h"

template< typename T >
struct Constrained {
    Constrained( T v , T mn , T mx ) : value(v),minimum(mn),maximum(mx) {}
    T value; //hold the current value of the constrained variable
    T minimum; //minimum value for this variable
    T maximum; //maximum value for this variable
    T &operator=(T set){ 
        if (set<minimum){
            value = minimum;
        }else if (set>maximum){
            value = maximum;
        }else if (set<=maximum && set>=minimum){
            value = set; 
        }
        return value; 
    }
    void operator=( Constrained& right ){
        value = right.value;
        minimum = right.minimum;
        maximum = right.maximum;
    }
    Constrained &operator++(){
        if (value+1<=maximum){ value++; }
        return *this;
    }
    Constrained operator++( int ){
        if (value+1<=maximum){ value++; }
    }
    Constrained &operator--(){
        if (value-1>=minimum){ value--; }
        return *this;
    }
    Constrained operator--( int ){
        if (value-1>=minimum){ value--; }
    }
    const Constrained &operator+=( T val){
        value += val;
        value = constrain( value, minimum, maximum );
        return *this;
    }
    const Constrained &operator-=( T val){
        value -= val;
        value = constrain( value, minimum, maximum );
        return *this;
    }
	bool operator<( const T i ) const{
		return value < i;
	}
	bool operator<=( const T i ) const{
		return !(value > i);
	}
	bool operator>( const T i ) const{
		return value > i;
	}
	bool operator>=( const T i ) const{
		return !(value < i);
	}
	
	bool operator==( const T i ) const{
		return value == i;
	}

	operator T() { return value; }
	operator int() { return value; }
	operator byte() { return value; }
	operator char() { return value; }
	operator long() { return value; }
}; //end Constrained

#endif

/*
|| @changelog
|| | 1.2 2009-05-01 - Alexander Brevig : Added boolean comarison operators
|| | 1.1 2009-05-01 - Alexander Brevig : Added conversion operators
|| | 1.0 2009-04-21 - Alexander Brevig : Initial Release
|| #
*/
