/*
   DONE BY : TEAM CE117s
*/
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;

const double PI = 3.14159265358979323846;

// Aggressive Frequency Pruning: Only keeps the very top-left corner of the DCT
void aggressivePrune(vector<float>& block, int keep_radius) {
    for (int u = 0; u < 8; ++u) {
        for (int v = 0; v < 8; ++v) {
            if (u + v > keep_radius) { 
                block[u * 8 + v] = 0; 
            }
        }
    }
}

// DCT and IDCT Logic
void applyDCT(vector<float>& block) {
    vector<float> result(64, 0.0f);
    for (int u = 0; u < 8; ++u) {
        for (int v = 0; v < 8; ++v) {
            float sum = 0.0f;
            for (int x = 0; x < 8; ++x) {
                for (int y = 0; y < 8; ++y) {
                    sum += (block[x * 8 + y] - 128) * cos((2 * x + 1) * u * PI / 16.0) * cos((2 * y + 1) * v * PI / 16.0);
                }
            }
            float cu = (u == 0) ? 1 / sqrt(2) : 1.0f;
            float cv = (v == 0) ? 1 / sqrt(2) : 1.0f;
            result[u * 8 + v] = 0.25f * cu * cv * sum;
        }
    }
    block = result;
}

void applyIDCT(vector<float>& block) {
    vector<float> result(64, 0.0f);
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            float sum = 0.0f;
            for (int u = 0; u < 8; ++u) {
                for (int v = 0; v < 8; ++v) {
                    float cu = (u == 0) ? 1 / sqrt(2) : 1.0f;
                    float cv = (v == 0) ? 1 / sqrt(2) : 1.0f;
                    sum += cu * cv * block[u * 8 + v] * cos((2 * x + 1) * u * PI / 16.0) * cos((2 * y + 1) * v * PI / 16.0);
                }
            }
            result[x * 8 + y] = max(0.0f, min(255.0f, (0.25f * sum) + 128));
        }
    }
    block = result;
}

int main() {
    int w, h, channels;
    unsigned char* img = stbi_load("input.jpg", &w, &h, &channels, 3);
    if (!img) return -1;

    vector<float> Y(w * h), Cb(w * h), Cr(w * h);

    // 1. RGB -> YCbCr
    for (int i = 0; i < w * h; ++i) {
        float r = img[i * 3], g = img[i * 3 + 1], b = img[i * 3 + 2];
        Y[i] = 0.299f * r + 0.587f * g + 0.114f * b;
        Cb[i] = 128 - 0.168736f * r - 0.331264f * g + 0.5f * b;
        Cr[i] = 128 + 0.5f * r - 0.418688f * g - 0.081312f * b;
    }

    // 2. CHROMA SUBSAMPLING (Averaging 2x2 pixels for color)
    // We effectively throw away 75% of Cb and Cr data
    for (int i = 0; i < h; i += 2) {
        for (int j = 0; j < w; j += 2) {
            float avgCb = (Cb[i*w+j] + Cb[i*w+j+1] + Cb[(i+1)*w+j] + Cb[(i+1)*w+j+1]) / 4.0f;
            float avgCr = (Cr[i*w+j] + Cr[i*w+j+1] + Cr[(i+1)*w+j] + Cr[(i+1)*w+j+1]) / 4.0f;
            Cb[i*w+j] = Cb[i*w+j+1] = Cb[(i+1)*w+j] = Cb[(i+1)*w+j+1] = avgCb;
            Cr[i*w+j] = Cr[i*w+j+1] = Cr[(i+1)*w+j] = Cr[(i+1)*w+j+1] = avgCr;
        }
    }

    // 3. Process Y blocks with Aggressive DCT Pruning
    for (int i = 0; i <= h - 8; i += 8) {
        for (int j = 0; j <= w - 8; j += 8) {
            vector<float> y_block(64);
            for (int x = 0; x < 8; ++x)
                for (int y = 0; y < 8; ++y)
                    y_block[x * 8 + y] = Y[(i + x) * w + (j + y)];

            applyDCT(y_block);
            aggressivePrune(y_block, 2); // ONLY KEEP TOP 3 LOW FREQUENCIES
            applyIDCT(y_block);

            for (int x = 0; x < 8; ++x)
                for (int y = 0; y < 8; ++y)
                    Y[(i + x) * w + (j + y)] = y_block[x * 8 + y];
        }
    }

    // 4. Convert back and save with LOW QUALITY flag
    vector<unsigned char> output(w * h * 3);
    for (int i = 0; i < w * h; ++i) {
        float y = Y[i], cb = Cb[i], cr = Cr[i];
        output[i * 3] = (unsigned char)max(0.0f, min(255.0f, y + 1.402f * (cr - 128)));
        output[i * 3 + 1] = 0.0f;
        output[i * 3 + 2] = 0.0f;
    }

    // Changing '90' to '30' here forces the library to compress harder
    stbi_write_jpg("output_50pct.jpg", w, h, 3, output.data(), 30);
    stbi_image_free(img);
    cout << "High Compression Finished! Check output_50pct.jpg" << endl;
    return 0;
}