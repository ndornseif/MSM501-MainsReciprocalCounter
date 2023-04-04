# MSM501-MainsReciprocalCounter
Measures the frequency of mains power.  
Hardware version: 1.1  
Firmware version: 1.6  
## Overview
This device was built as an exercise in digital electronics. It uses a reciprocal frequency counter approach to measure the frequency of the mains voltage supplying it.   
Using two SN74LV8154N 32bit counters, it counts pulses of the mains signal and the reference clock. The start and end of measurement are syncronized to the input signal using a D-FlipFlop to open the main gate. 
This eliminates the influence of gate time on the measurement resolution.  
The measurement result is then calculated as follows:  
$measuredFreq = (signalCounterValue * referenceClockFrequency) / referenceCounterValue$
## Connectors/Headers
- GND: Ground connection for scope ground clips.
- TP4: DC voltage rails.
- J2: Serial interface. Connected to D0/D1 of Arduino. (5V max.)
- J5: Debug pins. (SBUS0, SBUS1, SBUS2, SBUSE, CounterClear, GateTime, CounterRegClock, Reset)
- J6: CBUS. Used to read out the counter values.
- CN1: Mains input. Fuse with 5x20mm 32mA glass fuse in F1.
- CN2: External reference input. BNC. 0-5V square wave only. HiZ input.
## Jumpers
- JMP1 (J3): Selects if measured signal is inverted. This should not affect the measurement. Recommended setting is INV.
- JMP2 (J4): Selects external or internal reference clock. Check TP11 for selected signal.
## Test points
- TP1: Offset voltage to compensate for the clipping diode drop. (~ 500mV DC)
- TP2: Divided and clipped transformer output voltage. (~ 4V~p~ half wave rectified sine)
- TP3: Unregulated rectified and smoothed transformer voltage. (> 17V DC)
- TP4: DC Supply rails. (+5V and +3.3V DC)
- TP5: Reference oscillator signal. (1MHz or 10MHz 0-5V square wave)
- TP6: Gate-Enable signal from FlipFlop. High if main gate is open and measurement running.
- TP7: Input to the signal counter.
- TP8: Input to the reference counter.
- TP9: Transformer output voltage to measurement circuit. (> 12V AC)
- TP10: External reference clock input.
- TP11: Reference input to the main gate. Should show the same signal as TP10 or TP5 depending on JMP2 (Clock select).
- TP12: Signal input to the main gate. (0-5V square wave with same frequency as mains)
