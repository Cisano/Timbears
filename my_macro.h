;Copyright 2026 Cisano Carmelo
;
;This file is part of TimBears
;
;    TimBears is free software: you can redistribute it and/or modify
;    it under the terms of the GNU General Public License as published by
;    the Free Software Foundation, either version 3 of the License, or
;    (at your option) any later version.
;
;    TimBears is distributed in the hope that it will be useful,
;    but WITHOUT ANY WARRANTY; without even the implied warranty of
;    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;    GNU General Public License for more details.
;
;    You should have received a copy of the GNU General Public License
;    along with TimBears.  If not, see <http://www.gnu.org/licenses/>.

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;MACRO - SELECT 1/2 OR VS
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        MAC SELECT_OPTION_GAME
	lda WhichBlockShow ;used for select kind of game and start, only
        bne .exit ;zero is the block for the option
        lda #2 ;FALSE
	sta NUSIZ0
	sta NUSIZ1
        lda #FALSE
        sta ShowHideCupsP0 ;set FALSE=hide----------------------------------------------------------------------------
        sta ShowHideCupsP1 ;set FALSE=hide
        lda #YELLOW
	sta COLUP0
	sta COLUP1
        sta ShowTitleScreen ;boolean - TRUE show title screen

        lda #POSITION_SINGLE_PL ;position GRP0
	ldx #POSITION_VS ;position GRP1
	jsr setPositionAndPointers ;HMOVE HMCLR inside subroutine
;**********************************************
.checkFire0
        lda TimerComeBackOptions
        bne .exit
;it shows empty space after the selection
        lda #%10000000 ;fire?
        bit INPT4
        bne .checkFire1
;**********************************************
        lda #TRUE
        sta BooleanFire0 ;necessary otherwise it starts the game when enter in 1/2 mode
        lda #FALSE ;single player 1/2
        sta GameOption
        ;lda #SPEED_ENEGRY_LOST_1_2_B
        ;sta SpeedEnergyLost
        lda #GOAL_SINGLE_PLAYER
        sta Goal ;set Goal for each round
        lda #0
        sta TimerForCountDownP0 ;no timer - wait event fire button
        sta TimerForCountDownP1 ;no timer - wait event fire button
        ldx #BLOCK_COUNTDOWN
        jmp .preExit
;**********************************************
.checkFire1
        lda #%10000000 ;fire?
        bit INPT5
        bne .exit
;**********************************************
        ;store game option - VS
        lda #TRUE ;VS
        sta GameOption
        ;lda #TRUE
        sta BooleanFire0 ;necessary otherwise it starts the game when enter in 1/2 mode
        lda #SPEED_ENEGRY_LOST_VS
        sta SpeedEnergyLost
        ;lda #GOAL_VS_ROUND
        ;sta Goal ;set Goal for complete the game
        ;set timer for both countdown
        lda #COUNTDOWN_VALUE ;value=200 - 60=1 second
        sta TimerForCountDownP0 ;set timer for countdown VS mode
        sta TimerForCountDownP1 ;set timer for countdown VS mode
        ldx #BLOCK_COUNTDOWN
.preExit
        stx WhichBlockShow ;used to jump among the blocks
        ;reset both scores
        lda #0
	sta ScorePt1P0
	sta ScorePt2P0
	sta ScorePt1P1
	sta ScorePt2P1

        lda #FALSE
        sta ShowTitleScreen
.exit
        ENDM
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;MACRO COUNTDOWN
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        MAC COUNTDOWN_AND_CUP
.setColorGRP        
;set color for countdown. it changes for the cup
;in this way not need check if.. else.. 
        lda #VERY_LIGHT_BROWN ;color for the countdown
        sta COLUP0
        sta COLUP1
;********************************
.setPosGRP
	lda #POSITION_LEFT_CUP
        ldy #0 ;GRP0
	jsr roundPosition
        lda #POSITION_RIGHT_CUP
        ldy #1 ;GRP1
	jsr roundPosition
;********************************
.setCopyGRP       
        lda #0 ;1 copy
        sta NUSIZ0
	sta NUSIZ1
;********************************
        lda TimerForCountDownP0
        beq .countdownFinishedP0 ;if zero then is in game for the next check
        cmp #1 ;last cycle before zero
        bne .hereP0
;countdown (almost) completed then set vars for new game
        lda #FALSE
        sta GameOver_P0 ;for start game, otherwise is gameover
        ;jsr initVarsBeginGameP0
.hereP0
;check for beep sound
        ldx #0 ;for Bear0
        jsr checkForBeep ;return Y for 3..2..1
        jmp .setPointersP0 ;for 3..2..1
.countdownFinishedP0
	ldx #DARK_GREEN ;possible hide cup
        lda ShowHideCupsP0 ;check if show or hide
	beq .setColorGrp0 ;0=no cup
        ;lda #SFX_VICTORY_BEAR0 ;set victory song
	;sta IdSfxP0
	ldx #YELLOW ;set visible cup
.setColorGrp0
	stx COLUP0
        ldy #0 ;cup in the TableNumCountDown
;********************************
.setPointersP0
;Number of timer or cup
        ; Y from subroutine "checkForBeep" or from above
	lda TableNumCountDownLow,y
	sta Pointer_One_Low
	lda TableNumCountDownHi,y
	sta Pointer_One_Hi
;********************************
;********************************
;now manage for P1
        lda TimerForCountDownP1
        beq .countdownFinishedP1
        cmp #1 ;last cycle before zero
        bne .hereP1
;countdown (almost) completed then set vars for new game
        lda #FALSE
        sta GameOver_P1 ;for start game, otherwise is gameover
        ;jsr initVarsBeginGameP1
.hereP1
;check for beep sound
        ldx #1 ;for Bear1
        jsr checkForBeep ;return Y value
        jmp .setPointersP1 ;for 3..2..1
.countdownFinishedP1
	ldx #DARK_GREEN ;possible hide cup
        lda ShowHideCupsP1 ;check if show or hide
	beq .setColorGrp1
        ;lda #SFX_VICTORY_BEAR1 ;set victory song
	;sta IdSfxP1
	ldx #YELLOW ;set visible cup
.setColorGrp1
	stx COLUP1
        ldy #0 ;cup in the TableNumCountDown
;********************************
.setPointersP1
;Number of timer or cup
        ; Y from subroutine "checkForBeep" or from above
        lda TableNumCountDownLow,y
	sta Pointer_Two_Low
	lda TableNumCountDownHi,y
	sta Pointer_Two_Hi
        sta WSYNC
	sta HMOVE
        ENDM
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;MACRO SCORE_BOTH_PLAYERS
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        MAC SCORE_BOTH_PLAYERS
;def pointers per score P0
	ldy ScorePt1P0
        lda TableNumbersLow,y
	sta Pointer_One_Low
	lda TableNumbersHi,y
	sta Pointer_One_Hi
;1 byte for 2 number
;11110000 decimal number
;00001111 unit number
        lda ScorePt2P0 ;11110000 becomes 00001111
        lsr
        lsr
        lsr
        lsr ;now is a number used for read its value in the table
        tay
        lda TableNumbersLow,y
	sta Pointer_Two_Low
	lda TableNumbersHi,y
	sta Pointer_Two_Hi

        lda ScorePt2P0
        and #%00001111 ;now is a number used for read its value in the table
        tay
        lda TableNumbersLow,y
	sta Pointer_Three_Low
	lda TableNumbersHi,y
	sta Pointer_Three_Hi

;def pointers per score P1
	ldy ScorePt1P1
        lda TableNumbersLow,y
	sta Pointer_Four_Low
	lda TableNumbersHi,y
	sta Pointer_Four_Hi
;1 byte for 2 number
;11110000 decimal number
;00001111 unit number
        lda ScorePt2P1 ;11110000 becomes 00001111
        lsr
        lsr
        lsr
        lsr ;now is a number used for read its value in the table
        tay
        lda TableNumbersLow,y
	sta Pointer_Five_Low
	lda TableNumbersHi,y
	sta Pointer_Five_Hi

        lda ScorePt2P1
        and #%00001111 ;now is a number used for read its value in the table
        tay
        lda TableNumbersLow,y
	sta Pointer_Six_Low
	lda TableNumbersHi,y
	sta Pointer_Six_Hi

        ldy #6
.drawSprites
;score P0
        lda (Pointer_Two_Low),y
        and #%11110000 ;remove useless decimal number
        sta Temp

        sta WSYNC

        lda (Pointer_One_Low),y
        and #%00001111
        sta GRP0

        lda (Pointer_Three_Low),y
        and #%00001111
        ora Temp
        sta GRP1
;score P1
        lda (Pointer_Five_Low),y
        and #%11110000 ;remove useless decimal number
        sta Temp

        lda (Pointer_Four_Low),y
        and #%00001111
        sta GRP0

        lda (Pointer_Six_Low),y
        and #%00001111
        ora Temp
        sta GRP1

	dey
	bpl .drawSprites
        ENDM
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;MACRO DRAW_GRP_POSSIBLE_MISSING_TRUNK
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        MAC DRAW_GRP_POSSIBLE_MISSING_TRUNK
        ldx #7 ;8 lines with bear and possible missing trunk
.drawBears
	sta WSYNC
        lda CutSectionOfTrunkOne
	sta PF1
	sta PF2
	lda BearOne,y
	sta GRP0
	sta GRP1
	lda ColorBearOne,y
	sta COLUP0
	lda ColorBearTwo,y
	sta COLUP1

        SLEEP #12
        lda CutSectionOfTrunkTwo
	sta PF1
	sta PF2
	dey
        dex
	bpl .drawBears
        ENDM
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;MACRO DRAW GRP MISSILE AND BALL
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        MAC DRAW_GRP_MISSILE_AND_BALL
        lda #4 ;5 lines with missiles
        sta Temp
        ldx #2 ;enable missile and ball
.drawBears
        lda CutSectionOfTrunkOne
	sta WSYNC
	sta PF1
	sta PF2
	lda BearOne,y
	sta GRP0
	sta GRP1
	lda ColorBearOne,y
	sta COLUP0
        stx ENAM1
	lda ColorBearTwo,y
	sta COLUP1
        stx ENABL;ENAM0
        SLEEP #6
        lda CutSectionOfTrunkTwo
	sta PF1
	sta PF2
	dey
        dec Temp
	bpl .drawBears
        ENDM
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;MACRO
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        MAC DRAW_GRP_AND_BRANCHES
.drwawBranches
	ldx CursorArrayBranches
        lda #2 ;3 lines of brunch
        sta Temp
.loopDrawBranches
	sta WSYNC
	lda ArraySideLeftTreeOne,x
	sta PF1
	lda ArraySideRightTreeOne,x
	sta PF2
        
        lda BearOne,y
	sta GRP0
	sta GRP1
	lda ColorBearOne,y
	sta COLUP0
	lda ColorBearTwo,y
	sta COLUP1

	lda ArraySideLeftTreeTwo,x
	sta PF1
	lda ArraySideRightTreeTwo,x
	sta PF2
        dey
        dec Temp
        bpl .loopDrawBranches
        ENDM
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; MACRO sound generator
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        MAC PLAY_SOUND_MACRO
;CHANEL 0 for P0
.chanel0
        lda TempoCH0
        beq .getSfxP0 ;select new sfxg if sfx is over
        dec TempoCH0 ;ttl musical note
        bne .endCH0 ;check if musical note is over
.continuePlayCH0
        jsr SoundEffectCH0 ;play new musical note
        jmp .endCH0
.getSfxP0
        ldy IdSfxP0
        bmi .endCH0
        lda TableSfxLow,y
        sta SoundDataLow0
        lda TableSfxHi,y
        sta SoundDataHi0
        lda #0
        sta CursorSound0
        jsr SoundEffectCH0
.endCH0
        lda #255
        sta IdSfxP0
;**********************************************************
;CHANEL 1 for P1
.chanel1
        lda TempoCH1
        beq .getSfxP1 ;select new sfxg if sfx is over
        dec TempoCH1 ;ttl musical note
        bne .endCH1 ;check if musical note is over
.continuePlayCH1
        jsr SoundEffectCH1 ;play new musical note
        jmp .endCH1
;inizialize one of the songs
.getSfxP1
        ldy IdSfxP1
        bmi .endCH1
        lda TableSfxLow,y
        sta SoundDataLow1
        lda TableSfxHi,y
        sta SoundDataHi1
        lda #0
        sta CursorSound1
        jsr SoundEffectCH1
.endCH1
        lda #255
        sta IdSfxP1
    ENDM

;**********************************************************
;DRAW MULTIPLE BUSHES!!!!
;**********************************************************
        MAC MULTIPLE_SPRITE
.drawSprites
	sta WSYNC
        sta RESP1
	lda Bush,y
	sta GRP0
	lda Bush,y
	sta GRP1
        nop
        sta RESP1
        sta RESP0
        sta RESP1
        sta RESP0
        sta RESP1
        sta RESP0
        sta RESP1
        sta RESP0
        sta RESP1
        sta RESP0
        sta RESP1
        sta RESP0
        sta RESP1
        sta RESP0
        sta RESP1
        sta RESP0
	dey
	bpl .drawSprites
        ENDM