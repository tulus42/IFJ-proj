# subor "test2.sh" musi byt v adresari s projektom
# v adresari musi byt priecinok "our_tests", v ktorom su testy a subor "test_output"
# testy su pomenovane: "test_XX", kde XX = 1-n
# pri doplnani dalsich testov je potrebne skopirovat cats kodu platnu pre dany test
# a nasledne zmenit premennu "expected_output" v zavislosi od ocakavaneho vystupu vygenerovaneho kodu
# upozornenie: skript ignoruje "\n" na konci vystupu - nepisat ho do "expected_output"
# akekolvek dodatky a nove testy su vitane

RED='\033[0;31m'
GREEN='\033[0;32m'
NOCOLOR='\033[0m'


success=0
counter=0


#test_0
i=0
./ifj2018 < our_tests/test_$i &>/dev/null
./ic18int tmp_output > our_tests/test_output

expected_output="hello5"
our_file=$( < our_tests/test_output)

echo -e "TEST_$i \c"
if [ "$our_file" = "$expected_output" ]; then
    counter=$((counter+1))
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

##############################################
#test_1
i=$((i+1))

./ifj2018 < our_tests/test_$i &>/dev/null
./ic18int tmp_output > our_tests/test_output

# prepisat
expected_output="15"
our_file=$( < our_tests/test_output)

echo -e "TEST_$i \c"
if [ "$our_file" = "$expected_output" ]; then
    counter=$((counter+1))
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi


##############################################
#test_2
i=$((i+1))

./ifj2018 < our_tests/test_$i &>/dev/null
./ic18int tmp_output > our_tests/test_output

# prepisat
expected_output="15"
our_file=$( < our_tests/test_output)

echo -e "TEST_$i \c"
if [ "$our_file" = "$expected_output" ]; then
    counter=$((counter+1))
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi



##############################################
echo -e "\n# TESTY NA VSTAVANE FUNKCIE #\n"
##############################################






##############################################
echo -e "\n# TESTY NA UZIVATELSKE FUNKCIE #\n"
##############################################





##############################################
echo -e "\n# TESTY NA EXPRESSION #\n"
##############################################




##############
# vyhodnotenie

counter=(($counter/$i + 1)*100)
result=$counter

echo -e "\n# VYHODNOTENIE #\n"
if [ $counter > 75 ]; then
    echo -e "Percentual success is ${GREEN}$result%\n${NOCOLOR}"
else
    echo -e "Percentual success is ${RED}$result%\n${NOCOLOR}"
fi