


g++ -pthread -pg -g -std=c++23 out.cpp -o out.out

if [ $? -eq 0 ]; then
	echo -e "\nCompilation Ok."
	echo      "==============="
	#./out.out
else
	echo -e "\nCompilation error."
	echo      "=================="
fi




#  -no-c[omparison-checks] Disable mixed-sign comparison safety checks
#  -no-n[ull-checks]       Disable null safety checks
#  -no-s[ubscript-checks]  Disable subscript safety checks

#  -fno-e[xceptions]       Disable C++ EH - failed 'as' for 'variant' will assert
#  -fno-r[tti]             Disable C++ RTTI - using 'as' for '*'/'std::any' will assert
