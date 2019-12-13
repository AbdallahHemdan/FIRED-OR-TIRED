STARTING_PROCESS MACRO
    ; Open file
        MOV AH, 3Dh
        MOV AL, 0 ; read only
        LEA DX, startFilename
        INT 21h   
        MOV [StartFilehandle], AX
        ;ReadData
        MOV AH,3Fh
        MOV BX, [StartFilehandle]
        MOV CX,startWidth*startHeight ; number of bytes to read
        LEA DX, startData
        INT 21h	
        LEA BX , startData ; BL contains index at the current drawn pixel
        MOV CX,0
        MOV DX,0
        MOV AH,0ch
        
        ; Drawing loop
        drawLoop:
            MOV AL,[BX]
            INT 10h 
            INC CX
            INC BX
            CMP CX,startWidth
        JNE drawLoop 
            MOV CX , 0
            INC DX
            CMP DX , startHeight
        JNE drawLoop
            ;CloseFile
            MOV AH, 3Eh
            MOV BX, [startFilehandle]
            INT 21h

    GAME_MENU:
        MOV AH,0
        INT 16H 
        CMP AL,'1'
        jne LEVEL2
            MOV GAMELEVEL, 1
            JMP GAME_START
        LEVEL2:
            CMP AL,'2'
            JNE GAME_MENU
            MOV GAMELEVEL, 2
        JMP GAME_START

    JNE GAME_MENU ;PASS ONLY IF 1 OR 2 IS PRESSED 

    ;video mode
        GAME_START:
        MOV AH, 0
        MOV AL, 3h
        INT 10h
    ;DISPLAY GAME START STRING
    
        ;CLEAR SCREEN
        MOV AX, 0600H
        MOV BH, 7
        MOV CX, 0
        MOV DX ,184FH
        INT 10H

        ;DISPLAY THE START GAME STRING
        MOV AH,09
        MOV DH,0
        MOV DX,OFFSET GAME_START_STR
        INT 21H 

        ;moving the cursor AND SHOW PLAYER 1 NAME
            MOV DL, 5
            MOV DH, 5
            MOV BH, 0
            MOV AH, 02h
            int 10h

            MOV AH,09
            MOV DH,0
            MOV DX, OFFSET PLAYER1 + 7
            INT 21H
        ;moving the cursor AND SHOW PLAYER 2 NAME:
            MOV DL, 5
            MOV DH, 7
            MOV BH, 0
            MOV AH, 02h
            int 10h

            MOV AH,09
            MOV DH,0
            MOV DX,OFFSET PLAYER2 + 7
            INT 21H

        ;READ FIRST PLAYER NAME
            ;MOVE THE CURSOR THEN TAKE THE FIRST PLAYER NAME
            MOV DL, 19
            MOV DH, 5
            MOV BH, 0
            MOV AH, 02h
            int 10h

            MOV AH,0AH
            MOV DX,offset PLAYER1_NAME
            INT 21H

        ;READ SECOND PLAYER NAME
            MOV DL, 19
            MOV DH, 7
            MOV BH, 0
            MOV AH, 02h
            int 10h

            MOV AH,0AH
            MOV DX,offset PLAYER2_NAME
            INT 21H
             
             
        MOV CX,7
        MOV si,2
        NAMES: 
            CMP [PLAYER1_NAME+si],0DH
            JE NONAME1
            CMP [PLAYER1_NAME+si],'$'
            JE NONAME1
            MOV bh,[PLAYER1_NAME+si]
            MOV [PLAYER1+SI-2],BH
            NONAME1:  
            CMP [PLAYER2_NAME+si],'$'
            JE NONAME2
            CMP [PLAYER2_NAME+si],0DH
            JE NONAME2
            MOV bl,[PLAYER2_NAME+si]
            MOV [PLAYER2+SI-2],BL
            NONAME2:
            INC si
        LOOP NAMES

ENDM STARTING_PROCESS

INITIALZING_DATA MACRO
    
    MOV POWERUP1STATUS, 0
    MOV POWERUP2STATUS, 0
    MOV POWERUP3STATUS, 0

    MOV AX, INITIAL_P1_X
    MOV PLAYER1X, AX
    MOV AX, INITIAL_P1_Y
    MOV PLAYER1Y, AX
    MOV AX, INITIAL_P2_X
    MOV PLAYER2X, AX
    MOV AX, INITIAL_P2_Y
    MOV PLAYER2Y, AX

    MOV DX1, 0
    MOV DY1, 2
    MOV DX2, 0
    MOV DY2, 2
ENDM INITIALZING_DATA

RANDOMIZIONG_OF_POWERUPS MACRO
    INC RAND_NUMBER_DRAW
        CMP RAND_NUMBER_DRAW, 21
        JNE DONT_RESET_RAND_NUMBER
            MOV RAND_NUMBER_DRAW, 0
        DONT_RESET_RAND_NUMBER:
        
        ;ACTIVATE STATUES OF POWER UPS
        PUSH AX
        PUSH SI
        CMP RAND_NUMBER_DRAW,20
            JNE DONT_ACTIVE_POWERUP
                
                CMP POWERUP1STATUS,1
                JE DONT_DRAW_POWER1
                    MOV POWERUP1STATUS,1
                    MOV SI, RAND_INDEX
                    MOV AX, X_ARRAY_POWER_UP[SI]
                    MOV powerup1x, AX
                    MOV AX, Y_ARRAY_POWER_UP[SI]
                    MOV POWERUP1Y, AX
                    ADD RAND_INDEX,2
                    JMP DONT_ACTIVE_POWERUP
                DONT_DRAW_POWER1:

                CMP POWERUP2STATUS,1
                JE DONT_DRAW_POWER2
                    MOV POWERUP2STATUS,1
                    MOV SI, RAND_INDEX
                    MOV AX, X_ARRAY_POWER_UP[SI]
                    MOV powerup2x, AX
                    MOV AX, Y_ARRAY_POWER_UP[SI]
                    MOV POWERUP2Y, AX
                    ADD RAND_INDEX,2
                    JMP DONT_ACTIVE_POWERUP
                DONT_DRAW_POWER2:

                CMP POWERUP3STATUS,1
                JE DONT_DRAW_POWER3
                    MOV POWERUP3STATUS,1
                    MOV SI, RAND_INDEX
                    MOV AX, X_ARRAY_POWER_UP[SI]
                    MOV powerup3x, AX
                    MOV AX, Y_ARRAY_POWER_UP[SI]
                    MOV POWERUP3Y, AX
                    ADD RAND_INDEX,2
                    JMP DONT_ACTIVE_POWERUP
                DONT_DRAW_POWER3:
            DONT_ACTIVE_POWERUP:

            CMP RAND_INDEX, 18
            JNE DONT_RESET_RAND_INDEX
                MOV RAND_INDEX, 0
            DONT_RESET_RAND_INDEX:

ENDM RANDOMIZIONG_OF_POWERUPS

DRAW_STATIC MACRO  
    ;Draw BackGround
        MOV BH,0
        MOV CX,320D
        MOV AH,0Ch
        MOV AL,BGC
        XBGC:
            MOV DX,200D
            YBGC:
                INT 10H
                DEC DX
                CMP DX, 30
                JE OUT_BKG
            JNZ YBGC
            OUT_BKG:
        LOOP XBGC

    ;Draw Frame
        MOV AH, 0CH
        MOV AL, 0fh

        
        MOV CX, 320D
        MOV DX, 30
        UPPER_FRAME: INT 10H
        LOOP UPPER_FRAME
                        
            MOV CX, 320D
            MOV DX, 199D
        LOWER_FRAME: INT 10H
        LOOP LOWER_FRAME


            MOV CX, 319D
            MOV DX, 199D
        RIGHT_FRAME: INT 10H
            DEC DX 
            CMP DX, 30
        JNZ RIGHT_FRAME
                        
            MOV CX, 1
        LEFT_FRAME: INT 10H
            INC DX 
            CMP DX, 199
        JNZ LEFT_FRAME    

    ;display player1 name in BLUE
        ;moving the cursor
            mov dl,1h
            mov dh,1h
            mov bh,0
            mov ah,02h
            int 10h
        ;preparing registers for printing
            mov bl,01h
            mov ah,0eh
            mov di,7
            mov si,0
        DrawPLayer1Name:
            ;display the name
                mov al,[player1+si]
                int 10h
            ;moving the cursor right
                inc dl
                inc si
                dec di
        jnz DrawPLayer1Name
    ;display player2 name in BLUE
        ;moving the cursor
            mov dl,20h
            mov dh,1h
            mov bh,0
            mov ah,02h
            int 10h
        ;preparing registers for printing
            mov bl,01h
            mov ah,0eh
            mov di,7
            mov si,0
        DrawPLayer2Name:
            ;display the name
                mov al,[player2+si] ;if the seconde player  
                int 10h
            ;moving the cursor right
                inc dl
                inc si
                dec di
        jnz DrawPLayer2Name
        
    ;draw Word Score
        ;move the cursor
            mov dh,1
            mov dl,11h
            mov ah,02h
            int 10h
        ;draw 'Score'
            mov ah,0eh
            mov di,5
            mov si,0
            mov bl,8
            DrawScore:
                mov al,[Score+si]
                int 10h
                inc dl
                inc si
                dec di
            jnz DrawScore
    ;draw 'Health'
        ;move cursor
            mov dl,11h
            inc dh
            mov ah,02h
            int 10h
            mov bl,7
            mov ah,0eh
            mov di,6
            mov si,0
        DrawScore1:
            mov al,[HealthString+si]
            int 10h
            inc dl
            inc si
            dec di
        jnz DrawScore1

ENDM DRAW_STATIC 
              
DRAW_MAP MACRO COLOR1  
   
    MOV AH, 0CH
    MOV AL, COLOR1

    MOV CX, 40D
    MOV DX, STEPBEGINY
    STEP1: INT 10H
        INC CX
        CMP CX, 140D
    JNZ STEP1

    
    MOV CX, 180D
    STEP2: INT 10H
        INC CX
        CMP CX, 280D
    JNZ STEP2  


    MOV CX, 90D
    ADD DX, STEPINTERVALY
    STEP3: INT 10H
        INC CX
        CMP CX, 230D
    JNZ STEP3



    MOV CX, 40D
    ADD DX, STEPINTERVALY
    STEP4: INT 10H
        INC CX
        CMP CX, 140D
    JNZ STEP4

    
    MOV CX, 180D
    STEP5: INT 10H
        INC CX
        CMP CX, 280D
    JNZ STEP5  

ENDM DRAW_MAP 

DRAW_POWERUPS_MAC MACRO
    ;DRAW POWER UPS 
    CMP POWERUP1STATUS, 1
    JNE DONT_DRAW_POWERUP1
        MOV TEMPB,1
        CALL DRAW_POWERUPS
    DONT_DRAW_POWERUP1:

    CMP POWERUP2STATUS, 1
    JNE DONT_DRAW_POWERUP2
        MOV TEMPB,2
        CALL DRAW_POWERUPS
    DONT_DRAW_POWERUP2:

    CMP POWERUP3STATUS, 1
    JNE DONT_DRAW_POWERUP3
        MOV TEMPB,3
        CALL DRAW_POWERUPS
    DONT_DRAW_POWERUP3:
    ;END OF DRAWING POWER-UPS
ENDM DRAW_POWERUPS_MAC

CHECK_POWER_UP_COLLISION MACRO
    PUSH CX
    MOV CX, 3
    MOV TEMPB, 1
    MOV TEMPW,1
    POWERUPS_3_2:
        CALL CHECK_POWERUP_COLLISION
        INC TEMPW
    LOOP POWERUPS_3_2
    POP CX

    PUSH CX
    MOV tempW,1
    MOV CX, 3
    MOV TEMPB, 2    
    POWERUPS_3:
        CALL CHECK_POWERUP_COLLISION
        INC TEMPW
    LOOP POWERUPS_3
    POP CX
ENDM CHECK_POWER_UP_COLLISION

CHANGE_PLAYER_POSITIONS MACRO
    ;CHECK PLAYER 1 KEYS
    CMP DX1, 1 ;CHECK IF THE RIGHT ARROW IS CLICKED
    JNE NO_RIGHT1
        INC PLAYER1X
        JMP DONE_LR1
    NO_RIGHT1:

    CMP DX1, 2 ;CHECK IF THE LEFT ARROW IS CLICKED
    JNE NO_LEFT1
        DEC PLAYER1X
        JMP DONE_LR1
    NO_LEFT1:

    DONE_LR1:

    CMP DY1, 1 ;CHECK IF THE UP ARROW IS CLICKED
    JNE NO_UP1
        MOV DI, 3
        DEC_PLAYER1_Y:
            PUSH DI
            DEC PLAYER1Y
            
            MOV tempW,1
            CALL Check_Positions_Boundaries
            MOV TEMPB, 1
            CALL DRAW_PLYAERS

            POP DI
            DEC DI
            CMP DI, 0
        JNZ DEC_PLAYER1_Y
    NO_UP1:

    CMP DY1, 2 ;CHECK IF THE UP ARROW ISN'T CLICKED
    JNE NO_DOWN1
        CALL STANDING_CHECK
        CMP PLAYER1_STAND, 1
        JZ DONE_UD1
        INC PLAYER1Y
    NO_DOWN1:
    
    DONE_UD1:
    ;END OF PLAYER 1 KEYS CHECKING
    
    ;CHECK PLAYER 2 KEYS
    CMP DX2, 1 ;CHECK IF THE KEY D IS CLICKED
    JNE NO_RIGHT2
        INC PLAYER2X
    NO_RIGHT2:

    CMP DX2, 2 ;CHECK IF THE KEY A IS CLICKED
    JNE NO_LEFT2
        DEC PLAYER2X
    NO_LEFT2:

    CMP DY2, 1 ;CHECK IF THE KEY W IS CLICKED
    JNE NO_UP2
        MOV DI, 3
        DEC_PLAYER2_Y:
            PUSH DI
            DEC PLAYER2Y
            
            MOV tempW, 2
            CALL Check_Positions_Boundaries
            MOV TEMPB, 2
            CALL DRAW_PLYAERS

            POP DI
            DEC DI
            CMP DI, 0
        JNZ DEC_PLAYER2_Y
    NO_UP2:

    CMP DY2, 2 ;CHECK IF THE KEY W ISN'T CLICKED
    JNE NO_DOWN2
        CALL STANDING_CHECK
        CMP PLAYER2_STAND, 1
        JZ NO_DOWN2
        INC PLAYER2Y
    NO_DOWN2:

    ;END OF PLAYER 2 KEYS CHECKING

ENDM CHANGE_PLAYER_POSITIONS 

PLAYER_HITTING MACRO

    ;ADDED BY SHERIF TO CHECK THE ARROWS POSITIONS IN THE MAP
    CMP ARROW1_STATUS, 1
    JNE NO_ARROW1
    
    ;DIRECTIONS OF ARROWS
    INC ARROW1X
    
    MOV BX, PLAYER1X
    CMP BX, PLAYER2X
    JL NO_DEC_X1_ARROW
        SUB ARROW1X, 2
    NO_DEC_X1_ARROW:

    MOV tempW,1
    CALL CHECK_ARROWS
    NO_ARROW1:

    CMP ARROW2_STATUS,1
    JNE NO_ARROW2
    DEC ARROW2X

    MOV BX, PLAYER2X
    CMP BX, PLAYER1X
    JG NO_INC_X2_ARROW
        ADD ARROW2X, 2
    NO_INC_X2_ARROW:
    
    MOV tempW,2
    CALL CHECK_ARROWS
    NO_ARROW2:

    CMP ARROW1_STATUS,1
    JNE NOARROW1
    MOV TEMPB, 1
    CALL DRAW_ARROWS
    NOARROW1:
    
    CMP ARROW2_STATUS,1
    JNE NOARROW2
    MOV TEMPB, 2
    CALL DRAW_ARROWS
    NOARROW2:

ENDM PLAYER_HITTING 

GAMEOVER_AND_EXIT MACRO
    ;CONVERT TO VIDEO MODE 
    MOV AH, 0
    MOV AL, 3H
    INT 10H   
    ;KNOW WHO IS WINNER AND WHO IS LOSER 
    MOV CX,7
    MOV si,0
    MOV AL,SCORE1
    MOV AH,SCORE2
    CMP AL,AH
    JG ONE_WINS
    MOV SCORE_WINNER,AH
    MOV SCORE_LOSER,AL
    JMP NAMES2
    ONE_WINS:
    MOV SCORE_WINNER,AL
    MOV SCORE_LOSER,AH
    NAMES2:
        CMP SCORE_WINNER,AL
        JNE PLAYER2WINS
        MOV BH,[PLAYER1+SI]
        MOV [WINNER_NAME+SI],BH
        JMP DECLOOP
        PLAYER2WINS:
        MOV BH,[PLAYER2+SI]
        MOV [WINNER_NAME+SI],BH
        DECLOOP:
        INC si
    LOOP NAMES2
    ;DISPLAY GAME OVER STRING
        MOV AH,09
        MOV DH,0
        MOV DX,OFFSET GAME_OVER_STR
    ;MOVE CURSOR
        MOV DL,15
        MOV DH,15
        MOV AH,02h
        INT 10h
    ;DISPLAY GAME OVER STRING
        MOV AH,09
        MOV DH,0
        MOV DX,OFFSET GAME_OVER_STR
        INT 21H

        MOV AH,09
        MOV DH,0
        MOV DX,OFFSET CONGRATULATIONS
        INT 21H

        MOV AH,09
        MOV DH,0
        MOV DX,OFFSET WINNER_NAME
        INT 21H
        ;MOVE CURSOR
        MOV DL,12
        MOV DH,15
        MOV AH,02h
        INT 10h

        MOV AH,09
        MOV DH,0
        MOV DX,OFFSET SCOREIS
        INT 21H
        ;DISPLAY SCORE
        ADD SCORE_LOSER,'0'
        ADD SCORE_WINNER,'0'
        mov ah,2
        mov dl,SCORE_WINNER
        int 21h
        mov dl,':'
        int 21h
        mov dl,SCORE_LOSER
        int 21h
        
        MOV CX, 4
        ENDL_LOOP:
            MOV DL, ENDL
            INT 21H
        LOOP ENDL_LOOP
ENDM GAMEOVER_AND_EXIT  