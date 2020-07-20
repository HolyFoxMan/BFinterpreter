#include "process.hpp"
#include <cstdio>

using namespace BFInterpreter;

InterpRetCodes Process::ExecuteNextCommand(void) noexcept
{
    if (completeStatus)
        return FINISHED_SOURCE;

    auto command = currentListing[listingIndex];

    switch (command.first) {

    case INC_VAL:
        data[dataIndex] += command.second;
        ++listingIndex;
        break;

    case DEC_VAL:
        data[dataIndex] -= command.second;
        ++listingIndex;
        break;

    case PREV_VAR:
        if (dataIndex < static_cast<size_t>(command.second))
            dataIndex += dataCapacity - command.second;
        else
            dataIndex -= command.second;
        ++listingIndex;
        break;

    case NEXT_VAR:
        dataIndex = (dataIndex + command.second) % dataCapacity;
        ++listingIndex;
        break;

    case INPUT_VAL:
        data[dataIndex] = std::getc(stdin);
        ++listingIndex;
        break;

    case OUTPUT_VAL:
        std::putc(data[dataIndex], stdout);
        ++listingIndex;
        break;

    case BEGIN_LOOP:
        if (!data[dataIndex])
            listingIndex = command.second + 1;
        else
            ++listingIndex;
        break;

    case END_LOOP:
        if (!data[dataIndex])
            ++listingIndex;
        else
            listingIndex = command.second + 1;
        break;

    case END_SOURCE:
        completeStatus = true;
        break;

    default:
        return UNKNOWN_COMMAND;
    };

    return SUCCESSFUL;
}
