#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MAX_WIDTH 128
#define MAX_HEIGHT 128
#define MAX_COMPRESSED_SIZE (MAX_WIDTH * MAX_HEIGHT * 2) 

// Define matrix dimensions
#define IMAGE_ROWS MAX_HEIGHT
#define IMAGE_COLS MAX_WIDTH

// Function to find the longest match for LZ77 compression
void find_longest_match(uint8_t image_data[IMAGE_ROWS][IMAGE_COLS], uint32_t start_row, uint32_t start_col, uint32_t length, uint32_t* best_offset_row, uint32_t* best_offset_col, uint32_t* best_length) {
    *best_offset_row = 0;
    *best_offset_col = 0;
    *best_length = 0;

    for (uint32_t i = 0; i < start_row; i++) {
        for (uint32_t j = 0; j < IMAGE_COLS; j++) {
            uint32_t match_length = 0;

            // Compare until we find a mismatch or reach the maximum length
            while (match_length < length && image_data[i][j + match_length] == image_data[start_row][start_col + match_length]) {
                match_length++;
            }

            if (match_length > *best_length) {
                *best_length = match_length;
                *best_offset_row = start_row - i; // Calculate row offset
                *best_offset_col = start_col - j; // Calculate column offset
            }
        }
    }
}

// Function to compress grayscale image data using a simplified Deflate-like method
void compress_deflate_image(uint8_t image_data[IMAGE_ROWS][IMAGE_COLS], uint32_t width, uint32_t height, uint8_t* compressed_data, uint32_t* compressed_size) {
    uint32_t index_row = 0; // Current row
    uint32_t index_col = 0; // Current column
    *compressed_size = 0; // Initialize compressed size

    compress_deflate_image_label0:while (index_row < height) {
        uint32_t offset_row, offset_col, length;

        // Find the longest match for LZ77 compression
        find_longest_match(image_data, index_row, index_col, 255, &offset_row, &offset_col, &length);

        // If we found a match
        if (length > 1) {
            // Write the offset and length to the compressed data
            compressed_data[*compressed_size] = (uint8_t)(length - 1); // Length (0 for a literal)
            compressed_data[*compressed_size + 1] = (uint8_t)(offset_row & 0xFF); // Low byte of row offset
            compressed_data[*compressed_size + 2] = (uint8_t)((offset_row >> 8) & 0xFF); // High byte of row offset
            compressed_data[*compressed_size + 3] = (uint8_t)(offset_col & 0xFF); // Low byte of column offset
            compressed_data[*compressed_size + 4] = (uint8_t)((offset_col >> 8) & 0xFF); // High byte of column offset
            *compressed_size += 5;

            // Move the index forward by the length of the match
            index_col += length;
            if (index_col >= width) {
                index_col = 0;
                index_row++;
            }
        } else {
            // Write a literal value (no match found)
            compressed_data[*compressed_size] = 0; // Length of literal (0)
            compressed_data[*compressed_size + 1] = image_data[index_row][index_col]; // Literal value
            *compressed_size += 2;

            // Move to the next pixel
            index_col++;
            if (index_col >= width) {
                index_col = 0;
                index_row++;
            }
        }
    }
}


int main() {
    uint32_t width = 10;  
    uint32_t height = 10; 

    uint8_t image_data[IMAGE_ROWS][IMAGE_COLS];   // Grayscale image data
    uint8_t compressed_data[MAX_COMPRESSED_SIZE]; // Buffer for compressed data
    uint32_t compressed_size;


    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            image_data[y][x] = (x + y) % 256; 
        }
    }


    uint32_t original_size = width * height; 
    printf("Original Image Size: %ux%u = %u bytes\n", width, height, original_size);

    // Compress 
    compress_deflate_image(image_data, width, height, compressed_data, &compressed_size);

    printf("Compressed Data Size: %u bytes\n", compressed_size);
    for (uint32_t i = 0; i < compressed_size; i++) {
        printf("%02X ", compressed_data[i]);
    }
    printf("\n");

    return 0;
}

