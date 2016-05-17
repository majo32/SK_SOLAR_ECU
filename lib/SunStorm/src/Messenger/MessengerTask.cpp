/* 
 * File:   Task.cpp
 * Author: majo32
 * 
 * Created on January 26, 2016, 9:55 PM
 */

#include "MessengerTask.h"

#include <iostream>

void SunStorm::MessengerTask::Run(void){
    listener->handle(message);
}
