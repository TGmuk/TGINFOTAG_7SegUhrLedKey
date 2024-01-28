# Timer auf SPI Led&Key Shield

Timer auf 7Seg externem Shield

# Module

- Led&Key SPI Shield (STB: PB_12, CLK: PB_13, DIO: PB_15 (PB_14), Vcc: 3.3V, GND: GND)

## Notes

Der Timer zählt hoch.

Mit PA_10 kann er gestartet werden.  
Mit PA_6 kann er gestoppt werden.  
Mit PA_1 kann er zurückgesetzt werden.

Die Knöpfe auf dem Shield können auf PortC (LEDs) ausgeben werden, wenn man DIO zusätzlich auch noch mit PB_14 verbindet.  
Wenn dies nicht passiert, leuchten die LEDs dauerhaft.  
Alternativ kann die Codeziele auch aus dem Code auskommentiert werden.


Das Projekt kann sowohl in Keil Studio als auch mit PlatformIO geöffnet werden.

Gemacht von Leo