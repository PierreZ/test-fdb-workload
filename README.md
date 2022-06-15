# test-fdb-workload
A simple C++ Workload.
It uses the `ClientWorkload.h` and `fdb_c.h` headers from `foundationdb/bindings/c`.
It's compiled as a shared library with a dependency to `libfdb_c.so` and then loaded in the `fdbserver` as an `ExternalWorkload`.

## What is working
Building and linking the shared library work fine. It is loaded in the fdbserver as an `ExternalWorkload`. The `workloadFactory` symbol is found, called, and returns a valid `FDBWorkloadFactory` pointer. The create method is called and returns a valid `FDBWorkload` shared pointer. The `init`, `setup`, `start`, `check` and `getMetrics` methods are then called and if nothing was done inside, all tests would pass successfully.

## What is not working
Any call from the `workload.cpp` side on a pointer received from outside seems to crash or result in unexpected behaviors. For instance, any call to the `trace` method (either via an `FDBLogger` instance or an `FDBWorkloadContext` instance) produces invalid logs or crashes. Moreover, most calls in the `libfdb_c.so` result in segmentation faults. For example in the current `SimpleWorkload::init` implementation a call to `fdb_get_max_version` returns the expected value, but a call to `fdb_get_client_version` crashes. It seems like any function in `libfdb_c.so` which makes a relative jump is invalid. For example `fdb_get_client_version` seems to unexpectedly jump into `fdb_transaction_set_option` which leads to a segmentation fault.