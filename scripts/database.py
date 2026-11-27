"""Local SQLite database for aggregate analysis reports."""

import sqlite3
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
DB_PATH = ROOT / "data" / "dashboard.db"


def initialize_database():
    DB_PATH.parent.mkdir(parents=True, exist_ok=True)

    connection = sqlite3.connect(DB_PATH)

    try:
        connection.executescript("""
            CREATE TABLE IF NOT EXISTS image_reports (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                source_key TEXT UNIQUE NOT NULL,
                confidence_threshold REAL NOT NULL,
                total_detections INTEGER NOT NULL,
                object_counts TEXT NOT NULL
            );

            CREATE TABLE IF NOT EXISTS video_reports (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                source_key TEXT UNIQUE NOT NULL,
                frames_read INTEGER NOT NULL,
                sampled_frames INTEGER NOT NULL,
                sampling_interval INTEGER NOT NULL,
                confidence_threshold REAL NOT NULL,
                total_detections INTEGER NOT NULL,
                object_counts TEXT NOT NULL,
                processing_seconds REAL NOT NULL
            );

            CREATE TABLE IF NOT EXISTS review_records (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                source_key TEXT UNIQUE NOT NULL,
                timestamp TEXT NOT NULL,
                confidence_threshold REAL NOT NULL,
                total_detections INTEGER NOT NULL,
                object_counts TEXT NOT NULL,
                review_decision TEXT NOT NULL
            );
        """)

        connection.commit()
        print("Database initialized successfully.")
        print(f"Database location: {DB_PATH}")

        tables = connection.execute(
            "SELECT name FROM sqlite_master "
            "WHERE type='table' "
            "AND name NOT LIKE 'sqlite_%' "
            "ORDER BY name"
        ).fetchall()

        print("\nAvailable tables:")

        for table in tables:
            print("-", table[0])

    finally:
        connection.close()


if __name__ == "__main__":
    initialize_database()
