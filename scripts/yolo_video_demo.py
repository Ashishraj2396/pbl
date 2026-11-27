"""Educational sample video analysis demo."""

import argparse
import json
from datetime import datetime
from collections import Counter
import time
from pathlib import Path
import cv2


def main():
    parser = argparse.ArgumentParser(description=__doc__)

    parser.add_argument(
        "--video", required=True,
        help="Path to an approved sample video"
    )

    parser.add_argument(
        "--stride", type=int, default=10,
        help="Process every Nth frame"
    )

    parser.add_argument(
        "--model",
        help="Path to local YOLO model weights"
    )

    parser.add_argument(
        "--conf", type=float, default=0.25,
        help="Minimum detection confidence"
    )

    parser.add_argument(
        "--report", action="store_true",
        help="Save aggregate video analysis report"
    )

    args = parser.parse_args()

    if args.report and not args.model:
        parser.error("--report requires --model")

    if not 0 <= args.conf <= 1:
        parser.error("--conf must be between 0 and 1")

    model = None
    counts = Counter()

    if args.model:
        if not Path(args.model).is_file():
            raise SystemExit("Model file not found.")

        try:
            from ultralytics import YOLO
            model = YOLO(args.model)
        except Exception as exc:
            raise SystemExit(f"Model loading failed: {exc}")

    if args.stride < 1:
        parser.error("--stride must be at least 1")

    if not Path(args.video).is_file():
        raise SystemExit("Video file not found.")

    cap = cv2.VideoCapture(args.video)

    if not cap.isOpened():
        cap.release()
        raise SystemExit("Could not open video.")

    start_time = time.perf_counter()
    total_frames = 0
    sampled_frames = 0

    try:
        fps = cap.get(cv2.CAP_PROP_FPS)
        width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
        height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))

        while True:
            success, frame = cap.read()

            if not success:
                break

            if total_frames % args.stride == 0:
                sampled_frames += 1

                if model is not None:
                    try:
                        results = model.predict(
                            source=frame,
                            conf=args.conf,
                            verbose=False
                        )

                        if not results:
                            raise ValueError("No model result")

                        result = results[0]

                        if result.boxes is not None:
                            for box in result.boxes:
                                label = result.names[int(box.cls.item())]
                                counts[label] += 1

                    except Exception as exc:
                        raise SystemExit(f"Analysis failed: {exc}")

            total_frames += 1

    finally:
        cap.release()

    if total_frames == 0:
        raise SystemExit("No readable video frames.")

    print("\n========== VIDEO ANALYSIS ==========")
    print(f"Video FPS: {fps:.2f}")
    print(f"Frames read: {total_frames}")
    print(f"Sampling interval: {args.stride}")
    print(f"Sampled frames: {sampled_frames}")
    elapsed = time.perf_counter() - start_time

    print(f"Resolution: {width} x {height}")

    if fps > 0:
        print(f"Decoded video duration: {total_frames / fps:.2f} seconds")

    print(f"Processing time: {elapsed:.3f} seconds")

    if elapsed > 0:
        print(f"Overall frame processing speed: {total_frames / elapsed:.2f} FPS")

    if model is not None:
        print("\nAggregate object analysis:")
        print(f"Confidence threshold: {args.conf:.0%}")
        print(f"Total detections: {sum(counts.values())}")

        for label, count in sorted(counts.items()):
            print(f"{label}: {count}")

        print("Counts may include repeated objects across frames.")

    if args.report:
        report = {
            "frames_read": total_frames,
            "sampled_frames": sampled_frames,
            "sampling_interval": args.stride,
            "confidence_threshold": args.conf,
            "total_detections": sum(counts.values()),
            "object_counts": dict(counts),
            "processing_seconds": round(elapsed, 3),
            "unique_object_count": False
        }

        try:
            output_dir = Path("video_results")
            output_dir.mkdir(parents=True, exist_ok=True)

            timestamp = datetime.now().strftime("%Y%m%d_%H%M%S_%f")

            report_path = output_dir / f"video_{timestamp}.json"

            report_path.write_text(
                json.dumps(report, indent=4),
                encoding="utf-8"
            )

            print(f"JSON report saved to: {report_path}")

        except OSError as exc:
            raise SystemExit(f"Report saving failed: {exc}")

    print("====================================")


if __name__ == "__main__":
    main()
