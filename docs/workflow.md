# Event Workflow

1. The operator selects **Record a CCTV event**.
2. The operator enters a person's name and the monitored area/location.
3. The operator reports whether a suspicious/restricted object was observed.
4. The program checks the name against `data/authorized_personnel.txt`.
5. The program applies the prototype's rule-based risk classification:
   - Suspicious/restricted object reported → HIGH RISK.
   - Otherwise, person not authorized → MEDIUM RISK.
   - Otherwise → NORMAL.
6. The program displays the event report and alert message.
7. The program appends the event to `events.csv`.

**Important:** This workflow uses manual input. It is not automatic video analytics.
