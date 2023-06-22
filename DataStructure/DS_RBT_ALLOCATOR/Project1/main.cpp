#include "Flag.h"
#include <iostream>
#include <cstdlib>

#include <vector>

class MyClass {
    // class definition
};

int main() {
    // The batch file you want to execute
    // const char* cmd = "command.bat";
    // 
    // // Execute the batch file
    // int result = system(cmd);
    // 
    // // Check the result of the command execution
    // if (result == 0) {
    //     std::cout << "Commands executed successfully." << std::endl;
    // }
    // else {
    //     std::cout << "Command execution failed." << std::endl;
    // }

    /*
    @echo off
    cd C:\\Users\user\\AppData\\Roaming\\Com2uS\\Lv1Engine\\Projects
    for /L %%i in (0,1,10) do (
        set foldername = 
        if exist Project_%%i (
           RD /S /Q Project_%%i 
        )
    )
    */



    unsigned short cal1 = 16;
    unsigned short cal2 = 68;

    std::cout << "cal1 : " << std::bitset<16>(cal1) << std::endl;
    std::cout << "cal2 : " << std::bitset<16>(cal2) << std::endl;

    unsigned short result = cal1 & cal2;
    
    std::cout << "rest : " <<  std::bitset<16>(result) << std::endl;
    std::cout << "rest : " <<  result << std::endl;
    return 0;
}
