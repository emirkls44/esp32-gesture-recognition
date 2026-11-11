import os
import sys
import time

import serial

WINDOW_SECONDS = 10


def main():
    if len(sys.argv) != 3:
        print("Usage: python record.py <port> <label>")
        sys.exit(1)

    port, label = sys.argv[1], sys.argv[2]
    os.makedirs("data", exist_ok=True)

    ser = serial.Serial(port, 115200, timeout=1)
    time.sleep(2)
    ser.reset_input_buffer()

    n = 1
    while os.path.exists(f"data/{label}.{n:02d}.csv"):
        n += 1

    print(f"Recording '{label}'. Press Ctrl+C to stop.")

    try:
        while True:
            fname = f"data/{label}.{n:02d}.csv"
            t0 = time.time()
            count = 0
            with open(fname, "w") as f:
                f.write("timestamp,distance\n")
                while time.time() - t0 < WINDOW_SECONDS:
                    line = ser.readline().decode(errors="ignore").strip()
                    try:
                        value = float(line)
                    except ValueError:
                        continue
                    f.write(f"{int((time.time() - t0) * 1000)},{value}\n")
                    count += 1
            print(f"{fname} saved ({count} samples)")
            n += 1
    except KeyboardInterrupt:
        print("Stopped")
    finally:
        ser.close()


if __name__ == "__main__":
    main()
