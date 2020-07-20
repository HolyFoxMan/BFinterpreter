#pragma once
#include "interpreter_status.hpp"
#include "transfer_unit.hpp"

namespace BFInterpreter {
    IterpRetCodes execute(TransferUnit& tu, ProcessData& pd);
}
