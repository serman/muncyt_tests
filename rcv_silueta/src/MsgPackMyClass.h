//
//  ofxMPSerialized.h
//  nikeLIT
//
//  Created by 堀 宏行 on 2013/01/29.
//
//

#ifndef nikeLIT_ofxMPSerialized_h
#define nikeLIT_ofxMPSerialized_h
#include <msgpack.hpp>
class MsgPackMyClass
{
public:
    unsigned int mousePosX;
    unsigned int mousePosY;
    MsgPackMyClass(){}
    
    MsgPackMyClass( const int& mx, const int& my )
    {
        mousePosX = mx;
        mousePosY = my;
    }
    
    MSGPACK_DEFINE(mousePosX, mousePosY);
};


#endif
