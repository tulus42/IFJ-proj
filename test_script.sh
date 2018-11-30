#!/bin/sh
# Ok lze najit na http://collabedit.com/ktpgj a sami zde muzete pridat pro ostatni testy
# Wrong lze najit na http://collabedit.com/42s8p a sami zde muzete pridat pro ostatni testu
# Pridavejte to co vite ze ma byt spatne nebo dobre

code_ok='test_code_ok.in'
code_ok_path='http://meetingwords.com/ep/pad/export/1Ow2YQW0lO/latest?format=txt'
code_wrong='test_code_wrong.in'
code_wrong_path='http://meetingwords.com/ep/pad/export/vOhiBCS7cH/latest?format=txt'
latest_test='latest_test.in'
sep='$'

if [ $# -lt 1 ]; then
	echo "Incorrect amount of arguments. Sent just path to interpreter and optionally local files ok_code wrong_code"
	exit 0
elif [ $# -eq 3 ]; then
	echo ""
	echo "Pouziji se lokalni soubory '$2' a '$3'"
else
	wget -O $code_ok $code_ok_path
	if [ "$?" != "0" ]; then
		echo "INTERNI CHYBA: Nelze stahnout $code_ok_path"
		exit 1
	fi   
	wget -O $code_wrong $code_wrong_path 
	if [ "$?" != "0" ]; then
		echo "INTERNI CHYBA: Nelze stahnout $code_wrong_path"
		exit 1
	fi   
	echo ""
	echo "Online soubory stazene - Pokud jsou chybne je sance, ze jsi je stahl v dobe kdyz je nekdo zrovna upravuje"
fi

COMPILER=$1

echo "Test nezacal" > latest_test.in # Prepsani souboru

test_code_ok(){
	$COMPILER < $latest_test 1> /dev/null
	if [ "$?" != "0" ]; then
		echo "CHYBA: Kompiler interpretoval spravny kod jako chybny (kod je v $latest_test) a vypsany nize"
		if [ "$DISPLAY_TEST" != "0" ]; then
			cat $latest_test
		fi
		exit 1
	fi   
}

test_code_wrong(){
	printf "Error: "
	$COMPILER < $latest_test 1> /dev/null
	if [ "$?" == "0" ]; then
		echo "CHYBA: Kompiler interpretoval chybny kod jako spravny (kod je v $latest_test) a vypsany nize"
		if [ "$DISPLAY_TEST" != "0" ]; then
			cat $latest_test
		fi
		exit 1
	fi   
}

# Ok part

if [ ! -z $DISABLE_OK ]; then
	echo "WARNING: Ok_code se nekontroluje (je nastavene DISABLE_OK)"
else
	echo ""
	echo "----------------------------"
	echo "Testovani korektnich vstupu. Kompiler by nemel zahlasit chybu"
	echo "Testovaci kod se bere ze souboru '$code_ok'"
	echo "Pokud zahlasi chybu, testovany kod je v '$latest_test'"
	start=0
	while IFS= read -r line
	do
		if [ "$line" == "$sep" ]; then
			if [ $start -eq 1 ]; then 
				start=0 # end
				test_code_ok
			else
				start=1 # start
				printf "" > $latest_test
			fi
		else
			echo $line >> $latest_test
		fi
	done <"$code_ok"
	echo "------------ OK ------------"
fi

# Wrong code
if [ ! -z $DISABLE_WRONG ]; then
	echo "WARNING: Ok_code se nekontroluje (je nastavene DISABLE_OK)"
else
	echo ""
	echo "----------------------------"
	echo "Testovani spatneho vstupu. Kompiler by musi zahlasit chybu"
	echo "Testovaci kod se bere ze souboru '$code_wrong'"
	echo "Pokud nezahlasi chybu, testovany kod je v '$latest_test'"

	start=0
	while IFS= read -r line
	do
		if [ "$line" == "$sep" ]; then
			if [ $start -eq 1 ]; then 
				start=0 # end
				test_code_wrong
			else
				start=1 # start
				printf "" > $latest_test
			fi
		else
			echo $line >> $latest_test
		fi
	done <"$code_wrong"
	echo "------------ OK ------------"
fi

echo
echo "VSECHNY TESTY PROBEHLY OK"
echo 