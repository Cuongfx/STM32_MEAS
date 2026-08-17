# STM32F446RE — Teaching Projects

A collection of small, progressively-harder STM32 embedded projects built on the **Nucleo-F446RE** board, developed as teaching material for RPTU to introduce students to bare-metal / HAL-based STM32 programming with STM32CubeIDE.

Each project is self-contained: its own STM32CubeIDE project (`.project`/`.cproject`/`.ioc`), its own wiring diagram, and its own `README.md` explaining what it teaches and why it's built the way it is. Start at project 1 and work forward — each one reuses a concept from the previous one.

## Hardware

- **Board:** ST Nucleo-F446RE (STM32F446RETx, Cortex-M4)
- **IDE:** STM32CubeIDE (GCC / make toolchain — not Keil/IAR)

## Projects

| # | Project | Teaches |
|---|---|---|
| 1 | [1_Blink_LED](1_Blink_LED) | Non-blocking timing with `HAL_GetTick()` — two LEDs blinking at independent rates without `HAL_Delay()` blocking either one |
| 2 | [2_Button](2_Button) | Polling a button: mechanical debounce, single click, double click, long-press — all as a non-blocking state machine |
| 3.1 | [3_1_Button_Interupt](3_1_Button_Interupt) | Same button logic, but edge-detected via **EXTI interrupt**, restructured into a modular `lib/` with a reusable interrupt dispatcher (for when more than one interrupt source is needed) |
| 3.2 | [3_2_Button_Interrupt](3_2_Button_Interrupt) | Same interrupt-driven button logic, built **step by step in a single file** — the recommended path for walking a student through the reasoning, stage by stage, before showing them the modular version in 3.1 |
| 4.1 | [4_1_ADC_LED](4_1_ADC_LED) | True single-channel ADC read from a potentiometer, driving an LED's brightness through timer PWM |
| 4.2 | [4_2_Multi_ADC_LED](4_2_Multi_ADC_LED) | True multi-channel ADC using **hardware scan mode + DMA** — a potentiometer and an LDR read on the same ADC simultaneously, each driving its own LED's brightness independently |
| 5 | [5_Timer_Interrupt](5_Timer_Interrupt) | HC-SR04 ultrasonic distance sensor driving LED brightness — a **free-running timer (TIM3)** shared between `delay_us()` and Echo pulse timestamping, a **60ms Update interrupt (TIM6)** scheduling each ranging cycle, and **TIM4 PWM** for brightness; sensor logic isolated in its own `lib/sensor.c` |
| 6.1 | [6_1_UART_HELLO_WORLD](6_1_UART_HELLO_WORLD) | Talking to a PC over **UART** through a CH340G USB-to-serial bridge — blocking `HAL_UART_Transmit()` for output, **interrupt-driven receive** (`HAL_UART_Receive_IT` + `HAL_UART_RxCpltCallback`) for input, and a `volatile` flag that lets typed commands pause and resume the counting loop without ever blocking it |

## Folder structure (general)

```
STM32_F446RE/
├── README.md                    - this file
├── 1_Blink_LED/                 - project 1 (see table above)
├── 2_Button/                    - project 2
├── 3_1_Button_Interupt/         - project 3.1
├── 3_2_Button_Interrupt/        - project 3.2
├── 4_1_ADC_LED/                 - project 4.1
├── 4_2_Multi_ADC_LED/           - project 4.2
├── 5_Timer_Interrupt/           - project 5
└── 6_1_UART_HELLO_WORLD/        - project 6.1

# each numbered project folder follows the same internal layout:
<project>/
├── Core/Src, Core/Inc           - CubeMX-generated HAL init code + application code
├── Drivers/                     - STM32 HAL + CMSIS (generated, not hand-edited)
├── lib/                         - hand-written application modules, where present
├── <name>.ioc                   - CubeMX pin/clock configuration (open with STM32CubeIDE)
├── README.md                    - what this project teaches, pin mapping, wiring diagram
└── wiring_diagram.svg           - how to physically wire the breadboard for this project
```
