;
; Voorbeeld.asm
;
; Created: 01/04/2022 11:25:07
; Author : Yorben
;
.CSEG
Init:	ldi		r16,0b100000
		sts		0x0640,r16
		sts		0x0644,r16
		ldi		r16,0b10000000
		sts		0x0600,r16
Main:	sts		0x0607,r16
		ldi		r17,0
		ldi		r18,0
Loop:	inc		r17
		cpi		r17,0
		brne	Loop
		inc		r18
		cpi		r18,0
		brne	Loop
		jmp		Main
