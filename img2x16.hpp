/*********************************************************************
** Program Filename: img2x16
** Author: Jason Hill
** Ccopyright (C) 2025-06-19
** Description: Header for img2x16.cpp
** File: img2x16.hpp
** MIT License (see LICENSE file for details)
*********************************************************************/

#pragma once
#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <chrono>
#include "labcolor.hpp"
#include "cxxopts.hpp"

#define CXXOPTS_NO_EXCEPTIONS
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

//Only tested with four file types - should be easily expanded. 
#define STBI_NO_PSD
#define STBI_NO_TGA
#define STBI_NO_HDR
#define STBI_NO_PIC
#define STBI_NO_PNM   (.ppm and .pgm)
/*
#define STBI_NO_GIF
#define STBI_NO_JPEG
#define STBI_NO_PNG
#define STBI_NO_BMP
*/

//Function Declaration
int outputByteData(const std::vector<Color>&, unsigned char*, const char*, int, int, imageconversiontype, int);
int loadpalette(std::vector<Color>*, std::ifstream*);
void usage(cxxopts::Options*);
void configureflags();
void adaptPixels(const std::vector<Color>&, unsigned char*, int, int, imageconversiontype, int);