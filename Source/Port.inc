portinitialization MACRO  
	pusha
  ;Set Divisor Latch Access Bit
	mov dx, 3fbh 			; Line Control Register
	mov al, 10000000b	;Set Divisor Latch Access Bit
	out dx, al				;Out it

  ;Set LSB byte of the Baud Rate Divisor Latch register.
	mov dx, 3f8h			
	mov al, 0ch			
	out dx, al

  ;Set MSB byte of the Baud Rate Divisor Latch register.
	mov dx, 3f9h
	mov al, 00h
	out dx, al

  ;Set port configuration
  mov dx,3fbh
	mov al,00011011b
                  ;0:Access to Receiver buffer, Transmitter buffer
                  ;0:Set Break disabled
                  ;011:Even Parity
                  ;0:One Stop Bit
                  ;11:8bits
	out dx,al
	popa
ENDM portinitialization
