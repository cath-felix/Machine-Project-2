; imgCvtGrayFloatToInt.asm
section .data

section .text
bits 64
default rel

global imgCvtGrayFloatToInt

; void imgCvtGrayFloatToInt(float* input, unsigned char* output, int height, int width)
; RCX = input pointer
; RDX = output pointer
; R8  = height
; R9  = width

imgCvtGrayFloatToInt:
    push rbp
    mov rbp, rsp
    
    ; Save non-volatile registers
    push rbx
    push r12
    push r13
    
    ; Calculate total pixels = height * width
    mov rax, r8          ; height
    imul rax, r9         ; height * width
    mov r10, rax         ; r10 = total pixels
    
    ; Load constant 255.0 into XMM0
    movss xmm0, dword [float255]  ; Load 255.0
    
    ; Initialize loop counter
    xor r11, r11         ; i = 0
    
convert_loop:
    cmp r11, r10
    je done
    
    ; Load float pixel from input (RCX + i*4)
    movss xmm1, dword [rcx + r11*4]  ; load float
    
    ; Convert: int = float * 255.0
    mulss xmm1, xmm0                 ; xmm1 = float * 255.0
    
    ; Convert float to integer (truncate)
    cvttss2si eax, xmm1              ; eax = (int)(float * 255.0)
    
    ; Store result to output (RDX + i)
    mov byte [rdx + r11], al         ; store uint8
    
    inc r11
    jmp convert_loop
    
done:
    pop r13
    pop r12
    pop rbx
    mov rsp, rbp
    pop rbp
    ret

section .data
align 4
float255:
    dd 255.0
