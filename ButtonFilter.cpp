#include "ButtonFilter.h"

void ButtonFilter_Init(ButtonFilter *filter, uint8_t initial_state)
{
    if (filter == 0)
    {
        return;
    }

    initial_state = (initial_state != 0u) ? 1u : 0u;

    filter->state = initial_state;
    filter->press_edge = 0u;
    filter->release_edge = 0u;
    filter->candidate_state = initial_state;
    filter->stable_time_ms = BUTTON_FILTER_DEBOUNCE_MS;
}

void ButtonFilter_Update(ButtonFilter *filter, uint8_t raw_state)
{
    if (filter == 0)
    {
        return;
    }

    raw_state = (raw_state != 0u) ? 1u : 0u;
    filter->press_edge = 0u;
    filter->release_edge = 0u;

    if (raw_state != filter->candidate_state)
    {
        filter->candidate_state = raw_state;
        filter->stable_time_ms = 1u;
    }
    else if (filter->stable_time_ms < BUTTON_FILTER_DEBOUNCE_MS)
    {
        ++filter->stable_time_ms;
    }

    if ((filter->stable_time_ms >= BUTTON_FILTER_DEBOUNCE_MS) &&
        (filter->state != filter->candidate_state))
    {
        filter->state = filter->candidate_state;
        if (filter->state != 0u)
        {
            filter->press_edge = 1u;
        }
        else
        {
            filter->release_edge = 1u;
        }
    }
}

uint8_t ButtonFilter_GetState(const ButtonFilter *filter)
{
    return (filter != 0) ? filter->state : 0u;
}

uint8_t ButtonFilter_GetPressEdge(const ButtonFilter *filter)
{
    return (filter != 0) ? filter->press_edge : 0u;
}

uint8_t ButtonFilter_GetReleaseEdge(const ButtonFilter *filter)
{
    return (filter != 0) ? filter->release_edge : 0u;
}
