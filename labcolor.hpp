/*********************************************************************
** Program Filename: img2x16
** Author: Jason Hill
** Ccopyright (C) 2025-06-19
** Description: Header for labcolor.cpp
** File: labcolor.hpp
** MIT License (see LICENSE file for details)
*********************************************************************/

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>
#include <climits>

constexpr auto PIVOT = 1;       //For testing, two pivot options produce different results.
constexpr auto ALPHALIMIT = 253;//Threshold of what is transparent and what is not.

// A simple structure to store an RGBA color.
struct Color {
    //uint8_t b, g, r;
    uint8_t r = 0;
    uint8_t g = 0; 
    uint8_t b = 0; 
    uint8_t a = 255; 
};

// A structure to store a color in the CIELAB space.
struct LabColor {
    double L, a, b;
};

// Enumeration to define conversion being done on image.
enum imageconversiontype {
    DITHER,             //RGB with dither
    DITHERPERCEIEVED,   //LabColor with Dither
    NODITHER,           //RGB
    NODITHERPERCIEVED   //LabColor
};

// Helper function used when converting from XYZ to Lab.
// Two types can be used. Default is defined above: constexpr auto PIVOT
double pivotXYZ_1(double);
double pivotXYZ_2(double);


// Converts an sRGB color (with components in [0, 255])
// to its corresponding Lab representation.
// This conversion follows the standard sRGB -> XYZ -> Lab process.
LabColor rgbToLab(const Color&, int);

// Computes the squared Euclidean distance between two Lab colors (CIE76 metric)
double squaredDistanceLab(const LabColor&, const LabColor&);

// Finds the color in 'colors' that is perceptually closest to the target color.
// "Closeness" here is judged by the Euclidean distance in Lab color space,
// which better approximates human color perception.
// Returns structure of best match color from paletteS
Color findClosestColorPerceived(const Color& , const std::vector<Color>&);
Color findClosestPaletteColor(const Color& , const std::vector<Color>&);

unsigned char indexfindClosestPaletteColor(const Color&, const std::vector<Color>&);
unsigned char indexfindClosestColorPerceived(const Color&, const std::vector<Color>&);

//Dither routine, preserves transpariency as pallete[0] (if applicable)
void floydSteinbergColorDither(unsigned char*, int, int, const std::vector<Color>&,int, imageconversiontype);

//Pre calculate labcolor for palette
//This speeds up execution because it doesn't have to be called in each loop.
void precalcPaletteLabColor(std::vector<LabColor>&, const std::vector<Color>&);