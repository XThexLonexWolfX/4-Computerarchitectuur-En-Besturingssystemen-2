;
; Opdracht3.asm
;
; Created: 01/05/2022 19:51:21
; Author : yorbe
;
;**************************************************************************
	; init oscillator:   external xtal(16MHz) en pll*2  => 32Mhz clock

	; OSC.XOSCCTRL=0b11001011			=> externe 16MHz clock
	ldi		r16, 0b11001011
	sts		osc_xoscctrl, r16
	// enable external oscillator	
	ldi		r16,0b01000
	sts		osc_ctrl,r16
test1:
//check status off xoscrdy (bit3) ....wait until xos is ready
	lds		r16,osc_status
	sbrs	r16, 3        ; skip next instr if bit is set
	rjmp	test1
	// select xosc as source for PLL
	ldi		r16, 0b11000010
	sts		osc_pllctrl, r16
	// enable PLL and external oscillator	
	ldi		r16,0b00011000
	sts		osc_ctrl,r16
test2:
//check status off PLL (bit4)    ... wait until PLL is ok
	lds		r16,osc_status
	sbrs	r16, 4        ; skip next instr if bit is set
	rjmp	test2
	//enable ccp 
	ldi		r16, 0xd8
	sts		cpu_ccp,r16
	ldi		r16, 4
	sts		CLK_CTRL,r16		; select PLL as clock (32MHz)
;**************************************************************************

.CSEG

Init:
	; USARTD-MOSI
	ldi		r16,0b00001000; 3de bit op 1 om PC3/USARTD-MOSI als output te zetten
	sts		PORTD_DIR,r16; P3 zetten als output
	; PL9823-CS
	ldi		r16,0b01000000; 6de bit op 1 om PA6/PL9823-CS als output te zetten
	sts		PORTA_DIR,r16; PA6 als output zetten
	sts		PORTA_OUT,r16; Een 1 zetten op PL9823-CS, om de MOSFET te laten geleiden
	; VAUX-ENA
	ldi		r16,0b00100000; 5de bit op 1 om PC5/VAUX-ENA weer te geven
	sts		PORTC_DIR,r16; PC5 als output zetten
	sts		PORTC_OUT,r16; Een 1 zetten op VAUX-ENA

	
		
	; Variabelen
	ldi		r17,0; BitTeller
	ldi		r18,0; LedTeller
	ldi		r20,0; ResetTeller
	ldi		r21,0; ResetTeller

; Reset geven van 50us 
Reset:
	inc		r20; We gaan continu r20 incrementen en branchen naar Reset tot r20 overflowt en de zero carry flag wordt gezet, dan wordt r20 terug op 0 gezet en gaan we over brne Reset
	brne	Reset
	inc		r21
	cpi		r21,3; We gaan continu r21 incrementen tot hij gelijk is aan 3, zoniet branchen we terug naar Reset
	brne	Reset
	ldi		r20,0
	ldi		r21,0
	
Rood:
	call	Nul; Ik wil groen, dus deze bitstream moet een 0 geven
	cpi		r17,8
	brne	Rood; Zolang we niet 8 keer erdoor zijn gegaan blijven we branchen naar Rood
	ldi		r17,0; We resetten de teller

Groen:
	call	Een; Ik wil groen, dus deze bitstream moet een 1 geven
	cpi		r17,8; We moeten dit 8 keer doen
	brne	Groen; Zolang we niet 8 keer erdoor zijn gegaan blijven we branchen naar Groen
	ldi		r17,0; We resetten de teller

Blauw:
	call	Nul; Ik wil groen, dus deze bitstream moet een 0 geven
	cpi		r17,8; We moeten dit 8 keer doen
	brne	Blauw; Zolang we niet 8 keer erdoor zijn gegaan blijven we branchen naar Blauw
	ldi		r17,0; We resetten de teller

Leds:
	inc		r18
	cpi		r18,4; We moeten alle RGB Leds doorlopen hebben
	brne	Rood; Als dit niet het geval is doorlopen we de kleuren opnieuw voor de volgende LED

Loop:
	jmp		Loop; Kleur wordt niet verandert dus we blijven gewoon doorloopen

Nul:
	ldi		r16,0b00001000; Signaal hoog zetten
	sts		PORTD_OUT,r16; Signaal doorgeven aan de MOSFET
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	ldi		r16,0b00000000; Signaal laag zetten
	sts		PORTD_OUT,r16; Signaal doorgeven aan de MOSFET
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	inc		r17
	ret

Een:
	ldi		r16,0b00001000; Signaal hoog zetten
	sts		PORTD_OUT,r16; Signaal doorgeven aan de MOSFET
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	ldi		r16,0b00000000; Signaal laag zetten
	sts		PORTD_OUT,r16; Signaal doorgeven aan de MOSFET
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	inc		r17
	ret


