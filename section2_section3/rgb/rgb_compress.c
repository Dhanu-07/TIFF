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
    uint32_t *raster;

    // Read image dimensions
    TIFFGetField(in, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(in, TIFFTAG_IMAGELENGTH, &height);
    npixels = width * height;

    // Allocate memory for the raster
    raster = (uint32_t *) _TIFFmalloc(npixels * sizeof(uint32_t)); // uint32 for RGB
    if (raster == NULL) {
        fprintf(stderr, "Could not allocate memory for raster.\n");
        TIFFClose(in);
        return;
    }

    // Read the image as RGB
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

    // Set TIFF fields for output
    TIFFSetField(out, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(out, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(out, TIFFTAG_COMPRESSION, COMPRESSION_DEFLATE); // ZIP compression
    TIFFSetField(out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB); // For RGB
    TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 8); // 8 bits for each channel
    TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, 3); // 3 channels for RGB

    // Write the image
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

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input.tiff> <output.tiff>\n", argv[0]);
        return 1;
    }
    compress_and_write_tiff(argv[1], argv[2]);
    return 0;
}

