# System Architecture

## Current prototype

The current implementation is a console-based C++17 application. Event information is entered manually; there is no live camera or AI model connection.

```mermaid
flowchart TD
    A[Operator] --> B[Console Menu]
    B --> C[Enter Person, Location and Object Flag]
    C --> D[Authorized Personnel Lookup]
    D --> E[Risk Classification]
    E --> F[Display Alert and Event Report]
    F --> G[Append Record to events.csv]
    H[data/authorized_personnel.txt] --> D
    B --> I[Personnel Management]
    I --> H
    B --> J[View Event History]
    J --> G
```

## Proposed future AI-enabled architecture

This is a design proposal only; these components are not implemented in this repository yet.

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

Suggested integration boundary: the AI pipeline should produce a structured event (timestamp, camera/location, detection label, confidence, and authorized-match status). Validate model output and apply privacy/security controls before saving or displaying it.
