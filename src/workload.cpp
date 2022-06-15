#include <iostream>
#include "ClientWorkload.h"

class SimpleWorkload: public FDBWorkload {
    virtual std::string description() const override {
        std::cout << "SimpleWorkload::description()\n";
        return "SimpleWorkload description";
    }
	virtual bool init(FDBWorkloadContext* context) override {
        std::cout << "SimpleWorkload::init()\n";
        return true;
    }
	virtual void setup(FDBDatabase* db, GenericPromise<bool> done) override {
        std::cout << "SimpleWorkload::setup()\n";
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
    virtual std::shared_ptr<FDBWorkload> create(const std::string& name) {
        std::cout << "SimpleWorkloadFactory::create()\n";
        return std::make_shared<SimpleWorkload>();
    }
};

extern "C" FDBWorkloadFactory* workloadFactory() {
    std::cout << "workloadFactory()\n";
    return new SimpleWorkloadFactory();
}
