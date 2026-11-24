# Testing Checklist

Run the application from the repository root.

| Test | Action | Expected result |
|---|---|---|
| Authorized person and place | Both are listed; no suspicious object | NORMAL |
| Unknown person, approved place | Unknown name; approved location | MEDIUM RISK |
| Approved person, unapproved place | Authorized name; location absent from place list | HIGH RISK |
| Suspicious object | Mark yes | HIGH RISK |
| Add authorized place | Menu option 5; enter a new place | Place appended to authorized places file |
| Duplicate place | Add existing place with different case | Duplicate rejected |
| Event history | Record event, choose option 6 | CSV event shown |
| Invalid menu choice | Enter outside 1–7 | Validation message |

These are manual functional checks, not AI accuracy or camera-FPS tests.
