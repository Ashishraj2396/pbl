"""Import local aggregate analysis reports into SQLite."""

import json
import math
import sqlite3
from pathlib import Path
from database import ROOT, DB_PATH, initialize_database


def validate_summary(report):
    confidence = report["confidence_threshold"]
    total = report["total_detections"]
    counts = report["object_counts"]

    if type(confidence) not in (int, float):
        raise ValueError("Invalid confidence")

    if not math.isfinite(confidence) or not 0 <= confidence <= 1:
        raise ValueError("Invalid confidence")

    if type(total) is not int or total < 0:
        raise ValueError("Invalid total")

    if not isinstance(counts, dict):
        raise ValueError("Invalid object counts")

    for label, count in counts.items():
        if not label.strip() or not label.isprintable():
            raise ValueError("Invalid label")

        if type(count) is not int or count < 0:
            raise ValueError("Invalid object count")

    if sum(counts.values()) != total:
        raise ValueError("Detection counts do not match")

    return confidence, total, json.dumps(counts, sort_keys=True)


def import_images(conn):
    paths = [ROOT / "detection_output.json"]
    paths += sorted(
        (ROOT / "detection_results").glob("*.json")
    )

    for path in paths:
        if not path.is_file():
            continue

        try:
            report = json.loads(path.read_text())
            values = validate_summary(report)

            conn.execute("""
                INSERT INTO image_reports (
                    source_key, confidence_threshold,
                    total_detections, object_counts
                ) VALUES (?, ?, ?, ?)
                ON CONFLICT(source_key) DO UPDATE SET
                    confidence_threshold = excluded.confidence_threshold,
                    total_detections = excluded.total_detections,
                    object_counts = excluded.object_counts
            """, (str(path.relative_to(ROOT)), *values))

        except (OSError, ValueError, KeyError, TypeError):
            print("Skipping invalid image report:", path.name)


def import_videos(conn):
    for path in sorted(
        (ROOT / "video_results").glob("*.json")
    ):
        try:
            report = json.loads(path.read_text())
            confidence, total, counts = validate_summary(report)

            frames = report["frames_read"]
            sampled = report["sampled_frames"]
            stride = report["sampling_interval"]
            seconds = report["processing_seconds"]

            if any(type(v) is not int or v < 1
                   for v in (frames, sampled, stride)):
                raise ValueError("Invalid frame statistics")

            if sampled != (frames - 1) // stride + 1:
                raise ValueError("Invalid sampled frame count")

            if type(seconds) not in (int, float):
                raise ValueError("Invalid processing time")

            if not math.isfinite(seconds) or seconds < 0:
                raise ValueError("Invalid processing time")

            conn.execute("""
                INSERT OR IGNORE INTO video_reports (
                    source_key, frames_read, sampled_frames,
                    sampling_interval, confidence_threshold,
                    total_detections, object_counts,
                    processing_seconds
                ) VALUES (?, ?, ?, ?, ?, ?, ?, ?)
            """, (
                str(path.relative_to(ROOT)),
                frames, sampled, stride,
                confidence, total, counts, seconds
            ))

        except (OSError, ValueError, KeyError, TypeError):
            print("Skipping invalid video report:", path.name)


def import_reviews(conn):
    path = ROOT / "review_history.jsonl"

    if not path.is_file():
        return

    with path.open(encoding="utf-8") as file:
        for number, line in enumerate(file, 1):
            if not line.strip():
                continue

            try:
                report = json.loads(line)
                confidence, total, counts = validate_summary(report)

                decision = report["review_decision"]
                timestamp = report["timestamp"]

                if decision not in ("ACCEPTED", "NEEDS_RECHECK"):
                    raise ValueError("Invalid decision")

                if not isinstance(timestamp, str) or not timestamp:
                    raise ValueError("Invalid timestamp")

                conn.execute("""
                    INSERT OR IGNORE INTO review_records (
                        source_key, timestamp,
                        confidence_threshold, total_detections,
                        object_counts, review_decision
                    ) VALUES (?, ?, ?, ?, ?, ?)
                """, (
                    f"review_history.jsonl:{number}",
                    timestamp, confidence, total,
                    counts, decision
                ))

            except (ValueError, KeyError, TypeError):
                print("Skipping invalid review:", number)


def main():
    initialize_database()

    with sqlite3.connect(DB_PATH) as conn:
        import_images(conn)
        import_videos(conn)
        import_reviews(conn)

        print("\n======= DATABASE SUMMARY =======")

        for table in (
            "image_reports",
            "video_reports",
            "review_records"
        ):
            count = conn.execute(
                f"SELECT COUNT(*) FROM {table}"
            ).fetchone()[0]

            print(f"{table}: {count} records")

        print("================================")

    print("Report import completed successfully.")


if __name__ == "__main__":
    main()
