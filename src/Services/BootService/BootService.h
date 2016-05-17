/* 
 * File:   BootService.h
 * Author: majo32
 *
 * Created on Streda, 2016, marca 9, 13:27
 */

#ifndef BOOTSERVICE_H
#define	BOOTSERVICE_H

#include <SunStorm/Framework.h>
#include "BootService_messages.h"

namespace Services {

    class BootService : public SunStorm::Service {
    public:

        BootService(std::string name) :
        Service(name) {
        }
        void Initialize();

    private:

    };
};
#endif	/* BOOTSERVICE_H */

