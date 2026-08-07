# imgCvtGrayFloatToInt
Converts a grayscale image's pixel representation from single-precision float (0.0–1.0) to unsigned 8-bit integer (0–255), implemented in C and x86-64 assembly using scalar SIMD instructions.

## Files
- `main.c` — reads input, allocates memory, calls the assembly function, prints output, runs correctness check and timing tests
- `imgCvtGrayFloatToInt.asm` — the conversion function itself 

## Build & Run
nasm -f win64 imgCvtGrayFloatToInt.asm -o imgCvtGrayFloatToInt.obj

gcc main.c imgCvtGrayFloatToInt.obj -o main.exe

./main.exe

## How It Works
`imgCvtGrayFloatToInt(float* floatImage, unsigned char* intImage, int height, int width)` uses the Windows x64 calling convention (RCX/RDX/R8/R9 for the four parameters). For each pixel: load the float with `MOVSS`, scale by 255 with `MULSS`, truncate toward zero with `ROUNDSS`, convert to integer with `CVTSS2SI`, clamp to [0, 255], and store as a byte.

## Sample Outputs
Note:
* Verification is to confirm that the output in SASM matches the C output
* Performance Timing is to analyze the performance for 30 runs in different dimensions
  
### Sample Output 1 - Single Pixel
<img width="560" height="320" alt="4ca42fd2-3a5c-4ca2-8af1-d439f3269d53" src="https://github.com/user-attachments/assets/9d47cc6b-efc1-4b17-90ef-ed1a22402dd3" />

### Sample Output 2 - Random Numbers
<img width="674" height="430" alt="11746f9b-a08a-44d0-9480-8c34e7ea5766" src="https://github.com/user-attachments/assets/d9baf194-fe35-4df9-8b24-64adfb297c55" />

### Sample Output 3 - 'Black' Output (All Zeroes)
<img width="620" height="484" alt="a7184275-d485-470d-9510-715b1a15cc17" src="https://github.com/user-attachments/assets/f4815a23-7b48-4965-9274-a8bee36069f7" />

### Sample Output 4 - 'White' Output (All Ones)
<img width="570" height="372" alt="59c28acd-8002-49b7-bbe0-98af218c9950" src="https://github.com/user-attachments/assets/1f7a2bc4-95f9-4d2b-a0d9-def1aaa6decc" />

### Sample Output 5 - Out of Bounds Input
<img width="584" height="428" alt="a46b9f36-c041-437b-a8b2-b8cb802f213c" src="https://github.com/user-attachments/assets/8e7db8f7-bc56-4733-9769-768590c8cc30" />


### Sample Output 6 - 2x2 Grid: Vertical Input "Return/Enter" (Similar to Demo Video)
<img width="550" height="406" alt="c197153e-5cb7-4bd8-ad23-fcee76cf0d43" src="https://github.com/user-attachments/assets/15c370d1-fe3d-4a87-8d39-77dfb9b7afb9" />

### Sample Output 7 - 2x2 Grid: Horizontal Input "Spaces" (Similar to Demo Video)
<img width="550" height="354" alt="aea28c7f-04de-443f-abfe-1cb9753cfd57" src="https://github.com/user-attachments/assets/690a9e13-73ae-463c-acdb-e9ba2d65754c" />


## Demo Video
https://drive.google.com/file/d/12VVaNrkrDItgvctNPJd3gLRkl5MpZfmN/view?usp=sharing
