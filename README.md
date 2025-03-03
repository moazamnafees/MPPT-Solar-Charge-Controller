# ☀️ MPPT Solar Charge Controller with Boost Converter ⚡

This project is an **MPPT (Maximum Power Point Tracking) Solar Charge Controller** designed using a **Boost Converter** to optimize power transfer from a solar panel to a battery. The system efficiently tracks the MPP using advanced algorithms and ensures reliable battery charging under varying sunlight conditions.

---

## 🚀 **Key Features**
- **MPPT Algorithms Implemented**: Perturb & Observe (P&O), Incremental Conductance
- **Boost Converter** for efficient voltage step-up
- **Microcontroller-Based Control** with real-time monitoring
- **Battery Charging Optimization** with multi-stage control
- **Extensive Testing & Validation** under different environmental conditions
- **Efficiency, Reliability, and Cost-Effectiveness** ensured

---

## 📟 **Hardware Components**
- **Microcontroller**: ESP32 / STM32 / Arduino
- **Solar Panel**: 12V or 24V (as per system requirements)
- **Boost Converter Circuit** (MOSFET, Inductor, Capacitor, Diode)
- **Battery**: Li-ion, LiFePO4, or Lead Acid (12V / 24V)
- **Current & Voltage Sensors**: INA219, ACS712, ADS1115
- **MOSFET Driver**: IR2110 / IR2104
- **LCD Display (Optional)**: OLED / 16x2 LCD
- **Protection Circuitry**: Overvoltage, Overcurrent, and Temperature Monitoring

---

## 📜 **Project Workflow**
1. **Solar Panel Input Measurement**
   - Read **voltage & current** using sensors
2. **MPPT Algorithm Implementation**
   - Adjust **PWM duty cycle
