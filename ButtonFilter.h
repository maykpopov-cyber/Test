#ifndef BUTTON_FILTER_H
#define BUTTON_FILTER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BUTTON_FILTER_DEBOUNCE_MS 30u

typedef struct
{
    uint8_t state;           /* Stable state: 1 = pressed, 0 = released. */
    uint8_t press_edge;      /* 1 for the update that detects a press. */
    uint8_t release_edge;    /* 1 for the update that detects a release. */
    uint8_t candidate_state; /* Input level currently being qualified. */
    uint8_t stable_time_ms;  /* Consecutive milliseconds at candidate_state. */
} ButtonFilter;

/*
 * Initialize with the known current logical state (1 = pressed, 0 = released).
 * Edges are clear after initialization. Call once before using Update/Getters.
 */
void ButtonFilter_Init(ButtonFilter *filter, uint8_t initial_state);

/*
 * Supply one sample per millisecond. raw_state must be 1 when pressed and 0
 * when released. The stable state changes after 30 consecutive matching
 * samples; press_edge/release_edge are one-update pulses.
 */
void ButtonFilter_Update(ButtonFilter *filter, uint8_t raw_state);

uint8_t ButtonFilter_GetState(const ButtonFilter *filter);
uint8_t ButtonFilter_GetPressEdge(const ButtonFilter *filter);
uint8_t ButtonFilter_GetReleaseEdge(const ButtonFilter *filter);

#ifdef __cplusplus
}
#endif

#endif /* BUTTON_FILTER_H */
