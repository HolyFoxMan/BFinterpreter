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

TransferUnit::TransferUnit(std::istream strSource)
:
    listing(Instruction_t())
{

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

    auto prev_com = !listing.empty()? std::prev(listing.end()) : listing.end();

        /* if last catched token operator is semantically matches with current */
    if (prev_com != listing.end() && (prev_com->first == inc_op_token || prev_com->first == dec_op_token))
        prev_com->second += genTrend;
    else {

        token_t decisive;
        if (genTrend > 0)
            decisive = inc_op_token;
        else {
            decisive = dec_op_token;
            genTrend = -genTrend;
        }
        listing.push_back({decisive, genTrend});

    }

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
            std::cout << com.first << ", " << com.second << "; ";
        std::cout << std::endl;
    }
}
#endif
