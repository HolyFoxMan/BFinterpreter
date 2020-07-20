#pragma once

#include <stdexcept>
#include <string>

namespace BFInterpreter {

    enum InterpRetCodes : int {
        SUCCESSFUL,
        UNEXPECTED_CLOSED_BRACKET,
        NOT_CLOSED_BRACKET,
        UNKNOWN_COMMAND,
        EMPTY_SOURCE,
        FINISHED_SOURCE
    };

    class InterpreterException: public std::exception {

    private:
        std::string msg;
        InterpRetCodes code;

        std::string GetMsgByInterpRetCode(InterpRetCodes code) const;

    public:
        explicit InterpreterException(InterpRetCodes code) noexcept;
        virtual const char* what(void) const noexcept override final;
        InterpRetCodes GetCode(void) const noexcept;
    };
}
