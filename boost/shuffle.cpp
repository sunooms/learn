#include <iostream>     // std::cout
#include <algorithm>    // std::shuffle
#include <array>        // std::array
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include <string.h>
#include <unistd.h>

int main () {
    std::array<int,5> foo {1,2,3,4,5};
    std::vector<std::string> users;
    char buf[32] = {0};

    // now begin to fill user data
    for(int i=0; i<100; i++)
    {
        snprintf(buf, sizeof(buf), "1user_%d", i);
        users.push_back(buf);
    }

    for(int i=0; i<100; i++)
    {
        snprintf(buf, sizeof(buf), "2user_%d", i);
        users.push_back(buf);
        users.push_back(buf);
    }
    for(int i=0; i<100; i++)
    {
        snprintf(buf, sizeof(buf), "3user_%d", i);
        users.push_back(buf);
        users.push_back(buf);
        users.push_back(buf);
    }
    for(int i=0; i<100; i++)
    {
        snprintf(buf, sizeof(buf), "4user_%d", i);
        users.push_back(buf);
        users.push_back(buf);
        users.push_back(buf);
        users.push_back(buf);
    }
    for(int i=0; i<100; i++)
    {
        snprintf(buf, sizeof(buf), "5user_%d", i);
        users.push_back(buf);
        users.push_back(buf);
        users.push_back(buf);
        users.push_back(buf);
        users.push_back(buf);
    }



    // obtain a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    shuffle (foo.begin(), foo.end(), std::default_random_engine(seed));
    shuffle (users.begin(), users.end(), std::default_random_engine(seed));

    std::cout << "shuffled elements:";
    for (int& x: foo) std::cout << ' ' << x;
    std::cout << '\n';

    FILE *fp=NULL;

    fp = fopen("./users.txt", "a");
    if(NULL == fp)
    {
        printf("can't open users.txt file\n");
        return -1;
    }
    for (std::string& user: users)
    {
        //int count = atoi(user.c_str());
        //snprintf(buf,sizeof(buf), "%s,%d\n", user.c_str(), count);
        snprintf(buf,sizeof(buf), "%s,1\n", user.c_str());
        fwrite(buf, strlen(buf), 1, fp);
    }

    return 0;
}
