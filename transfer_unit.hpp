#pragma once
#include <vector>
#include <utility>
#include <string>
#include "interpreter_status.hpp"

#define TU_DEBUG

namespace BFInterpreter {

    typedef char token_t;
    typedef long int tokenArg_t;

    enum Tokens : token_t {
        INC_VAL = '+',
        DEC_VAL = '-',
        PREV_VAR = '<',
        NEXT_VAR = '>',
        INPUT_VAL = ',',
        OUTPUT_VAL = '.',
        BEGIN_LOOP = '[',
        END_LOOP = ']',
        END_SOURCE = '\0'
    };

    const tokenArg_t UNSET_ARG = -1;

    class TransferUnit {

    public:

        typedef std::pair<token_t, tokenArg_t> Command_t;

        explicit TransferUnit(const char* strSrc);
        explicit TransferUnit(std::string strSrc);

        ~TransferUnit()                                  = default;
        TransferUnit(const TransferUnit&)                = default;
        TransferUnit& operator=(const TransferUnit&)     = default;
        TransferUnit(TransferUnit&&) noexcept            = default;
        TransferUnit& operator=(TransferUnit&&) noexcept = default;

        Command_t operator[](size_t index) const noexcept;
        size_t GetLength(void) const noexcept;

        #ifdef TU_DEBUG
            friend void PrintListing(TransferUnit&);
        #endif

    private:

        typedef std::vector<Command_t> Instruction_t;

        Instruction_t listing;

        InterpRetCodes TranslateSource(const char*);
        size_t PushReducedOP(token_t, token_t, size_t, const char*);
        void BindBracketsByIndex(void);
    };

}
