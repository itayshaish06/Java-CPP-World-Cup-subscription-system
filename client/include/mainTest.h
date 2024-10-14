#pragma once
#include <stdlib.h>
#include "../include/ConnectionHandler.h"
#include "../include/summary.h"
#include "../include/user.h"
#include "../include/StompProtocol.h"


class mainTest
{
public:
    static void test();
    static void testWithOutThreads(string loginCommand);
    static void sleep(int millis);
private:
    static string commmands[5];
    static void test2Run(string line, StompProtocol &protocol, ConnectionHandler &connectionHandler);
};
