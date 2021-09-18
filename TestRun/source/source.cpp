#include <VkNuts.h>

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>

int main() {

    auto app = nuts::Engine::getInstance();

    app->run();

    app->close();

}
