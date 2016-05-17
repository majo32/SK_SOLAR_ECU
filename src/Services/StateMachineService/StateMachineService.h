/* 
 * File:   StateMachineService.h
 * Author: majo32
 *
 * Created on Streda, 2016, marca 9, 13:27
 */

#ifndef STATEMACHINESERVICE_H
#define	STATEMACHINESERVICE_H

#include <vector>

#include <SunStorm/Framework.h>

#include "StateMachineService_messages.h"
#include "StateMachineService_types.h"

#define TRANSACTION_TIMEOUT 100

namespace Services {

    class StateMachineService : public SunStorm::Service {
    public:

        StateMachineService(std::string name) :
        Service(name), context(), stateMachineListener(*this, context), stateMachineTimerListener(*this, context) {

        }
        void Initialize(){
            this->RegisterListener("__INIT__",stateMachineListener);
            this->RegisterListener("StateMachineService::REGISTER_SERVICE_RESPONSE",stateMachineListener);
            this->RegisterListener("StateMachineService::CHANGE_TO_STATE_CONFIRM",stateMachineListener);
            this->RegisterListener("StateMachineService::CHANGE_TO_STATE_REJECT",stateMachineListener);
            this->RegisterListener("StateMachineService::CHANGE_TO_STATE_REQUEST",stateMachineListener);
        }

        class Context {
        public:

            Context() :
            prev_prev_state(Types::StateMachine::STATES::DEFAULT),
            prev_state(Types::StateMachine::STATES::DEFAULT),
            state(Types::StateMachine::STATES::DEFAULT),
            inTransaction(false),
            transactionId(-1),
            transactionTimerId(-1) {
            }

        public:
            Types::StateMachine::STATES prev_prev_state;
            Types::StateMachine::STATES prev_state;
            Types::StateMachine::STATES state;
            bool inTransaction;
            long transactionId;
            long transactionTimerId;

        };

        class BaseListener : public SunStorm::ServiceListener<StateMachineService, Context> {
        public:

            BaseListener(StateMachineService& service, Context& context) :
            ServiceListener(service, context) {
            }


            virtual void handle(const SunStorm::Message & message) = 0;
        };

        class StateMachineListener : public BaseListener {
        public:

            StateMachineListener(StateMachineService& service, Context& context) :
            BaseListener(service, context) {
            }

            void handle(const SunStorm::Message & message) {
                if (message.GetName() == "__INIT__") {
                    this->handleInitMessage();

                }
                if (message.GetName() == "StateMachineService::REGISTER_SERVICE_RESPONSE") {
                    this->handleRegisterServiceResponse(message.GetContent<Messages::StateMachineService::REGISTER_SERVICE_RESPONSE>());

                }
                if (message.GetName() == "StateMachineService::CHANGE_TO_STATE_CONFIRM") {
                    this->handleChangeStateConfirm(message.GetContent<Messages::StateMachineService::CHANGE_TO_STATE_CONFIRM>());

                }
                if (message.GetName() == "StateMachineService::CHANGE_TO_STATE_REJECT") {
                    this->handleChangeStateReject(message.GetContent<Messages::StateMachineService::CHANGE_TO_STATE_REJECT>());

                }
                if (message.GetName() == "StateMachineService::CHANGE_TO_STATE_REQUEST") {
                    this->handleChangeStateRequest(message.GetContent<Messages::StateMachineService::CHANGE_TO_STATE_REQUEST>());

                }
            }
        protected:

            void handleInitMessage() {
                Log().debug("Init");
                this->GetService().InitSystem();
            }

            void handleRegisterServiceResponse(Messages::StateMachineService::REGISTER_SERVICE_RESPONSE response) {
                Log().debug("Registering service");
                this->GetService().RegisterService(response.id_service);
            }

            void handleChangeStateConfirm(Messages::StateMachineService::CHANGE_TO_STATE_CONFIRM confirm) {
                Log().debug("State confirm");
                this->GetService().ConfirmService(confirm.id_service);
            }

            void handleChangeStateReject(Messages::StateMachineService::CHANGE_TO_STATE_REJECT reject) {
                Log().debug("State reject");
                this->GetService().RejectService(reject.id_service);
            }

            void handleChangeStateRequest(Messages::StateMachineService::CHANGE_TO_STATE_REQUEST request) {
               Log().debug("Change state ");
                this->GetService().ChangeToStateStart(request.state);
            }
        };

        class StateMachineTimerListener : public BaseListener {
        public:

            StateMachineTimerListener(StateMachineService& service, Context& context) :
            BaseListener(service, context) {
            }

            void handle(const SunStorm::Message & message) {
                SunStorm::Clock::ClockResponse clockResponse = message.GetContent<SunStorm::Clock::ClockResponse>();
                this->GetService().TransactionTimeout(clockResponse.GetIdentifier().GetId());
            }
        protected:


        };
    protected:

        void InitSystem() {
            this->SendRegisterServiceRequest();
        }

        void TransactionTimeout(long id) {
            if (id == context.transactionTimerId && context.inTransaction) {
                this->RollBack();
            }
        }

        void ChangeToStateStart(Types::StateMachine::STATES state) {
            if (!context.inTransaction) {
                this->ChangeToState(state);
                this->BeginTransaction();
                Messages::StateMachineService::CHANGE_TO_STATE_START_TRANSACTION start = {
                    context.prev_state,
                    context.state,
                    context.transactionId
                };
                SunStorm::Message msg("StateMachineService::CHANGE_TO_STATE_START_TRANSACTION");
                msg.SetContent<Messages::StateMachineService::CHANGE_TO_STATE_START_TRANSACTION>(start);
                this->SendMessage(msg);
            } else {
                // TODO: if transaction not complete .. push to some queue
            }
        }

        void RegisterService(long id) {
            this->transaction.Register(id);
        }

        void ConfirmService(long id) {
            auto response = this->transaction.ConfirmOne(id);
            if (response.GetState() == SunStorm::TransactionState::SUCCESS_TRANSACTION_FINISHED) {
                this->CommitTransaction();
            }

        }

        void RejectService(long id) {
            this->RollBack();
        }
    private:

        void SendRegisterServiceRequest() {
            Messages::StateMachineService::REGISTER_SERVICE_REQUEST request = {0};
            SunStorm::Message msg("StateMachineService::REGISTER_SERVICE_REQUEST");
            msg.SetContent<Messages::StateMachineService::REGISTER_SERVICE_REQUEST>(request);
            this->SendMessage(msg);
        }

        void RollBack() {
            this->transaction.RollBack();
            Log().error("Transaction rollback");
            this->EndTransaction();
            this->Fatality();
        }

        void BeginTransaction() {
            auto response = this->transaction.BeginTransaction();
            auto timerId = this->RegisterTimeout(TRANSACTION_TIMEOUT, this->stateMachineTimerListener);
            if (response.GetState() != SunStorm::TransactionState::SUCCESS_OK) {
                Log().error("Transaction could not begin");
            }
            this->StartTransaction(response.GetTransactionId(), timerId);
        }

        void CommitTransaction() {
            Messages::StateMachineService::CHANGE_TO_STATE_COMMIT_TRANSACTION commit = {
                context.prev_state,
                context.state,
                context.transactionId
            };
            SunStorm::Message msg("StateMachineService::CHANGE_TO_STATE_COMMIT_TRANSACTION");
            msg.SetContent<Messages::StateMachineService::CHANGE_TO_STATE_COMMIT_TRANSACTION>(commit);
            this->SendMessage(msg);
            this->EndTransaction();
        }
    private:

        void StartTransaction(long id, long timerId) {
            context.inTransaction = true;
            context.transactionId = id;
            context.transactionTimerId = timerId;
        }

        void EndTransaction() {
            context.inTransaction = false;
            context.transactionId = -1;
        }

        void ChangeToState(Types::StateMachine::STATES state) {
            context.prev_state = context.prev_prev_state;
            context.prev_state = context.state;
            context.state = state;
        }

        void ChangeBackToState() {
            context.prev_state = context.prev_prev_state;
            context.state = context.prev_state;
        }

        void Fatality() {
            Log().error("FATALITY!");
        }
        /*class SuccessCallback : public SunStorm::TransactionCallback<long> {
        public:

            SuccessCallback() :
            SunStorm::TransactionCallback<long>() {
            }

            void Run(SunStorm::TransactionResponse<long> a) {

            }
        };
        SuccessCallback successCallback;

        class ErrorCallback : public SunStorm::TransactionCallback<long> {
        public:

            ErrorCallback() :
            SunStorm::TransactionCallback<long>() {
            }

            void Run(SunStorm::TransactionResponse<long> a) {

            }
        };
        ErrorCallback errorCallback;*/

    private:
        Context context;
        StateMachineListener stateMachineListener;
        StateMachineTimerListener stateMachineTimerListener;
        SunStorm::Transaction<long> transaction;


    };
};
#endif	/* STATEMACHINESERVICE_H */

