# Intelligent CCTV Surveillance Using AI-Based Detection and Recognition

A C++17 PBL prototype for demonstrating access checks, event classification, security alerts, and event-history logging. The current repository implements the **application-logic prototype**; it is not yet connected to a live CCTV camera or an AI inference model.

## Current features

- Menu-driven console interface.
- Checks names against `data/authorized_personnel.txt` (case-insensitive).
- Add, list, and delete authorized personnel (deletion requires confirmation).
- Add, list, and delete authorized places/areas using `data/authorized_places.txt` (deletion requires confirmation).
- Check both person authorization and location authorization.
- Record a manually entered event with location and suspicious-object flag.
- Classifies events using simple demo rules:
  - **HIGH RISK** — suspicious/restricted object reported OR location is not authorized.
  - **MEDIUM RISK** — person is not authorized, but the location is approved and no suspicious object is reported.
  - **NORMAL** — person and location are authorized and no suspicious object is reported.
- Displays a corresponding alert message.
- Saves timestamped records to `events.csv`, with CSV escaping and a header.
- View event history from the application.

## Repository structure

```text
pbl-main/
├── data/
│   ├── authorized_personnel.txt
│   └── authorized_places.txt
├── docs/
│   ├── architecture.md
│   ├── workflow.md
│   └── testing.md
├── src/
│   └── main.cpp
├── .gitignore
├── README.md
└── requirements.txt
```

## Requirements

- C++ compiler with C++17 support (GCC, Clang, or MSVC).
- Terminal / command prompt.
- No third-party Python packages are required for the current C++ prototype. `requirements.txt` is reserved for a future AI integration.

## Compile and run

Run commands from the repository root so the relative `data/` path resolves correctly.

### macOS / Linux

```bash
g++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp -o cctv
./cctv
```

### Windows (MinGW)

```bash
g++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp -o cctv.exe
cctv.exe
```

## How to use

1. Select **2** to view authorized personnel or **3** to add a person.
2. Select **8** to delete an authorized person: choose the numbered entry and confirm.
3. Select **4** to view authorized places or **5** to add an area.
4. Select **9** to delete an authorized place: choose the numbered entry and confirm.
5. Select **1** to record a demo event; enter the person's name and area/location, then answer the suspicious-object prompt.
6. Review the risk result and alert.
7. Select **6** to view event history. The records are also stored in `events.csv`.
8. Select **7** to exit.

## Event data

`events.csv` is created automatically after the first event. Columns:

`timestamp, person, location, person_authorized, place_authorized, suspicious_object, risk_level`

Do not commit real CCTV footage, personal data, or sensitive security logs to a public repository. Use synthetic/sample records for demonstrations.

## Architecture and workflow

See:
- [System architecture](docs/architecture.md)
- [Event workflow](docs/workflow.md)
- [Testing checklist](docs/testing.md)

## Current limitations and proposed next phase

The current program is a **manual-input C++ demonstration**. It does not currently:
- Capture video from a CCTV/IP camera.
- Detect people or objects automatically.
- Run YOLOv13, OpenCV, PyTorch, or PETALface.
- Perform biometric face recognition.
- Send SMS, email, or mobile push notifications.
- Provide a web-based dashboard.

These are proposed integration tasks, not implemented features. A future design can use a Python/OpenCV video pipeline and a supported object-detection/recognition model, then pass verified events to a database and this application's event-management layer. Any face-recognition deployment should use consent, access controls, retention limits, and applicable law/policy.

## Team

- Ritika Jajodia
- Saket Sharma
- Ashish Raj
- Paridhi Kejriwal
- Saraswati More

## License

Add a license file if the team intends to permit reuse or distribution.
