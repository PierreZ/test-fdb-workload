srcdir = src/
libdir = lib/
fdbdir = /usr/include/foundationdb/

libs = -lfdb_c -lm -lpthread -lrt
opts = -fpic -shared -std=c++17

src = $(srcdir)workload.cpp
lib = $(libdir)libcppwl.so
ALL = $(lib)

$(lib): $(src)
	mkdir -p $(libdir)
	g++ $(opts) $< $(libs) -o $@ -I $(fdbdir)