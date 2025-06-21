/*********************************************************************
** Program Filename: img2x16
** Author: Jason Hill
** Ccopyright (C) 2025-06-19
** Description: Supporting function to convert image files to VERA binary streams.
** File: labcolor.cpp
** MIT License (see LICENSE file for details)
*********************************************************************/

#include "labcolor.hpp"

extern  std::vector<Color> x16vpal;
std::vector<LabColor> actPaletteLabColor_lut;

// Helper function used when converting from XYZ to Lab.
// The function f(t) is defined piecewise: if t > 0.008856, use the cube root; 
// otherwise, use a linear transformation.
// Two options return slightly different results.
double pivotXYZ_1(double t) {

    const double delta = 0.00885645167903563081717167575546; // (6/29)^3
    if (t > delta)
        return std::cbrt(t);
    else
        return (7.787 * t) + (16.0 / 116.0);
}

double pivotXYZ_2(double t) {
    return (t <= 0.04045) ? t / 12.92 : std::pow((t + 0.055) / 1.055, 2.4);
}

// Converts an sRGB color (with components in [0, 255])
// to its corresponding Lab representation.
// This conversion follows the standard sRGB -> XYZ -> Lab process.
LabColor rgbToLab(const Color& color, int method) {
    // Normalize the color components to [0, 1]
    double r = color.r / 255.0;
    double g = color.g / 255.0;
    double b = color.b / 255.0;

    // Apply gamma correction (inverse sRGB companding)
    auto gammaCorrect = [](double channel) -> double {
        return (channel > 0.04045) ? std::pow((channel + 0.055) / 1.055, 2.4)
            : (channel / 12.92);
        };

    double R = gammaCorrect(r);
    double G = gammaCorrect(g);
    double B = gammaCorrect(b);

    // Convert RGB to XYZ using the sRGB color space matrix (D65 illuminant)
    double X = 0.4124564 * R + 0.3575761 * G + 0.1804375 * B;
    double Y = 0.2126729 * R + 0.7151522 * G + 0.0721750 * B;
    double Z = 0.0193339 * R + 0.1191920 * G + 0.9503041 * B;

    // Scale XYZ values to the range [0, 100]
    X *= 100.0;
    Y *= 100.0;
    Z *= 100.0;

    // Reference white point for D65
    const double refX = 95.047;
    const double refY = 100.000;
    const double refZ = 108.883;

    // Normalize and convert each component using the pivot function
    double xRatio = X / refX;
    double yRatio = Y / refY;
    double zRatio = Z / refZ;

    double fx = 0;
    double fy = 0;
    double fz = 0;

    switch (method) {
    case 1:
        fx = pivotXYZ_2(xRatio);
        fy = pivotXYZ_2(yRatio);
        fz = pivotXYZ_2(zRatio);
        break;
    default:
        fx = pivotXYZ_1(xRatio);
        fy = pivotXYZ_1(yRatio);
        fz = pivotXYZ_1(zRatio);
        break;
    }

    // Calculate final Lab values
    LabColor lab = { 0, 0, 0 };
    lab.L = (116.0 * fy) - 16;
    lab.a = 500.0 * (fx - fy);
    lab.b = 200.0 * (fy - fz);

    return lab;
}

// Computes the squared Euclidean distance between two Lab colors (CIE76 metric)
// Reference: https://en.wikipedia.org/wiki/CIELAB_color_space
double squaredDistanceLab(const LabColor& lab1, const LabColor& lab2) {
    double dL = lab1.L - lab2.L;
    double da = lab1.a - lab2.a;
    double db = lab1.b - lab2.b;
    return dL * dL + da * da + db * db;
}

//Pre calculate labcolor for palette
void precalcPaletteLabColor(std::vector<LabColor>& working, const std::vector<Color>& wpal) {
    for (unsigned int z = 0; z < wpal.size(); ++z) {
        LabColor testpixel = { 0,0,0 };
        testpixel = rgbToLab(wpal[z], PIVOT);
        working.push_back(testpixel);
    }
}

// Finds the color in 'colors' that is perceptually closest to the target color.
// "Closeness" here is judged by the Euclidean distance in Lab color space,
// which better approximates human color perception (closest Euclidean distance with Labcolor).
// Preserves transpariency as pallete[0] (if applicable)
Color findClosestColorPerceived(const Color& pixel, const std::vector<Color>& palette) {

    Color bestMatch = palette[0];

    if (pixel.a < ALPHALIMIT) {
        bestMatch.a = 0;
        bestMatch.r = 0;
        bestMatch.g = 0;
        bestMatch.b = 0;
        return bestMatch;
    }
    
    LabColor targetLab = rgbToLab(pixel, PIVOT);
    LabColor bestMatchLab = rgbToLab(bestMatch, PIVOT);
    double minDistance = squaredDistanceLab(targetLab, bestMatchLab);

    for (size_t i = 1; i < palette.size(); i++) {
        /* --> Pre-calculated for speed
        LabColor lab = rgbToLab(colors[i],0);
        double distance = squaredDistanceLab(targetLab, lab);
        *///<--
        //double distance = squaredDistanceLab(targetLab, x16labcolor[i]);
        //TODO:
        double distance = squaredDistanceLab(targetLab, actPaletteLabColor_lut[i]);
        if (distance < minDistance) {
            minDistance = distance;
            bestMatch = palette[i];
        }
    }
    return bestMatch;
}

unsigned char indexfindClosestColorPerceived(const Color& pixel, const std::vector<Color>& palette) {
    unsigned char closest = 0;
    LabColor targetLab = rgbToLab(pixel, PIVOT);

    //The initial 'bestMatch' should not be a color in the pallete
    //Color bestMatch = { 253, 253, 253, 255 };
    Color bestMatch = palette[palette.size() - 1];
    Color workingColor = { 0, 0, 0, 255 };

    //.a=255 is solid color, .a=0 is completely transparent
    if (pixel.a < ALPHALIMIT) {
        return closest;
    }

    LabColor bestMatchLab = rgbToLab(bestMatch, PIVOT);
    double minDistance = squaredDistanceLab(targetLab, bestMatchLab);

    for (unsigned int x = 1; x < palette.size(); ++x) {
        double distance = squaredDistanceLab(targetLab, actPaletteLabColor_lut[x]);

        if (distance < minDistance) {
            minDistance = distance;
            bestMatch = palette[x];
            closest = (unsigned char)x;
        }
    }

    return closest;

}

// Returns the palette index of the closest color to the rgba pixel provided
// Closest Euclidean distance in RGB color
// Preserves transpariency as pallete[0] (if applicable)
Color findClosestPaletteColor(const Color& pixel, const std::vector<Color>& palette) {
    int minDist = INT_MAX;
    Color closest = palette[0];
    
    if (pixel.a< ALPHALIMIT) {
        closest.a = 0;
        return closest;
    }
    //else
    closest.a = 255;
    
    for (const auto& c : palette) {
        int dr = pixel.r - c.r;
        int dg = pixel.g - c.g;
        int db = pixel.b - c.b;
        int dist = dr * dr + dg * dg + db * db;
        if (dist < minDist) {
            minDist = dist;
            closest = c;
        }
    }
    return closest;
}

unsigned char indexfindClosestPaletteColor(const Color& pixel, const std::vector<Color>& palette) {
    int minDist = INT_MAX;

    unsigned char closest = 0;

    if (pixel.a < ALPHALIMIT) {
        return 0;
    }

    //Assume palettes <256 bytes
    closest = (unsigned char)(palette.size() - 1);

    for (unsigned int x = 0; x < palette.size(); ++x) {
        Color c = palette[x];
        int dr = pixel.r - c.r;
        int dg = pixel.g - c.g;
        int db = pixel.b - c.b;
        int dist = dr * dr + dg * dg + db * db;

        if (dist < minDist) {
            minDist = dist;
            closest = (unsigned char)x;
        }
    }

    return closest;
}

//Applies an in-place dither (permenantly altering) on an image bit-stream provided.
void floydSteinbergColorDither(unsigned char* image, int width, int height, const std::vector<Color>& palette, int channels, imageconversiontype method) {
    
    std::vector<float> buffer(image, image + width * height * channels);
    
    Color newPixel;
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int i = (y * width + x) * channels;
            Color oldPixel = { (uint8_t)buffer[i], (uint8_t)buffer[i + 1], (uint8_t)buffer[i + 2], 255};

            if (channels == 4) {
                oldPixel.a = (uint8_t)buffer[i + 3];
            }

            if (method == DITHER) {
                newPixel = findClosestPaletteColor(oldPixel, palette);
            }
            else {
                newPixel = findClosestColorPerceived(oldPixel, palette);
            }

            float errR = buffer[i] - newPixel.r;
            float errG = buffer[i + 1] - newPixel.g;
            float errB = buffer[i + 2] - newPixel.b;

            buffer[i] = newPixel.r;
            buffer[i + 1] = newPixel.g;
            buffer[i + 2] = newPixel.b;
            
            if(channels==4)
                buffer[i + 3] = newPixel.a;


            //Lamda function to perform Floyd-Steinberg dither on pixel array
            //https://en.wikipedia.org/wiki/Floyd%E2%80%93Steinberg_dithering

            auto distribute = [&](int dx, int dy, float factor) {
                int nx = x + dx, ny = y + dy;
                if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                    int ni = (ny * width + nx) * channels;
                    buffer[ni] = std::clamp(buffer[ni] + errR * factor, 0.0f, 255.0f);
                    buffer[ni + 1] = std::clamp(buffer[ni + 1] + errG * factor, 0.0f, 255.0f);
                    buffer[ni + 2] = std::clamp(buffer[ni + 2] + errB * factor, 0.0f, 255.0f);
                    //Ignnore [ni+3] (the alpha channel)
                }
                };

            distribute(1, 0, 7.0f / 16);
            distribute(-1, 1, 3.0f / 16);
            distribute(0, 1, 5.0f / 16);
            distribute(1, 1, 1.0f / 16);
        }
    }

    for (int i = 0; i < width * height * channels; ++i) {
        image[i] = static_cast<unsigned char>(buffer[i]);
    }
}
