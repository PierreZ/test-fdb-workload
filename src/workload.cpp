#include <iostream>
#include "ClientWorkload.h"
#define FDB_API_VERSION 710
#include "fdb_c.h"

class SimpleWorkload: public FDBWorkload {
    virtual std::string description() const override {
        std::cout << "SimpleWorkload::description()\n";
        return "SimpleWorkload description";
    }
	virtual bool init(FDBWorkloadContext* context) override {
        std::cout << "SimpleWorkload::init()\n";
        //context->trace(FDBSeverity::WarnAlways, "FDBWorkloadContext::trace()", {});
        std::cout << "fdb_get_max_api_version() = " << fdb_get_max_api_version() << "\n";
        //std::cout << "fdb_get_client_version() = "  << fdb_get_client_version()  << "\n";
        return true;
    }
	virtual void setup(FDBDatabase* db, GenericPromise<bool> done) override {
        std::cout << "SimpleWorkload::setup()\n";
        FDBTransaction* trx = nullptr;
        //fdb_database_create_transaction(db, &trx);
        //std::cout << "fdb_database_create_transaction()\n";
        done.send(true);
    }
	virtual void start(FDBDatabase* db, GenericPromise<bool> done) override {
        std::cout << "SimpleWorkload::start()\n";
        done.send(true);
    }
	virtual void check(FDBDatabase* db, GenericPromise<bool> done) override {
        std::cout << "SimpleWorkload::check()\n";
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
        return std::make_shared<SimpleWorkload>();
    }
};

extern "C" FDBWorkloadFactory* workloadFactory(FDBLogger* logger) {
    std::cout << "workloadFactory()\n";
    //logger->trace(FDBSeverity::WarnAlways, "FDBLogger::trace()", {});
    return new SimpleWorkloadFactory();
}
