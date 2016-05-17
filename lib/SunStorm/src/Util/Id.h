/* 
 * File:   Id.h
 * Author: majo32
 *
 * Created on Utorok, 2016, februára 16, 20:10
 */
#pragma once
#ifndef SUNSTORM_ID_H
#define	SUNSTORM_ID_H

namespace SunStorm {

    class IDHandler {
    public:

        IDHandler() :
        next(0) {
        }

        long getNext() {
            return next++;
        }

        static IDHandler & getInstance() {
            static IDHandler instance;

            return instance;
        }

    private:
        long next;
    };

    class IDClass {
    public:

        IDClass(): __id(IDHandler::getInstance().getNext()){
            
        }
        IDClass(const IDClass& orig): __id(orig.__id){
        
        }
        
        virtual ~IDClass() {
        }
        
        long GetID() const {
            return __id;
        }
    private:
        const long __id;
    };
};

#endif	/* ID_H */

