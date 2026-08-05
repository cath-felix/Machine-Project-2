; imgCvtGrayFloatToInt.asm
; Converts grayscale image from float (0.0-1.0) to uint8 (0-255)
; Uses scalar SIMD instructions with truncation

section .data
    align 16
    scale dd 255.0

section .text
    bits 64
    default rel

    global imgCvtGrayFloatToInt

; void imgCvtGrayFloatToInt(float* floatImage, unsigned char* intImage, int height, int width)
; Parameters (Windows x64 calling convention):
;   RCX: floatImage pointer (1st parameter)
;   RDX: intImage pointer (2nd parameter)
;   R8:  height (3rd parameter)
;   R9:  width (4th parameter)
; Return: void

imgCvtGrayFloatToInt:
    ; Prologue - save non-volatile registers
    push rbp
    mov rbp, rsp
    push rbx
    push r12
    push r13
    push r14
    push r15
    
    ; Save parameters in non-volatile registers
    mov r12, rcx        ; floatImage pointer
    mov r13, rdx        ; intImage pointer
    mov r14, r8         ; height
    mov r15, r9         ; width
    
    ; Calculate total pixels = height * width
    mov rax, r14        ; height
    imul rax, r15       ; height * width
    mov rcx, rax        ; total pixels in RCX
    
    ; Check if we have any pixels
    cmp rcx, 0
    je .done
    
    ; Load constant (255.0) into XMM2
    movss xmm2, [scale]     ; xmm2 = 255.0
    
    ; Initialize counter
    xor rbx, rbx        ; rbx = 0 (pixel counter)
    
.loop:
    ; Load float pixel value from memory into XMM0
    movss xmm0, [r12 + rbx*4]   ; xmm0 = floatImage[rbx]
    
    ; Multiply by 255.0
    mulss xmm0, xmm2            ; xmm0 = floatImage[rbx] * 255.0
    
    ; Round toward zero (truncate) using roundss
    ; Immediate operand 3 = round toward zero
    roundss xmm0, xmm0, 3
    
    ; Convert to integer
    cvtss2si rax, xmm0          ; rax = (int)(floatImage[rbx] * 255.0)
    
    ; Clamp to [0, 255]
    cmp rax, 255
    jle .check_min
    mov rax, 255                ; clamp to 255
    jmp .store
    
.check_min:
    cmp rax, 0
    jge .store
    xor rax, rax                ; clamp to 0
    
.store:
    ; Store the byte (unsigned char)
    mov byte [r13 + rbx], al    ; intImage[rbx] = (unsigned char)rax
    
    ; Increment counter
    inc rbx
    cmp rbx, rcx
    jl .loop
    
.done:
    ; Epilogue - restore non-volatile registers
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    pop rbp
    ret
