#download gtest code from github
   https://github.com/google/googletest

   now latest release gtest is 1.8.0

#install in linux

   ```
   cd ${gtest_src}
   mkdir mybuild
   cd mybulid
   make;make install
   ```

#write test code
   see the gtest demo

#generate xml report
    ./gtest_demo --gtest_output=xml:/tmp/detail_gtest.xml
