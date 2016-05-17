/* 
 * File:   ServiceThread.h
 * Author: majo32
 *
 * Created on Utorok, 2016, marca 8, 16:30
 */
#pragma once
#ifndef SERVICETHREAD_H
#define	SERVICETHREAD_H


namespace SunStorm {

    class ServiceThread : public Thread {
    public:

        ServiceThread(Service& service) :
        Thread(), service(service) {
        }

        Service& GetService() {
            return service;
        }
        
        
    private:
        Service & service;
    };
};
#endif	/* SERVICETHREAD_H */

