/* 
 * File:   Runnable.h
 * Author: majo32
 *
 * Created on January 26, 2016, 10:04 PM
 */
#pragma once
#ifndef SUNSTORM_RUNNABLE_H
#define	SUNSTORM_RUNNABLE_H

namespace SunStorm {

    class Runnable {
    public:
        Runnable(){
        
        }
        Runnable(const Runnable& orig){
        
        }
        virtual ~Runnable(){
        
        }
        virtual void Run(void)= 0;
    private:

    };
}
#endif	/* RUNNABLE_H */

