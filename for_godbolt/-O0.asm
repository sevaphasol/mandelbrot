compute_mandelbrot_avx512:
        push    rbp
        mov     rbp, rsp
        and     rsp, -64
        sub     rsp, 3208
        mov     QWORD PTR -64[rsp], rdi
        mov     DWORD PTR -68[rsp], esi
        mov     rax, QWORD PTR -64[rsp]
        vmovsd  xmm0, QWORD PTR 24[rax]
        vmovsd  QWORD PTR 2680[rsp], xmm0
        vmovsd  xmm0, QWORD PTR 2680[rsp]
        vmovsd  xmm1, QWORD PTR .LC0[rip]
        vdivsd  xmm0, xmm0, xmm1
        vmovsd  QWORD PTR 2672[rsp], xmm0
        vmovsd  xmm0, QWORD PTR 2680[rsp]
        vmovsd  xmm1, QWORD PTR .LC0[rip]
        vdivsd  xmm0, xmm0, xmm1
        vmovsd  QWORD PTR 1640[rsp], xmm0
        vbroadcastsd    zmm0, QWORD PTR 1640[rsp]
        vmovapd ZMMWORD PTR 2568[rsp], zmm0
        vmovsd  xmm1, QWORD PTR 2672[rsp]
        vmovsd  xmm0, QWORD PTR .LC1[rip]
        vmulsd  xmm0, xmm1, xmm0
        vmovsd  QWORD PTR 1648[rsp], xmm0
        vbroadcastsd    zmm0, QWORD PTR 1648[rsp]
        vmovapd ZMMWORD PTR 2504[rsp], zmm0
        mov     rax, QWORD PTR -64[rsp]
        vmovsd  xmm0, QWORD PTR 16[rax]
        vmovsd  xmm2, QWORD PTR 2680[rsp]
        vmovsd  xmm1, QWORD PTR .LC2[rip]
        vmulsd  xmm1, xmm2, xmm1
        vsubsd  xmm0, xmm0, xmm1
        vmovsd  QWORD PTR 1656[rsp], xmm0
        vbroadcastsd    zmm0, QWORD PTR 1656[rsp]
        vmovapd ZMMWORD PTR 2440[rsp], zmm0
        vmovsd  xmm0, QWORD PTR 2672[rsp]
        vaddsd  xmm0, xmm0, xmm0
        vmovsd  xmm2, QWORD PTR 2672[rsp]
        vmovsd  xmm1, QWORD PTR .LC3[rip]
        vmulsd  xmm1, xmm2, xmm1
        vmovsd  xmm3, QWORD PTR 2672[rsp]
        vmovsd  xmm2, QWORD PTR .LC4[rip]
        vmulsd  xmm2, xmm3, xmm2
        vmovsd  xmm4, QWORD PTR 2672[rsp]
        vmovsd  xmm3, QWORD PTR .LC5[rip]
        vmulsd  xmm3, xmm4, xmm3
        vmovsd  xmm5, QWORD PTR 2672[rsp]
        vmovsd  xmm4, QWORD PTR .LC6[rip]
        vmulsd  xmm4, xmm5, xmm4
        vmovsd  xmm6, QWORD PTR 2672[rsp]
        vmovsd  xmm5, QWORD PTR .LC7[rip]
        vmulsd  xmm5, xmm6, xmm5
        vmovsd  QWORD PTR 1720[rsp], xmm5
        vmovsd  QWORD PTR 1712[rsp], xmm4
        vmovsd  QWORD PTR 1704[rsp], xmm3
        vmovsd  QWORD PTR 1696[rsp], xmm2
        vmovsd  QWORD PTR 1688[rsp], xmm1
        vmovsd  QWORD PTR 1680[rsp], xmm0
        vmovsd  xmm0, QWORD PTR 2672[rsp]
        vmovsd  QWORD PTR 1672[rsp], xmm0
        vxorpd  xmm0, xmm0, xmm0
        vmovsd  QWORD PTR 1664[rsp], xmm0
        vmovsd  xmm1, QWORD PTR 1720[rsp]
        vmovsd  xmm0, QWORD PTR 1712[rsp]
        vunpcklpd       xmm1, xmm0, xmm1
        vmovsd  xmm2, QWORD PTR 1704[rsp]
        vmovsd  xmm0, QWORD PTR 1696[rsp]
        vunpcklpd       xmm0, xmm0, xmm2
        vinsertf128     ymm1, ymm0, xmm1, 0x1
        vmovsd  xmm2, QWORD PTR 1688[rsp]
        vmovsd  xmm0, QWORD PTR 1680[rsp]
        vunpcklpd       xmm2, xmm0, xmm2
        vmovsd  xmm3, QWORD PTR 1672[rsp]
        vmovsd  xmm0, QWORD PTR 1664[rsp]
        vunpcklpd       xmm0, xmm0, xmm3
        vinsertf128     ymm0, ymm0, xmm2, 0x1
        vinsertf64x4    zmm0, zmm0, ymm1, 0x1
        vmovapd zmm1, zmm0
        mov     rax, QWORD PTR -64[rsp]
        vmovsd  xmm0, QWORD PTR 8[rax]
        vmovsd  xmm3, QWORD PTR 2680[rsp]
        vmovsd  xmm2, QWORD PTR .LC2[rip]
        vmulsd  xmm2, xmm3, xmm2
        vsubsd  xmm0, xmm0, xmm2
        vmovsd  QWORD PTR 1728[rsp], xmm0
        vbroadcastsd    zmm0, QWORD PTR 1728[rsp]
        vmovapd ZMMWORD PTR 1800[rsp], zmm0
        vmovapd ZMMWORD PTR 1736[rsp], zmm1
        vmovapd zmm0, ZMMWORD PTR 1800[rsp]
        vaddpd  zmm0, zmm0, ZMMWORD PTR 1736[rsp]
        vmovapd ZMMWORD PTR 2376[rsp], zmm0
        vmovsd  xmm0, QWORD PTR .LC9[rip]
        vmovsd  QWORD PTR 1912[rsp], xmm0
        vbroadcastsd    zmm0, QWORD PTR 1912[rsp]
        vmovapd ZMMWORD PTR 2312[rsp], zmm0
        mov     QWORD PTR 1920[rsp], 1
        vpbroadcastq    zmm0, QWORD PTR 1920[rsp]
        vmovdqa64       ZMMWORD PTR 2248[rsp], zmm0
        mov     DWORD PTR 3204[rsp], 0
        jmp     .L10
.L32:
        vmovapd zmm0, ZMMWORD PTR 2440[rsp]
        vmovapd ZMMWORD PTR 3080[rsp], zmm0
        mov     DWORD PTR 3076[rsp], 0
        jmp     .L11
.L31:
        vmovapd zmm0, ZMMWORD PTR 2376[rsp]
        vmovapd ZMMWORD PTR 2952[rsp], zmm0
        mov     DWORD PTR 2948[rsp], 0
        jmp     .L12
.L29:
        vmovapd zmm0, ZMMWORD PTR 3080[rsp]
        vmovapd ZMMWORD PTR 2824[rsp], zmm0
        vmovapd zmm0, ZMMWORD PTR 2952[rsp]
        vmovapd ZMMWORD PTR 2760[rsp], zmm0
        vpxor   xmm0, xmm0, xmm0
        vmovdqa64       ZMMWORD PTR 2696[rsp], zmm0
        mov     DWORD PTR 2692[rsp], 0
        jmp     .L14
.L27:
        vmovapd zmm0, ZMMWORD PTR 2760[rsp]
        vmovapd ZMMWORD PTR 1288[rsp], zmm0
        vmovapd zmm0, ZMMWORD PTR 2760[rsp]
        vmovapd ZMMWORD PTR 1224[rsp], zmm0
        vmovapd zmm0, ZMMWORD PTR 1288[rsp]
        vmulpd  zmm0, zmm0, ZMMWORD PTR 1224[rsp]
        vmovapd ZMMWORD PTR 2184[rsp], zmm0
        vmovapd zmm0, ZMMWORD PTR 2824[rsp]
        vmovapd ZMMWORD PTR 1416[rsp], zmm0
        vmovapd zmm0, ZMMWORD PTR 2824[rsp]
        vmovapd ZMMWORD PTR 1352[rsp], zmm0
        vmovapd zmm0, ZMMWORD PTR 1416[rsp]
        vmulpd  zmm0, zmm0, ZMMWORD PTR 1352[rsp]
        vmovapd ZMMWORD PTR 2120[rsp], zmm0
        vmovapd zmm0, ZMMWORD PTR 2184[rsp]
        vmovapd ZMMWORD PTR 1544[rsp], zmm0
        vmovapd zmm0, ZMMWORD PTR 2120[rsp]
        vmovapd ZMMWORD PTR 1480[rsp], zmm0
        vmovapd zmm0, ZMMWORD PTR 1544[rsp]
        vaddpd  zmm0, zmm0, ZMMWORD PTR 1480[rsp]
        vmovapd ZMMWORD PTR 2056[rsp], zmm0
        vmovapd zmm0, ZMMWORD PTR 2056[rsp]
        mov     eax, -1
        kmovw   k1, eax
        vcmppd  k0{k1}, zmm0, ZMMWORD PTR 2312[rsp], 1
        kmovw   eax, k0
        mov     BYTE PTR 2055[rsp], al
        movzx   eax, BYTE PTR 2055[rsp]
        movzx   edx, BYTE PTR 2055[rsp]
        mov     WORD PTR 1638[rsp], dx
        mov     WORD PTR 1636[rsp], ax
        movzx   edx, WORD PTR 1636[rsp]
        movzx   eax, WORD PTR 1638[rsp]
        kmovw   k3, eax
        kmovw   k4, edx
        kortestw        k3, k4
        sete    al
        movzx   eax, al
        movzx   eax, ax
        test    eax, eax
        jne     .L34
        movzx   eax, BYTE PTR 2055[rsp]
        vmovdqa64       zmm0, ZMMWORD PTR 2696[rsp]
        vmovdqa64       ZMMWORD PTR 520[rsp], zmm0
        mov     BYTE PTR 519[rsp], al
        vmovdqa64       zmm0, ZMMWORD PTR 2696[rsp]
        vmovdqa64       ZMMWORD PTR 392[rsp], zmm0
        vmovdqa64       zmm0, ZMMWORD PTR 2248[rsp]
        vmovdqa64       ZMMWORD PTR 328[rsp], zmm0
        movzx   eax, BYTE PTR 519[rsp]
        vmovdqa64       zmm1, ZMMWORD PTR 328[rsp]
        vmovdqa64       zmm0, ZMMWORD PTR 520[rsp]
        kmovw   k2, eax
        vpaddq  zmm0{k2}, zmm1, ZMMWORD PTR 392[rsp]
        nop
        vmovdqa64       ZMMWORD PTR 2696[rsp], zmm0
        vmovapd zmm0, ZMMWORD PTR 2760[rsp]
        vmovapd ZMMWORD PTR 648[rsp], zmm0
        vmovapd zmm0, ZMMWORD PTR 2824[rsp]
        vmovapd ZMMWORD PTR 584[rsp], zmm0
        vmovapd zmm0, ZMMWORD PTR 648[rsp]
        vmulpd  zmm0, zmm0, ZMMWORD PTR 584[rsp]
        vmovapd ZMMWORD PTR 1928[rsp], zmm0
        vmovapd zmm0, ZMMWORD PTR 2184[rsp]
        vmovapd ZMMWORD PTR 776[rsp], zmm0
        vmovapd zmm0, ZMMWORD PTR 2120[rsp]
        vmovapd ZMMWORD PTR 712[rsp], zmm0
        vmovapd zmm0, ZMMWORD PTR 776[rsp]
        vsubpd  zmm0, zmm0, ZMMWORD PTR 712[rsp]
        vmovapd ZMMWORD PTR 904[rsp], zmm0
        vmovapd zmm0, ZMMWORD PTR 2952[rsp]
        vmovapd ZMMWORD PTR 840[rsp], zmm0
        vmovapd zmm0, ZMMWORD PTR 904[rsp]
        vaddpd  zmm0, zmm0, ZMMWORD PTR 840[rsp]
        vmovapd ZMMWORD PTR 2760[rsp], zmm0
        vmovapd zmm0, ZMMWORD PTR 1928[rsp]
        vmovapd ZMMWORD PTR 1032[rsp], zmm0
        vmovapd zmm0, ZMMWORD PTR 1928[rsp]
        vmovapd ZMMWORD PTR 968[rsp], zmm0
        vmovapd zmm0, ZMMWORD PTR 1032[rsp]
        vaddpd  zmm0, zmm0, ZMMWORD PTR 968[rsp]
        vmovapd ZMMWORD PTR 1160[rsp], zmm0
        vmovapd zmm0, ZMMWORD PTR 3080[rsp]
        vmovapd ZMMWORD PTR 1096[rsp], zmm0
        vmovapd zmm0, ZMMWORD PTR 1160[rsp]
        vaddpd  zmm0, zmm0, ZMMWORD PTR 1096[rsp]
        vmovapd ZMMWORD PTR 2824[rsp], zmm0
        add     DWORD PTR 2692[rsp], 1
.L14:
        cmp     DWORD PTR 2692[rsp], 127
        jbe     .L27
        jmp     .L20
.L34:
        nop
.L20:
        mov     rax, QWORD PTR -64[rsp]
        mov     rax, QWORD PTR [rax]
        mov     edx, DWORD PTR 3076[rsp]
        mov     ecx, edx
        sal     ecx, 10
        mov     edx, DWORD PTR 2948[rsp]
        add     edx, ecx
        mov     edx, edx
        sal     rdx, 3
        add     rax, rdx
        mov     QWORD PTR 192[rsp], rax
        vmovdqa64       zmm0, ZMMWORD PTR 2696[rsp]
        vmovdqa64       ZMMWORD PTR 72[rsp], zmm0
        mov     rax, QWORD PTR 192[rsp]
        vmovdqa64       zmm0, ZMMWORD PTR 72[rsp]
        vmovdqa64       ZMMWORD PTR [rax], zmm0
        nop
        vmovapd zmm0, ZMMWORD PTR 2952[rsp]
        vmovapd ZMMWORD PTR 264[rsp], zmm0
        vmovapd zmm0, ZMMWORD PTR 2504[rsp]
        vmovapd ZMMWORD PTR 200[rsp], zmm0
        vmovapd zmm0, ZMMWORD PTR 264[rsp]
        vaddpd  zmm0, zmm0, ZMMWORD PTR 200[rsp]
        vmovapd ZMMWORD PTR 2952[rsp], zmm0
        add     DWORD PTR 2948[rsp], 8
.L12:
        cmp     DWORD PTR 2948[rsp], 1023
        jbe     .L29
        vmovapd zmm0, ZMMWORD PTR 3080[rsp]
        vmovapd ZMMWORD PTR 8[rsp], zmm0
        vmovapd zmm0, ZMMWORD PTR 2568[rsp]
        vmovapd ZMMWORD PTR -56[rsp], zmm0
        vmovapd zmm0, ZMMWORD PTR 8[rsp]
        vaddpd  zmm0, zmm0, ZMMWORD PTR -56[rsp]
        vmovapd ZMMWORD PTR 3080[rsp], zmm0
        add     DWORD PTR 3076[rsp], 1
.L11:
        cmp     DWORD PTR 3076[rsp], 1023
        jbe     .L31
        add     DWORD PTR 3204[rsp], 1
.L10:
        mov     eax, DWORD PTR 3204[rsp]
        cmp     eax, DWORD PTR -68[rsp]
        jb      .L32
        mov     eax, 0
        leave
        ret
.LC0:
        .long   0
        .long   1083179008
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
.LC9:
        .long   0
        .long   1079574528
