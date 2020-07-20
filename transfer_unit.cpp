#include "transfer_unit.hpp"
#include <stack>

using namespace BFInterpreter;

TransferUnit::TransferUnit(const char* strSource)
:
    listing(Instruction_t())
{
    if (strSource == nullptr)
        throw std::invalid_argument("buf arg pointer is null");

    InterpRetCodes ret = TranslateSource(strSource);
    if (ret != SUCCESSFUL)
        throw InterpreterException(ret);
}

TransferUnit::TransferUnit(std::string strSource)
:
    listing(Instruction_t())
{
    if (strSource.length() == 0)
        throw std::invalid_argument("string of source if empty");

    InterpRetCodes ret = TranslateSource(strSource.c_str());
    if (ret != SUCCESSFUL)
        throw InterpreterException(ret);
}

InterpRetCodes TransferUnit::TranslateSource(const char* strSource)
{
    enum States : int {
        OUT,
        GET_TOKEN,
        REDUCE_OP_VAL,
        REDUCE_OP_VAR
    }
        state = GET_TOKEN;
    size_t  index = 0,
            bracketsRatio = 0;

    do {
        switch(state) {

        case GET_TOKEN:

            switch (strSource[index]) {

            case DEC_VAL:
            case INC_VAL:

                state = REDUCE_OP_VAL;
                break;

            case PREV_VAR:
            case NEXT_VAR:

                state = REDUCE_OP_VAR;
                break;

            /* brackets ratio checking */
            case BEGIN_LOOP:

                listing.push_back({BEGIN_LOOP, UNSET_ARG});
                ++bracketsRatio;
                ++index;
                break; // state == GET_TOKEN

            case END_LOOP:

                if (!bracketsRatio)
                    return UNEXPECTED_CLOSED_BRACKET;
                listing.push_back({END_LOOP, UNSET_ARG});
                --bracketsRatio;
                ++index;
                break; // state == GET_TOKEN

            case INPUT_VAL:

                listing.push_back({INPUT_VAL, UNSET_ARG});
                ++index;
                break;

            case OUTPUT_VAL:

                listing.push_back({OUTPUT_VAL, UNSET_ARG});
                ++index;
                break;

            default:

                if (strSource[index] == END_SOURCE) {
                    if (bracketsRatio)
                        return NOT_CLOSED_BRACKET;
                    state = OUT;
                } else
                    ++index;
                break;
            };
            break; // GET_TOKEN state

        case REDUCE_OP_VAL:

            index = PushReducedOP(INC_VAL, DEC_VAL, index, strSource);
            state = GET_TOKEN;
            break;  // REDUCE_OP_VAL state

        case REDUCE_OP_VAR:

            index = PushReducedOP(NEXT_VAR, PREV_VAR, index, strSource);
            state = GET_TOKEN;
            break;  // REDUCE_OP_VAR state

        case OUT:
            break;

        };
    } while (state); // != OUT

    if (listing.empty())
        return EMPTY_SOURCE;

    BindBracketsByIndex();

    return SUCCESSFUL;
}

size_t TransferUnit::PushReducedOP(token_t inc_op_token, token_t dec_op_token, size_t index, const char* strSource)
{
    tokenArg_t genTrend = 0;

    for (;;) {
        if (strSource[index] == inc_op_token)
            ++genTrend;
        else if (strSource[index] == dec_op_token)
            --genTrend;
        else
            break;
        ++index;
    }

    token_t decisive_token = genTrend > 0? inc_op_token : dec_op_token;
    Command_t prev_com = !listing.empty()? listing.back() : Command_t({END_SOURCE, UNSET_ARG});

    /* if last catched token operator is semantically matches with current */
    if (prev_com.first == inc_op_token || prev_com.first == dec_op_token)
        prev_com.second += genTrend;
    else
        listing.push_back({decisive_token, genTrend > 0? genTrend : -genTrend});

    return index;
}

void TransferUnit::BindBracketsByIndex(void)
{
    std::stack<Instruction_t::iterator> bracketsScopes;

    for (auto op = listing.begin(); op != listing.end(); ++op) {

        if (op->first == BEGIN_LOOP)    // token
            bracketsScopes.push(op);
        else if (op->first == END_LOOP) {
            /* index exchange */
            bracketsScopes.top()->second = op - listing.begin();
            op->second = bracketsScopes.top() - listing.begin();
            bracketsScopes.pop();
        }
    }
}

TransferUnit::Command_t TransferUnit::operator[](size_t index) const noexcept
{
    if (index >= listing.size())    // to avoid exception throwing by vector
        return Command_t({END_SOURCE, UNSET_ARG});

    return listing[index];
}

size_t TransferUnit::GetLength(void) const noexcept
{
    return listing.size();
}

#ifdef TU_DEBUG
#include <iostream>
namespace BFInterpreter {
    void PrintListing(TransferUnit& tu)
    {
        for(auto& com : tu.listing)
            std::cout << com.first << "arg: " << com.second << std::endl;
    }
}
#endif
