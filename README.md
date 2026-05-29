# TM4-Traffic-Controller
Embedded software and supporting tools for a traffic-light controller running on the Texas Instruments Tiva(TM) TM4C1294NCPDT microcontroller.

Refer to documentation in the documentation folder for Requirements, Architecture and Design, and Testing:
1. [Requirements](documentation/1_Engineering.md)
2. [Architecture & Design](documentation/2_Architecture_Design.md)
3. [Testing](documentation/3_Testing.md)

Summary
- **What:** Real-time traffic light controller firmware, a desktop prototype, and test/data tools.
- **Hardware:** TI Tiva TM4C1294NCPDT (see `tm4c1294ncpdt_startup_ccs.c`, `tm4c1294ncpdt.cmd`).

Repository layout
- `main.c` — firmware entry point and application logic.
- `tm4c1294ncpdt_startup_ccs.c`, `tm4c1294ncpdt.cmd` — startup and linker script for CCS.
- `prototype/` — desktop prototype implementation (`prototype.cpp`).
- `testing/` — test harnesses and scripts (`testing.py`) and `data/` with sample CSVs.
- `documentation/` — requirements, architecture and testing documents.

Build & flash (TI Code Composer Studio)
- Open a CCS project and include `main.c`, the startup file, and the linker command file `tm4c1294ncpdt.cmd`.
- Build in CCS and flash using the supported debug probe (e.g. XDS). Adjust project settings to match your board/target.

Run the prototype locally
- Compile the prototype (requires a C++ toolchain):

	g++ -std=c++17 -O2 prototype/prototype.cpp -o prototype/sim
	./prototype/sim

Testing
- Run the Python test harness (requires Python 3):

	python3 testing/testing.py

See `testing/data/` for sample input CSVs used by the test harness.

License
- This project is licensed under the Apache License 2.0. See the bundled `LICENSE` file for details.

Documentation
- Requirements: [documentation/1_Engineering.md](documentation/1_Engineering.md)
- Architecture & Design: [documentation/2_Architecture_Design.md](documentation/2_Architecture_Design.md)
- Testing: [documentation/3_Testing.md](documentation/3_Testing.md)

Contributing
- Issues and pull requests are welcome. Keep changes focused and provide reproduction steps for bugs.

Contact
- For questions, open an issue or review the top-level documentation files in `documentation/`.

Authors
- Rustenis Tolpeznikas
- Obed Oyandut
- Copyright 2023