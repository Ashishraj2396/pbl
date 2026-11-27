"""Validate and review aggregate object detection reports."""

import argparse
import json
import math
from pathlib import Path
from datetime import datetime


def load_report(path):
    report = json.loads(path.read_text(encoding="utf-8"))

    if not isinstance(report, dict):
        raise ValueError("Invalid report structure")

    confidence = report.get("confidence_threshold")
    total = report.get("total_detections")
    counts = report.get("object_counts")

    if type(confidence) not in (int, float):
        raise ValueError("Invalid confidence threshold")

    if not math.isfinite(confidence) or not 0 <= confidence <= 1:
        raise ValueError("Confidence must be between 0 and 1")

    if type(total) is not int or total < 0:
        raise ValueError("Invalid total detections")

    if not isinstance(counts, dict):
        raise ValueError("Invalid object counts")

    for label, count in counts.items():
        if not label.strip() or not label.isprintable():
            raise ValueError("Invalid object label")

        if type(count) is not int or count < 0:
            raise ValueError("Invalid object count")

    if sum(counts.values()) != total:
        raise ValueError("Object counts do not match total")

    return report


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--report",
        default="detection_output.json"
    )
    args = parser.parse_args()

    try:
        report = load_report(Path(args.report))
    except (OSError, ValueError) as exc:
        raise SystemExit(f"Report validation failed: {exc}")

    print("\n======= AI DETECTION REVIEW =======")
    print("Report validation: PASSED")
    print(f"Confidence threshold: {report['confidence_threshold']:.0%}")
    print(f"Total detections: {report['total_detections']}")

    print("\nObject summary:")

    for label, count in sorted(report["object_counts"].items()):
        print(f"{label}: {count}")

    print("\nReview status: PENDING HUMAN REVIEW")
    print("Model predictions are not verified facts.")
    print("===================================")

    print("\nReview Options:")
    print("1. Accept detection summary")
    print("2. Mark for rechecking")
    print("3. Cancel review")

    choice = input("Choose review option: ").strip()

    decisions = {
        "1": "ACCEPTED",
        "2": "NEEDS_RECHECK",
        "3": "CANCELLED"
    }

    if choice not in decisions:
        raise SystemExit("Invalid review option.")

    decision = decisions[choice]
    print(f"Review decision: {decision}")

    if decision == "CANCELLED":
        print("Review cancelled. Nothing saved.")
        return

    record = {
        "timestamp": datetime.now().astimezone().isoformat(),
        "confidence_threshold": report["confidence_threshold"],
        "total_detections": report["total_detections"],
        "object_counts": report["object_counts"],
        "review_decision": decision
    }

    history_path = Path("review_history.jsonl")

    try:
        with history_path.open("a", encoding="utf-8") as file:
            file.write(json.dumps(record) + "\n")
    except OSError as exc:
        raise SystemExit(f"Could not save review: {exc}")

    print(f"Review saved to: {history_path}")


if __name__ == "__main__":
    main()
