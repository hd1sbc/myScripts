#!/bin/bash

returnArrNoDups=()

removeDupsString(){
        returnArrString=""

        for element in $1; do
                foundMatchBool=0
                for element2 in $1; do
                        if [ "$element" == "$element2" ]; then foundMatchBool=$((foundMatchBool + 1)); fi
                done

                if [ $foundMatchBool -le 1 ]; then
                        returnArrString+=$element
                fi
        done

        echo $returnArrString
}

checkDupsString(){
        for element in $@; do
                foundNumMatches=0
                for element2 in $@; do
                        if [ $element == $element2 ]; then foundNumMatches=$((foundNumMatches + 1)); fi
                done

                if [ $foundNumMatches -gt 1 ]; then
                        echo "Found $foundNumMatches matches for $element"
                fi
        done

}

removeDuplicatesFromArr(){
	for element in ${1[@]}; do
		if [[ ! " ${1[@]} " =~ " ${element} " ]]; then
			returnArrNoDups+=("$element")
		fi
	done
}

