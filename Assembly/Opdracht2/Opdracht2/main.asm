;
; Opdracht2.asm
;
; Created: 15/04/2022 14:23:09
; Author : Yorben
;
Init:	
	; LED's
	ldi		r16,0b00100000; We zetten 5de bit op 1 voor verder VAUX-Enable op 1 te zetten en PC5 als output te zetten
	sts		PORTC_DIR,r16; PC5 zetten als output
	sts		PORTC_OUT,r16; Een 1 zetten op VAUX-Enable
	ldi		r16,0b10000000; We zetten 7de bit op 1 voor PA7 als output te zetten
	sts		PORTA_DIR,r16; PA7 zetten als output
	ldi		r16,0b00000111; 0de - 2de bit op 1 voor PB0-2 als output te zetten
	sts		PORTB_DIR,r16; PB0-2 zetten als output
	ldi		r16,0b11111111; Elke led uitzetten
	sts		PORTA_OUT,r16; Elke led uitzetten
	sts		PORTB_OUT,r16; Elke led uitzetten
	; Joystick
	ldi		r16,0b00011000; Pull up weerstand aanzetten
	sts		PORTB_PIN3CTRL,r16; Pull up weerstand Up
	sts		PORTB_PIN4CTRL,r16; Pull up weerstand Left
	sts		PORTB_PIN5CTRL,r16; Pull up weerstand Down
	sts		PORTB_PIN6CTRL,r16; Pull up weerstand Right
	sts		PORTB_PIN7CTRL,r16; Pull up weerstand Center	
	; Variabelen
	ldi		r18,0; Delay
	ldi		r19,0; Delay
	ldi		r20,0; Delay
	ldi		r27,5; DelayVariabele
	ldi		r21,1; Stapgrootte veranderen Delay
	ldi		r22,0; LedTeller
	ldi		r23,0b11111110; Initiele ledwaarden, we beginnen met richting links
	mov		r16,r23
	ldi		r24,1; Richting, we beginnen met richting links (1 = links, 2 = rechts)
	ldi		r25,0; Joystickwaarde

; Loopen door te zien op welke LED we zitten
Loop:
	cpi		r22,4; Zien of we op de laatste LED geraakt zijn
	brne	Verder; Zo niet gaan we gewoon verder
	ldi		r22,0; Zo ja dan resetten we de LedTeller
	mov		r16,r23; We starten terug bij de initiele ledwaarden

Verder:
	call	Joystick; Richting joystick uitlezen
	call	Output; Waardes doorgeven aan de LED's
	inc		r22; Teller van de Leds verhogen
	cpi		r24,1; We kijken of de richting gelijk is aan links
	breq	Links; Zo ja branchen we naar Links, anders gaan we verder
	cpi		r24,2; We kijken of de richting gelijk is aan rechts
	breq	Rechts; Zo ja gaan we branchen naar Rechts, anders gaan we verder
	jmp		Loop

Links:
	sec; Carry op 1 zetten anders gaan er twee leds aan, want de carry flag wordt op bit 0 ingevoegd bij rol
	rol		r16; De carry flag wordt op bit 0 ingevoegd, waardoor alle bits 1 plek naar links opschuiven
	call	Delay; We roepen de delay op
	jmp		Loop

Rechts:
	sec; Carry op 1 zetten anders gaan er twee leds aan, want de carry flag wordt op bit 7 ingevoegd bij ror
	ror		r16; De carry flag wordt op bit 7 ingevoegd, waardoor alle bits 1 plek naar rechts opschuiven
	call	Delay; We roepen de delay op
	jmp		Loop

Delay:
	inc		r18; We gaan continu r18 incrementen en branchen naar Delay tot r18 overflowt en de zero carry flag wordt gezet, dan wordt r19 terug op 0 gezet en gaan we over brne Delay
	brne	Delay
	inc		r19; We gaan continu r19 incrementen en branchen naar Delay tot r19 overflowt en de zero carry flag wordt gezet, dan wordt r19 terug op 0 gezet en gaan we over brne Delay
	brne	Delay
	inc		r20
	cp		r20,r27
	brne	Delay; We gaan continu r20 incrementen tot hij gelijk is aan r27, zoniet branchen we terug naar Delay
	ldi		r18,0
	ldi		r19,0
	ldi		r20,0
	ret; Als we tot hier geraakt zijn springen we terug naar waar de delay is opgeroepen

Output:
	sts		PORTB_OUT,r16; 
	bst		r16,3; De 3de bit van r16 op T zetten want deze duid LED4 aan, maar om die aan te sturen moeten we de waarde op bit 7 zetten
	bld		r26,7; De waarde die we opgeslagen hebben in T op de 7de bit zetten om LED4 aan te sturen
	sts		PORTA_OUT,r26
	ret

; Joystick inlezen
Joystick:
	lds		r25,PORTB_IN; We lezen de Joystick waarde in
	ori		r25,0b00000111; De eerste 3 bits zijn van de LED's, de andere van de Joystick. Dus willen we met die van de LED's geen rekening houden
	cpi		r25,0b11101111; We kijken of de 4de bit/PB4 op 0 staat, dit zou Links aanduiden
	breq	RichtingLinks; Als dit inderdaad zo is branchen we naar RichtingLinks, anders gaan we verder
	cpi		r25,0b10111111; We kijken of de 6de bit/PB6 op 0 staat, dit zou Rechts aanduiden
	breq	RichtingRechts; Als dit inderdaad zo is branchen we naar RichtingRechts, anders gaan we verder
	cpi		r25,0b11110111; We kijken of de 3de bit/PB3 op 0 staat, dit zou Boven aanduiden
	breq	RichtingBoven; Als dit inderdaad zo is branchen we naar RichtingBoven, anders gaan we verder
	cpi		r25,0b11011111; We kijken of de 5de bit/PB5 op 0 staat, dit zou Onder aanduiden
	breq	RichtingOnder; Als dit inderdaad zo is branchen we naar RichtingOnder, anders gaan we verder
	cpi		r25,0b01111111; We kijken of de 7de bit/PB7 op 0 staat, dit zou Center aanduiden
	breq	RichtingCenter; Als dit inderdaad zo is branchen we naar RichtingCenter, anders gaan we verder
	ret; We gaan terug naar waar we Joystick hebben opgeroepen

; Richting veranderen naar links
RichtingLinks:
	ldi		r23,0b11111110; Initiele ledwaarden aanpassen rekening houdend met de richting
	ldi		r24,1; De richting zetten op links
	jmp		Joystick; Teruggaan naar Joystick

; Richting veranderen naar rechts
RichtingRechts:
	ldi		r23,0b11110111; Initiele ledwaarden aanpassen rekening houdend met de richting, hier nemen we bit 3 ipv 7, door de logica in Output
	ldi		r24,2; De richting zetten op rechts
	jmp		Joystick; Teruggaan naar Joystick

; Delay verlagen/Snelheid verhogen
RichtingBoven:
	sub		r27,r21; De stapgrootte (r21) van de delayvariabele (r20) aftrekken
	jmp		Joystick; Teruggaan naar Joystick

; Delay verhogen/Snelheid verlagen
RichtingOnder:
	add		r27,r21; De delayvariabele (r20) optellen met de stapgrootte (r21)
	jmp		Joystick; Teruggaan naar Joystick

; Hier zetten we alles terug op de basis waarden
RichtingCenter:
	ldi		r24,1; Terug op richting links zetten
	ldi		r27,5; DelayVariabele terug resetten
	jmp		Joystick
	




