# System Workflow

## 1. Application Startup

- Activate the Python virtual environment.
- Compile the C++17 application.
- Run the application from the project root.
- Select an option from the 11-option menu.

## 2. Manual Event Management

- Select option 1.
- Enter the required event information.
- The application checks the stored authorization records.
- The application displays a rule-based demo event report.
- The event is saved in events.csv.
- Select option 6 to view event history.

## 3. Personnel Management

- Option 2: View personnel records.
- Option 3: Add a personnel record.
- Option 7: Delete a personnel record.

## 4. Place Management

- Option 4: View place records.
- Option 5: Add a place record.
- Option 8: Delete a place record.

## 5. Image Library

- Select option 9.
- Enter the path of a permitted sample image.
- The image is copied into data/images/.

## 6. AI Object Detection

- Select option 10.
- Choose an image from the saved-image library.
- The C++ application invokes the Python script.
- YOLO11n detects objects in the selected image.
- The annotated image is saved as detection_output.jpg.

## 7. Application Exit

- Select option 11 to close the application.

## Important Limitations

- Manual events and AI detection are separate workflows.
- Object detection does not establish identity or intent.
- Live CCTV integration is not implemented.
- Use synthetic or consented demonstration data.
- AI results require human review.
