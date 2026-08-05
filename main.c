#define _CRT_SECURE_NO_WARNINGS  // Disable scanf warnings
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>  // For QueryPerformanceCounter

extern void imgCvtGrayFloatToInt(float* input, unsigned char* output, int height, int width);

// Function to generate random float pixels (0.0 to 1.0)
void generateRandomImage(float* image, int size) {
    for (int i = 0; i < size; i++) {
        image[i] = (float)rand() / RAND_MAX;
    }
}

// Measure execution time of assembly function
double measureExecutionTime(float* input, unsigned char* output, int height, int width) {
    LARGE_INTEGER frequency, start, end;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    // Call assembly function
    imgCvtGrayFloatToInt(input, output, height, width);

    QueryPerformanceCounter(&end);

    return (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
}

int main() {
    // Fix: Cast time() to unsigned int to avoid C4244 warning
    srand((unsigned int)time(NULL));

    int sizes[][2] = { {10, 10}, {100, 100}, {1000, 1000} };
    int numSizes = 3;
    int runs = 30;

    printf("=== Grayscale Image Converter (Float to Uint8) ===\n");
    printf("Using x86-64 Assembly with SIMD Scalar Instructions\n\n");

    // --- Test with the example from the spec ---
    printf("--- Correctness Check (Example from spec) ---\n");
    float testInput[12] = { 0.25, 0.35, 0.45, 0.33,
                           0.55, 0.65, 0.75, 0.33,
                           0.85, 0.95, 0.15, 0.33 };
    unsigned char testOutput[12];
    int testHeight = 3, testWidth = 4;

    imgCvtGrayFloatToInt(testInput, testOutput, testHeight, testWidth);

    printf("Input (3x4):\n");
    for (int i = 0; i < testHeight; i++) {
        for (int j = 0; j < testWidth; j++) {
            printf("%.2f ", testInput[i * testWidth + j]);
        }
        printf("\n");
    }
    printf("\nOutput (3x4):\n");
    for (int i = 0; i < testHeight; i++) {
        for (int j = 0; j < testWidth; j++) {
            printf("%d ", testOutput[i * testWidth + j]);
        }
        printf("\n");
    }
    printf("\nExpected:\n64 89 114 84\n140 166 191 84\n216 242 38 84\n\n");

    // --- Performance Testing ---
    printf("--- Performance Testing (30 runs each) ---\n");

    for (int s = 0; s < numSizes; s++) {
        int height = sizes[s][0];
        int width = sizes[s][1];
        int size = height * width;

        printf("\nTesting %dx%d...\n", height, width);

        float* input = (float*)malloc(size * sizeof(float));
        unsigned char* output = (unsigned char*)malloc(size * sizeof(unsigned char));

        if (input == NULL || output == NULL) {
            printf("Memory allocation failed!\n");
            return 1;
        }

        // Generate random input
        generateRandomImage(input, size);

        double totalTime = 0.0;

        for (int run = 0; run < runs; run++) {
            totalTime += measureExecutionTime(input, output, height, width);
        }

        double avgTime = totalTime / runs;
        printf("  Average time: %.9f seconds\n", avgTime);

        // Print first few outputs for correctness check
        printf("  First 10 output pixels: ");
        for (int i = 0; i < 10 && i < size; i++) {
            printf("%d ", output[i]);
        }
        printf("\n");

        free(input);
        free(output);
    }

    printf("\n=== Done ===\n");
    system("pause");
    return 0;
}
