#include <Catch/catch.hpp>
#include <SunStorm/Framework.h>

#include "transaction-helper-classes.h"

TEST_CASE("Transaction helper sync", "[transaction helper]") {
    SunStorm::Transaction<long> tr;
    REQUIRE(tr.Register(4).GetState() == SunStorm::TransactionState::SUCCESS_OK);
    REQUIRE(tr.Register(5).GetState() == SunStorm::TransactionState::SUCCESS_OK);
    REQUIRE(tr.Register(9).GetState() == SunStorm::TransactionState::SUCCESS_OK);
    REQUIRE(tr.Register(65).GetState() == SunStorm::TransactionState::SUCCESS_OK);
    REQUIRE(tr.Register(4).GetState() == SunStorm::TransactionState::ERROR_ITEM_REGISTERED);


    REQUIRE(tr.BeginTransaction().GetState() == SunStorm::TransactionState::SUCCESS_OK);
    REQUIRE(tr.ConfirmOne(5).GetState() == SunStorm::TransactionState::SUCCESS_OK);
    REQUIRE(tr.ConfirmOne(4).GetState() == SunStorm::TransactionState::SUCCESS_OK);
    REQUIRE(tr.ConfirmOne(9).GetState() == SunStorm::TransactionState::SUCCESS_OK);
    REQUIRE(tr.ConfirmOne(65).GetState() == SunStorm::TransactionState::SUCCESS_TRANSACTION_FINISHED);

    REQUIRE(tr.ConfirmOne(5).GetState() == SunStorm::TransactionState::ERROR_NOT_IN_TRANSACTION);
    REQUIRE(tr.ConfirmOne(4).GetState() == SunStorm::TransactionState::ERROR_NOT_IN_TRANSACTION);
    REQUIRE(tr.ConfirmOne(9).GetState() == SunStorm::TransactionState::ERROR_NOT_IN_TRANSACTION);

    REQUIRE(tr.BeginTransaction().GetState() == SunStorm::TransactionState::SUCCESS_OK);
    REQUIRE(tr.ConfirmOne(5).GetState() == SunStorm::TransactionState::SUCCESS_OK);
    REQUIRE(tr.ConfirmOne(4).GetState() == SunStorm::TransactionState::SUCCESS_OK);
    REQUIRE(tr.ConfirmOne(9).GetState() == SunStorm::TransactionState::SUCCESS_OK);
    REQUIRE(tr.ConfirmOne(65).GetState() == SunStorm::TransactionState::SUCCESS_TRANSACTION_FINISHED);
}

void test1() {
    SunStorm::Transaction<long> tr;
    
    REQUIRE(tr.Register(4).GetState() == SunStorm::TransactionState::SUCCESS_OK);
    REQUIRE(tr.Register(5).GetState() == SunStorm::TransactionState::SUCCESS_OK);
    REQUIRE(tr.Register(9).GetState() == SunStorm::TransactionState::SUCCESS_OK);
    REQUIRE(tr.Register(65).GetState() == SunStorm::TransactionState::SUCCESS_OK);
    REQUIRE(tr.Register(4).GetState() == SunStorm::TransactionState::ERROR_ITEM_REGISTERED);

    struct res results({false, SunStorm::TransactionState::SUCCESS_OK, false});

    TransactionCallbackSuccess trS(results);
    TransactionCallbackError trE(results);

    INFO("Registering callback")
    tr.RegisterSuccessCallback(trS);
    tr.RegisterErrorCallback(trE);

    REQUIRE(tr.BeginTransaction().GetState() == SunStorm::TransactionState::SUCCESS_OK);
    REQUIRE(tr.ConfirmOne(5).GetState() == SunStorm::TransactionState::SUCCESS_OK);
    REQUIRE(tr.ConfirmOne(4).GetState() == SunStorm::TransactionState::SUCCESS_OK);
    REQUIRE(tr.ConfirmOne(9).GetState() == SunStorm::TransactionState::SUCCESS_OK);
    REQUIRE(tr.ConfirmOne(65).GetState() == SunStorm::TransactionState::SUCCESS_TRANSACTION_FINISHED);

    REQUIRE(results.succ == true);
    REQUIRE(results.err == false);

    REQUIRE(tr.BeginTransaction().GetState() == SunStorm::TransactionState::SUCCESS_OK);
    REQUIRE(tr.ConfirmOne(5).GetState() == SunStorm::TransactionState::SUCCESS_OK);
    REQUIRE(tr.ConfirmOne(4).GetState() == SunStorm::TransactionState::SUCCESS_OK);
    REQUIRE(tr.BeginTransaction().GetState() == SunStorm::TransactionState::ERROR_MULTIPLE_TRANSACTION_REQUEST);
    REQUIRE(results.err == true);
    REQUIRE(results.errState == SunStorm::TransactionState::ERROR_MULTIPLE_TRANSACTION_REQUEST);
    REQUIRE(tr.ConfirmOne(9).GetState() == SunStorm::TransactionState::SUCCESS_OK);
    REQUIRE(tr.ConfirmOne(65).GetState() == SunStorm::TransactionState::SUCCESS_TRANSACTION_FINISHED);
    
}

TEST_CASE("Transaction helper async", "[transaction helper]") {
    test1();
}