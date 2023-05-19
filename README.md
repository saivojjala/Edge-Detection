# Edge-Detection
This repository contains the code for Edge Detection in Images using CUDA. 

To find the edges we apply the Sobel Operator. The Sobel Operator calculates the gradient magnitude and direction at each pixel using a two 3x3 matrices, one for detecting vertical edges and the other for horizontal edges. By applying the Sobel operator, the image is transformed into a representation where edges are emphasized, and other areas are suppressed. 

To use the code run the following commands:
```sh
$ git clone https://github.com/saivojjala/Edge-Detection.git
$ cd Edge\ Detection
$ nvcc main.cu -o edgeDetection
$ ./edgeDetection images/messi.png images/op_messi.png
```
##### Input Image:
![messi](https://github.com/saivojjala/Edge-Detection/assets/75254081/edc1eed4-8405-411f-86e7-5d5d8b1efec2)
##### Output Image: 
![op_messi](https://github.com/saivojjala/Edge-Detection/assets/75254081/6c08ddc0-d0c4-4d01-b087-2bcd1287fa55)
