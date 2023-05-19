#include <stdio.h>
#include <string.h>
#include <dirent.h> 
#include <string.h>
#include <stdlib.h>

#include "image.h"
#include "edge_detection_filter.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Incorrect number of arguments.\n");
        return 1;
    }

    char* input_image = argv[1];
    char* output_image = argv[2];

    printf("Edge detection to image %s.\n", input_image);

    int width, height, channels;
    stbi_uc *image = loadImage(input_image, &width, &height, &channels);
    if (image == NULL) {
        printf("Could not load image %s.\n", input_image);
        return 1;
    }

    // edge detection
    stbi_uc *filtered_image = edgeDetection(image, width, height, channels);

    writeImage(output_image, filtered_image, width, height, channels);
    printf("Edge Detection Image saved as %s\n", output_image);      
    return 0;
}