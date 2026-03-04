
Both channels can execute independent programs while sharing the same controller.

---

# Current Development Status

This project is **experimental** and under active development.

Currently implemented:

- multi trajectory planner support
- channel-based motion execution
- named axis infrastructure
- axis ownership system
- motion commands for axis acquire / release

Planned features:

- multi-channel G-code interpreter
- channel synchronization primitives
- UI support for multiple channels
- channel aware HAL signals
- program scheduling and synchronization

---

# Why This Project Exists

LinuxCNC is extremely powerful but currently limited to **single-channel execution**.

Many modern CNC machines require:

- parallel machining processes
- multiple tool carriers
- coordinated spindle handoff
- simultaneous machining operations

This fork explores how LinuxCNC can evolve into a **true multi-channel CNC control platform**.

---

# Contributing

This project is experimental and contributions are welcome.

Areas of interest:

- motion control architecture
- interpreter extensions
- real-time performance
- UI integration
- advanced CNC machine support

---

# License

This project remains licensed under the **GNU General Public License v2**, following the licensing of the original LinuxCNC project.