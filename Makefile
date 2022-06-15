srcdir = src/
libdir = lib/

src = $(srcdir)workload.cpp
lib = $(libdir)libcppwl.so
ALL = $(lib)

$(lib): $(src)
	mkdir -p $(libdir)
	g++ -fpic -shared $< -o $@