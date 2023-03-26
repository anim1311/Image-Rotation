#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<stdint.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <input_image> <output_image> <angle_to_rotate>\n", argv[0]);
        return 1;
    }
    
    char *input_file = argv[1];
    char *output_file = argv[2];
    float angle = atof(argv[3]);
    
    // Load input image
    int width, height, channels;
    uint8_t *image_data = stbi_load(input_file, &width, &height, &channels, 0);
    if (image_data == NULL) {
        printf("Error loading image: %s\n", input_file);
        return 1;
    }
    
    // Convert angle to radians
    float radians = angle * M_PI / 180.0f;
    
    // Compute sine and cosine of angle
    float s = sin(radians);
    float c = cos(radians);
    
    // Compute new image size and allocate memory for it
    int new_width = (int) round(abs(width * c) + abs(height * s));
    int new_height = (int) round(abs(width * s) + abs(height * c));
    int new_channels = channels;
    uint8_t *new_image_data = (uint8_t *) malloc(new_width * new_height * new_channels * sizeof(uint8_t));
    
    // Initialize new image with transparent black pixels
    for (int i = 0; i < new_width * new_height * new_channels; i += new_channels) {
        new_image_data[i] = 0;
        new_image_data[i+1] = 0;
        new_image_data[i+2] = 0;
        if (new_channels == 4) {
            new_image_data[i+3] = 0;
        }
    }
    
    // Compute center of old and new images
    float old_cx = (float) width / 2.0f;
    float old_cy = (float) height / 2.0f;
    float new_cx = (float) new_width / 2.0f;
    float new_cy = (float) new_height / 2.0f;
    
    // Iterate over new image pixels and map them back to old image pixels
    for (int y = 0; y < new_height; y++) {
        for (int x = 0; x < new_width; x++) {
            // Compute old image pixel coordinates corresponding to current new image pixel
            float old_x = (x - new_cx) * c + (y - new_cy) * s + old_cx;
            float old_y = -(x - new_cx) * s + (y - new_cy) * c + old_cy;
            
            // Check if old image pixel coordinates are within bounds
            if (old_x >= 0 && old_x < width && old_y >= 0 && old_y < height) {
                // Compute indices of old image pixel
                int old_index = ((int) old_x + (int) old_y * width) * channels;
                
                // Copy old image pixel to new image
                int new_index = (x + y * new_width) * new_channels;

                // Copy old image pixel to new image
                new_image_data[new_index] = image_data[old_index];
                new_image_data[new_index+1] = image_data[old_index+1];
                new_image_data[new_index+2] = image_data[old_index+2];
                if (new_channels == 4) {
                    new_image_data[new_index+3] = image_data[old_index+3];
                }
            }
        }
    }
    
    // Save output image
    int success = stbi_write_png(output_file, new_width, new_height, new_channels, new_image_data, new_width * new_channels);
    if (!success) {
        printf("Error saving image: %s\n", output_file);
        return 1;
    }
    
    // Free memory
    stbi_image_free(image_data);
    free(new_image_data);
    
    printf("Image rotated successfully!\n");
    
    return 0;
}