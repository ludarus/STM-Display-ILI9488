# Specifications
- Resolution = 480x320
- ST7796 GRAM = 345.6Kb
- F091RC SRAM = 32KiB
- F091RC FLASH = 256KiB

# BMP conversion
Use the python script to convert a 1bpp BMP file to a c header file. 

Single File usage:
```bash
$ python BMP_parser.py <input file> <output directory>
```
Full Directory usage:
```bash
$ python BMP_parser.py <input file> <output directory> -d
```
