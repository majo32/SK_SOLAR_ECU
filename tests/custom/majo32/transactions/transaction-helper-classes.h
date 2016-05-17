#include <Catch/catch.hpp>
#include <SunStorm/Framework.h>

struct res {
    bool err;
    SunStorm::TransactionState errState;
    bool succ;
};

class TransactionCallbackSuccess : public SunStorm::TransactionCallback<long> {
public:

    TransactionCallbackSuccess(struct res & results) : TransactionCallback<long>(), results(results) {
    }

    void Run(SunStorm::TransactionResponse<long> response) {
        INFO("Transaction Success");
        //REQUIRE(response.Success() == true);
        results.succ = true;
    }
    
    struct res & results;
};

class TransactionCallbackError : public SunStorm::TransactionCallback<long> {
public:

    TransactionCallbackError(struct res & results) : TransactionCallback<long>(), results(results) {
    }

    void Run(SunStorm::TransactionResponse<long> response) {
        INFO("Transaction Error")
        INFO("Transaction Error Status -> " << response.GetState());
        //REQUIRE(response.Success() == false);
        results.err = true;
        
        results.errState = response.GetState();
    }
    struct res & results;
};
