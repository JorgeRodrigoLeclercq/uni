.text
study_energy:
  # SAVE THE ra IN THE LAST POSITION OF sp
  sb ra 0(sp)
 
  # MOVE THE WORD TO a1
  mv a1 a0
  
  # GET THE FIRST LETTER OF THE ALPHABET
  li t0 97
  
  # SET THE LIMIT OF THE ALPHABET
  li t1 122
  
  while_s: bgt t0 t1 end_s # IF THE ALPHABET IS FINISHED GOES TO END
    # PRINT LETTER
    li a7 11
    mv a0 t0
    ecall

    # PRINT PUNTUATION
    li a0 ':'
    li a7 11
    ecall
	
    # LOAD THE LETTER IN a0 FOR THE FUNCTION string_compare
    addi sp, sp -4 # SPACE TO STORE: LETTER + /0
    sb t0, 0(sp) # LETTER
    sb x0, 1(sp) # ZERO
	mv a0 sp # COPY sp INTO a0 FOR THE FUNCTION
    
    # START THE COUNT OF CYCLES
    rdcycle t2
   	# COMPARE THE LETTER AND THE PASSWORD
    jal ra string_compare
    # FINISH THE COUNT OF CYCLES
    rdcycle t3
    
    # ACTUAL NUMBER OF CYCLES
    sub t2 t3 t2

    # PRINT CYCLES
    li a7 1
    mv a0 t2
    ecall

    # PRINT ENTER
    li a0 '\n'
    li a7 11
    ecall

    # LOADS THE NEXT BYTE; THE NEXT LETTER
    addi sp, sp 4
    addi t0 t0 1
	
    # RETURN TO WHILE
    j while_s
    
  end_s:
  	# GO TO THE LAST POSITION OF sp TO GET THE ra, TO RETURN
    lbu ra 0(sp) # ra = ra at the beggining of the function
  	jr ra



string_compare:
    # LOAD WORD 1
    mv t3 a0
    # LOAD WORD 2
    mv t4 a1 

    # INITIALIZE BOTH VARIABLES TO THE FIRST CHAR
    lbu t5 0(t3)
    lbu t6 0(t4)
    
    # LOOK FOR ERRORS
    noerror:
    	# IF THE ANY WORD IS EMPTY, THERE IS AN ERROR
    	beq t5 x0 error
        beq t6 x0 error
        # GOES TO WHILE
        j while

    error:
    	# RETURN -1 (ERROR) AND RETURN TO RA
        li a0 -1
        jr ra
    
    # COMPARE STRINGS
    while:
    	# IF THE CHARACTERS AREN'T EQUAL GOES TO IF
    	bne t5 t6 if

        # IF ANY OF THE WORDS HAS FINISHED GO TO IF
        beq t5 x0 if
        beq t6 x0 if

        # ADD 1 TO LOAD NEXT BYTE
        addi t3 t3 1
        addi t4 t4 1

        # LOAD NEXT CHARACTER
        lbu t5 0(t3)
        lbu t6 0(t4)

        # RETURN TO WHILE
        j while
        
    if: 
    	# IF ONE OF THE CHARACTERS IS NOT 0 GO TO END
        bne t5 x0 end
    	bne t6 x0 end

        # RETURN 1 AND RETURN TO RA
    	li a0 1
        jr ra

    end: 
    	# RETURN 0 AND RETURN TO RA
    	li a0 0
        jr ra
    	
