/* 
 * File:   Function.h
 * Author: majo32
 *
 * Created on Streda, 2016, apríla 6, 14:37
 */
#pragma once
#ifndef SUNSTORM_FUNCTION_H
#define	SUNSTORM_FUNCTION_H

#include <functional>

namespace SunStorm {

    template<typename ... _Args>
    class Function {
    public:

        Function() {
        }
        Function(const Function& orig) = delete;

        virtual ~Function() {
        }

        virtual void Run(_Args ... __args) = 0;
    private:

    };
/*
    template<typename ... _Args>
    class FunctionWrapper : public Function<_Args ...> {
    public:

        FunctionWrapper(std::function<void (_Args...) > lambda) :
        Function<_Args ...>(), lambda(lambda) {
        }
        
        void Run(_Args ... __args){
            lambda(__args ...);
        }

    private:
        std::function<void (_Args...) > lambda;

    };
*/
};
#endif	/* FUNCTION_H */

