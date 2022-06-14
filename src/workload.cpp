#include <iostream>
#include "workloads.h"

class SimpleWorkload: public FDBWorkload {
    // TODO
};

class SimpleWorkloadFactory: public FDBWorkloadFactory {
    virtual std::shared_ptr<FDBWorkload> create(const std::string& name) {
        return new SimpleWorkload();
    }
};

extern "C" FDBWorkloadFactory* workloadFactory() {
    std::cout << "workloadFactory()\n";
    return new SimpleWorkloadFactory();
}

