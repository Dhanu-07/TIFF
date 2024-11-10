#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <tiffio.h>

void convert_grayscale_to_color(uint8_t* gray_data, uint8_t* color_data, int width, int height) {
    for (int i = 0; i < width * height; i++) {
        uint8_t gray_value = gray_data[i];
        color_data[i * 3] = gray_value;     // Red
        color_data[i * 3 + 1] = gray_value; // Green
        color_data[i * 3 + 2] = gray_value; // Blue
    }
}

int load_tiff_image(const char* filename, uint8_t** image_data, int* width, int* height, int* channels) {
    TIFF* tif = TIFFOpen(filename, "r");
    if (tif == NULL) {
        printf("Error opening TIFF file: %s\n", filename);
        return 0;
    }

    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFGetField(tif, TIFFTAG_IMAGEHEIGHT, height);
    *channels = 1;  // Assuming grayscale (1 channel)

    size_t size = (*width) * (*height) * 1;  // Grayscale image, 1 channel per pixel
    *image_data = (uint8_t*)malloc(size);
    if (*image_data == NULL) {
        printf("Error allocating memory for TIFF image.\n");
        TIFFClose(tif);
        return 0;
    }

    for (int y = 0; y < *height; y++) {
        uint8_t* row = *image_data + y * (*width);
        TIFFReadScanline(tif, row, y, 0);
    }

    TIFFClose(tif);
    return 1;
}

int save_tiff_image(const char* filename, uint8_t* image_data, int width, int height) {
    TIFF* tif = TIFFOpen(filename, "w");
    if (tif == NULL) {
        printf("Error opening TIFF file for writing: %s\n", filename);
        return 0;
    }

    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGEHEIGHT, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 3);  // RGB (3 channels)
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

    size_t size = width * height * 3;  // RGB (3 channels per pixel)
    for (int y = 0; y < height; y++) {
        uint8_t* row = image_data + y * width * 3;
        TIFFWriteScanline(tif, row, y, 0);
    }

    TIFFClose(tif);
    return 1;
}

int main() {
    const char* input_file = "input_gray.tif";  // Hardcoded input path (grayscale TIFF)
    const char* output_file = "output_rgb.tif"; // Hardcoded output path (RGB TIFF)

    uint8_t* image_data = NULL;
    int width, height, channels;

    if (!load_tiff_image(input_file, &image_data, &width, &height, &channels)) {
        return -1;
    }

    if (channels == 1) {
        // Grayscale image, convert to RGB
        uint8_t* color_data = (uint8_t*)malloc(width * height * 3);
        if (color_data == NULL) {
            printf("Error allocating memory for color data.\n");
            return -1;
        }
        convert_grayscale_to_color(image_data, color_data, width, height);

        // Save the RGB image as TIFF
        save_tiff_image(output_file, color_data, width, height);
        free(color_data);
    } else {
        printf("Input image is not grayscale.\n");
    }

    free(image_data);
    printf("Conversion complete. Output saved to: %s\n", output_file);
    return 0;
}

