        processor 6502
        include "vcs.h"
        include "macro.h"

        SEG
        ORG $F000

Reset

StartOfFrame
        lda #0
        sta VBLANK

        lda #2
        sta VSYNC

        sta WSYNC
        sta WSYNC
        sta WSYNC

        lda #0
        sta VSYNC

        ldx #0
VBlank  sta WSYNC
        inx
        cpx #37
        bne VBlank

        lda #0
    
        REPEAT 192
                inx
                stx COLUBK
                sta WSYNC
        REPEND

        lda #%01000010
        sta VBLANK

        sta WSYNC
        sta WSYNC
        sta WSYNC
        sta WSYNC
        sta WSYNC
        sta WSYNC
        sta WSYNC
        sta WSYNC
        sta WSYNC
        sta WSYNC
        sta WSYNC
        sta WSYNC
        sta WSYNC
        sta WSYNC
        sta WSYNC
        sta WSYNC
        sta WSYNC
        sta WSYNC
        sta WSYNC
        sta WSYNC
        sta WSYNC
        sta WSYNC
        sta WSYNC
        sta WSYNC
        sta WSYNC
        sta WSYNC
        sta WSYNC
        sta WSYNC
        sta WSYNC
        sta WSYNC

        jmp StartOfFrame

        ORG $FFFA
        .word Reset
        .word Reset
        .word Reset

END