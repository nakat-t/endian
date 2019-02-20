test: ../include/endian/endian.h main.cpp
	$(CXX) -Wall -Wextra -Wc++11-compat -DENDIAN_IS_LITTLE -I../include main.cpp

run: test
	./a.out

clean:
	$(RM) a.out
