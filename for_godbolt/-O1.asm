compute_mandelbrot_avx512:
        mov     r8d, esi
        vmovsd  xmm2, QWORD PTR 24[rdi]
        vmulsd  xmm0, xmm2, QWORD PTR .LC0[rip]
        vbroadcastsd    zmm14, xmm0
        vmulsd  xmm10, xmm0, QWORD PTR .LC1[rip]
        vbroadcastsd    zmm10, xmm10
        vmulsd  xmm2, xmm2, QWORD PTR .LC2[rip]
        vmovsd  xmm13, QWORD PTR 16[rdi]
        vsubsd  xmm13, xmm13, xmm2
        vbroadcastsd    zmm13, xmm13
        vmovsd  xmm1, QWORD PTR 8[rdi]
        vsubsd  xmm2, xmm1, xmm2
        vaddsd  xmm4, xmm0, xmm0
        vmulsd  xmm5, xmm0, QWORD PTR .LC3[rip]
        vmulsd  xmm3, xmm0, QWORD PTR .LC4[rip]
        vmulsd  xmm6, xmm0, QWORD PTR .LC5[rip]
        vmulsd  xmm1, xmm0, QWORD PTR .LC6[rip]
        vmulsd  xmm7, xmm0, QWORD PTR .LC7[rip]
        vunpcklpd       xmm1, xmm1, xmm7
        vunpcklpd       xmm3, xmm3, xmm6
        vinsertf128     ymm3, ymm3, xmm1, 0x1
        vunpcklpd       xmm4, xmm4, xmm5
        vxorpd  xmm1, xmm1, xmm1
        vunpcklpd       xmm0, xmm1, xmm0
        vinsertf128     ymm0, ymm0, xmm4, 0x1
        vinsertf64x4    zmm0, zmm0, ymm3, 0x1
        vbroadcastsd    zmm1, xmm2
        vaddpd  zmm12, zmm0, zmm1
        mov     r9d, 0
        vbroadcastsd    zmm7, QWORD PTR .LC10[rip]
        mov     eax, 1
        vpbroadcastq    zmm9, rax
        test    esi, esi
        jne     .L2
.L3:
        mov     eax, 0
        ret
.L4:
        mov     rax, QWORD PTR [rdi]
        vmovdqa64       ZMMWORD PTR [rax+rcx], zmm3
        vaddpd  zmm6, zmm6, zmm10
        add     rcx, 64
        cmp     rcx, rsi
        je      .L6
.L8:
        vmovapd zmm2, zmm6
        vmovapd zmm0, zmm11
        mov     edx, 128
        vpxor   xmm3, xmm3, xmm3
.L5:
        vmulpd  zmm1, zmm2, zmm2
        vmulpd  zmm4, zmm0, zmm0
        vaddpd  zmm5, zmm1, zmm4
        vcmppd  k1, zmm5, zmm7, 1
        kmovw   eax, k1
        movzx   eax, al
        kmovw   k0, eax
        kortestw        k0, k0
        je      .L4
        vpaddq  zmm3{k1}, zmm3, zmm9
        vmulpd  zmm0, zmm0, zmm2
        vsubpd  zmm1, zmm1, zmm4
        vaddpd  zmm2, zmm1, zmm6
        vaddpd  zmm0, zmm0, zmm0
        vaddpd  zmm0, zmm0, zmm8
        sub     edx, 1
        jne     .L5
        jmp     .L4
.L6:
        vaddpd  zmm8, zmm8, zmm14
        add     rsi, 8192
        cmp     rsi, 8396800
        je      .L7
.L9:
        lea     rcx, -8192[rsi]
        vmovapd zmm6, zmm12
        vmovapd zmm11, zmm8
        jmp     .L8
.L7:
        add     r9d, 1
        cmp     r8d, r9d
        je      .L3
.L2:
        vmovapd zmm8, zmm13
        mov     esi, 8192
        jmp     .L9
.LC0:
        .long   0
        .long   1062207488
.LC1:
        .long   0
        .long   1075838976
.LC2:
        .long   0
        .long   1071644672
.LC3:
        .long   0
        .long   1074790400
.LC4:
        .long   0
        .long   1074266112
.LC5:
        .long   0
        .long   1075052544
.LC6:
        .long   0
        .long   1075314688
.LC7:
        .long   0
        .long   1075576832
.LC10:
        .long   0
        .long   1079574528
