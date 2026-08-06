#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

// External assembly function
extern void imgCvtGrayFloatToInt(float* floatImage, unsigned char* intImage, int height, int width);

// Function to verify correctness
int verifyConversion(float* floatImage, unsigned char* intImage, int height, int width) {
    int errors = 0;
    int maxErrors = 10;
    int total = height * width;

    for (int i = 0; i < total; i++) {
        // Use truncation (no rounding) - matches assembly
        unsigned char expected = (unsigned char)(floatImage[i] * 255.0f);
        if (intImage[i] != expected) {
            if (errors < maxErrors) {
                printf("Error at index %d: float=%.4f, expected=%d, got=%d\n",
                    i, floatImage[i], expected, intImage[i]);
            }
            errors++;
        }
    }

    if (errors == 0) {
        printf("All pixels converted correctly!\n");
    }
    else {
        printf("%d errors found (showing first %d)\n", errors, maxErrors);
    }

    return errors;
}

// Function to print float image
void printFloatImage(float* image, int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%.2f ", image[i * width + j]);
        }
        printf("\n");
    }
}

// Function to print integer image
void printIntImage(unsigned char* image, int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0) {
                printf("%d", image[i * width + j]);
            }
            else {
                printf(", %d", image[i * width + j]);
            }
        }
        printf("\n");
    }
}

//to simulate the tests (provides random float image and providees run time)
void generateRandomFloatImage(float* image, int height, int width) {
    int total = height * width;
    for (int i = 0; i < total; i++) {
        image[i] = (float)rand() / (float)RAND_MAX;
    }
}

void runTimingTest(int height, int width, int numRuns) {
    int totalPixels = height * width;
    float* floatImage = (float*)malloc(totalPixels * sizeof(float));
    unsigned char* intImage = (unsigned char*)malloc(totalPixels * sizeof(unsigned char));

    generateRandomFloatImage(floatImage, height, width);

    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);

    double totalTime = 0.0;
    for (int run = 0; run < numRuns; run++) {
        QueryPerformanceCounter(&start);
        imgCvtGrayFloatToInt(floatImage, intImage, height, width);
        QueryPerformanceCounter(&end);
        totalTime += (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
    }

    printf("Size %dx%d: average time = %.6f sec over %d runs\n",
        height, width, totalTime / numRuns, numRuns);

    free(floatImage);
    free(intImage);
}
//end of simulation code

int main() {
    int height, width;

    // Read height and width
    printf("Enter height and width: ");
    scanf("%d %d", &height, &width);

    int totalPixels = height * width;

    // Allocate memory for float image
    float* floatImage = (float*)malloc(totalPixels * sizeof(float));
    if (floatImage == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // Read float pixel values
    printf("Enter %d float pixel values (0.0 to 1.0):\n", totalPixels);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            scanf("%f", &floatImage[i * width + j]);
        }
    }

    // Allocate memory for integer image
    unsigned char* intImage = (unsigned char*)malloc(totalPixels * sizeof(unsigned char));
    if (intImage == NULL) {
        printf("Memory allocation failed!\n");
        free(floatImage);
        return 1;
    }

    // Print input
    printf("\nInput (Float):\n");
    printFloatImage(floatImage, height, width);

    // Call assembly function to convert
    imgCvtGrayFloatToInt(floatImage, intImage, height, width);

    // Print output
    printf("\nOutput (Uint8):\n");
    printIntImage(intImage, height, width);

    // Verify correctness
    printf("\nVerification:\n");
    verifyConversion(floatImage, intImage, height, width);
    
    // Performace Check
    printf("\nPerformance Timing:\n");
    srand((unsigned int)time(NULL));
    runTimingTest(10, 10, 30);
    runTimingTest(100, 100, 30);
    runTimingTest(1000, 1000, 30);

    // Free memory
    free(floatImage);
    free(intImage);

    return 0;
}
