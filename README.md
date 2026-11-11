# stm32-bare-metal

Bare-metal C on the STM32 NUCLEO-F446RE. My code uses no HAL and no vendor register headers; the register definitions are my own, with each address and bit position checked against the reference manual (RM0390).

- **Board:** NUCLEO-F446RE (STM32F446RE, Arm Cortex-M4)
- **Language:** C11 (gnu11 for firmware)
- **Tools:** STM32CubeIDE (firmware), VS Code + GCC (host-side tests)

## Projects

| Folder | What it is | Status |
|---|---|---|
| [`bit-toolkit/`](bit-toolkit/) | Header-only helpers to set, clear, toggle, and test bits and insert bit fields, with tests that run on a PC | Done |
| [`fake-peripheral/`](fake-peripheral/) | A register-block driver tested against a fake device in RAM | Done |
| [`01-led-button-blink/`](01-led-button-blink/) | Blinks LD2 at three speeds chosen with the B1 button, using polling and edge detection | Done |

## How to build

- **Host-side tests:** in `bit-toolkit/` or `fake-peripheral/`, run
  `gcc -Wall -Wextra -Werror main.c -o main`.
- **Firmware:** import `01-led-button-blink/` into STM32CubeIDE
  (File > Import > Existing Projects into Workspace) and run it on the board.

## What I'm learning

- C for embedded work
- Reading reference manuals
- Register-level programming, including mapping register structs onto hardware addresses
- Debugging with the ST-LINK
- Writing code that can be tested

## Roadmap

- UART output over the ST-LINK virtual COM port
- Replace busy-wait delays with a hardware timer
- ADC-based oscilloscope with data streaming to a PC