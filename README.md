# Intelligent CCTV Surveillance Using AI-Based Detection and Recognition

## Project overview
This is a basic C++17 prototype based on our PBL presentation. It demonstrates restricted-area event handling, authorized-person checking, risk classification, alerts, and event logging.

## Features
- Checks a person's name against an authorized-personnel list.
- Records the location of the event.
- Classifies an event as NORMAL, MEDIUM RISK, or HIGH RISK.
- Displays an alert for medium/high-risk events.
- Saves event details in `events.csv`.

## Risk rules used in this basic prototype
- HIGH RISK: a suspicious/restricted object is reported.
- MEDIUM RISK: the person is not in the authorized list.
- NORMAL: the person is authorized and no suspicious/restricted object is reported.

## Folder structure
```text
pbl/
├── src/
│   └── main.cpp
├── data/
│   └── authorized_personnel.txt
├── README.md
└── .gitignore
```

## Run the project
From the repository folder:

```bash
g++ -std=c++17 src/main.cpp -o pbl
./pbl
```

On Windows:
```bash
g++ -std=c++17 src/main.cpp -o pbl.exe
pbl.exe
```

## Important limitation
This is only the basic application-logic prototype. It does not connect to a CCTV camera and does not actually run YOLOv13 or PETALface. The person name and object information are entered manually. The PPT proposes Python, OpenCV, YOLOv13, PETALface, and PyTorch for the real AI detection/recognition pipeline; those components must be integrated separately.

## Team
- Ritika Jajodia
- Saket Sharma
- Ashish Raj
- Paridhi Kejriwal
- Saraswati More
