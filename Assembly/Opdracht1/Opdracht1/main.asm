;
; Opdracht1Ext.asm
;
; Created: 15/04/2022 15:07:49
; Author : Yorben
;
.CSEG

Init:	ldi		r16,0b00100000; We zetten 5de bit op 1 voor verder VAUX-Enable op 1 te zetten en PC5 als output te zetten
		sts		PORTC_DIR,r16; PC5 zetten als output
		sts		PORTC_OUT,r16; Een 1 zetten op VAUX-Enable
		ldi		r16,0b10000000; We zetten 7de bit op 1 voor PA7 als output te zetten
		sts		PORTA_DIR,r16; PA7 zetten als output
		ldi		r16,0b00000111; 0de - 2de bit op 1 voor PB0-2 als output te zetten
		sts		PORTB_DIR,r16; PB0-2 zetten als output
		ldi		r16,0b11111111; Elke led uitzetten
		sts		PORTA_OUT,r16; Elke led uitzetten
		sts		PORTB_OUT,r16; Elke led uitzetten
		ldi		r18,0; Delay
		ldi		r19,0; Delay
		ldi		r20,0; Delay

Main:	ldi		r17,0; Deze gebruik ik voor de loop

Loop:	ldi		r16,0b11111110; PB0 op 0 om eerste led aan te zetten
		sts		PORTB_OUT,r16; Waarde doorgeven aan de output

		call Delay

		ldi		r16,0b11111111; Alle leds uitzetten
		sts		PORTB_OUT,r16; Waarde doorgeven aan de output

		call Delay

		ldi		r16,0b11111101; PB1 op 0 om tweede led aan te zetten
		sts		PORTB_OUT,r16; Waarde doorgeven aan de output

		call Delay

		ldi		r16,0b11111111; Alle leds uitzetten
		sts		PORTB_OUT,r16; Waarde doorgeven aan de output

		call Delay

		ldi		r16,0b11111011; PB2 op 0 om derde led aan te zetten en vorige uit
		sts		PORTB_OUT,r16; Waarde doorgeven aan de output

		call Delay

		ldi		r16,0b11111111; Alle leds uizetten
		sts		PORTB_OUT,r16; Waarde doorgeven aan de output

		call Delay

		ldi		r16,0b01111111; PA7 op 0 zetten om vierde led aan te zetten
		sts		0x604,r16; Waarde doorgeven aan de output

		call Delay

		ldi		r16,0b11111111; PA7 op 1 om vierde led uit te zetten
		sts		0x0604,r16; Waarde doorgeven aan de output

		call Delay

		inc		r17; We gaan continu r17 incrementen tot hij overflowt en de zero carry flag wordt gezet, dan zetten we r17 terug op nul en beginnen opnieuw
		brne	Loop
		jmp		Main

Delay:	inc		r18; We gaan continu r18 incrementen en branchen naar Delay tot r18 overflowt en de zero carry flag wordt gezet, dan wordt r19 terug op 0 gezet en gaan we over brne Delay
		brne	Delay
		inc		r19; We gaan continu r19 incrementen en branchen naar Delay tot r19 overflowt en de zero carry flag wordt gezet, dan wordt r19 terug op 0 gezet en gaan we over brne Delay
		brne	Delay
		inc		r20
		cpi		r20,30
		brne	Delay; We gaan continu r20 incrementen tot hij gelijk is aan 30, zoniet branchen we terug naar Delay
		ldi		r18,0
		ldi		r19,0
		ldi		r20,0
		ret		; Als we tot hier geraakt zijn springen we terug naar waar de delay is opgeroepen
