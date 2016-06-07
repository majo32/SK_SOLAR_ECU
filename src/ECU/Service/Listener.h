/* 
 * File:   ECUListener.h
 * Author: majo32
 *
 * Created on Štvrtok, 2016, apríla 28, 14:25
 */
#pragma once
#ifndef ECU_LISTENER_H
#define	ECU_LISTENER_H

#include <SunStorm/Framework.h>

namespace ECU {

    template <class S, class C>
    class Listener : public SunStorm::ServiceListener<S, C> {
    public:

        Listener(S& service, C& context) :
        SunStorm::ServiceListener<S, C>(service, context) {
        }

    };
}

#endif	/* ECULISTENER_H */

