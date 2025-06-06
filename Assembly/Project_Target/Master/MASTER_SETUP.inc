;************************************************************************************
;										    *
;   Filename:	    MASTER_SETUP.inc						    *
;   Date:	    April 22, 2025						    *
;   File Version:   2								    *
;   Author:	    Alex Wheelock and Andrew Keller				    *
;   Company:	    Idaho State University					    *
;   Description:    Firmware for setting up a PIC16F1788 for the master of the	    *
;		    Laser Shooting Game project.				    *
;										    *
;************************************************************************************

;************************************************************************************
;										    *
;   Revision History:								    *
;										    *
;   1:	Iniitialize file, setup how I think it should be setup for the project.	    *
;										    *
;   2:	  Added UART functionality, I2C capability to control up to 8 targets	    *
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
;UNUSED		|UNUSED		|UNUSED		|UNUSED		|UNUSED		|UNUSED	 	|UNUSED		|UNUSED		*
;		|		|		|		|		|	 	|		|		*
;		|		|		|		|		|		|		|		*
;		|		|		|		|		|		|		|		*		
;-------------------------------------------------------------------------------------------------------------------------------*
		
;===============================================================================================================================*
;							   PORTB BITMAP								*
;===============================================================================================================================*		
;		|		|		|		|		|		|		|		*
;	7	|	6	|	5	|	4	|	3	|	2	|	1	|	0	*
;---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------*	
;DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	*
;OUTPUT		|OUTPUT		|OUTPUT		|OUTPUT		|OUTPUT		|OUTPUT		|OUTPUT		|OUTPUT		*
;---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------*		
;UNUSED		|UNUSED		|UNUSED		|UNUSED		|UNUSED		|UNUSED		|UNUSED		|UNUSED		*
;		|		|		|		|		|		|		|		*
;		|		|		|		|		|		|		|		*
;		|		|		|		|		|		|		|		*		
;-------------------------------------------------------------------------------------------------------------------------------*
		
;===============================================================================================================================*
;							   PORTC BITMAP								*
;===============================================================================================================================*		
;		|		|		|		|		|		|		|		*
;	7	|	6	|	5	|	4	|	3	|	2	|	1	|	0	*
;---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------*	
;DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	*
;OUTPUT		|OUTPUT		|OUTPUT		|OUTPUT		|OUTPUT		|OUTPUT		|OUTPUT		|OUTPUT		*
;---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------*		
;UNUSED	(UART)	|UNUSED	(UART)	|UNUSED		|UNUSED (I2C)	|UNUSED (I2C)	|UNUSED		|UNUSED		|UNUSED		*
;		|		|		|		|		|		|		|		*		
;		|		|		|		|		|		|		|		*
;		|		|		|		|		|		|		|		*		
;-------------------------------------------------------------------------------------------------------------------------------*			
		
INITIALIZE_CODE	CODE		
INITIALIZE	
	
;*** INTOSC SETUP *******
	
		BANKSEL	    OSCCON
		MOVLW	    0x7A			;ENABLE INTOSC, SET TO 16MHz
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
		BCF	    INTCON, IOCIE		;ENABLE IOC INTERUPT
		BCF	    INTCON, INTE		;DISABLE INT EXTERNAL INTERUPT
		BSF	    INTCON, PEIE		;ENABLE PERIPHERAL INTERUPTS
		BSF	    INTCON, GIE			;ENABLE ALL UNMASKED INTERUPTS
		
;*** SET PIE1-4 REG: *****
		
		BANKSEL	    PIE1
		MOVLW	    B'00101000'
		MOVWF	    PIE1			;DISABLE ALL PIE1 INTERUPTS, EXCEPT SSP1IE, RCIE
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
		
;*** ALTERNATE PIN FUNCTION *****
		
		BANKSEL	    APFCON1
		CLRF	    APFCON1			;SELECTS PORT MAPPING
		CLRF	    APFCON2			;SEE DATASHEET (pg 132-133) FOR MAPPING WITH BOTH CLEARED
 
;*** PORT A SETUP **** PORT A SET AS LOGIC OUTPUT (NOT USED)

		BANKSEL	    PORTA
		CLRF	    PORTA			;CLEAR PORTA
		BANKSEL	    LATA
		CLRF	    LATA			;CLEAR LATA
		BANKSEL	    TRISA
		MOVLW	    0x00
		MOVWF	    TRISA			;SET PORTA ALL AS OUTPUT
		BANKSEL	    ODCONA
		CLRF	    ODCONA			;DISABLE PORTA OPEN-DRAIN		
		BANKSEL	    ANSELA			
		CLRF	    ANSELA			;SET PORTA ALL AS DIGITAL INPUT
		BANKSEL	    SLRCONA
		CLRF	    SLRCONA			;SET THE SLEW RATE FOR PORT A TO MAXIMUM
		BANKSEL	    IOCAP
		CLRF	    IOCAP			;DISABLE PORTA INTERRUPT ON RISING EDGE
		BANKSEL	    IOCAN
		CLRF	    IOCAN			;DISABLE PORTA INTERRUPT ON FALLING EDGE
		
;*** PORT B SETUP **** PORT B SET AS LOGIC OUTPUT (NOT USED)
		
		BANKSEL	    PORTB
		CLRF	    PORTB			;CLEAR PORTB	
		BANKSEL	    LATB
		CLRF	    LATB			;CLEAR LATB
		BANKSEL	    TRISB
		MOVLW	    0x00				
		MOVWF	    TRISB			;SET PORTB ALL AS OUTPUTS
		BANKSEL	    ODCONB
		CLRF	    ODCONB			;DISABLE PORTB OPEN-DRAIN
		BANKSEL	    ANSELB
		CLRF	    ANSELB			;MAKE PORTB DIGITAL INPUTS
		BANKSEL	    SLRCONB
		CLRF	    SLRCONB			;SET THE SLEW RATE FOR PORT B TO MAXIMUM
		BANKSEL	    IOCBP
		CLRF	    IOCBP			;DISABLE PORTB INTERRUPT ON RISING EDGE
		BANKSEL	    IOCBN
		CLRF	    IOCBN			;DISABLE PORTB INTERRUPT ON FALLING EDGE
		BANKSEL	    INLVLB
		CLRF	    INLVLB			;SET PORTB INPUT THRESHOLD TO TTL (IGNORED NOISE BETTER FOR TRIGGER INPUT)	    	    
		
;*** PORT C SETUP ****  PORT C SET AS LOGIC INPUT (USED FOR I2C AND UART)
		
		BANKSEL	    PORTC
		CLRF	    PORTC			;CLEAR PORTC
		BANKSEL	    LATC
		CLRF	    LATC			;CLEAR LATC
		BANKSEL	    TRISC
		MOVLW	    0xFF				
		MOVWF	    TRISC			;SET PORTC ALL AS INPUTS
		BANKSEL	    ODCONC
		CLRF	    ODCONC			;DISABLE PORTC OPEN-DRAIN
		BANKSEL	    ANSELC
		CLRF	    ANSELC			;MAKE PORTC ALL DIGITAL I/O
		BANKSEL	    SLRCONC
		CLRF	    SLRCONC			;SET THE SLEW RATE FOR PORT C TO MAXIMUM
		
;*** I2C SETUP ****
		
		BANKSEL	    SSPADD
		MOVLW	    0x27			;SET BAUD RATE TO 100KHz w/ 16MHz OSCILLATOR
		MOVWF	    SSPADD
		BANKSEL	    SSPCON
		MOVLW	    0x08			;SET TO I2C MASTER MODE
		MOVWF	    SSPCON			;
		BSF	    SSPCON,5			;ENABLE SERIAL PORT FOR I2C
		BANKSEL	    SSPCON2
		CLRF	    SSPCON2
		BANKSEL	    SSPCON3
		CLRF	    SSPCON3
		BANKSEL	    SSPSTAT
		MOVLW	    0xC0
		MOVWF	    SSPSTAT
		
;*** UART SETUP ****
		
		BANKSEL	    SPBRGH
		MOVLW	    0x01			;SET UART BAUD RATE TO 9600 W/ 16MHz OSCILLATOR
		MOVWF	    SPBRGH
		BANKSEL	    SPBRGL
		MOVLW	    0xA0
		MOVWF	    SPBRGL
		BANKSEL	    BAUDCON
		MOVLW	    B'00001000'			;CONFIGURE FOR 16-BIT BAUD RATE GENERATOR
		MOVWF	    BAUDCON
		BANKSEL	    RCSTA
		MOVLW	    B'10010000'			;ENABLE UART PORT, ENABLE RECEIVER
		MOVWF	    RCSTA
		BANKSEL	    TXSTA
		MOVLW	    B'00100110'			;ENABLE TRANSMITTER, HIGH SPEED MODE, ASYNCHRONOUS
		MOVWF	    TXSTA
		
;*** BOOT CODE ****
		
		BANKSEL	    INTCON
		BCF	    INTCON,0
		BCF	    INTCON,IOCIF		;CLEAR IOCIF
		
		;CLEAR GENERAL REGISTERS
		BANKSEL	    PORTA
		CLRF	    ENABLE_TARGET
		CLRF	    CURRENT_ADDRESS
		CLRF	    INUSE_TARGET
		
		
		RETURN					;RETURN TO .asm FILE