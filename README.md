# MSM501-MainsReciprocalCounter
Measures the frequency of mains power.


Test points:
TP1 - Offset voltage to compensate for clipping diode drop. (~500mV DC)
TP2 - Divided and clipped transformer output voltage. (~4Vp half wave rectified sine)
TP3 - Unregulated rectified and smoothed transformer voltage. (>17V DC)
TP4 - DC Supply rails. (+5V and +3.3V DC)
TP5 - Reference oscillator signal. (1MHz or 10MHz 0-5V square wave)
TP6 - Gate-Enable signal from MCU. High if measurement is about to start or running.
TP7 - Input to the signal counter.
TP8 - Input to the reference counter.
TP9 - Transformer output voltage to measurement circuit. (>12V AC)
TP10 - External reference clock input.
TP11 - Reference input to the main gate. Should show same signal as TP10 or TP5 depending on JMP2 (Clock select).
TP12 - Signal input to the main gate. (0-5V square wave with same frequency as mains)

Jumpers:
JMP1 (J3) - Selects if measured signal is inverted. This should not affect the measurement. Recommend setting to INV not BUF. 
JMP2 (J4) - Selects external or internal reference clock. Check TP11 for selected signal.

Connectors and Headers:
GND - Ground connection for scope ground clips.
TP4 - DC voltage rails.
J2 - Serial interface. Connected to D0/D1 of arduino. (5V max.)
J5 - Debug pins. (SBUS0, SBUS1, SBUS2, SBUSE, CounterClear, GateTime, CounterRegClock, Reset)
J6 - CBUS. Used to read out the counter values.
CN1 - Mains input. Fuse with 5x20mm 32mA glass fuse in F1.
CN2 - External refernece input. BNC. 0-5V square wave only. HiZ input.
