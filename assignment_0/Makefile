# Environment variable containing flags to pass to the gnu c++ compiler
CPPFLAGS = -std=c++17 -Wall -Wextra -pedantic -march=native

# Environment variable containing the name of headers that will be used
HEADERS = VByte.hpp vpacked_array.hpp vbit_array.hpp

# Tells make to not expect to actually create files called "clean" or "debug".
.PHONY: clean debug

# Tells make how to create the "main" file.
main: main.cpp $(HEADERS)
	g++ $(CPPFLAGS) -DNDEBUG -Ofast -o main main.cpp
# Tells make what to do when "make clean" is called.
# Here we simply remove the "main" binary.
clean:
	rm -f main
# Tells make what to do when "make debug" is called.
# Here we compile the binary with different flags to support debugging.
debug: main.cpp $(HEADERS) 
	g++ $(CPPFLAGS) -DDEBUG -g -Og -o main main.cpp

# Generic rule that tells make that nothing needs to be done about c++ header files.
%.hpp: