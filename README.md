# airPi

Software + schematics for controlling ebm-papst fans with a Pi Pico.  
Inspired by [svenjust/room-ventilation-system](https://github.com/svenjust/room-ventilation-system).

## Software

The project is built using PlatformIO.

Configuration is done using the [`configuration.hpp`](code/include/configuration.hpp) file.

## Hardware

> ⚠ The 4-pin molex connectors are vertically mirrored.  
> The fans' plugs must be re-pinned when using the current schematics!

Schematics are can be found in [schematics](schematics/).

### Pico Pinout

| Function      | Pin | GPIO |
|---------------|-----|------|
| SDA           | 6   | GP4  |
| SCL           | 7   | GP5  |
| Mode Switch 1 | 16  | GP12 |
| Mode Switch 2 | 15  | GP11 |
| Mode Switch 3 | 14  | GP10 |
| Button        | 20  | GP15 |
| Tacho 1       | 21  | GP16 |
| Tacho 2       | 22  | GP17 |
| PWM 1         | 25  | GP19 |
| PWM 2         | 27  | GP21 |
| Trimmer       | 31  | GP26 |
| Temp Sensor   | 32  | GP27 |

### Components

- Controller
  - 2x BL 1X20G8 2,54 20pol. Buchsenleiste ✔
- Display
  - BL 1X20W8 2,54 20pol. Buchsenl., gewinkelt ✔
- Optokoppler: 2x SFH 617A-3 VIS ✔
- Stecker
  - 2x MOLEX 39300040 Molex, Stiftleiste - Mini-Fit Jr ✔
  - 3x MSTBA 3-G 5,08 Leiterplatten-Grundleiste ✔
  - 3x MSTB 3-ST 5,08 Leiterplattenstecker ✔
  - 1x MOLEX 39301020 Molex, Stiftleiste - Mini-Fit Jr ✔
  - 1x MSTBA 4-G 5,08 Leiterplatten-Grundleiste ✔
- Platine
  - LR-DS-79 Lochrasterplatine ✔
- Netzteil
  - AC/DC: IRM-10-12 AC/DC-Wandler ✔
    - **Lochraster checken**
  - Schotkydiode: 1N 5819RL STM ✔
  - LMO78_05-0.5 DC/DC-Wandler ✔
    - TANTAL 10/25 Tantal-Kondensator, Rm 2,5, 10µF/25V ✔
    - TANTAL 22/16 Tantal-Kondensator, Rm 2,5, 22µF/16V ✔
- Passive Komponenten
  - Sicherung (+ Halter)
    - Strom: 1,6A durch Motoren + 0,15A Netzteil
    - ESKA 522.021 Feinsicherung 5x20mm, träge (t), 2,5A ✔
    - ESKA 522.022 Feinsicherung 5x20mm, träge (t), 3,15A ✔
    - 2x PL 120000 Sicherungshalter, 5x20mm, max. 6,3A-500V ✔
    - 2-3x SWC 777098 Temperatursicherung ✔
    - EPC B72220-S 302 Varistor ✔
  - Poti
    - 64P-10K Präzisionspotentiometer ✔
  - Diode aus Schaltplan
    - ZD 11 Zenerdiode ✔
  - K/RES-E3 Sortiment, E3-Widerstände ✔
  - min. 2x KERKO 100N Keramik-Kondensator, 100 nF ✔
- SIK 275 2 Silikonkabel, 0,75 mm² ✔
- KUPFER 0,7MM Kupferlackdraht ✔
