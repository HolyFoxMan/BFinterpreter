#include "interpreter_status.hpp"

using namespace BFInterpreter;

std::string InterpreterException::GetMsgByInterpRetCode(InterpRetCodes code) const
{
    std::string genMsg;

    switch (code) {

        case SUCCESSFUL:
            break;

        default:
            genMsg = "Unknown code";
            break;
    };

    return genMsg;
}

InterpreterException::InterpreterException(InterpRetCodes code) noexcept
:
    msg(GetMsgByInterpRetCode(code)),
    code(code)
{}

const char* InterpreterException::what(void) const noexcept
{
    return msg.c_str();
}

InterpRetCodes InterpreterException::GetCode(void) const noexcept
{
    return code;
}
