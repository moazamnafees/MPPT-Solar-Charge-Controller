# ☀️ MPPT Solar Charge Controller with Boost Converter ⚡

This project is an **MPPT (Maximum Power Point Tracking) Solar Charge Controller** designed using a **Boost Converter** to optimize power transfer from a solar panel to a battery. The system efficiently tracks the MPP using advanced algorithms and ensures reliable battery charging under varying sunlight conditions. It is implemented using **ESP32**, **ADS1115 ADC**, and **PWM modulation** for real-time control.

---

## 🚀 **Key Features**
- **MPPT Algorithms Implemented**: Perturb & Observe (P&O), Incremental Conductance
- **Boost Converter** for efficient voltage step-up
- **Dual-Core ESP32 Multitasking Support**
- **Battery Charging Optimization** with multi-stage control
- **Extensive Testing & Validation** under different environmental conditions
- **Efficiency, Reliability, and Cost-Effectiveness** ensured

---

## 📟 **Hardware Components**
- **Microcontroller**: ESP32
- **Solar Panel**: 12V or 24V (as per system requirements)
- **Boost Converter Circuit** (MOSFET, Inductor, Capacitor, Diode)
- **Current & Voltage Sensors**: ADS1115, INA219, ACS712
- **MOSFET Driver**: IR2110 / IR2104
- **Protection Circuitry**: Overvoltage, Overcurrent, and Temperature Monitoring

---

## 📜 **Project Workflow**
1. **Solar Panel Input Measurement**
   - Read **voltage & current** using **ADS1115 ADC**
2. **MPPT Algorithm Implementation**
   - Adjust **PWM duty cycle** dynamically to track MPP
   - Use **P&O or Incremental Conductance**
3. **Boost Converter Control**
   - Increase voltage to match battery charging requirements
4. **Battery Charging Management**
   - Implement **multi-stage charging** (Bulk, Absorption, Float)
5. **Monitoring & Safety Protections**
   - Display data on LCD / Send to Serial Monitor
   - Prevent **overvoltage, overcurrent, and overheating**

---

## 📜 **Code Implementation Overview**
- **Real-time voltage and current sensing** using ADS1115
- **Dual-core processing** for efficient multitasking
- **PWM control** using ESP32 hardware timers
- **Dynamic adjustment of PWM duty cycle** for optimal power conversion
- **Device protection mechanisms** to prevent damage under abnormal conditions

---


