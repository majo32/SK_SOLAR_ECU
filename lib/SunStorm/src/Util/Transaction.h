/* 
 * File:   Transaction.h
 * Author: majo32
 *
 * Created on Štvrtok, 2016, apríla 28, 19:33
 */
#pragma once
#ifndef SUNSTORM_TRANSACTION_H
#define	SUNSTORM_TRANSACTION_H

#include <vector>
#include <mutex>

#include "../FunctionComposer/FunctionComposer.h"
namespace SunStorm {
    template<typename T>
    class Transaction;

    enum TransactionState {
        SUCCESS_OK, SUCCESS_TRANSACTION_FINISHED, ERROR_ROLLBACK, ERROR_MULTIPLE_TRANSACTION_REQUEST, ERROR_NOT_IN_TRANSACTION, ERROR_ITEM_REGISTERED
    };

    template<typename T>
    class TransactionResponse {
    public:

        TransactionResponse(TransactionState state, Transaction<T>& transaction) :
        state(state), transaction(&transaction), transactionId(transaction.GetLastId()), inTransaction(transaction.InTransaction()) {

        }

        TransactionState GetState() const {
            return state;
        }

        Transaction<T>& GetTransaction() const {
            return *transaction;
        }

        long GetTransactionId() const {
            return transactionId;
        }

        bool InTransaction() const {
            return inTransaction;
        }


    private:
        TransactionState state;
        Transaction<T> * transaction;
        long transactionId;
        bool inTransaction;
    };

    template<typename T>
    class TransactionCallback : public Function<TransactionResponse<T>>
    {
        public:

        TransactionCallback() : Function<TransactionResponse < T >> () {
        }


    };

    template<typename T>
    class Transaction {
    public:

        Transaction() :
        transaction(false), item_list(), lastId(-1), mutex() {
        }


        Transaction(const Transaction<T>& other) = delete;

        TransactionResponse<T> Register(T item) {
            bool found = false;
            auto it = item_list.begin();
            while (it != item_list.end()) {
                if (it->GetItem() == item) {
                    found = true;
                }
                it++;
            }
            if (found) {
                return this->TriggerError(TransactionState::ERROR_ITEM_REGISTERED);
            }
            item_list.push_back(TransactionItem(item));

            return TransactionResponse<T>(TransactionState::SUCCESS_OK, *this);
        }

        TransactionResponse<T> ConfirmOne(T item) {
            if (!transaction) {
                return this->TriggerError(TransactionState::ERROR_NOT_IN_TRANSACTION);
            }
            bool all = true;
            auto it = item_list.begin();
            while (it != item_list.end()) {
                if (it->GetItem() == item) {
                    it->SetResponded();
                }
                if (!(it->IsResponded())) {
                    all = false;
                }
                it++;
            }
            if (all) {
                auto response = this->TriggerSuccess();
                this->ResetTransaction();
                return response;
            }
            return TransactionResponse<T>(TransactionState::SUCCESS_OK, *this);
        }

        TransactionResponse<T> BeginTransaction() {
            if (transaction) {
                return this->TriggerError(TransactionState::ERROR_MULTIPLE_TRANSACTION_REQUEST);
            }
            this->ResetTransaction();
            transaction = true;
            counter++;
            lastId = counter;
            return TransactionResponse<T>(TransactionState::SUCCESS_OK, *this);
        }

        TransactionResponse<T> RollBack() {
            if (!transaction) {
                return this->TriggerError(TransactionState::ERROR_NOT_IN_TRANSACTION);
            }
            this->ResetTransaction();
            return this->TriggerError(TransactionState::ERROR_ROLLBACK);
        }

        bool InTransaction() {
            return transaction;
        }

        long RegisterSuccessCallback(Function<TransactionResponse<T>> &fn) {
            return successCallbacks.AddFunction(fn);
        }

        long RegisterErrorCallback(Function<TransactionResponse<T>> &fn) {
            return errorCallbacks.AddFunction(fn);
        }
    protected:

        long GetLastId() const {
            return lastId;
        }

    protected:

        void ResetTransaction() {
            transaction = false;
            lastId = -1;
            auto it = item_list.begin();
            while (it != item_list.end()) {
                it->Reset();
                it++;
            }
        }

        TransactionResponse<T> TriggerError(TransactionState state) {
            TransactionResponse<T> response(state, *this);
            errorCallbacks.Run(response);
            return response;

        }

        TransactionResponse<T> TriggerSuccess() {
            TransactionResponse<T> response(TransactionState::SUCCESS_TRANSACTION_FINISHED, *this);
            successCallbacks.Run(response);
            return response;

        }


    private:

        class TransactionItem {
        public:

            TransactionItem(T item) :
            item(item), responded(false) {
            }

            T GetItem() const {
                return item;
            }

            bool IsResponded() const {
                return responded;
            }

            void Reset() {
                responded = false;
            }

            void SetResponded() {
                responded = true;
            }

        private:
            T item;
            bool responded;
        };
        bool transaction;
        std::vector<TransactionItem> item_list;
        FunctionComposer<TransactionResponse<T>> successCallbacks;
        FunctionComposer<TransactionResponse<T>> errorCallbacks;
        long counter;
        long lastId;
        std::mutex mutex;

        friend class TransactionResponse<T>;
    };


}

#endif	/* TRANSACTION_H */

