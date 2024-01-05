.text
attack:
  # SAVE THE ra IN THE LAST POSITION OF sp
  sb ra 0(sp)
  
  mv a2 a0 # LOAD PASSWORD
  mv a3 a1 # LOAD DUMMY
  
  # FIRST LETTER OF PASSWORD
  lbu t0 0(a2)
  
  # MOVE SP FOR INSERTING THE FIRST LETTER OF THE PASSWORD
  addi sp sp -4

  while_a: beq t0 x0 end_a
    # FIRST LETTER OF THE PASSWORD
    sb t0 0(sp)
    sb x0 1(sp)
    mv a0 sp
    
    # MOVE SP FOR SAVING THE FOLLOWING ra
    addi sp sp -4

    # CALL THE FUNCTION study_energy, THAT WILL RETURN THE CORRECT LETTER
    jal ra study_energy
    
    # SAVE THE CORRECT LETTER IN DUMMY
    sb a0 0(a3)
   
    addi a2 a2 1 # NEXT PASSWORD'S LETTER
    addi a3 a3 1 # NEXT POSITION IN DUMMY
    addi a4 a4 1 # GETS THE LENGHT OF THE PASSWORD
    
    addi sp sp 4 # RESTART SP FOR ANOTHER LETTER (TO OPTIMIZE MEMORY)
    
    # NEW LETTER OF THE PASSWORD
    lbu t0 0(a2)

    # LOOP
    j while_a
    
  end_a:
    # ADD THE 0 TO DUMMY
    sb x0 0(a3)

    # RETURN TO THE FIRST POSITION OF DUMMY
    sub a3 a3 a4

    # SAVES DUMMY AGAIN ON a1
    mv a1 a3

    # GETS THE FIRST VALUE STORED ON SP: ra before the function
    addi sp sp 4
    lbu ra 0(sp)
    jr ra
	

study_energy:
  # SAVE THE ra IN sp
  sb ra 0(sp)

  # CHANGE THE PASSWORD FROM a0 TO a1
  mv a1 a0

  # GET THE FIRST LETTER OF THE ALPHABET
  li t0 97

  # SET THE LIMIT OF THE ALPHABET
  li t1 122
  
  while_s: bgt t0 t1 end_s # IF THE ALPHABET IS FINISHED GO TO END
	
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
    
    # NUMBER OF CYCLES
    sub t2 t3 t2
	
    if_s:
    	li t3 97
    	bne t0 t3 else_s 
        # IF IT IS THE FIRST LETTER: a, IT SAVES THE FIRST NUMBER OF CYCLES ON A GLOBAL VARIABLE
        mv a5 t2

    else_s:
      # IF THE SAVED CYCLES ARE GREATER THAN THE NEW ONES THAT MEANS THAT THE PREVIOUS LETTER WAS THE CORRECT
      bgt a5 t2 end_s

      # IF THE FOLLOWING CYCLES ARE BIGGER SAVES IT ON a5
      mv a5 t2
    

    # LOAD THE NEXT BYTE; THE NEXT LETTER
    addi sp, sp 4
    addi t0 t0 1
	
    # RETURN TO WHILE
    j while_s
    
  end_s:
    # TAKE THE PREVIOUS LETTER
    addi t0 t0 -1

    # RETURN THE LETTER
    mv a0 t0

    # GO TO THE LAST POSITION OF sp TO GET THE ra, TO RETURN
    addi sp sp 4

    lbu ra 0(sp) # ra = ra at the beggining of the function
    jr ra

string_compare:
    # LOAD WORD 1
    mv t3 a0
    # LOAD WORD 2
    mv t4 a1 

    # INITIALIZE BOTH VARIABLES = FIRST CHAR
    lbu t5 0(t3)
    lbu t6 0(t4)
    
    # LOOKS FOR ERRORS
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
    	# IF ONE OF THE CHARACTERS AREN'T 0 GO TO END
      bne t5 x0 end
    	bne t6 x0 end

      # RETURN 1 AND RETURN TO RA
    	li a0 1
      jr ra
      
    end: 
    	# RETURN 0 AND RETURN TO RA
    	li a0 0
        jr ra
