/* 
 * File:   FunctionChain.h
 * Author: majo32
 *
 * Created on Streda, 2016, apríla 6, 14:37
 */
#pragma once
#ifndef SUNSTORM_FUNCTIONCHAIN_H
#define	SUNSTORM_FUNCTIONCHAIN_H
#include <list>

#include "Function.h"
namespace SunStorm {

    template<typename ... _Args>
    class FunctionComposer {
    private:

        class FunctionHolder {
        public:

            FunctionHolder(Function<_Args ...>* function) :
            function(function) {
            }

            long GetOrdNum() {
                return 0;
            };
            
            Function<_Args ...>* GetFunction() const {
                return function;
            }
        private:
            Function<_Args ...> * function;
        };
        
    public:

        FunctionComposer() {
        }
        FunctionComposer(const FunctionComposer& orig) = delete;

        virtual ~FunctionComposer() {
        }

        long AddFunction(Function<_Args ...> & function) {
            functions.push_back(FunctionHolder(&function));
            return functions.size() - 1;
        }

        void Run(_Args ... __args) {
            auto it = functions.begin();
            while (it != functions.end()) {
                it->GetFunction()->Run(std::forward<_Args>(__args)...);
                ++it;
            }
        }
    private:
        std::list<FunctionHolder> functions;
    };
};
#endif	/* FUNCTIONCHAIN_H */

