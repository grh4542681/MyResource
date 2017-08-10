#!/bin/bash

##  (1) English - United States
##  (2) Deutsch - Deutschland
##  (3) Español - España
##  (4) Français - France
##  (5) Italiano - Italia
##  (6) 日本語
##  (7) 한국어
##  (8) Português - Brasil
##  (9) Русский
##  (10) 中文 - 中华人民共和国
##  (11) 中文 - 台灣

Locale=`echo $LANG|awk -F '.' '{print $1}'`
if [[ $Locale == "en_US" ]]
then
	echo "1"
elif [[ $Locale == "zh_CN" ]]
then 
	echo "10"
elif [[ $Locale == "ja_JP" ]]
then 
	echo "6"
elif [[ $Locale == "fr_FR" ]]
then 
	echo "4"
elif [[ $Locale == "ko_KR" ]]
then 
	echo "7"
elif [[ $Locale == "it_IT" ]]
then 
	echo "5"
elif [[ $Locale == "es_ES" ]]
then 
	echo "3"
elif [[ $Locale == "zh_TW" ]]
then 
	echo "11"
elif [[ $Locale == "de_DE" ]]
then 
	echo "2"
elif [[ $Locale == "pt_BR" ]]
then 
	echo "8"
elif [[ $Locale == "ru_RU" ]]
then 
	echo "9"
fi
echo $LANGID
