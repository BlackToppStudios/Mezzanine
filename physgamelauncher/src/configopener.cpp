#include "configopener.h"

using namespace std;

ConfigOpener::ConfigOpener()
{

    configfile.open ("config.txt",fstream::in);

}

string ConfigOpener::GetExeWindows(){
    string   ExeName = "";

    configfile >> ExeName;
    return ExeName;
}
