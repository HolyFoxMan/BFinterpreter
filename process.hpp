#pragma once
#include "transfer_unit.hpp"

namespace BFInterpreter {

    class Process {

    public:
        typedef unsigned char element_t;

        Process(TransferUnit& entryPoint, size_t dataCapacity = DEF_DATA_CAPACITY);
        ~Process() noexcept;
        Process(const Process&);
        Process& operator=(const Process&);
        Process(Process&&) noexcept;
        Process& operator=(Process&&) noexcept;

        bool IsFinished(void) const noexcept;
        InterpRetCodes ExecuteNextCommand(void) noexcept;
        void FullExecute(void);

    private:

        static constexpr size_t DEF_DATA_CAPACITY = 30000;

        element_t* data;
        size_t dataCapacity,
               dataIndex,
               listingIndex;
        bool completeStatus;
        TransferUnit& currentListing;

        void AssignFundVars(const Process&);
    };

}
