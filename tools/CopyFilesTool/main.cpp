#include <iostream>
#include <string>
using namespace std;

#define WINDOWS
#define GNU

void CopyFile(string Source, string Destination);

int main(int ArgCount, char** ArgValueArray)
{
    if(2 == ArgCount)
    {
        cout<< "Do Logic Here";
    }else{
        cout<< "Display usage here";
    }

    return 0;
}


void CopyFile(string Source, string Destination)
{
    #ifdef WINDOWS
    //system()
    #endif

    #ifdef GNU

    #endif
}
