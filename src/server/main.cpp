#define DRAWSOMETHING_SERVER_VERSION "1.0.0"
#define DRAWSOMETHING_SERVER_NAME "DrawSomething Server"

#include <iostream>

#include "../shared/core/Strings.h"

int main() {
    system("");
    Strings::PrintDivider(DRAWSOMETHING_SERVER_NAME, false);
    std::cout << Strings::FStringColors("&3%s %s", DRAWSOMETHING_SERVER_NAME, DRAWSOMETHING_SERVER_VERSION) << std::endl;
    std::cout << Strings::FStringColors("[Server] &aHello there!") << std::endl;
    Strings::PrintDivider();



    Strings::PrintDivider("Destroying");
    Strings::PrintDivider();

    return 0;
}
