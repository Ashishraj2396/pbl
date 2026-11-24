# Event Workflow

1. The operator selects **Record a CCTV event**.
2. The operator enters a person's name and the monitored area/location.
3. The operator reports whether a suspicious/restricted object was observed.
4. The program checks the person's name against `data/authorized_personnel.txt`.
5. The program checks the exact location against `data/authorized_places.txt`.
6. Risk rules: suspicious object OR unapproved location = HIGH RISK; unknown person at an approved location = MEDIUM RISK; approved person at an approved location with no suspicious object = NORMAL.
7. The program displays the event report and alert, then appends both authorization results to `events.csv`.

**Important:** This workflow uses manual input. It is not automatic video analytics. Location matching ignores letter case and surrounding spaces.
