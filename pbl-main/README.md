# Intelligent CCTV Surveillance Using AI-Based Detection and Recognition

A C++17 PBL prototype for demonstrating access checks, event classification, security alerts, and event-history logging. The current repository implements the **application-logic prototype**; it is not yet connected to a live CCTV camera or an AI inference model.

## Current features

- Menu-driven console interface.
- Checks names against `data/authorized_personnel.txt` (case-insensitive).
- Add and list authorized personnel.
- Add and list authorized places/areas using `data/authorized_places.txt`.
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

1. Select **2** to view the authorized-personnel list.
2. Select **3** to add an authorized person.
3. Select **1** to record a demo event.
4. Enter the person's name and area/location.
5. Answer whether a suspicious/restricted object was reported.
6. Review the risk result and alert.
7. Select **4** to view event history. The records are also stored in `events.csv`.
8. Select **5** to exit.

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
