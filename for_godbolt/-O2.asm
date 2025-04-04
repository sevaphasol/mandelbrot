compute_mandelbrot_avx512:
        vmovsd  xmm1, QWORD PTR 24[rdi]
        mov     eax, 1
        mov     r8d, esi
        xor     r9d, r9d
        vmulsd  xmm0, xmm1, QWORD PTR .LC0[rip]
        vpbroadcastq    zmm9, rax
        vmovsd  xmm5, QWORD PTR .LC2[rip]
        vmulsd  xmm8, xmm0, QWORD PTR .LC7[rip]
        vmovsd  xmm12, xmm1, xmm1
        vmulsd  xmm3, xmm0, QWORD PTR .LC4[rip]
        vfnmadd213sd    xmm1, xmm5, QWORD PTR 8[rdi]
        vmulsd  xmm7, xmm0, QWORD PTR .LC5[rip]
        vfnmadd213sd    xmm12, xmm5, QWORD PTR 16[rdi]
        vmulsd  xmm2, xmm0, QWORD PTR .LC6[rip]
        vmulsd  xmm6, xmm0, QWORD PTR .LC3[rip]
        vaddsd  xmm4, xmm0, xmm0
        vbroadcastsd    zmm13, xmm0
        vmulsd  xmm10, xmm0, QWORD PTR .LC1[rip]
        vbroadcastsd    zmm11, xmm1
        vunpcklpd       xmm3, xmm3, xmm7
        vbroadcastsd    zmm12, xmm12
        vunpcklpd       xmm2, xmm2, xmm8
        vbroadcastsd    zmm8, QWORD PTR .LC10[rip]
        vinsertf128     ymm3, ymm3, xmm2, 0x1
        vxorpd  xmm2, xmm2, xmm2
        vunpcklpd       xmm4, xmm4, xmm6
        vunpcklpd       xmm0, xmm2, xmm0
        vbroadcastsd    zmm10, xmm10
        vinsertf128     ymm0, ymm0, xmm4, 0x1
        vinsertf64x4    zmm0, zmm0, ymm3, 0x1
        vaddpd  zmm11, zmm11, zmm0
        test    esi, esi
        je      .L3
.L2:
        vmovapd zmm7, zmm12
        mov     esi, 8192
.L9:
        lea     rcx, -8192[rsi]
        vmovapd zmm6, zmm11
.L8:
        vmovapd zmm2, zmm6
        vmovapd zmm0, zmm7
        mov     edx, 128
        vpxor   xmm3, xmm3, xmm3
        jmp     .L5
.L14:
        vmulpd  zmm0, zmm0, zmm2
        vsubpd  zmm1, zmm1, zmm4
        vpaddq  zmm3{k1}, zmm3, zmm9
        vaddpd  zmm2, zmm1, zmm6
        vaddpd  zmm0, zmm0, zmm0
        vaddpd  zmm0, zmm0, zmm7
        sub     edx, 1
        je      .L4
.L5:
        vmulpd  zmm1, zmm2, zmm2
        vmulpd  zmm4, zmm0, zmm0
        vaddpd  zmm5, zmm1, zmm4
        vcmppd  k1, zmm5, zmm8, 1
        kmovw   eax, k1
        movzx   eax, al
        kmovw   k0, eax
        kortestw        k0, k0
        jne     .L14
.L4:
        mov     rax, QWORD PTR [rdi]
        vaddpd  zmm6, zmm6, zmm10
        vmovdqa64       ZMMWORD PTR [rax+rcx], zmm3
        add     rcx, 64
        cmp     rsi, rcx
        jne     .L8
        add     rsi, 8192
        vaddpd  zmm7, zmm7, zmm13
        cmp     rsi, 8396800
        jne     .L9
        add     r9d, 1
        cmp     r8d, r9d
        jne     .L2
.L3:
        xor     eax, eax
        vzeroupper
        ret
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
