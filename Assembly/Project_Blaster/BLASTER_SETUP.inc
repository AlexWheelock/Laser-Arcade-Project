;************************************************************************************
;										    *
;   Filename:	    BLASTER_SETUP.inc						    *
;   Date:	    November 5, 2024						    *
;   File Version:   2								    *
;   Author:	    Alex Wheelock						    *
;   Company:	    Idaho State University					    *
;   Description:    Firmware for setting up a PIC16F1788 for the laser gun of the  *
;		    Laser ARCADE project.				    *
;										    *
;************************************************************************************

;************************************************************************************
;										    *
;   Revision History:								    *
;										    *
;   1:	  (NOV 2024) Got everything for the gun working the way that I think it     *
;	  should with base features.						    *
;										    *		
;   2:	  (APRIL 3, 2025) Added an audio board, and connected all of PORTA, and	    *
;	  RC7,6,1, & 0 as triggers for the audio board, only RA0 & RA1 are being    *
;	  used as triggers currently, RC7 toggles in MAIN to ensure the volume is   *
;	  maxed out.								    *
;										    *
;	  Code was also updated to work properly with the fire select switch that   *
;	  will be used in the final product.					    *
;										    *	
;************************************************************************************
		
;===============================================================================================================================*
;							   PORTA BITMAP								*
;===============================================================================================================================*		
;		|		|		|		|		|		|		|		*
;	7	|	6	|	5	|	4	|	3	|	2	|	1	|	0	*
;---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------*	
;DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	*
;OUTPUT		|OUTPUT		|OUTPUT		|OUTPUT		|OUTPUT		|OUTPUT		|OUTPUT		|OUTPUT		*
;---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------*		
;DRIVES TRIG7	|DRIVES TRIG6	|DRIVES TRIG5	|DRIVES TRIG4	|DRIVES TRIG3	|DRIVES TRIG2 	|DRIVES TRIG1	|DRIVES TRIG0	*
;OF THE AUDIO	|OF THE AUDIO	|OF THE AUDIO	|OF THE AUDIO	|OF THE AUDIO	|OF THE AUDIO 	|OF THE AUDIO	|OF THE AUDIO	*
;BOARD		|BOARD		|BOARD		|BOARD		|BOARD		|BOARD		|BOARD		|BOARD		*
;		|		|		|		|		|		|		|		*		
;-------------------------------------------------------------------------------------------------------------------------------*
		
;===============================================================================================================================*
;							   PORTB BITMAP								*
;===============================================================================================================================*		
;		|		|		|		|		|		|		|		*
;	7	|	6	|	5	|	4	|	3	|	2	|	1	|	0	*
;---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------*	
;DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	*
;INPUT		|INPUT		|INPUT		|INPUT		|INPUT		|INPUT		|INPUT		|INPUT		*
;---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------*		
;	     USED FOR		|UNUSED		|INPUT USED FOR	|INPUT USED FOR	|INPUT USED FOR	|INPUT USED FOR	|INPUT USED FOR	*
;	   PROGRAMMING		|		|SEMI-AUTO MODE	|BURST-FIRE	|CONTINUOUS/	|THE FREQUENCY	|TRIGGER BUTTON	*
;	      ONLY		|		|SELECT		|MODE SELECT	|FULL-AUTO MODE	|SELECT SWITCH	|		*
;				|		|		|		|SELECT		|		|		*		
;-------------------------------------------------------------------------------------------------------------------------------*
		
;===============================================================================================================================*
;							   PORTC BITMAP								*
;===============================================================================================================================*		
;		|		|		|		|		|		|		|		*
;	7	|	6	|	5	|	4	|	3	|	2	|	1	|	0	*
;---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------*	
;DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|PWM		|PWM		|DIGITAL	*
;OUTPUT		|OUTPUT		|OUTPUT		|OUTPUT		|OUTPUT		|OUTPUT		|OUTPUT		|OUTPUT		*
;---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------*		
;DRIVES VOL+	|DRIVES TRIG10	|DRIVES TRIG9	|UNUSED		|OUTPUT USED TO	|OUTPUT USED TO	|OUTPUT USED TO	|DRIVES TRIG8	*
;OF THE AUDIO	|OF THE AUDIO	|OF THE AUDIO	|		|CONTROL THE	|CONTROL/DRIVE	|CONTROL/DRIVE 	|OF THE AUDIO	*
;BOARD		|BOARD		|BOARD		|		|SOLENOID	|LASER1	    	|LASER2		|BOARD		*
;		|		|		|		|		|		|		|		*		
;-------------------------------------------------------------------------------------------------------------------------------*			
		
INITIALIZE_CODE	CODE		
INITIALIZE	
	
;*** INTOSC SETUP *******
	
		BANKSEL	    OSCCON
		MOVLW	    0xEA			;ENABLE INTOSC, SET TO 4MHz
		MOVWF	    OSCCON
		BANKSEL	    CLKRCON
		CLRF	    CLKRCON			;DISABLE CLOCK REFERENCE
		
;*** SFR SETUP **********
 
		
;*** SET OPTION_REG: ****
		
		BANKSEL	    OPTION_REG
		BCF	    OPTION_REG, PS0		;\\
		BCF	    OPTION_REG, PS1		; >>PRESCALER RATIO SET 1:1
		BCF	    OPTION_REG, PS2		;//
		BSF	    OPTION_REG, PSA		;PRESCALER ASSIGNED TO WDT 
		BCF	    OPTION_REG, TMR0SE		;TMR0 EDGE SET RISING
		BCF	    OPTION_REG, TMR0CS		;TMR0 CLOCK SOURCE SET TO INTERNAL
		BSF	    OPTION_REG, INTEDG		;RB0/INT SET TO RISING EDGE
		BSF	    OPTION_REG, NOT_WPUEN	;WEAK PULLUP ENABLED
		
;*** SET INTCON REG: ****
		
		BANKSEL INTCON
		BSF	    INTCON, IOCIE		;ENABLE IOC INTERUPT
		BCF	    INTCON, INTE		;DISABLE INT EXTERNAL INTERUPT
		BSF	    INTCON, PEIE		;ENABLE PERIPHERAL INTERUPTS
		BSF	    INTCON, GIE			;ENABLE ALL UNMASKED INTERUPTS
		
;*** SET PIE1-4 REG: *****
		
		BANKSEL	    PIE1
		MOVLW	    0x01
		MOVWF	    PIE1			;DISABLE ALL PIE1 INTERUPTS, EXCEPT TMR1IE
		BANKSEL	    PIE2
		CLRF	    PIE2			;DISABLE ALL PIE2 INTERUPTS
		BANKSEL	    PIE3
		CLRF	    PIE3			;DISABLE ALL PIE3 INTERUPTS
		BANKSEL	    PIE4
		CLRF	    PIE4			;DISABLE ALL PIE4 INTERUPTS
		
;*** CLEAR INTERUPT FLAGS ***
		
		BANKSEL	    PIR1
		CLRF	    PIR1			;CLEAR ALL PIR1 INTERUPT FLAGS
		BANKSEL	    PIR2
		CLRF	    PIR2			;CLEAR ALL PIR2 INTERUPT FLAGS
		BANKSEL	    PIR3
		CLRF	    PIR3			;CLEAR ALL PIR3 INTERUPT FLAGS
		BANKSEL	    PIR4
		CLRF	    PIR4			;CLEAR ALL PIR4 INTERUPT FLAGS
		
;*** SET CCP1CON REG: **
		
		BANKSEL	    TRISC
		BSF	    TRISC,2			;DISABLE PWM OUTPUT DRIVER FOR CCP1 SETUP
		BANKSEL	    CCP1CON
		MOVLW	    0x0C		
		MOVWF	    CCP1CON			;SELECT PWM MODE FOR CCP1
		BANKSEL	    CCPR1L
		MOVLW	    0x19			;PW FOR INITIALIZING 95%DC AT 38kHz
		;MOVLW	    0x11			;PW FOR INITIALIZING 95%DC AT 56kHz
		MOVWF	    CCPR1L

;*** TIMER 1 SETUP *****
		
		BANKSEL	    T1CON
		MOVLW	    0xA8			;ENABLE & SELECT TMR1 LP OSCILLATOR, SET PRESCALE TO 1:4, DISABLE TMR1 AT THIS POINT
		BANKSEL	    T1GCON
		CLRF	    T1GCON			;DISABLE TMR1 GATE
		
;*** TIMER 2 SETUP *****

		BANKSEL	    T2CON			
		CLRF	    T2CON
		BANKSEL	    PR2				
		MOVLW	    0x19			;SET PR2 TO 25 FOR 38kHz PWM FREQUENCY
		;MOVLW	    0x11			;SET PR2 TO 17 FOR 56kHz PWM FREQUENCY
		MOVWF	    PR2				
		
;*** ALTERNATE PIN FUNCTION *****
		
		BANKSEL	    APFCON1
		CLRF	    APFCON1			;SELECTS PORT MAPPING
		CLRF	    APFCON2			;SEE DATASHEET (pg 132-133) FOR MAPPING WITH BOTH CLEARED
 
;*** PORT A SETUP **** PORT A SET AS LOGIC OUTPUT

		BANKSEL	    PORTA
		MOVLW	    0xFF
		MOVWF	    PORTA			;SET PORTA
		BANKSEL	    LATA
		MOVWF	    LATA			;CLEAR LATA
		BANKSEL	    TRISA
		MOVLW	    0x00  
		MOVWF	    TRISA			;SET PORTA ALL AS OUTPUTS
		BANKSEL	    ODCONA
		CLRF	    ODCONA			;DISABLE PORTA OPEN-DRAIN		
		BANKSEL	    ANSELA			
		CLRF	    ANSELA			;SET PORTA ALL AS DIGITAL INPUT
		BANKSEL	    SLRCONA
		CLRF	    SLRCONA			;SET THE SLEW RATE FOR PORT A TO MAXIMUM
		
;*** PORT B SETUP **** PORT B USED AS LOGIC INPUT
		
		BANKSEL	    PORTB
		CLRF	    PORTB			;CLEAR PORTB	
		BANKSEL	    LATB
		CLRF	    LATB			;CLEAR LATB
		BANKSEL	    TRISB
		MOVLW	    0xFF				
		MOVWF	    TRISB			;SET PORTB ALL AS INPUTS
		BANKSEL	    ODCONB
		CLRF	    ODCONB			;DISABLE PORTB OPEN-DRAIN
		BANKSEL	    ANSELB
		CLRF	    ANSELB			;MAKE PORTB DIGITAL INPUTS
		BANKSEL	    SLRCONB
		CLRF	    SLRCONB			;SET THE SLEW RATE FOR PORT B TO MAXIMUM
		BANKSEL	    IOCBP
		MOVLW	    0x0F			;ENABLE PORTB IOC FOR POSITIVE EDGES ON RB3:0
		MOVWF	    IOCBP
		BANKSEL	    IOCBN
		MOVLW	    0x0E			;ENABLE PORTB IOC FOR NEGATIVE EDGES ON RB3:1
		MOVWF	    IOCBN
		BANKSEL	    INLVLB
		CLRF	    INLVLB			;SET PORTB INPUT THRESHOLD TO TTL (IGNORED NOISE BETTER FOR TRIGGER INPUT)	    	    
		
;*** PORT C SETUP **** 
		
		BANKSEL	    PORTC
		MOVLW	    0xF1
		MOVWF	    PORTC			;SET ALL OF THE AUDIO BOARD OUTPUTS RC7:5 & RC0, CLEAR THE LASER/SOLENOID OUTPUTS
		BANKSEL	    LATC
		MOVWF	    LATC			;CLEAR LATC
		BANKSEL	    TRISC
		MOVLW	    0x00				
		MOVWF	    TRISC			;SET PORTC ALL AS OUTPUTS
		BANKSEL	    ODCONC
		CLRF	    ODCONC			;DISABLE PORTC OPEN-DRAIN
		BANKSEL	    ANSELC
		CLRF	    ANSELC			;MAKE PORTC ALL DIGITAL I/O
		BANKSEL	    SLRCONC
		CLRF	    SLRCONC			;SET THE SLEW RATE FOR PORT C TO MAXIMUM
		
;*** BOOT CODE ****
		
		BCF	    INTCON,0
		BANKSEL	    T1CON
		BSF	    T1CON,0			;ENABLE TIMER 1
		BANKSEL	    T2CON
		BSF	    T2CON,2			;ENABLE TIMER 2
		BANKSEL	    TRISC
		BSF	    TRISC,2			;DISABLE OUTPUT DRIVER ON STARTUP (WAS ENABLED ON STARTUP WITHOUT THIS)
		BANKSEL	    PORTA
		RETURN					;RETURN TO .asm FILE
		
		