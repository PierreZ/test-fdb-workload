srcdir = src/
libdir = lib/
fdbdir = /usr/include/foundationdb/

src = $(srcdir)workload.cpp
lib = $(libdir)libcppwl.so
ALL = $(lib)

$(lib): $(src)
	mkdir -p $(libdir)
	g++ -fpic -shared $< -lfdb_c -o $@ -I $(fdbdir)