# Functionality
- Image and text displaying
    - Blit with OR mode or overwrite mode to either replace the current display section or draw on top of it
- Variable image location
    - Off screen drawing is supported
- 1 bit per pixel resolution
- Partial display updates
    - Write only the updated region of the screen to the display controller
- Serial connection 
    - Receive, process and send diagnostics via USART serial connection
- CAN connection
    - Receive and process CAN 2.0A commands

# Documentation
- Pixels are bitpacked to save memory
- Pixels are stored LSB first
- A lookup table is used for fast byte unpacking
    - Expansion to 3 bit per pixel colour resolution, plus an additional pixel of padding
- RLE is used to compress the images
- Bitpacking is used to compress the font characters
- Chunking and a dual buffer is used to transmit the image data via the SPI connection

# BMP conversion
- Use the [BMP_parser](BMP_parser.py) script to compress a 1bpp BMP file to a c header file.
- The output directory should be [Core/Inc/](Core/Inc)

**Single file usage**
```bash
$ python BMP_parser.py <input file> <output directory>
```
**Full directory usage**
```bash
$ python BMP_parser.py <input file> <output directory> -d
```

**Font usage**
```bash
$ python BMP_parser.py <input file> <output directory> -f -cw <character width in px>
```

# Lookup table generation
- Use the [generate_lookup](generate_lookup.py) script to generate a lookup table from the specified on and off colours
```bash
$ python generate_lookup.py <on colour in R B G> <off colour in R B G>
```
