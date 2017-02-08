#how to user cppcheck
####Author xgmlovebee

####目录
[TOC]

##cppcheck compile and install
####download cppcheck from github
    url: https://github.com/danmar/cppcheck
####compile cppcheck
    because 1.7x support c++11, cents6.5 default NOT support, use 1.6x
    cd cppcheck
    git checkout 1.69
    make SRCDIR=build CFGDIR=/usr/bin/cfg HAVE_RULES=yes
    make install CFGDIR=/usr/bin/cfg

#####note: cppcheck: Failed to load library configuration file 'std.cfg'. File not found 
    this is because compile without CFGDIR


##how to use cppcheck
   
    Check test.cpp, enable all checks:
       cppcheck --enable=all --inconclusive --std=posix test.cpp

    Check f.cpp and search include files from inc1/ and inc2/:
        cppcheck -I inc1/ -I inc2/ f.cpp

