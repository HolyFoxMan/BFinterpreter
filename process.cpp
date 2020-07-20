#include "process.hpp"
#include <stdexcept>
#include <cstring>

using namespace BFInterpreter;

Process::Process(TransferUnit& entryPoint, size_t dataCapacity)
:
    data(new element_t[dataCapacity]),
    dataCapacity(dataCapacity),
    dataIndex(0),
    listingIndex(0),
    completeStatus(false),
    currentListing(entryPoint)
{
    if (!dataCapacity)
        throw std::range_error("data capacity must be greater than zero");
    std::memset(data, 0, dataCapacity);
}

Process::~Process() noexcept
{
    delete[] data;
}

Process::Process(const Process& other)
:
    data(new element_t[other.dataCapacity]),
    dataCapacity(other.dataCapacity),
    dataIndex(other.dataIndex),
    listingIndex(other.listingIndex),
    completeStatus(other.completeStatus),
    currentListing(other.currentListing)
{
    std::memcpy(data, other.data, dataCapacity);
}

Process::Process(Process&& other) noexcept
:
    data(other.data),
    dataCapacity(other.dataCapacity),
    dataIndex(other.dataIndex),
    listingIndex(other.listingIndex),
    completeStatus(other.completeStatus),
    currentListing(other.currentListing)
{
    other.data = nullptr;
}

Process& Process::operator=(const Process& other)
{
    if (this == &other)
        return *this;

    if (dataCapacity != other.dataCapacity) {
        delete[] data;
        data = new element_t[other.dataCapacity];
    }

    AssignFundVars(other);
    std::memcpy(data, other.data, dataCapacity);

    return *this;
}

Process& Process::operator=(Process&& other) noexcept
{
    if (this == &other)
        return *this;

    delete[] data;
    data = other.data;
    other.data = nullptr;

    AssignFundVars(other);

    return *this;
}

void Process::AssignFundVars(const Process& other)
{
    dataCapacity = other.dataCapacity;
    dataIndex = other.dataIndex;
    listingIndex = other.listingIndex;
    completeStatus = other.completeStatus;
    currentListing = other.currentListing;
}

bool Process::IsFinished(void) const noexcept
{
    return completeStatus;
}

void Process::FullExecute(void)
{
    for(;;) {

        InterpRetCodes stat = ExecuteNextCommand();
        if (stat == UNKNOWN_COMMAND)
            throw InterpreterException(stat);

        if (completeStatus)
            break;
    }
}
