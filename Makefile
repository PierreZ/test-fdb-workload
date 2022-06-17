srcdir = src/
libdir = lib/
fdbdir = /usr/include/foundationdb/

libs = -lfdb_c -lm -lpthread -lrt
opts = -O3 -fPIC -shared -fno-omit-frame-pointer -std=gnu++17 -static-libstdc++ -static-libgcc -mavx

src = $(srcdir)workload.cpp
lib = $(libdir)libcppwl.so
ALL = $(lib)

$(lib): $(src)
	mkdir -p $(libdir)
	g++ $(opts) $< $(libs) -o $@ -I $(fdbdir)