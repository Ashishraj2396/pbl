# Intelligent CCTV Surveillance System

A college Problem-Based Learning (PBL) prototype built with **C++17**
and **Python 3**. It demonstrates personnel and place authorization,
manual CCTV event logging, image-library management, and YOLO-based
object detection on saved images.

> **Project scope:** This is an academic prototype. It does not connect
> to a live CCTV/IP camera, continuously monitor video, or identify
> individual people.

## Features

The C++ menu provides these options:

1.  Record a CCTV event (manual demo)
2.  View authorized personnel
3.  Add authorized personnel
4.  View authorized places
5.  Add authorized place
6.  View event history
7.  Delete authorized personnel
8.  Delete authorized place
9.  Add image to image library
10. Detect objects from saved images
11. Exit

### Manual event management

-   Checks a manually entered person's name against
    `data/authorized_personnel.txt`.
-   Checks a manually entered location against
    `data/authorized_places.txt`.
-   Applies the prototype's demo risk rules and displays an event
    report.
-   Saves event history to `events.csv`.

### Image library

-   Option 9 copies a selected image into `data/images/`.
-   Supported filename extensions: `.jpg`, `.jpeg`, `.png`, `.bmp`, and
    `.webp`.
-   The source image must already exist at the path entered by the user.

### YOLO image detection

-   Option 10 lists images saved in `data/images/`.
-   The selected image is passed to `scripts/yolo_image_demo.py`.
-   The Python script uses the local `yolo11n.pt` model and saves an
    annotated image as `detection_output.jpg`.
-   The current demo processes one still image at a time. It is not live
    video detection or person identification.

## Technologies

-   C++17
-   Python 3
-   Ultralytics YOLO
-   OpenCV
-   Git and GitHub

## Project structure

``` text
pbl/
├── data/
│   ├── authorized_personnel.txt
│   ├── authorized_places.txt
│   └── images/                 # Local image library; do not commit private footage
├── docs/
│   ├── architecture.md
│   ├── workflow.md
│   └── testing.md
├── scripts/
│   ├── yolo_image_demo.py
│   └── petalface_demo.py
├── src/
│   └── main.cpp
├── requirements.txt
├── .gitignore
└── README.md
```

The `.venv/` Python environment, compiled `cctv` executable, YOLO model
weights, runtime event log, saved image library, and generated detection
output are local/runtime files and should not be committed.

## Requirements

-   macOS or Linux
-   A C++ compiler with C++17 support (Apple Clang or GCC)
-   Python 3
-   Python packages used by the YOLO script: Ultralytics and OpenCV
-   YOLO model weights named `yolo11n.pt` in the project root

## Setup on macOS

Run these commands from the repository root.

### 1. Create and activate a virtual environment

``` bash
python3 -m venv .venv
source .venv/bin/activate
```

### 2. Install Python dependencies

Install the pinned dependencies:

```bash
python -m pip install -r requirements.txt
python -m pip check
```

The dependency versions are recorded in requirements.txt.

### 3. Add the YOLO model

Place the compatible `yolo11n.pt` model file in the project root. Model
weights are not included in this repository; obtain them from the
official Ultralytics distribution and comply with its licensing terms.

### 4. Compile the C++ application

``` bash
g++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp -o cctv
```

### 5. Run the application

``` bash
./cctv
```

Run the program from the repository root so that its relative paths
(such as `data/authorized_personnel.txt`, `data/images/`, and
`scripts/yolo_image_demo.py`) resolve correctly.

## How to use

### Add an image (Option 9)

1.  Start the application with `./cctv`.
2.  Enter `9` at the menu.
3.  Enter the image path. For example, if the image is in the project
    root, enter `test.png`.
4.  The application copies it into `data/images/`.

### Detect objects (Option 10)

1.  Enter `10` at the menu.
2.  Select an image number from the saved-image list.
3.  The application runs the YOLO Python script using
    `.venv/bin/python`.
4.  If detection completes successfully, the annotated result is saved
    as `detection_output.jpg` and the application attempts to open it on
    macOS.

You can also test the Python script directly:

``` bash
.venv/bin/python scripts/yolo_image_demo.py \
  --model yolo11n.pt \
  --image data/images/your_image.png \
  --output detection_output.jpg
```

Replace `your_image.png` with the actual saved image filename.

## Testing checklist

-   [ ] Compile the C++ application without errors.
-   [ ] Confirm the menu displays options 1 through 11.
-   [ ] Add a permitted sample image using Option 9.
-   [ ] Confirm the image appears in `data/images/`.
-   [ ] Run Option 10 and select the saved image.
-   [ ] Confirm `detection_output.jpg` is created and inspect the
    annotated result.
-   [ ] Test manual event logging and verify `events.csv`.

Only mark a test as complete after running it on the target computer.

## Limitations and future work

-   No live CCTV/IP camera capture or continuous video processing.
-   YOLO detection is currently limited to individual images.
-   Object detection does not establish a person's identity.
-   The PETALFace demo/placeholder has not been verified as a working
    face-recognition implementation.
-   Manual event recording and YOLO image detection are separate
    demonstrations; YOLO results are not automatically converted into
    CCTV event records.
-   Future work may include a live video pipeline, carefully evaluated
    recognition features, database-backed event management, and alert
    integrations.

## Privacy and responsible use

Use only images you have permission to process. Do not commit real CCTV
footage, identifiable personal data, credentials, or sensitive security
logs to a public repository. Keep sample images synthetic or otherwise
approved for public demonstration. Any future biometric or
face-recognition feature requires appropriate consent, safeguards, and
compliance with applicable law and institutional policy.

## Team

-   Ritika Jajodia
-   Saket Sharma
-   Ashish Raj
-   Paridhi Kejriwal
-   Saraswati More

## License

No license is specified yet. Add a license file only after the team
agrees on the project's reuse and distribution terms.

## Demo Data Setup

For a fresh clone, initialize local records using synthetic examples:

    cp -n data/examples/personnel.txt data/authorized_personnel.txt
    cp -n data/examples/places.txt data/authorized_places.txt

Private local records are excluded from Git tracking.

## Advanced YOLO Image Detection

The upgraded Python module supports:

- Configurable confidence threshold.
- Individual detection confidence scores.
- Object category counting.
- Structured JSON summary reports.
- Timestamp-based archived results.
- Improved error handling.

### Optional command-line arguments

- --conf: Minimum confidence between 0 and 1.
  Default: 0.25.
- --archive: Save unique image and JSON outputs
  inside detection_results/.

Generated results remain local and are excluded
from Git tracking.

Detection scores and counts are model predictions,
not independently verified ground truth.

The module processes individual approved sample
images without identity recognition or tracking.

## Human-Reviewed AI Detection Management

The system provides a 13-option C++ menu.

### New menu options

- Option 11: Review AI detection summary.
- Option 12: View AI review history.
- Option 13: Exit.

### Review features

- JSON report validation.
- Aggregate object detection summaries.
- Human review decisions.
- Accepted or needs-recheck status.
- Cancellation without saving.
- Timestamp-based local review history.
- Invalid report handling.

### New Python modules

- scripts/review_detection.py
- scripts/view_review_history.py

Review records are stored locally in
review_history.jsonl and excluded from Git.

Manual CCTV events and AI review records
remain separate. AI predictions do not establish
identity, authorization, intent or suspicious behavior.

## Recorded Video Analysis

The system now provides a 15-option C++ menu.

### New menu options

- Option 13: Analyze sample video.
- Option 14: View video analysis reports.
- Option 15: Exit.

### Video analysis features

- OpenCV-based video reading.
- MP4, MOV and AVI file selection.
- Configurable frame sampling interval.
- YOLO11n object detection on sampled frames.
- Configurable confidence threshold through Python CLI.
- Aggregate object category counts.
- Video resolution and FPS reporting.
- Processing performance statistics.
- Timestamp-based JSON reports.
- Input validation and error handling.

### Default C++ settings

- Frame sampling interval: 10.
- Confidence threshold: 0.50.

### New Python module

scripts/yolo_video_demo.py

### Local directories

- data/videos/: Approved sample videos.
- video_results/: Generated JSON reports.

Both directories are excluded from Git tracking.

### Limitations

Aggregate detection counts may include repeated
objects across multiple frames.

The module does not perform identity recognition,
individual tracking or live CCTV monitoring.

Use only synthetic or approved demonstration videos.

## GUI Dashboard and SQLite Database

The system now provides a 16-option C++ menu.

### New menu options

- Option 15: Launch GUI Dashboard.
- Option 16: Exit.

### Dashboard features

- Professional dark-blue interface.
- System overview and report statistics.
- Interactive image detection charts.
- Recorded video analysis reports.
- Human review history.
- Report selection and data tables.
- Manual report refresh button.
- Local SQLite database integration.
- Duplicate prevention for repeated report imports.

### Dashboard technologies

- Streamlit
- Pandas
- Plotly
- SQLite

### New project files

- dashboard.py
- scripts/database.py
- scripts/import_reports.py
- .streamlit/config.toml

### Launch the dashboard

Run the C++ application and select option 15.

Alternatively, run these commands:

    python scripts/import_reports.py
    python -m streamlit run dashboard.py

Open http://127.0.0.1:8501 in a browser.

### Privacy and limitations

The dashboard runs locally.
The SQLite database is excluded from Git tracking.
Usage telemetry is disabled in the project configuration.

Detection statistics represent aggregate model predictions,
not verified identities or unique individuals.
