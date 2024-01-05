.text
string_compare:
    # LOAD WORD 1
    mv t0 a0

    # LOAD WORD 2
    mv t1 a1 

    # INITIALIZE BOTH VARIABLES TO THE FIRST CHAR
    lbu t2 0(t0)
    lbu t3 0(t1)
    
    # LOOK FOR ERRORS
    noerror:
    	# IF THE ANY WORD IS EMPTY, THERE IS AN ERROR
    	beq t2 x0 error
        beq t3 x0 error
        # GOES TO WHILE
        j while

    error:
    	# RETURN -1 (ERROR) AND RETURN TO RA
        li a0 -1
        jr ra
    
    # COMPARE STRINGS
    while:
    	# IF THE CHARACTERS AREN'T EQUAL GOES TO IF
    	bne t2 t3 if

        # IF ANY OF THE WORDS HAS FINISHED GO TO IF
        beq t2 x0 if
        beq t3 x0 if

        # ADD 1 TO LOAD NEXT BYTE
        addi t0 t0 1
        addi t1 t1 1

        # LOAD NEXT CHARACTER
        lbu t2 0(t0)
        lbu t3 0(t1)

        # RETURN TO WHILE
        j while
        
    if: 
    	# IF ONE OF THE CHARACTERS IS NOT 0 GO TO END
        bne t2 x0 end
    	bne t3 x0 end

        # RETURN 1 AND RETURN TO RA
    	li a0 1
        jr ra
        
    end: 
    	# RETURN 0 AND RETURN TO RA
    	li a0 0
        jr ra
    	
