#include <stdio.h>
#include <stdint.h>
#include <tiffio.h>

void compress_and_write_tiff(const char *input_file, const char *output_file) {
    TIFF *in = TIFFOpen(input_file, "r");
    if (in == NULL) {
        fprintf(stderr, "Could not open input TIFF file.\n");
        return;
    }

    // Get image dimensions
    uint32_t width, height;
    size_t npixels;
    uint8_t *raster;

    // Read image dimensions
    TIFFGetField(in, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(in, TIFFTAG_IMAGELENGTH, &height);
    npixels = width * height;

    // Allocate memory for the raster (one byte per pixel for grayscale)
    raster = (uint8_t *) _TIFFmalloc(npixels * sizeof(uint8_t)); // uint8_t for grayscale
    if (raster == NULL) {
        fprintf(stderr, "Could not allocate memory for raster.\n");
        TIFFClose(in);
        return;
    }

    // Read the image as grayscale (assuming the input TIFF is already grayscale)
    for (uint32_t row = 0; row < height; row++) {
        if (TIFFReadScanline(in, raster + row * width, row, 0) < 0) {
            fprintf(stderr, "Failed to read the image data.\n");
            _TIFFfree(raster);
            TIFFClose(in);
            return;
        }
    }

    // Open output TIFF file
    TIFF *out = TIFFOpen(output_file, "w");
    if (out == NULL) {
        fprintf(stderr, "Could not open output TIFF file.\n");
        _TIFFfree(raster);
        TIFFClose(in);
        return;
    }

    // Set TIFF fields for output (grayscale image)
    TIFFSetField(out, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(out, TIFFTAG_IMAGELENGTH, height);
    //TIFFSetField(out, TIFFTAG_COMPRESSION, COMPRESSION_NONE); // ZIP compression
    TIFFSetField(out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK); // Grayscale, black is 0
    TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 8); // 8 bits per pixel for grayscale
    TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, 1); // 1 channel for grayscale

    // Write the image data (grayscale)
    for (uint32_t row = 0; row < height; row++) {
        if (TIFFWriteScanline(out, raster + row * width, row, 0) < 0) {
            fprintf(stderr, "Failed to write the image data.\n");
            _TIFFfree(raster);
            TIFFClose(out);
            TIFFClose(in);
            return;
        }
    }

    // Close the output file
    TIFFClose(out);

    // Free resources
    _TIFFfree(raster);
    TIFFClose(in);
}

int main() {
    // Hardcoded input and output paths
    const char *input_file = "/home/arunp24/tiff/test.tif";  // Replace with your input file path
    const char *output_file = "/home/arunp24/tiff/out_test.tif";  // Replace with your desired output path

    compress_and_write_tiff(input_file, output_file);

    return 0;
}

