#!/bin/bash
 
RED='\033[0;31m'
GREEN='\033[0;32m'
NOCOLOR='\033[0m'

i=0
success=0
counter=0

### 		LEXICAL ANALYSIS		###
lex_t=500
echo -e "\nTests for lexical analysis:\n"
# These tests are numbered from 500
echo -e "TEST_500: Invalid identifier name\c"
./ifj2018 < our_tests/test_$lex_t &>/dev/null
if [ $? -eq 1 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

lex_t=$((lex_t + 1))

echo -e "TEST_501: Invalid identifier name\c"
./ifj2018 < our_tests/test_$lex_t &>/dev/null
if [ $? -eq 1 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

lex_t=$((lex_t + 1))

echo -e "TEST_502: Invalid identifier name\c"
./ifj2018 < our_tests/test_$lex_t &>/dev/null
if [ $? -eq 1 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

lex_t=$((lex_t + 1))

echo -e "TEST_503: Invalid identifier name\c"
./ifj2018 < our_tests/test_$lex_t &>/dev/null
if [ $? -eq 1 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

lex_t=$((lex_t + 1))

echo -e "TEST_504: Invalid block comment\c"
./ifj2018 < our_tests/test_$lex_t &>/dev/null
if [ $? -eq 1 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

lex_t=$((lex_t + 1))

echo -e "TEST_505: Invalid block comment\c"
./ifj2018 < our_tests/test_$lex_t &>/dev/null
if [ $? -eq 1 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

lex_t=$((lex_t + 1))

echo -e "TEST_506: Invalid token\c"
./ifj2018 < our_tests/test_$lex_t &>/dev/null
if [ $? -eq 1 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

lex_t=$((lex_t + 1))

echo -e "TEST_507: Invalid string escape sequence\c"
./ifj2018 < our_tests/test_$lex_t &>/dev/null
if [ $? -eq 0 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

lex_t=$((lex_t + 1))

echo -e "TEST_508: Missing quotation marks at the end\c"
./ifj2018 < our_tests/test_$lex_t &>/dev/null
if [ $? -eq 1 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

lex_t=$((lex_t + 1))

echo -e "TEST_509: Invalid scientific notation\c"
./ifj2018 < our_tests/test_$lex_t &>/dev/null
if [ $? -eq 1 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

lex_t=$((lex_t + 1))

echo -e "TEST_510: Invalid scientific notation\c"
./ifj2018 < our_tests/test_$lex_t &>/dev/null
if [ $? -eq 1 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

lex_t=$((lex_t + 1))

echo -e "TEST_511: Invalid string escape sequence\c"
./ifj2018 < our_tests/test_$lex_t &>/dev/null
if [ $? -eq 1 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

lex_t=$((lex_t + 1))

echo -e "TEST_512: Hex num\c"
./ifj2018 < our_tests/test_$lex_t &>/dev/null
if [ $? -eq 0 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

lex_t=$((lex_t + 1))

echo -e "TEST_513: Correct lexical analysis\c"
./ifj2018 < our_tests/test_$lex_t &>/dev/null
if [ $? -eq 0 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

lex_t=$((lex_t + 1))

echo -e "TEST_514: Correct lexical analysis\c"
./ifj2018 < our_tests/test_$lex_t &>/dev/null
if [ $? -eq 0 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

lex_t=$((lex_t + 1))

echo -e "TEST_515: Correct lexical analysis\c"
./ifj2018 < our_tests/test_$lex_t &>/dev/null
if [ $? -eq 0 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

lex_t=$((lex_t + 1))

echo -e "\nTests for syntactical analysis:\n"
# These tests are numbered from 600
syn_t=600
echo -e "TEST_600: Missing if condition\c"
./ifj2018 < our_tests/test_$syn_t &>/dev/null
if [ $? -eq 2 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

syn_t=$((syn_t+1))

echo -e "TEST_601: Missing while condition\c"
./ifj2018 < our_tests/test_$syn_t &>/dev/null
if [ $? -eq 2 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

syn_t=$((syn_t+1))

echo -e "TEST_602: a = a\c"
./ifj2018 < our_tests/test_$syn_t &>/dev/null
if [ $? -eq 0 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

syn_t=$((syn_t+1))

echo -e "TEST_603: More elses\c"
./ifj2018 < our_tests/test_$syn_t &>/dev/null
if [ $? -eq 2 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

syn_t=$((syn_t+1))

echo -e "TEST_604: No else\c"
./ifj2018 < our_tests/test_$syn_t &>/dev/null
if [ $? -eq 2 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

syn_t=$((syn_t+1))



echo -e "\nTests for expressions:\n"
syn_t=650
echo -e "TEST_650: Invalid expression\c"
./ifj2018 < our_tests/test_$syn_t &>/dev/null
if [ $? -eq 2 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

syn_t=$((syn_t+1))

echo -e "TEST_651: Invalid expression\c"
./ifj2018 < our_tests/test_$syn_t &>/dev/null
if [ $? -eq 2 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

syn_t=$((syn_t+1))

echo -e "TEST_652: Invalid expression\c"
./ifj2018 < our_tests/test_$syn_t &>/dev/null
if [ $? -eq 2 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

syn_t=$((syn_t+1))

echo -e "TEST_653: Invalid expression\c"
./ifj2018 < our_tests/test_$syn_t &>/dev/null
if [ $? -eq 2 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

syn_t=$((syn_t+1))

echo -e "TEST_654: Invalid expression\c"
./ifj2018 < our_tests/test_$syn_t &>/dev/null
if [ $? -eq 2 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

syn_t=$((syn_t+1))

echo -e "TEST_655: Non-existing variable\c"
./ifj2018 < our_tests/test_$syn_t &>/dev/null
if [ $? -eq 3 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

syn_t=$((syn_t+1))


echo -e "\n"
### other tests

echo -e "TEST_0:\c"
./ifj2018 < our_tests/test_$i &>/dev/null
if [ $? -eq 0 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))

echo -e "TEST_1:\c"
./ifj2018 < our_tests/test_$i &>/dev/null
if [ $? -eq 0 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))

echo -e "TEST_2:\c"
./ifj2018 < our_tests/test_$i &>/dev/null
if [ $? -eq 0 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))

echo -e "TEST_3:\c"
./ifj2018 < our_tests/test_$i &>/dev/null
if [ $? -eq 4 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))

echo -e "TEST_4:\c"
./ifj2018 < our_tests/test_$i &>/dev/null
if [ $? -eq 0 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))

echo -e "TEST_5:\c"
./ifj2018 < our_tests/test_$i &>/dev/null
if [ $? -eq 0 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))

echo -e "TEST_6:\c"
./ifj2018 < our_tests/test_$i &>/dev/null
if [ $? -eq 1 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))

echo -e "TEST_7:\c"
./ifj2018 < our_tests/test_$i &>/dev/null
if [ $? -eq 3 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))

echo -e "TEST_8:\c"
./ifj2018 < our_tests/test_$i &>/dev/null
if [ $? -eq 1 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))

echo -e "TEST_9:\c"
./ifj2018 < our_tests/test_$i &>/dev/null
if [ $? -eq 0 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))

echo -e "TEST_10:\c"
./ifj2018 < our_tests/test_$i &>/dev/null
if [ $? -eq 0 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))

echo -e "TEST_11:\c"
./ifj2018 < our_tests/test_$i &>/dev/null
if [ $? -eq 5 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))

echo -e "TEST_12:\c"
./ifj2018 < our_tests/test_$i &>/dev/null
if [ $? -eq 0 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))

echo -e "TEST_13: if without else\c"
./ifj2018 < our_tests/test_$i &>/dev/null
if [ $? -eq 0 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))

echo -e "\nAdamove testy:\n"
a_tests=100

echo -e "TEST_100:\c"
./ifj2018 < our_tests/test_$a_tests &>/dev/null
if [ $? -eq 0 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

a_tests=$((a_tests+1))

echo -e "TEST_101:\c"
./ifj2018 < our_tests/test_$a_tests &>/dev/null
if [ $? -eq 0 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

a_tests=$((a_tests+1))

echo -e "TEST_102: testing if-else\c"
./ifj2018 < our_tests/test_$a_tests &>/dev/null
if [ $? -eq 0 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

a_tests=$((a_tests+1))

echo -e "TEST_103: testing more functions\c"
./ifj2018 < our_tests/test_$a_tests &>/dev/null
if [ $? -eq 0 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

a_tests=$((a_tests+1))



