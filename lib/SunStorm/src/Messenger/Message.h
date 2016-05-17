/* 
 * File:   Message.h
 * Author: majo32
 *
 * Created on January 26, 2016, 9:54 PM
 */
#pragma once
#ifndef SUNSTORM_MESSAGE_H
#define	SUNSTORM_MESSAGE_H


#include <memory>
#include <string>


namespace SunStorm {

    class MessageIndentifier {
    public:

        MessageIndentifier() :
        group(-1), reciever(-1), name() {
        }

        MessageIndentifier(std::string name, long group, long reciever) :
        group(group), reciever(reciever), name(name) {
        }

        MessageIndentifier(long group, long reciever) :
        group(group), reciever(reciever), name() {
        }

        MessageIndentifier(std::string name) :
        group(-1), reciever(-1), name(name) {
        }

        std::string GetName() const {
            return name;
        }
        
        void SetName(std::string name) {
            this->name = name;
        }


    private:
        long group;
        long reciever;
        std::string name;
    };

    class Message {
    public:

        Message() : used(false), identifier() {

        }

        Message(std::string name) : used(false), identifier(name) {

        }

        Message(const Message& orig) : used(orig.used), content(orig.content), identifier(orig.identifier) {

        }

        virtual ~Message() {

        }

        template <typename T> void SetContent(const T& c) {
            used = true;
            content = std::shared_ptr<void>(new T(c));
           
        };

        void SetContent(std::shared_ptr<void> c) {
            used = true;
            content = c;
        };

        template <typename T> const T GetContent(void) const {
            return *((T*) content.get());
        };

        template <typename T> const T* GetContentPtr(void) const {
            return ((T*) content.get());
        };

        std::shared_ptr<void> GetContentSharedPtr(void) const {
            return content;
        };

        std::string GetName() const {
            return identifier.GetName();
        };

        const MessageIndentifier & GetIdentifier() const {
            return identifier;
        }


    private:
        bool used;
        std::shared_ptr<void> content;
        MessageIndentifier identifier;
    };
}
#endif	/* MESSAGE_H */

