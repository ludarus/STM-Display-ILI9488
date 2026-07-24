# will parse a table and output correct coordinates
import argparse
import os
import sys
import math

OLD_WIDTH = 240
OLD_HEIGHT = 128

NEW_WIDTH = 480
NEW_HEIGHT = 320


def parse_file(filePath: str) -> list[int]:
    with open(filePath, "rt") as f:
        file = f.read()

    lines = file.splitlines()

    indicies: list[int] = []

    for line in lines:
        # isolating each hex number

        # first two are different than the rest
        hindex = line.find("H")
        firstByte = line[:hindex]
        line = line[hindex + 1 :]

        hindex = line.find("H")
        secondByte = line[2:hindex]
        line = line[hindex + 1 :]

        indicies.append(int(firstByte, 16) + (int(secondByte, 16) << 8) - 256)

        while line.find("H") != -1:
            hindex = line.find("H")
            firstByte = line[3:hindex]
            line = line[hindex + 1 :]

            hindex = line.find("H")
            secondByte = line[2:hindex]
            line = line[hindex + 1 :]
            indicies.append(int(firstByte, 16) + (int(secondByte, 16) << 8) - 256)

    return indicies


def convert_to_coords(indicies: list[int]) -> list[tuple]:
    coords: list[tuple] = []
    for num in indicies:
        # getting coordinates from global index
        x = num % OLD_WIDTH
        y = num // OLD_WIDTH

        # scaling and rounding to nearest 8
        x = int(((x * (NEW_WIDTH / OLD_WIDTH)) // 8) * 8)
        y = int(y * (NEW_HEIGHT / OLD_HEIGHT))

        coords.append((x, y))

    return coords


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("inputFile", help="a .trc file to emulate")

    args = parser.parse_args()

    out = parse_file(args.inputFile)

    print(out)
    print(len(out))

    print(convert_to_coords(out))

    # checking if input is a file
    if not os.path.isfile(args.inputFile):
        print(f"ERROR: File not found: {args.inputFile}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
