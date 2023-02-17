.DEFAULT_GOAL := all

ASTYLE        := astyle
CHECKTESTDATA := checktestdata
CPPCHECK      := cppcheck
DOXYGEN       := doxygen
SHELL         := bash
VALGRIND      := valgrind

ifeq ($(shell uname -s), Darwin)
    BOOST    := /usr/local/include/boost
    CXX      := clang++
    CXXFLAGS := --coverage -g -std=c++20 -I/usr/local/include/ -Wall -Wextra -Wpedantic
    GCOV     := llvm-cov gcov
    GTEST    := /usr/local/include/gtest
    LDFLAGS  := -lgtest -lgtest_main
    LIB      := /usr/local/lib
    VALGRIND := valgrind
else ifeq ($(shell uname -p), unknown)
    BOOST    := /usr/include/boost
    CXX      := g++
    CXXFLAGS := --coverage -g -std=c++20 -Wall -Wextra -Wpedantic
    GCOV     := gcov
    GTEST    := /usr/include/gtest
    LDFLAGS  := -lgtest -lgtest_main -pthread
    LIB      := /usr/lib
    VALGRIND :=
else
    BOOST    := /usr/local/opt/boost-1.67/include/boost
    CXX      := g++-11
    CXXFLAGS := --coverage -g -std=c++20 -Wall -Wextra -Wpedantic
    GCOV     := gcov-11
    GTEST    := /usr/local/include/gtest
    LDFLAGS  := -L/usr/local/opt/boost-1.77/lib/ -lgtest -lgtest_main -pthread
    LIB      := /usr/local/lib
    VALGRIND := valgrind-3.17
endif

# run docker
docker:
	docker run --rm -it -v $(PWD):/usr/gcc -w /usr/gcc gpdowning/gcc

# get git config
config:
	git config -l

# get git log
Voting.log.txt:
	git log > Voting.log.txt

# get git status
status:
	make clean
	@echo
	git branch
	git remote -v
	git status

# download files from the Voting code repo
pull:
	make clean
	@echo
	git pull
	git status

# upload files to the Voting code repo
# git add .gitignore
# git add .gitlab-ci.yml
push:
	make clean
	@echo
	git add Voting.cpp
	git add Voting.hpp
	-git add Voting.log.txt
	-git add html
	git add Makefile
	git add README.md
	git add RunVoting.cpp
	git add RunVoting.ctd.txt
	git add TestVoting.cpp
	git add test.txt
	git commit -m "refactored some things, finished test cases. Solved issues #9 and #13"
	git push
	git status

# compile run harness
RunVoting: Voting.hpp Voting.cpp RunVoting.cpp
	-$(CPPCHECK) Voting.cpp
	-$(CPPCHECK) Voting.hpp
	-$(CPPCHECK) RunVoting.cpp
	$(CXX) $(CXXFLAGS) Voting.cpp RunVoting.cpp -o RunVoting

# compile test harness
TestVoting: Voting.hpp Voting.cpp TestVoting.cpp
	-$(CPPCHECK) Voting.cpp
	-$(CPPCHECK) TestVoting.cpp
	$(CXX) $(CXXFLAGS) Voting.cpp TestVoting.cpp -o TestVoting $(LDFLAGS)

# run/test files, compile with make all
FILES :=        \
    RunVoting  \
    TestVoting

# compile all
all: $(FILES)

# execute test harness with coverage
test: TestVoting
	$(VALGRIND) ./TestVoting
ifeq ($(shell uname -s), Darwin)
	$(GCOV) TestVoting.cpp | grep -B 2 "cpp.gcov"
else
	gcc -c --coverage Voting.cpp
	$(GCOV) -b TestVoting-Voting.cpp | grep -B 4 "cpp.gcov"

endif

# clone the Voting test repo
../cs371p-voting-tests:
	git clone https://gitlab.com/gpdowning/cs371p-voting-tests.git ../cs371p-voting-tests

# test files in the Voting test repo
T_FILES := `ls ../cs371p-voting-tests/*.in.txt`

# generate a random input file
ctd-generate:
	for v in {1..200}; do $(CHECKTESTDATA) -g RunVoting.ctd.txt >> RunVoting.gen.txt; done

# execute the run harness against a test file in the Voting test repo and diff with the expected output
../cs371p-voting-tests/%: RunVoting
	$(CHECKTESTDATA) RunVoting.ctd.txt $@.in.txt
	./RunVoting < $@.in.txt > RunVoting.tmp.txt
	diff RunVoting.tmp.txt $@.out.txt

# execute the run harness against your test files in the Voting test repo and diff with the expected output
run: ../cs371p-voting-tests
	-make ../cs371p-voting-tests/richiewahidin-RunVoting # change gpdowning to your GitLab-ID

# execute the run harness against all of the test files in the Voting test repo and diff with the expected output
run-all: ../cs371p-voting-tests
	-for v in $(T_FILES); do make $${v/.in.txt/}; done

# auto format the code
format:
	$(ASTYLE) Voting.cpp
	$(ASTYLE) Voting.hpp
	$(ASTYLE) RunVoting.cpp
	$(ASTYLE) TestVoting.cpp

# you must edit Doxyfile and
# set EXTRACT_ALL     to YES
# set EXTRACT_PRIVATE to YES
# set EXTRACT_STATIC  to YES
# create Doxfile
Doxyfile:
	$(DOXYGEN) -g

# create html directory
html: Doxyfile
	$(DOXYGEN) Doxyfile

# check files, check their existence with make check
C_FILES :=          \
    .gitignore      \
    .gitlab-ci.yml  \
    Voting.log.txt \
    html

# check the existence of check files
check: $(C_FILES)

# remove executables and temporary files
clean:
	rm -f  *.gcda
	rm -f  *.gcno
	rm -f  *.gcov
	rm -f  *.gen.txt
	rm -f  *.tmp.txt
	rm -f  RunVoting
	rm -f  TestVoting
	rm -rf *.dSYM

# remove executables, temporary files, and generated files
scrub:
	make clean
	rm -f  Voting.log.txt
	rm -f  Doxyfile
	rm -rf html
	rm -rf latex

# output versions of all tools
versions:
	uname -p

	@echo
	uname -s

	@echo
	which $(ASTYLE)
	@echo
	$(ASTYLE) --version

	@echo
	which $(CHECKTESTDATA)
	@echo
	$(CHECKTESTDATA) --version | head -n 1

	@echo
	which $(CPPCHECK)
	@echo
	$(CPPCHECK) --version

	@echo
	which $(DOXYGEN)
	@echo
	$(DOXYGEN) --version

	@echo
	which $(CXX)
	@echo
	$(CXX) --version | head -n 1

	@echo
	which $(GCOV)
	@echo
	$(GCOV) --version | head -n 1

	@echo
	which git
	@echo
	git --version

	@echo
	which make
	@echo
	make --version | head -n 1

ifneq ($(VALGRIND),)
	@echo
	which $(VALGRIND)
	@echo
	$(VALGRIND) --version
endif

	@echo
	which vim
	@echo
	vim --version | head -n 1

	@echo
	grep "#define BOOST_LIB_VERSION " $(BOOST)/version.hpp

	@echo
	ls -al $(GTEST)/gtest.h
	@echo
	pkg-config --modversion gtest
	@echo
	ls -al $(LIB)/libgtest*.a
