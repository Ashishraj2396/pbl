"""Educational YOLO image demo for a single image (no tracking or identity matching)."""
import argparse
from pathlib import Path


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--model', required=True, help='Path to compatible YOLO model weights')
    parser.add_argument('--image', required=True, help='Path to a single sample image')
    parser.add_argument('--output', default='detection_output.jpg', help='Output annotated image')
    args = parser.parse_args()

    if not Path(args.image).is_file():
        raise SystemExit(f"Image not found: {args.image}")
    if not Path(args.model).is_file():
        raise SystemExit(f"Model file not found: {args.model}")

    try:
        from ultralytics import YOLO
    except ImportError as exc:
        raise SystemExit('Install requirements first: python3 -m pip install -r requirements.txt') from exc

    model = YOLO(args.model)
    results = model.predict(source=args.image, verbose=False)
    if not results:
        print('No results returned.')
        return
    results[0].save(filename=args.output)
    print(f'Annotated sample image saved to: {args.output}')
    print('This demo processes one image only; it does not track people or identify individuals.')


if __name__ == '__main__':
    main()
