# img2x16
`img2x16` is a program to convert image files to VERA binary streams for the Commander X16.

Unlike a lot of existing tools, `img2x16` is designed to take a given image, and adapt it to a given color palette for the [VERA](https://github.com/X16Community/x16-docs/blob/master/X16%20Reference%20-%2009%20-%20VERA%20Programmer's%20Reference.md) used in the [Commander X16](https://www.commanderx16.com). It curently only supports 8bpp color depth (i.e 16-bit palette entries).

* Image preview file generation is optionally supported (`-s`)
* Dither is also optionally supported (`-d`)
* If an image file (GIF or PNG) has transparencies, they will be supported in the output binary.
  * *Great for sprites*

This program works by looking at each pixel's color in the image, and finding the closest matching color in the palette. There are two user selectable methods do to this: the closest color in RGB color space (default), and the closest perceived color (`-p`) using the [CIE LabColor](https://en.wikipedia.org/wiki/CIELAB_color_space) space.

If no color palatte file is given, the program will use the default VERA color palette. The color palette file needs to be in the same format as used on the X16; up to 256 entries of 16-bits each, but can be of any arbitrary size up to 256 entries (512 bytes). The first index of the color palette is always treated as transparent, regardless of it's actual color.

## About
This project started in an effort to quickly adapt multiple image files to a single color palette. Many of the current tools either create their own palette for each image, or require software and dependencies that make it harder to use across platforms.

This tools is quick, command line driven, and makes it easy to integrate in scripts and build tools. It also requires no dependencies or libraries outside of the normal build chain (>= C++ 17). Binaries are provided for Windows, Ubuntu (*should work on most Debian distros*), and Raspberry PI OS (arm7L and aarch64).

## Usage
Valid input image format are: BMP, PNG, GIF, JPG

There are no checks or resizing for image size. This should be done before conversion.

```
Usage:
  [OPTION...]

  -i arg  Required - Input image files (i.e. -i"inputimage.png")
  -v arg  Optional - Use alternate VERA palette file (i.e. -v"bw.pal")
  -s      Optional - Save preview PNG image alongside binary image file
  -d      Optional - Dither the image
  -p      Optional - Use perceived color formula
  -n      Optional - Do NOt output a VERA binary image file
  -x arg  Optional - 4-bpp, use palette offset number required (i.e. -x1
          for the 2nd index [zero based])
  -o arg  Optional - output filename, default is infilename.bin (i.e.
          -o"image.bin"

        **** Output Files are overwritten without prompting ****
```
## Examples
This example will take the file `theimage.png` and covert it to a binary image file using the defeault VERA palette. 

#### Simple command line usage
`img2x16 -i"theimage.png"`

This would output a file called `theimage.bin`. The image would use the default VERA color palette, no dither, and the closest RGB calculation for color matching.

#### BASIC Load example of the binary file (256 color)
```
10 SCREEN $80
20 BVLOAD"THEIMAGE.BIN",8,0,0
```

#### Multi-options example command line usage
`img2x16 -i"photo.bmp" -v"bw.pal" -sdp -o"output.img"`

This would create a binary file called `output.img` that used the palette file `bw.pal`, and a preview image called `photo_.png`. The conversion would use dithering and percieved color for its color matching.

Output:
```
Opening file: sdtest.png        Width: 320      Height: 240
Using 256 palette colors from default VERA palette.
Writing X16 (raw) file: output.img      in 2.49538 seconds.
Writing image file:     sdtest_.png     in 0.205782 seconds.
```

#### 4-bit per pixel
`img2x16 -i"photo.png" -s -x1 -o"output.bin"`

This would create a binary file called `output.bin` that used the default VERA palette. It would create a 4bpp binary file that was created using the 2nd index of the pallete (greyscale). It will also save a preview PNG file with it.


## Suggestions
* Don't dither images with transparencies
* Perceived color mode usually ends up oversaturated. The default setting is usually best.

## Bulding
If you would like to complile from source just use your favorite C++ 17 compiler. `Makefile` provided for a POSIX system example and a `Make.bat` file example which should work with Visual Studio under the *Developer Command Prompt*.
