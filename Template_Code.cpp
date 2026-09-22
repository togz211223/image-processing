#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <string>

using namespace std;

// Class to represent an image as a 3D matrix
class Image {
private:
    int width, height, maxVal, channels;
    vector<vector<vector<int>>> data; // [height][width][channel]

public:
    // Default constructor
    Image() {
        width = 0;
        height = 0;
        maxVal = 255;
        channels = 3;
    }

    // Create blank image
    Image(int w, int h, int ch = 3) {
        width = w;
        height = h;
        maxVal = 255;
        channels = ch;
        data.resize(height, vector<vector<int>>(width, vector<int>(channels, 0)));
    }

    // Get image dimensions
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getChannels() const { return channels; }

    // Set number of channels
    void setChannels(int ch) {
        channels = ch;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                data[y][x].resize(channels, 0);
            }
        }
    }

    // Pixel access
    int& operator()(int y, int x, int channel) {
        return data[y][x][channel];
    }

    const int& operator()(int y, int x, int channel) const {
        return data[y][x][channel];
    }

    // Load PPM image (P3 format)
    bool loadPPM(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Could not open file " << filename << endl;
            return false;
        }

        string format;
        file >> format;
        if (format != "P3") {
            cerr << "Error: Only P3 PPM format is supported" << endl;
            return false;
        }

        file >> width >> height >> maxVal;
        channels = 3;
        data.resize(height, vector<vector<int>>(width, vector<int>(channels, 0)));

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                for (int c = 0; c < channels; c++) {
                    file >> data[y][x][c];
                }
            }
        }

        file.close();
        return true;
    }

    // Save PPM image (P3 format)
    bool savePPM(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Could not create file " << filename << endl;
            return false;
        }

        file << "P3\n" << width << " " << height << "\n" << maxVal << "\n";

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (channels == 1) {
                    // For grayscale images, write the same value for all three channels
                    int gray = data[y][x][0];
                    file << gray << " " << gray << " " << gray << " ";
                } else {
                    // For color images, write all three channels
                    for (int c = 0; c < 3; c++) {
                        file << data[y][x][c] << " ";
                    }
                }
            }
            file << "\n";
        }

        file.close();
        return true;
    }

    // Print image data to console (for small images)
    void print() const {
        cout << "Image " << width << "x" << height << " (" << channels << " channels):\n";
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                cout << "(";
                for (int c = 0; c < channels; c++) {
                    cout << data[y][x][c];
                    if (c < channels - 1) cout << ",";
                }
                cout << ") ";
            }
            cout << endl;
        }
    }
};

/**
 * Converts a color image to grayscale
 *
 * Steps:
 * 1. Create a new single-channel image with the same width and height
 * 2. For each pixel in the input image:
 *    - Get the R, G, and B values
 *    - Calculate the grayscale value using the formula:
 *        gray = 0.299 * R + 0.587 * G + 0.114 * B
 *    - Set the grayscale value in the output image
 * 3. Return the grayscale image
 */
Image convertToGrayscale(const Image& input) {
    int height = input.getHeight();
    int width = input.getWidth();
    Image output(width, height, 1); // Single channel for grayscale
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int R = input(y, x, 0);
            int G = input(y, x, 1);
            int B = input(y, x, 2);
            int gray = static_cast<int>(0.299 * R + 0.587 * G + 0.114 * B);
            output(y, x, 0) = gray;
        }
    }
    return output;
}

/**
 * Flips image horizontally (left to right)
 *
 * Steps:
 * 1. Create a new image with the same dimensions as the input
 * 2. For each pixel in the input image:
 *    - Copy the pixel from position (y, x) in the input
 *    - To position (y, width - 1 - x) in the output
 * 3. Return the flipped image
 */
Image flipHorizontal(const Image& input) {
    int height = input.getHeight();
    int width = input.getWidth();
    int channels = input.getChannels();
    Image output(width, height, channels);

    // TODO: Implement this function
    // For each pixel and each channel:
    //   output(y, width-1-x, c) = input(y, x, c)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int c = 0; c < channels; c++) {
                output(y, width - 1 - x, c) = input(y, x, c);
            }
        }
    }

    return output;
}

/**
 * Flips image vertically (top to bottom)
 *
 * Steps:
 * 1. Create a new image with the same dimensions as the input
 * 2. For each pixel in the input image:
 *    - Copy the pixel from position (y, x) in the input
 *    - To position (height - 1 - y, x) in the output
 * 3. Return the flipped image
 */
Image flipVertical(const Image& input) {
    int height = input.getHeight();
    int width = input.getWidth();
    int channels = input.getChannels();
    Image output(width, height, channels);

    // TODO: Implement this function
    // For each pixel and each channel:
    //   output(height-1-y, x, c) = input(y, x, c)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int c = 0; c < channels; c++) {
                output(height - 1 - y, x, c) = input(y, x, c);
            }
        }
    }

    return output;
}

/**
 * Adjusts image brightness
 *
 * Steps:
 * 1. Create a new image with the same dimensions as the input
 * 2. For each pixel and each color channel:
 *    - Add the brightness value to the pixel value
 *    - Clamp the result between 0 and 255
 * 3. Return the adjusted image
 */
Image adjustBrightness(const Image& input, int value) {
    int height = input.getHeight();
    int width = input.getWidth();
    int channels = input.getChannels();
    Image output(width, height, channels);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int c = 0; c < channels; c++) {
                output(y, x, c) = clamp(input(y, x, c) + value, 0, 255);
            }
        }
    }

    return output;
}

/**
 * Adjusts image contrast
 *
 * Steps:
 * 1. Create a new image with the same dimensions as the input
 * 2. For each pixel and each color channel:
 *    - Subtract 128 from the pixel value to center around 0
 *    - Multiply by the contrast factor
 *    - Add 128 to center back around 128
 *    - Clamp the result between 0 and 255
 * 3. Return the adjusted image
 */
Image adjustContrast(const Image& input, float factor) {
    int height = input.getHeight();
    int width = input.getWidth();
    int channels = input.getChannels();
    Image output(width, height, channels);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int c = 0; c < channels; c++) {
                float adjustedValue = factor * (input(y, x, c) - 128) + 128;
                output(y, x, c) = clamp(static_cast<int>(adjustedValue), 0, 255);
            }
        }
    }

    return output;
}

/**
 * Applies a simple blur filter
 *
 * Steps:
 * 1. Create a new image with the same dimensions as the input
 * 2. For each pixel (excluding borders):
 *    - For each color channel:
 *        - Calculate the average of the 3x3 neighborhood
 *        - Set the output pixel to this average value
 * 3. Return the blurred image
 */
Image applyBlur(const Image& input) {
    int height = input.getHeight();
    int width = input.getWidth();
    int channels = input.getChannels();
    Image output(width, height, channels);

    // TODO: Implement this function
    // For each pixel (from y=1 to height-2, x=1 to width-2) and each channel:
    //   sum = 0
    //   For each neighbor (ky from -1 to 1, kx from -1 to 1):
    //     sum += input(y+ky, x+kx, c)
    //   output(y, x, c) = sum / 9
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            for (int c = 0; c < channels; c++) {

                int sum = 0;

                for (int ky = -1; ky <= 1; ky++) {
                    for (int kx = -1; kx <= 1; kx++) {
                        sum += input(y + ky, x + kx, c);
                    }
                }

                output(y, x, c) = sum / 9;
            }
        }
    }

    return output;
}

/**
 * Rotates image 90 degrees clockwise
 *
 * Steps:
 * 1. Create a new image with swapped dimensions (height becomes width, width becomes height)
 * 2. For each pixel in the input image:
 *    - Copy the pixel from position (y, x) in the input
 *    - To position (x, height - 1 - y) in the output
 * 3. Return the rotated image
 */
Image rotate90(const Image& input) {
    int height = input.getHeight();
    int width = input.getWidth();
    int channels = input.getChannels();
    Image output(height, width, channels); // Width and height are swapped

    // TODO: Implement this function
    // For each pixel and each channel:
    //   output(x, height-1-y, c) = input(y, x, c)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int c = 0; c < channels; c++) {
                output(x, height - 1 - y, c) = input(y, x, c);
            }
        }
    }

    return output;
}

bool comparePixel(const Image& img, int y, int x, int r, int g, int b) {
    if (img.getChannels() == 1) {
        return img(y, x, 0) == r;
    }
    return img(y, x, 0) == r && img(y, x, 1) == g && img(y, x, 2) == b;
}

void runQATests(const Image& original) {
    int passed = 0;
    int total = 6;

    cout << "\n================================================\n";
    cout << "   RUNNING AUTOMATED QA TESTS (SRS SECTION 6)   \n";
    cout << "================================================\n\n";

    Image gray = convertToGrayscale(original);
    if (gray.getChannels() == 1 && gray(0, 0, 0) == 76) {
        cout << "[PASS] Grayscale: Red(255,0,0) converted exactly to 76.\n";
        passed++;
    } else {
        cout << "[FAIL] Grayscale: Expected 76 at (0,0), got " << gray(0,0,0) << ".\n";
    }

    Image flippedH = flipHorizontal(original);
    if (comparePixel(flippedH, 0, 0, 255, 255, 255) &&
        comparePixel(flippedH, 0, 3, 255, 0, 0)) {
        cout << "[PASS] Horizontal Flip: Row 0 correctly mirrored.\n";
        passed++;
    } else {
        cout << "[FAIL] Horizontal Flip: Pixels did not map correctly.\n";
    }

    Image flippedV = flipVertical(original);
    if (comparePixel(flippedV, 0, 0, 128, 255, 128)) {
        cout << "[PASS] Vertical Flip: Row 0 correctly matches original Row 3.\n";
        passed++;
    } else {
        cout << "[FAIL] Vertical Flip: Pixels did not map correctly.\n";
    }

    Image bright = adjustBrightness(original, 50);
    if (comparePixel(bright, 0, 0, 255, 50, 50) &&
        comparePixel(bright, 1, 3, 178, 178, 178)) {
        cout << "[PASS] Brightness +50: Values shifted and safely clamped to [0,255].\n";
        passed++;
    } else {
        cout << "[FAIL] Brightness +50: Math or clamping logic is incorrect.\n";
    }

    Image contrast = adjustContrast(original, 1.5f);
    if (comparePixel(contrast, 0, 0, 255, 0, 0)) {
        cout << "[PASS] Contrast x1.5: Float math and clamping strictly followed.\n";
        passed++;
    } else {
        cout << "[FAIL] Contrast x1.5: Expected (255,0,0) after clamping.\n";
    }

    Image rotated = rotate90(original);
    if (comparePixel(rotated, 0, 3, 255, 0, 0) &&
        comparePixel(rotated, 3, 3, 255, 255, 255)) {
        cout << "[PASS] Rotate 90: Dimensions swapped and pixels mapped accurately.\n";
        passed++;
    } else {
        cout << "[FAIL] Rotate 90: Pixel mapping failed.\n";
    }

    cout << "\n================================================\n";
    cout << "   TEST SUMMARY: " << passed << " / " << total << " PASSED\n";
    cout << "================================================\n\n";
}

void createTestImage(const string& filename) {
    Image img(4, 4);

    img(0, 0, 0) = 255; img(0, 0, 1) = 0;   img(0, 0, 2) = 0;
    img(0, 1, 0) = 0;   img(0, 1, 1) = 255; img(0, 1, 2) = 0;
    img(0, 2, 0) = 0;   img(0, 2, 1) = 0;   img(0, 2, 2) = 255;
    img(0, 3, 0) = 255; img(0, 3, 1) = 255; img(0, 3, 2) = 255;

    img(1, 0, 0) = 255; img(1, 0, 1) = 255; img(1, 0, 2) = 0;
    img(1, 1, 0) = 255; img(1, 1, 1) = 0;   img(1, 1, 2) = 255;
    img(1, 2, 0) = 0;   img(1, 2, 1) = 255; img(1, 2, 2) = 255;
    img(1, 3, 0) = 128; img(1, 3, 1) = 128; img(1, 3, 2) = 128;

    img(2, 0, 0) = 255; img(2, 0, 1) = 128; img(2, 0, 2) = 0;
    img(2, 1, 0) = 128; img(2, 1, 1) = 255; img(2, 1, 2) = 0;
    img(2, 2, 0) = 128; img(2, 2, 1) = 0;   img(2, 2, 2) = 255;
    img(2, 3, 0) = 255; img(2, 3, 1) = 128; img(2, 3, 2) = 128;

    img(3, 0, 0) = 128; img(3, 0, 1) = 255; img(3, 0, 2) = 128;
    img(3, 1, 0) = 128; img(3, 1, 1) = 128; img(3, 1, 2) = 255;
    img(3, 2, 0) = 255; img(3, 2, 1) = 255; img(3, 2, 2) = 128;
    img(3, 3, 0) = 0;   img(3, 3, 1) = 0;   img(3, 3, 2) = 0;

    img.savePPM(filename);
}

int main() {
    createTestImage("test_image.ppm");

    Image input;
    if (!input.loadPPM("test_image.ppm")) {
        cerr << "Failed to load image. Exiting.\n";
        return 1;
    }

    runQATests(input);

    return 0;
}
