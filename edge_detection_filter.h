
#include "image.h"

int edge_mask_x_3[] = {-1, 0, 1,
                -2, 0, 2,
                -1, 0, 1};
int edge_mask_y_3[] = {1,  2,  1,
                0,  0,  0,
               -1, -2, -1};

int edge_mask_dimension_3 = 3;


const int MAX_THREADS = 1024; 

__device__ bool isOutOfBounds(int x, int y, int image_width, int image_height) {
    if (x < 0 || y < 0 || x >= image_width || y >= image_height) {
        return true;
    } else {
        return false;
    }
}

__global__ void convolve(const stbi_uc* input_image, stbi_uc* output_image, const int width, const int height, const int* mask, const int mask_dimension) {
    const int x_coordinate = blockIdx.x * blockDim.x + threadIdx.x;
    const int y_coordinate = blockIdx.y * blockDim.y + threadIdx.y;
    
    int half_mask_size = mask_dimension / 2;

    Pixel current_pixel;
    int red = 0;
    int blue = 0;
    int green = 0;
    // int alpha = 0;
    for (int i = 0; i < mask_dimension; i++) {
        for (int j = 0; j < mask_dimension; j++) {
            int current_x_global = x_coordinate - half_mask_size + i;
            int current_y_global = y_coordinate - half_mask_size + j;
            if (isOutOfBounds(current_x_global, current_y_global, width, height)) {
                continue;
            }
            getPixel(input_image, width, current_x_global, current_y_global, &current_pixel);
            int mask_element = mask[i * mask_dimension + j];

            red += current_pixel.r * mask_element;
            green += current_pixel.g * mask_element;
            blue += current_pixel.b * mask_element;
        }
    }

    Pixel pixel;
    if (red < 0) {
        pixel.r = 0;
    } else if (red > 255) {
        pixel.r = 255;
    } else {
        pixel.r = red;
    }
    if (green < 0) {
        pixel.g = 0;
    } else if (green > 255) {
        pixel.g = 255;
    } else {
        pixel.g = green;
    }
    if (blue < 0) {
        pixel.b = 0;
    } else if (blue > 255) {
        pixel.b = 255;
    } else {
        pixel.b = blue;
    }

    setPixel(output_image, width, x_coordinate, y_coordinate, &pixel);
}


stbi_uc* convolve(const stbi_uc* input_image, int width, int height, int channels, const int* mask, int mask_dimension) {
    int* d_mask;

    cudaMallocManaged(&d_mask, mask_dimension * mask_dimension * sizeof(int));
    cudaMemcpy(d_mask, mask, mask_dimension * mask_dimension * sizeof(int), cudaMemcpyHostToDevice);
    
    int image_size = channels * width * height * sizeof(stbi_uc);
    stbi_uc* d_input_image;
    stbi_uc* d_output_image;
    stbi_uc* h_output_image = (stbi_uc*) malloc(image_size);

    for (int i = 0; i < width * height * channels; i++) {
        h_output_image[i] = input_image[i];
    }

    cudaMallocManaged(&d_input_image, image_size);
    cudaMallocManaged(&d_output_image, image_size);
    cudaMemcpy(d_input_image, input_image, image_size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_output_image, input_image, image_size, cudaMemcpyHostToDevice);

    int total_threads = width * height;
    int threads = min(MAX_THREADS, total_threads);

    int square_root = (int) sqrt(threads);
    dim3 block(square_root, square_root);
    dim3 grid(((width + block.x - 1) / block.x),((width + block.y - 1) / block.y));

    convolve<<<grid, block>>>(d_input_image, d_output_image, width, height, d_mask, mask_dimension);
    
    cudaMemcpy(h_output_image, d_output_image, image_size, cudaMemcpyDeviceToHost);
    return h_output_image;
}


stbi_uc* edgeDetection(stbi_uc* input_image, int width, int height, int channels) {
    stbi_uc* output = convolve(input_image, width, height, channels, edge_mask_x_3, edge_mask_dimension_3);
    output = convolve(output, width, height, channels, edge_mask_y_3, edge_mask_dimension_3);

    return output;
}

