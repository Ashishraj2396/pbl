"""Display local aggregate detection review history."""

import json
from pathlib import Path


def main():
    path = Path("review_history.jsonl")

    if not path.is_file():
        print("No review history available.")
        return

    print("\n========== REVIEW HISTORY ==========")

    count = 0

    try:
        with path.open(encoding="utf-8") as file:
            for line in file:
                if not line.strip():
                    continue

                try:
                    record = json.loads(line)

                    timestamp = record["timestamp"]
                    decision = record["review_decision"]
                    threshold = record["confidence_threshold"]
                    total = record["total_detections"]

                    count += 1

                    print(f"\nReview #{count}")
                    print(f"Timestamp: {timestamp}")
                    print(f"Decision: {decision}")
                    print(f"Confidence threshold: {threshold:.0%}")
                    print(f"Total detections: {total}")
                    print("------------------------------------")

                except (ValueError, KeyError, TypeError):
                    print("Skipping invalid history record.")

    except OSError as exc:
        raise SystemExit(f"Could not read history: {exc}")

    print(f"\nTotal reviews: {count}")
    print("====================================")


if __name__ == "__main__":
    main()
