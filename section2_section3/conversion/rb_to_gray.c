#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <tiffio.h>

void convert_color_to_grayscale(uint8_t* color_data, uint8_t* gray_data, int width, int height) {
    for (int i = 0; i < width * height; i++) {
        uint8_t r = color_data[i * 3];
        uint8_t g = color_data[i * 3 + 1];
        uint8_t b = color_data[i * 3 + 2];
        gray_data[i] = (uint8_t)(0.2989 * r + 0.587 * g + 0.114 * b);
    }
}

int load_tiff_image(const char* filename, uint8_t** image_data, int* width, int* height, int* channels) {
    TIFF* tif = TIFFOpen(filename, "r");
    if (tif == NULL) {
        printf("Error opening TIFF file: %s\n", filename);
        return 0;
    }

    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, height);
    *channels = 3;  // Assuming RGB (3 channels)

    size_t size = (*width) * (*height) * 3;  // RGB image, 3 channels per pixel
    *image_data = (uint8_t*)malloc(size);
    if (*image_data == NULL) {
        printf("Error allocating memory for TIFF image.\n");
        TIFFClose(tif);
        return 0;
    }

    for (int y = 0; y < *height; y++) {
        uint8_t* row = *image_data + y * (*width) * 3;
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
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);  // Grayscale (1 channel)
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

    size_t size = width * height;  // Grayscale (1 channel per pixel)
    for (int y = 0; y < height; y++) {
        uint8_t* row = image_data + y * width;
        TIFFWriteScanline(tif, row, y, 0);
    }

    TIFFClose(tif);
    return 1;
}

int main() {
    const char* input_file = "/home/arunp24/section2_section3/brain_398.tif";  // Hardcoded input path (RGB TIFF)
    const char* output_file = "/home/arunp24/section2_section3/output_gray.tif"; // Hardcoded output path (grayscale TIFF)

    uint8_t* image_data = NULL;
    int width, height, channels;

    if (!load_tiff_image(input_file, &image_data, &width, &height, &channels)) {
        return -1;
    }

    if (channels == 3) {
        // RGB image, convert to grayscale
        uint8_t* gray_data = (uint8_t*)malloc(width * height);
        if (gray_data == NULL) {
            printf("Error allocating memory for grayscale data.\n");
            return -1;
        }
        convert_color_to_grayscale(image_data, gray_data, width, height);

        // Save the grayscale image as TIFF
        save_tiff_image(output_file, gray_data, width, height);
        free(gray_data);
    } else {
        printf("Input image is not RGB.\n");
    }

    free(image_data);
    printf("Conversion complete. Output saved to: %s\n", output_file);
    return 0;
}

