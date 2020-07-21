#include "transfer_unit.hpp"

using namespace BFInterpreter;

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
