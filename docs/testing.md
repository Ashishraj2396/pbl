# System Testing

## Test Environment

- macOS
- C++17
- Python 3.9
- Ultralytics 8.4.218
- OpenCV 4.11.0.86
- YOLO11n model

## Compilation Test

g++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp -o cctv

## Application Test Checklist

- [x] Application startup
- [x] Display all 11 menu options
- [x] Manual event recording
- [x] View personnel records
- [x] Add personnel record
- [x] View place records
- [x] Add place record
- [x] View event history
- [x] Delete personnel record
- [x] Delete place record
- [x] Add image to library
- [x] YOLO object detection
- [x] Application exit

## Dependency Tests

- [x] Ultralytics installed
- [x] OpenCV installed
- [x] Python dependency compatibility check

## Error Handling Tests

- [ ] Invalid menu input
- [ ] Duplicate personnel record
- [ ] Duplicate place record
- [ ] Invalid image path
- [ ] Unsupported image format
- [ ] Empty image library

## Testing Guidelines

- Use synthetic personnel records.
- Use permitted sample images.
- Verify generated output files.
- Record actual results before marking tests as passed.
- AI detection results require human review.

## Current Limitations

Live CCTV and facial recognition are not implemented.
Functional testing does not establish AI accuracy.

## Phase 1 Test Result

All 11 menu options were tested on macOS, as confirmed by the project developer.
Detailed error handling and AI accuracy evaluation remain pending.
