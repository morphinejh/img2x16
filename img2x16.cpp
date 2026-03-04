/*********************************************************************
** Program Filename: img2x16
** Author: Jason Hill
** Ccopyright (C) 2025-06-19
** Description: Program to convert image files to VERA binary streams.
** File: img2x16.cpp
** MIT License (see LICENSE file for details)
*********************************************************************/

#include "img2x16.hpp"

const std::string ver = "Image to X16 (VERA) palette: 0.5\n";

//Default LabColor palette to be pre-calculated once
extern std::vector<LabColor> actPaletteLabColor_lut;

//Default X16 VERA palette Color vector (r, g, b, [a] )
 std::vector<Color> x16vpal = { //{r, g, b, a}

{   0, 0, 0, 255}, { 255,255,255, 255}, { 136, 0 , 0, 255}, { 170,255,238, 255}, { 204,68,204, 255}, {  0,204,85, 255}, {   0,   0, 170, 255}, { 238,238,119, 255}, { 221, 136,  85, 255}, { 102,  68,   0, 255}, { 255, 119, 119, 255}, {  51,  51,  51, 255}, { 119, 119, 119, 255}, { 170, 255, 102, 255}, {   0, 136, 255, 255}, { 187,187,187, 255},
{   0, 0, 0, 255}, {  17, 17, 17, 255}, { 34, 34, 34, 255}, {  51, 51, 51, 255}, { 68, 68, 68, 255}, { 85, 85,85, 255}, { 102, 102, 102, 255}, { 119,119,119, 255}, { 136, 136, 136, 255}, { 153, 153, 153, 255}, { 170, 170, 170, 255}, { 187, 187, 187, 255}, { 204, 204, 204, 255}, { 221, 221, 221, 255}, { 238, 238, 238, 255}, { 255,255,255, 255},
{  34,17,17, 255}, { 68,51,51, 255}, { 102,68,68, 255}, { 136,102,102, 255}, { 170,136,136, 255}, { 204,153,153, 255}, { 255,187,187, 255}, { 34,17,17, 255}, { 68,34,34, 255}, { 102,51,51, 255}, { 136,68,68, 255}, { 170,85,85, 255}, { 204,102,102, 255}, { 255,119,119, 255}, { 34,0,0, 255}, { 68,17,17, 255}, { 102,17,17, 255},
{ 136,34,34, 255}, { 170,34,34, 255}, { 204,51,51, 255}, { 255,51,51, 255}, { 34,0,0, 255}, { 68,0,0, 255}, { 102,0,0, 255}, { 136,0,0, 255}, { 170,0,0, 255}, { 204,0,0, 255}, { 255,0,0, 255}, { 34,34,17, 255}, { 68,68,51, 255}, { 102,102,68, 255}, { 136,136,102, 255}, { 170,170,136, 255},
{ 204,204,153, 255}, { 255,238,187, 255}, { 34,17,17, 255}, { 68,51,34, 255}, { 102,85,51, 255}, { 136,119,68, 255}, { 170,153,85, 255}, { 204,187,102, 255}, { 255,221,119, 255}, { 34,17,0, 255}, { 68,51,17, 255}, { 102,85,17, 255}, { 136,102,34, 255}, { 170,136,34, 255}, { 204,170,51, 255}, { 255,204,51, 255},
{ 34,17,0, 255}, { 68,51,0, 255}, { 102,68,0, 255}, { 136,102,0, 255}, { 170,136,0, 255}, { 204,153,0, 255}, { 255,187,0, 255}, { 17,34,17, 255}, { 51,68,51, 255}, { 85,102,68, 255}, { 119,136,102, 255}, { 153,170,136, 255}, { 187,204,153, 255}, { 221,255,187, 255}, { 17,34,17, 255}, { 51,68,34, 255},
{ 68,102,51, 255}, { 102,136,68, 255}, { 136,170,85, 255}, { 153,204,102, 255}, { 187,255,119, 255}, { 17,34,0, 255}, { 34,68,17, 255}, { 68,102,17, 255}, { 85,136,34, 255}, { 102,170,34, 255}, { 136,204,51, 255}, { 153,255,51, 255}, { 17,34,0, 255}, { 34,68,0, 255}, { 51,102,0, 255}, { 68,136,0, 255},
{ 85,170,0, 255}, { 102,204,0, 255}, { 119,255,0, 255}, { 17,34,17, 255}, { 51,68,51, 255}, { 68,102,85, 255}, { 102,136,102, 255}, { 136,170,136, 255}, { 153,204,170, 255}, { 187,255,204, 255}, { 17,34,17, 255}, { 34,68,34, 255}, { 51,102,68, 255}, { 68,136,85, 255}, { 85,170,102, 255}, { 102,204,136, 255},
{ 119,255,153, 255}, { 0,34,0, 255}, { 17,68,17, 255}, { 17,102,34, 255}, { 34,136,51, 255}, { 34,170,68, 255}, { 51,204,85, 255}, { 51,255,102, 255}, { 0,34,0, 255}, { 0,68,17, 255}, { 0,102,17, 255}, { 0,136,34, 255}, { 0,170,34, 255}, { 0,204,51, 255}, { 0,255,51, 255}, { 17,34,34, 255},
{ 51,68,68, 255}, { 68,102,102, 255}, { 102,136,136, 255}, { 136,170,170, 255}, { 153,204,204, 255}, { 187,255,255, 255}, { 17,34,34, 255}, { 34,68,68, 255}, { 51,102,102, 255}, { 68,136,136, 255}, { 85,170,170, 255}, { 102,204,204, 255}, { 119,255,255, 255}, { 0,34,34, 255}, { 17,68,68, 255}, { 17,102,102, 255},
{ 34,136,136, 255}, { 34,170,170, 255}, { 51,204,204, 255}, { 51,255,255, 255}, { 0,34,34, 255}, { 0,68,68, 255}, { 0,102,102, 255}, { 0,136,136, 255}, { 0,170,170, 255}, { 0,204,204, 255}, { 0,255,255, 255}, { 17,17,34, 255}, { 51,51,68, 255}, { 68,85,102, 255}, { 102,102,136, 255}, { 136,136,170, 255},
{ 153,170,204, 255}, { 187,204,255, 255}, { 17,17,34, 255}, { 34,34,68, 255}, { 51,68,102, 255}, { 68,85,136, 255}, { 85,102,170, 255}, { 102,136,204, 255}, { 119,153,255, 255}, { 0,0,34, 255}, { 17,17,68, 255}, { 17,34,102, 255}, { 34,51,136, 255}, { 34,68,170, 255}, { 51,85,204, 255}, { 51,102,255, 255},
{ 0,0,34, 255}, { 0,17,68, 255}, { 0,17,102, 255}, { 0,34,136, 255}, { 0,34,170, 255}, { 0,51,204, 255}, { 0,51,255, 255}, { 17,17,34, 255}, { 51,51,68, 255}, { 85,68,102, 255}, { 119,102,136, 255}, { 153,136,170, 255}, { 187,153,204, 255}, { 221,187,255, 255}, { 17,17,34, 255}, { 51,34,68, 255},
{ 68,51,102, 255}, { 102,68,136, 255}, { 136,85,170, 255}, { 153,102,204, 255}, { 187,119,255, 255}, { 17,0,34, 255}, { 34,17,68, 255}, { 68,17,102, 255}, { 85,34,136, 255}, { 102,34,170, 255}, { 136,51,204, 255}, { 153,51,255, 255}, { 17,0,34, 255}, { 34,0,68, 255}, { 51,0,102, 255}, { 68,0,136, 255},
{ 85,0,170, 255}, { 102,0,204, 255}, { 119,0,255, 255}, { 34,17,34, 255}, { 68,51,68, 255}, { 102,68,102, 255}, { 136,102,136, 255}, { 170,136,170, 255}, { 204,153,204, 255}, { 255,187,238, 255}, { 34,17,17, 255}, { 68,34,51, 255}, { 102,51,85, 255}, { 136,68,119, 255}, { 170,85,153, 255}, { 204,102,187, 255},
{ 255,119,221, 255}, { 34,0,17, 255}, { 68,17,51, 255}, { 102,17,85, 255}, { 136,34,102, 255}, { 170,34,136, 255}, { 204,51,170, 255}, { 255,51,204, 255}, { 34,0,17, 255}, { 68,0,51, 255}, { 102,0,68, 255}, { 136,0,102, 255}, { 170,0,136, 255}, { 204,0,153, 255}, { 255,0,187, 255}

};

static imageconversiontype thismethod = DITHER;
static bool fsave = false;
static bool dither = false;
static bool perceived = false;
static bool nooutputbin = false;
static bool externalpal = false;
static std::vector<Color>* activePal;
static bool indexed = false;

int main(int argc, char* argv[]) {

    int width, height, channels;
    int rqstChannels = 4;               //Always request 4, if no Alpha channel, it is 100% opaque
    unsigned char* img = nullptr;
    activePal = new std::vector<Color>;
    *activePal = x16vpal;
	int palette16_index=-1;
    cxxopts::ParseResult result;

    std::string infilename, palfilename, outbinaryname, outputpngname;

    cxxopts::Options cmdoptions(argv[0], ver.c_str());
    cmdoptions.add_options()
        ("i", "Required - Input image files (i.e. -i\"inputimage.png\")", cxxopts::value(infilename))
        ("v", "Optional - Use alternate VERA palette file (i.e. -v\"bw.pal\")", cxxopts::value(palfilename))
        ("s", "Optional - Save preview PNG image alongside binary image file", cxxopts::value(fsave))
        ("d", "Optional - Dither the image", cxxopts::value(dither))
        ("p", "Optional - Use perceived color formula", cxxopts::value(perceived))
        ("n", "Optional - Do NOt output a VERA binary image file", cxxopts::value(nooutputbin))
        ("x", "Optional - 4-bpp, use palette offset number required (i.e. -x1 for the 2nd index [zero based])", cxxopts::value(palette16_index))
        ("o", "Optional - output filename, default is infilename.bin (i.e. -o\"image.bin\"", cxxopts::value(outbinaryname));
    
    try {
        result = cmdoptions.parse(argc, argv);
    }
    catch (const cxxopts::exceptions::exception& e){
        std::cout << e.what() << std::endl << std::endl << std::endl;
        usage(&cmdoptions);
        return EXIT_FAILURE;
    }

    if (result.count("i")==0) {
        std::cout << "No input image given." << std::endl << std::endl;
        usage(&cmdoptions);
        delete activePal;
        return EXIT_FAILURE;
    }

    std::ifstream infile(infilename, std::ios::binary);
    std::ifstream palfile(palfilename, std::ios::binary);
    if(result.count("v")!=0){
        if (!palfile) {
            std::cerr << "Error: Could not open palette file: " << palfilename << std::endl;
            usage(&cmdoptions);
            palfile.close();
            delete activePal;
            return EXIT_FAILURE;
        }
        else {
            if (loadpalette(activePal, &palfile)) {
                std::cerr << "Palette file is the incorrect format." << std::endl;
                delete activePal;
                return EXIT_FAILURE;
            }
            //File is closed in loadpalette
            externalpal = true;
        }
    }


    if (result.count("x") > 0) {
        
        int palette_index_count = 16;
        int palette_length = 16;

        if (externalpal = true) {
            palette_index_count = (int)(activePal->size() >> 4);
        }

        if ((activePal->size() % 16)) {

            palette_length = activePal->size() - (palette16_index * 16);
            if (palette_length > 16) {
                palette_length = 16;
            }
        }
        //We need to make sure the pallate has the propeor size
        if (palette16_index <= palette_index_count && palette16_index >= 0) {
            std::vector<Color>* tempPal = new std::vector<Color>(activePal->begin() + (palette16_index * 16), activePal->begin() + (palette16_index * 16) + palette_length);
            delete activePal;
            activePal = tempPal;

            indexed = true;
        }
        else {
            std::cout << "Inavlid palette offset given of "<< palette16_index << " and only " << palette_length << " available." << std::endl;
            usage(&cmdoptions);
            return EXIT_FAILURE;
        }
    }

    //Configure image flags for dither/perceived
    configureflags();

    if (!infile) {
        std::cerr << "Error: Could not open image file: " << infilename << std::endl;
        usage(&cmdoptions);
        delete activePal;
        return EXIT_FAILURE;
    }

    //Set Output filename(s)
    outputpngname = infilename;

    if (outputpngname[outputpngname.length() - 4] == '.') {
        size_t pos = outputpngname.length() - 4;
        if (pos > 0) {
            if (result.count("o") == 0) {
                outbinaryname = outputpngname;
                outbinaryname.replace(pos, 4, ".bin");
            }
            outputpngname.replace(pos, 4, "_.png");
        }
        else {
            if (result.count("o") == 0) {
                outbinaryname = outputpngname;
                outbinaryname.append(".bin");
            }
            outputpngname.append("_.png");
        }
    }

    //REQUIRED once before call to 'findClosestPaletteColor': Saves a lot of time later
    //It's better here once, most other places loop.
    precalcPaletteLabColor(actPaletteLabColor_lut, *activePal);

	//Start time measurement
	auto startc = std::chrono::high_resolution_clock::now();
	 
    std::cout << "Opening file: " << infilename;
    img = stbi_load(infilename.c_str(), &width, &height, &channels, rqstChannels);

    std::cout << "\tWidth: " << width << "\tHeight: " << height << std::endl;
    std::cout << "Using " << activePal->size() << " palette colors";
    if (externalpal) {
        std::cout << " from file: " << palfilename;
    }
    else {
        std::cout << " from default VERA palette.";
    }
    std::cout << std::endl;

    if (!img) {
        std::cerr << "Failed to open image:" << infilename << ". Possibly unknown format." << std::endl;
        usage(&cmdoptions);
        delete activePal;
        return EXIT_FAILURE;
    }

    adaptPixels(*activePal, img, width, height, thismethod, rqstChannels);
    if(!nooutputbin){
        std::cout << "Writing X16 (raw) file:\t" << outbinaryname << " \t";
        if (outputByteData(*activePal, img, outbinaryname.c_str(), width, height, thismethod, rqstChannels, indexed)) {
            std::cerr << "Error: Could not create file: " << outbinaryname << std::endl;
            usage(&cmdoptions);
        } else {
            auto endc = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = endc - startc;
            std::cout << "in " << elapsed.count() << " seconds." << std::endl;
        }
    }

    if (fsave) {
        std::cout << "Writing image file:\t" << outputpngname << "\t";
        
        startc = std::chrono::high_resolution_clock::now();
        
        stbi_write_png(outputpngname.c_str(), width, height, rqstChannels, img, width * rqstChannels);
        
        auto endc = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = endc - startc;
		std::cout << "in " << elapsed.count() << " seconds." << std::endl;
    }

    stbi_image_free(img);
    delete activePal;
    infile.close();
    return EXIT_SUCCESS;
}

void adaptPixels(const std::vector<Color> &activePalette, unsigned char* imgpixels, int width, int height, imageconversiontype method, int channels){

    if (method == DITHER || method == DITHERPERCEIEVED) {
        floydSteinbergColorDither(imgpixels, width, height, activePalette, channels, method);
    }
    else {
        for (int it = 0; it < (width * height * channels); it += channels) {
            unsigned char x16idx = 0;
            Color testpixel = { 0,0,0,255 };

            testpixel.r = *(imgpixels + it);
            testpixel.g = *(imgpixels + it + 1);
            testpixel.b = *(imgpixels + it + 2);

            if (channels == 4) {
                testpixel.a = *(imgpixels + it + 3);
            }

            if (method == DITHERPERCEIEVED || method == NODITHERPERCIEVED) {
                x16idx = indexfindClosestColorPerceived(testpixel, activePalette) & 0xFF;
            }
            else {
                x16idx = indexfindClosestPaletteColor(testpixel, activePalette) & 0xFF;
            }

            if (x16idx < activePalette.size()) {
                imgpixels[it + 0] = (unsigned char)(activePalette[x16idx].r);
                imgpixels[it + 1] = (unsigned char)(activePalette[x16idx].g);
                imgpixels[it + 2] = (unsigned char)(activePalette[x16idx].b);
            }
            else {
                //This shouldn't happen, if so just return 'Green' for troubleshooting
                imgpixels[it + 0] = 0;
                imgpixels[it + 1] = 255;
                imgpixels[it + 2] = 0;

            }
        }
    }
}

int outputByteData(const std::vector<Color> &activePalette, unsigned char* pixels, const char* fileName, int width, int height, imageconversiontype method, int channels, bool _indexed) {

    std::string outbinaryname = fileName;
    std::ofstream outfile(outbinaryname, std::ios::binary);

    if (!outfile) {
        std::cerr << "Error: Could not create file: " << outbinaryname << std::endl;
        return -1;
    }

	int pixel_counter=0;

    for (int it = 0; it < (width * height * channels); it += channels) {
        unsigned char x16idx = 0;
        Color testpixel = { 0,0,0,255 };

        //Time could be save by pre-generating(i.e. saving) all the indcies in the 'adapt' stage.
        testpixel.r = *(pixels + it);
        testpixel.g = *(pixels + it + 1);
        testpixel.b = *(pixels + it + 2);
		testpixel.a = *(pixels + it + 3);
        if (method == DITHERPERCEIEVED || method == NODITHERPERCIEVED) {
            x16idx = indexfindClosestColorPerceived(testpixel, activePalette) & 0xFF;
        }
        else {
            x16idx = indexfindClosestPaletteColor(testpixel, activePalette) & 0xFF;
        }

		pixel_counter++;
		
        if (_indexed) {   //Grab two pixels per byte
            unsigned char temp=0;
            x16idx <<= 4;
			x16idx &= 0xF0;
			it +=channels;
            testpixel.r = *(pixels + it);
            testpixel.g = *(pixels + it + 1);
            testpixel.b = *(pixels + it + 2);
            if (method == DITHERPERCEIEVED || method == NODITHERPERCIEVED)
                temp = indexfindClosestColorPerceived(testpixel, activePalette) & 0x0F;
            else
                temp = indexfindClosestPaletteColor(testpixel, activePalette) & 0x0F;
            x16idx |= temp;
            outfile.write((char*)&x16idx, 1);
			pixel_counter++;
        }
        else {
            outfile.write((char*)&x16idx, 1);
        }
        
    }

	std::cout<<std::endl<<width<<" "<<height<<" "<<channels<<std::endl<<"Pixel Count: "<<pixel_counter<<std::endl;
	
    return 0;

}

void configureflags() {

    int testflag = (int)dither + (int)(perceived<<1);

    switch (testflag) {
        case 0:
            thismethod = NODITHER;
            break;
        case 1:
            thismethod = DITHER;
            break;
        case 2:
            thismethod = NODITHERPERCIEVED;
            break;
        case 3:
            thismethod = DITHERPERCEIEVED;
            break;
    }

}

void usage(cxxopts::Options *options) {
    //std::cout << std::endl;
    std::cout << options->help({ "", "Group" });

    std::cout << "\t**** Output Files are overwritten without prompting ****" << std::endl;
}

//Sets the active palette from 'default VERA' to custom file
int loadpalette(std::vector<Color>* workingPal, std::ifstream *file) {
    int retVal = -1;
    char bytes[2];
    Color temp;

    file->seekg(0, std::ios::end);
    long fileSize = (long)file->tellg();
    file->seekg(0, std::ios::beg);

    //Check if palette file is 16-bit divible
    retVal = fileSize % 2;

    //Check if palette file is small enough for VERA
    if (retVal || fileSize > 512) {
        return retVal;
    }

    workingPal->resize(0);

    while (file->read(bytes, 2)) {

        temp.g = ((bytes[0] & 0xF0) >> 4) * 16;
        temp.b = (bytes[0] & 0x0F) * 16;
		temp.r = (bytes[1] & 0x0F)*16;
		
        workingPal->push_back(temp);
    }
   
    file->close();
    return retVal;
}
