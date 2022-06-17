# test-fdb-workload
A simple C++ Workload.
It uses the `ClientWorkload.h` and `fdb_c.h` headers from `foundationdb/bindings/c`.
It's compiled as a shared library with a dependency to `libfdb_c.so` and then loaded in the `fdbserver` as an `ExternalWorkload` using the command:
```
fdbserver -r simulation -f ./test_file.txt
```

## What is working
Building and linking the shared library work fine. It is loaded in the fdbserver as an `ExternalWorkload`. The `workloadFactory` symbol is found, called, and returns a valid `FDBWorkloadFactory` pointer. The `create` method is called and returns a valid `FDBWorkload` shared pointer. The `init`, `setup`, `start`, `check` and `getMetrics` methods are then called and if nothing was done inside, all tests would pass successfully. Calling C bindings are somewhat working. For example it is possible to get the client version or create and delete a transaction.

## What is not working
A lot of calls from the `workload.cpp` side on a pointer received from outside seems to crash or result in unexpected behaviors. For instance, any call to the `trace` method (either through a `FDBLogger` instance or a `FDBWorkloadContext` instance) produces invalid logs or crashes. In most cases it will log a message similar to:
```
Trace event detail Type ` 7s\xe9\xda\x7f' is invalid
```
Moreover, calls to more "advanced" C bindings also result in crashes like `fdb_future_block_until_ready`.

We suspect the cause of all this is a difference in compilation of the 2 sides. `workload.cpp` is compiled using the simple `Makefile` on this repo, whereas the Cpp example workload of FoundationDB is supposedly compiled using:
```
/opt/rh/devtoolset-8/root/usr/bin/c++ -DBOOST_ERROR_CODE_HEADER_ONLY -DBOOST_SYSTEM_NO_DEPRECATED -DNO_INTELLISENSE -Dc_workloads_EXPORTS -I/root/foundationdb -I. -Ibindings/c -I/root/foundationdb/bindings/c -Ibindings/c/foundationdb -O3 -DNDEBUG -fPIC -DCMAKE_BUILD -ggdb -fno-omit-frame-pointer -mavx -Wno-pragmas -Wno-attributes -Wno-error=format -Wunused-variable -Wno-deprecated -fvisibility=hidden -Wreturn-type -fPIC -Wclass-memaccess -DHAVE_OPENSSL -std=gnu++17 -MD -MT bindings/c/CMakeFiles/c_workloads.dir/test/workloads/SimpleWorkload.cpp.o -MF bindings/c/CMakeFiles/c_workloads.dir/test/workloads/SimpleWorkload.cpp.o.d -o bindings/c/CMakeFiles/c_workloads.dir/test/workloads/SimpleWorkload.cpp.o -c /root/foundationdb/bindings/c/test/workloads/SimpleWorkload.cpp

&& /opt/rh/devtoolset-8/root/usr/bin/c++ -fPIC -O3 -DNDEBUG  -static-libstdc++ -static-libgcc -Wl,--version-script=/root/foundationdb/bindings/c/external_workload.map,-z,nodelete -shared -Wl,-soname,libc_workloads.so -o share/foundationdb/libc_workloads.so bindings/c/CMakeFiles/c_workloads.dir/test/workloads/workloads.cpp.o bindings/c/CMakeFiles/c_workloads.dir/test/workloads/SimpleWorkload.cpp.o  -Wl,-rpath,/root/build_output/lib  lib/libfdb_c.so && :
```
Which we currently can't make sense of.