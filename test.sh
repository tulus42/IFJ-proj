#!/bin/bash
 
RED='\033[0;31m'
GREEN='\033[0;32m'
NOCOLOR='\033[0m'

i=0
success=0
counter=0

echo -e "TEST_0:\c"
./ifj2018 < our_tests/test_1 &>/dev/null
if [ $? -eq 0 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))

echo -e "TEST_1:\c"
./ifj2018 < our_tests/test_2 &>/dev/null
if [ $? -eq 0 ]; then
	counter=$((counter+1))
	echo -e "${GREEN}[OK]${NOCOLOR}"
else
	success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))



