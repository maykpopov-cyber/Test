# ButtonFilter (STM32 / CubeMX compatible)

A small 30 ms software debounce filter for a button. The implementation is C++, while its public header exposes only C-compatible declarations and `uint8_t`, so generated CubeMX files such as `main.c` do not need to be renamed or compiled as C++.

## Add to a CubeMX project

1. Copy `ButtonFilter.h` and `ButtonFilter.cpp` into the project (for example, `Core/Inc` and `Core/Src`).
2. Make sure the project includes `ButtonFilter.cpp` in its build and uses a C++ compiler for that file. Keep generated `main.c` and other generated C files unchanged. STM32CubeIDE projects with C++ support can compile the `.cpp` alongside C files; for other toolchains, enable C++ in the project while retaining C compilation for `.c` files.
3. Include `ButtonFilter.h` from `main.c`. Initialize the filter once, then call `ButtonFilter_Update` once per millisecond from the SysTick callback. Pass logical `1` for pressed and `0` for released. Convert active-low GPIO input to this logical convention at the call site.

```c
#include "ButtonFilter.h"

static ButtonFilter user_button;

/* During application initialization, after determining the current input: */
ButtonFilter_Init(&user_button, 0u);

/* Called at a 1 ms cadence from main.c: */
void HAL_SYSTICK_Callback(void)
{
    uint8_t pressed = (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) ? 1u : 0u;
    ButtonFilter_Update(&user_button, pressed);
}
```

Replace the GPIO port/pin and polarity conversion with the project's configuration. If SysTick runs faster or slower than 1 ms, call the update at a 1 ms cadence or adjust the debounce implementation accordingly. No peripheral clock setup is performed by this library.

## Read state and edges

`ButtonFilter_GetState()` returns the debounced state. `ButtonFilter_GetPressEdge()` and `ButtonFilter_GetReleaseEdge()` return `1` only on the update where that transition is accepted, and are cleared by the next update. These functions and `ButtonFilter_Update()` use `uint8_t` values; no C++ `bool` is exposed.
