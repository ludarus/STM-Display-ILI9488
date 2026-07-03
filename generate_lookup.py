import argparse


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "onColour",
        type=int,
        help="Colour of the on pixels in integer form. The 3 bit format is R B G.",
    )
    parser.add_argument(
        "offColour",
        type=int,
        help="Colour of the off pixels in integer form. The 3 bit format is R B G.",
    )
    args = parser.parse_args()

    ON = args.onColour
    OFF = args.offColour

    if args.onColour > 0b1110 or args.onColour < 0:
        ON = 0b1110
    if args.offColour > 0b1110 or args.offColour < 0:
        OFF = 0b1110

    print("const uint32_t table[256] = {")

    for i in range(256):
        result = 0
        for shift in (0, 2, 4, 6):
            lo = ON if (i >> (shift)) & 1 else OFF
            hi = ON if (i >> (shift + 1)) & 1 else OFF
            result = (result >> 8) | (((lo << 4) | hi) << 24)

        comma = "," if i < 255 else " "
        print(f"    0x{result:08X}{comma}")

    print("};")


if __name__ == "__main__":
    main()
