#include <iostream>
#include "process.hpp"
#include <type_traits>

using namespace BFInterpreter;

int main()
{
    try {

        TransferUnit tu(" +++ --- +++ 343+- +++ ++  ++[>+++++++>++++r ggfdgdf++++++>+dfdf ___ %$%$ this is test++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.");
        PrintListing(tu);
        Process proc(tu);

        proc.FullExecute();

    } catch(const InterpreterException& ie) {
        std::cout << "Error: " << ie.what() << ", ret code: " << ie.GetCode() << std::endl;
    }

    return 0;
}
