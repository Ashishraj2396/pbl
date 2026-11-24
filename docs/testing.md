# Testing Checklist

Run the application from the repository root.

| Test | Input / action | Expected result |
|---|---|---|
| Authorized person | Enter a name present in the personnel file; object = no | NORMAL |
| Name case handling | Enter an authorized name using different letter case | Still recognized as authorized |
| Unknown person | Enter a name not in the personnel file; object = no | MEDIUM RISK |
| Suspicious object | Any name; object = yes | HIGH RISK |
| Add personnel | Use menu option 3, then enter a new name | Name appended to personnel file |
| Duplicate personnel | Add an existing name with different letter case | Duplicate is rejected |
| Event history | Record an event, then choose option 4 | CSV header and event row are displayed |
| Invalid menu choice | Enter a value outside 1–5 | Validation message is displayed |

These are manual functional checks. They do not measure AI accuracy, precision, recall, or camera FPS because the current program has no AI/video inference pipeline.
