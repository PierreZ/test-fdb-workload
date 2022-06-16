#include <iostream>
#include "ClientWorkload.h"
#define FDB_API_VERSION 710
#include "fdb_c.h"

void handle_error(fdb_error_t code) {
    if (code != 0) {
        std::cout << "\terrno = " << code << "\n";
        std::cout << "\terror = " << fdb_get_error(code) << "\n";
    }
}

class SimpleWorkload: public FDBWorkload {
    virtual std::string description() const override {
        std::cout << "SimpleWorkload::description()\n";
        return "SimpleWorkload description";
    }
	virtual bool init(FDBWorkloadContext* context) override {
        std::cout << "SimpleWorkload::init()\n";
        //context->trace(FDBSeverity::WarnAlways, "FDBWorkloadContext::trace()", {});
        std::cout << "fdb_get_max_api_version() = " << fdb_get_max_api_version() << "\n";
        std::cout << "fdb_get_client_version() = "  << fdb_get_client_version()  << "\n";
        return true;
    }
	virtual void setup(FDBDatabase* db, GenericPromise<bool> done) override {
        std::cout << "SimpleWorkload::setup()\n";
        done.send(true);
    }
	virtual void start(FDBDatabase* db, GenericPromise<bool> done) override {
        std::cout << "SimpleWorkload::start()\n";
        FDBTransaction* trx = nullptr;
        std::cout << "fdb_database_create_transaction()\n";
        fdb_error_t error = fdb_database_create_transaction(db, &trx);
            handle_error(error);
            const char* key = "foo";
            const char* val = "bar";
            std::cout << "fdb_transaction_set()\n";
            fdb_transaction_set(trx, (uint8_t*)key, 3, (uint8_t*)val, 3);
            //std::cout << "fdb_transaction_commit()\n";
            //FDBFuture* f = fdb_transaction_commit(trx);
            //std::cout << "fdb_future_block_until_ready()\n";
            //error = fdb_future_block_until_ready(f);
            //handle_error(error);
            //std::cout << "fdb_future_destroy()\n";
            //fdb_future_destroy(f);
        std::cout << "fdb_transaction_destroy()\n";
        fdb_transaction_destroy(trx);
        done.send(true);
    }
	virtual void check(FDBDatabase* db, GenericPromise<bool> done) override {
        std::cout << "SimpleWorkload::check()\n";
        FDBTransaction* trx = nullptr;
        std::cout << "fdb_database_create_transaction()";
        fdb_error_t error = fdb_database_create_transaction(db, &trx);
            handle_error(error);
            const char* key = "foo";
            std::cout << "fdb_transaction_get()\n";
            FDBFuture* f = fdb_transaction_get(trx, (uint8_t*)key, 3, false);
            std::cout << "fdb_future_block_until_ready()\n";
            error = fdb_future_block_until_ready(f);
            handle_error(error);
            fdb_bool_t out_present;
            uint8_t const* out_value;
            int out_value_length;
            std::cout << "fdb_future_get_value()\n";
            error = fdb_future_get_value(f, &out_present, &out_value, &out_value_length);
            handle_error(error);
            std::cout << "\tout_present: " << out_present << "\n";
            if (out_present) {
                std::cout << "\tout_value_length: " << out_value_length << "\n";
                std::cout << "\tout_value: " << out_value << "\n";
            }
            std::cout << "fdb_future_destroy()\n";
            fdb_future_destroy(f);
        std::cout << "fdb_transaction_destroy()\n";
        fdb_transaction_destroy(trx);
        done.send(true);
    }
	virtual void getMetrics(std::vector<FDBPerfMetric>& out) const override {
        std::cout << "SimpleWorkload::getMetrics()\n";
    }
	//virtual double getCheckTimeout() { return 3000; }
};

class SimpleWorkloadFactory: public FDBWorkloadFactory {
    virtual std::shared_ptr<FDBWorkload> create(const std::string& name) override {
        std::cout << "SimpleWorkloadFactory::create()\n";
        std::shared_ptr<FDBWorkload> workload = std::make_shared<SimpleWorkload>();
        std::cout << "workload created\n";
        return workload;
    }
};

extern "C" FDBWorkloadFactory* workloadFactory(FDBLogger* logger) {
    std::cout << "workloadFactory()\n";
    static bool initialized = false;
    if (!initialized) {
        std::cout << "fdb_select_api_version(" << FDB_API_VERSION << ")\n";
        fdb_error_t error = fdb_select_api_version(FDB_API_VERSION);
        handle_error(error);
        initialized = true;
    }
    //logger->trace(FDBSeverity::WarnAlways, "FDBLogger::trace()", {});
    return new SimpleWorkloadFactory();
}
