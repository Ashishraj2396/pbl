"""Educational YOLO image demo for a single image (no tracking or identity matching)."""
import argparse
import json
from pathlib import Path
from datetime import datetime


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--model', required=True, help='Path to compatible YOLO model weights')
    parser.add_argument('--image', required=True, help='Path to a single sample image')
    parser.add_argument('--output', default='detection_output.jpg', help='Output annotated image')
    parser.add_argument('--conf', type=float, default=0.25,
                        help='Minimum detection confidence (0 to 1)')
    parser.add_argument('--archive', action='store_true',
                        help='Save results with unique filenames')
    args = parser.parse_args()

    if not 0.0 <= args.conf <= 1.0:
        parser.error('--conf must be between 0 and 1')

    if not Path(args.image).is_file():
        raise SystemExit(f"Image not found: {args.image}")
    if not Path(args.model).is_file():
        raise SystemExit(f"Model file not found: {args.model}")

    try:
        from ultralytics import YOLO
    except ImportError as exc:
        raise SystemExit('Install requirements first: python3 -m pip install -r requirements.txt') from exc

    try:
        model = YOLO(args.model)
    except Exception as exc:
        raise SystemExit(f"Model loading failed: {exc}") from exc

    try:
        results = model.predict(
            source=args.image,
            conf=args.conf,
            verbose=False
        )
    except Exception as exc:
        raise SystemExit(f"Detection failed: {exc}") from exc

    if not results:
        raise SystemExit("No results returned by the model.")
    result = results[0]
    counts = {}

    print("\n========== DETECTION REPORT ==========")
    print(f"Confidence threshold: {args.conf:.0%}")

    if result.boxes is not None:
        for box in result.boxes:
            class_id = int(box.cls.item())
            confidence = float(box.conf.item())
            label = result.names[class_id]

            counts[label] = counts.get(label, 0) + 1

            print(f"{label}: {confidence:.2%}")

    print(f"Total objects detected: {sum(counts.values())}")

    print("\nObject counts:")
    for label, count in sorted(counts.items()):
        print(f"{label}: {count}")

    print("======================================")

    try:
        if args.archive:
            output_dir = Path("detection_results")
            output_dir.mkdir(parents=True, exist_ok=True)
            timestamp = datetime.now().strftime("%Y%m%d_%H%M%S_%f")
            args.output = str(
                output_dir / f"detection_{timestamp}.jpg"
            )

        result.save(filename=args.output)

        report = {
            "confidence_threshold": args.conf,
            "total_detections": sum(counts.values()),
            "object_counts": counts
        }

        report_path = Path(args.output).with_suffix(".json")
        report_path.write_text(
            json.dumps(report, indent=4),
            encoding="utf-8"
        )

    except Exception as exc:
        raise SystemExit(f"Output saving failed: {exc}") from exc

    print(f"JSON report saved to: {report_path}")
    print(f'Annotated sample image saved to: {args.output}')
    print('This demo processes one image only; it does not track people or identify individuals.')


if __name__ == '__main__':
    main()
