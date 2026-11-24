# System Architecture

## Current prototype

The current implementation is a console-based C++17 application. Event information is entered manually; there is no live camera or AI model connection.

```mermaid
flowchart TD
    A[Operator] --> B[Console Menu]
    B --> C[Enter Person, Location and Object Flag]
    C --> D[Authorized Personnel Lookup]
    C --> E[Authorized Place Lookup]
    D --> F[Risk Classification]
    E --> F
    F --> G[Display Alert and Event Report]
    G --> H[Append Record to events.csv]
    I[data/authorized_personnel.txt] --> D
    J[data/authorized_places.txt] --> E
    B --> K[Manage Personnel and Places]
    K --> I
    K --> J
    B --> L[View Event History]
    L --> H
```

## Proposed future AI-enabled architecture

This is a design proposal only; these components are not implemented yet. A future camera integration should map each camera to a configured location ID rather than infer a physical location from video.

```mermaid
flowchart LR
    A[CCTV / IP Camera] --> B[Video Capture]
    B --> C[Python + OpenCV]
    C --> D[Object Detection Model]
    D --> E[Optional Face Recognition]
    E --> F[Event Validation]
    F --> G[Application / Alert Layer]
    G --> H[(Database)]
    G --> I[Authorized Dashboard]
    H --> J[Reports]
```
