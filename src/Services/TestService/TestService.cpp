/* 
 * File:   TestService.cpp
 * Author: majo32
 * 
 * Created on Utorok, 2016, februára 16, 15:58
 */

#include "TestService.h"
#include <iostream>

/**
 * registruju sa listenery : meno správy, instancia listeneru
 */


void Services::TestService::Initialize() {
    this->RegisterListener("L1_MSG", this->l1);
    long l = this->RegisterListener("L2_MSG", this->l2);
    this->RegisterInterval(2000, this->l3);
    this->RemoveListener(l);

    

}

/**
 * vykoná sa ked príde správa, na ktorú počúva
 * @param message
 */
void Services::TestService::Listener_1::handle(const SunStorm::Message& message) {
    std::string s = message.GetContent<std::string>(); // vytiahnutia obsahu správy, <std::string> je typ ktorý viem ze obsahuje tá správa (zvycajne classa)

    Log().debug("L1 with message :" + s);


    SunStorm::Message m("L2_MSG"); // vytvorenie novej správy z názvom L2_MSG
    Services::TestService::MSG_A a({5, 4}); // vytvorenie nejakej struktury obsahu (zadefinovane v hlavickovom subore)

    this->ctx.a++; // priklad práci s kontextom
    a.k = this->ctx.a;

    m.SetContent<Services::TestService::MSG_A>(a); // nastavenie obsahu pre správu , v <Services::TestService::MSG_A> typ obsahu

    this->SendMessage(m); // odoslanie správy

}

/**
 * vykoná sa ked príde správa, na ktorú počúva
 * @param message
 */
void Services::TestService::Listener_2::handle(const SunStorm::Message& message)  {
    //Services::TestService::MSG_A a = message.GetContent<Services::TestService::MSG_A>();

    //std::cout << " L2 : k->" << a.k << " l-> " << a.l << std::endl;
}

void Services::TestService::Listener_3::handle(const SunStorm::Message& message)  {

    Log().debug("L3");
}