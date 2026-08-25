#include "core/core.h"
// #include "core/globals.h"


int main()
{
    Core nani;
    if(nani.start()){
        nani.Run();
    }
    nani.shutdown();
}
