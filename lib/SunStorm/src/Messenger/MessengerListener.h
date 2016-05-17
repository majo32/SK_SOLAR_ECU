/* 
 * File:   Listener.h
 * Author: majo32
 *
 * Created on January 26, 2016, 9:55 PM
 */
#pragma once
#ifndef SUNSTORM_MESSENGERLISTENER_H
#define	SUNSTORM_MESSENGERLISTENER_H



#include "Message.h"


namespace SunStorm {

    class Listener {
    public:

        Listener() {

        }
        Listener(const Listener& orig) = delete;

        virtual ~Listener() {

        }
        virtual void handle(const Message & message) = 0;



    private:
        
    };
}
#endif	/* LISTENER_H */

