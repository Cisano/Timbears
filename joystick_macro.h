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
        MAC JOYSTICK_MACRO
verifyGameOverP0
        lda GameOver_P0
        beq preCheckJoy0
        jmp verifyGameOverP1
preCheckJoy0
        lda BooleanJoy0 ;check if joy0 direction was pressed
        beq CheckJoy0Left ;if released checkjoy0
;verify if joy0 has not directions pressed
        lda #%01000000 ;left?
        bit SWCHA
        beq verifyGameOverP1 ;preCheckJoy1
        lda #%10000000 ;right?
        bit SWCHA
        beq verifyGameOverP1 ;preCheckJoy1
;if joy0 has not direction, reset the flag
        lda #FALSE ;joy0 has no direction
        sta BooleanJoy0 ;reset flag

CheckJoy0Left
;necessary only here for P0
        lda BarTimeP0 ;if BarTime is 0 the GameOver for only P0
        beq verifyGameOverP1 ;preCheckJoy1
;necessary only here for P0
        lda TimerFreezeP0 ;collision then P0 freezed
        bne verifyGameOverP1 ;preCheckJoy1
;*************************************
        lda #%01000000 ;left?
	bit SWCHA
        bne CheckJoy0Right
;*******increase bar of eneregy*******
        sec
        ror BarTimeP0
;*******increase bar of eneregy*******
        lda #TRUE
        sta BooleanJoy0 ;joy keeps its direction
        lda #SFX_PISTOL_BEAR_LEFT
        sta IdSfxP0

        lda #FALSE
        sta ReflectP0
        lda #POSITION_LEFT_P0 ;left side of the trunk
	sta CoordXBearP0

        jsr deleteTrunkAndBranchTreeOne
        ;jsr updateScoreAndBranchesPL0 ;this is now after VBLANC is ON
        jmp verifyGameOverP1 ;preCheckJoy1

CheckJoy0Right
	lda #%10000000 ;right?
	bit SWCHA
        bne verifyGameOverP1 ;preCheckJoy1
;*******increase bar of eneregy*******
        sec
        ror BarTimeP0
;*******increase bar of eneregy*******

        lda #TRUE
        sta BooleanJoy0 ;joy keeps its direction
        lda #SFX_PISTOL_BEAR_LEFT
        sta IdSfxP0

        lda #TRUE
        sta ReflectP0
        lda #POSITION_RIGHT_P0 ;right side of the trunk
	sta CoordXBearP0

        jsr deleteTrunkAndBranchTreeOne
        ;jsr updateScoreAndBranchesPL0 ;this is now after VBLANC is ON
;*********************************************************

verifyGameOverP1
        lda GameOver_P1
        beq preCheckJoy1
        jmp .endJoy
preCheckJoy1
;necessary only here for P1
        lda BarTimeP1 ;if BarTime is 0 the GameOver for only P1
        beq .endJoy
;necessary only here for P1
        lda TimerFreezeP1 ;collision then P1 freezed
        bne .endJoy
;*************************************
        lda BooleanJoy1 ;check if joy1 direction was pressed
        beq CheckJoy1Left ;if released checkjoy0
;verify if joy1 has not directions pressed
        lda #%00000100 ;left?
        bit SWCHA
        beq .endJoy
        lda #%00001000 ;right?
        bit SWCHA
        beq .endJoy
;if joy1 has not direction, reset the flag
        lda #FALSE ;joy1 has no direction
        sta BooleanJoy1 ;reset flag

CheckJoy1Left
        lda #%00000100 ;left?
	bit SWCHA
        bne CheckJoy1Right
;*******increase bar of eneregy*******
        sec
        ror BarTimeP1
;*******increase bar of eneregy*******
        lda #TRUE
        sta BooleanJoy1 ;joy keeps its direction
        lda #SFX_PISTOL_BEAR_RIGHT
        sta IdSfxP1

        lda #FALSE
        sta ReflectP1
        lda #POSITION_LEFT_P1 ;left side of the trunk
	sta CoordXBearP1

        jsr deleteTrunkAndBranchTreeTwo
        ;jsr updateScoreAndBranchesPL1 ;this is now after VBLANC is ON
CheckJoy1Right
	lda #%00001000 ;right?
	bit SWCHA
        bne .endJoy
;*******increase bar of eneregy*******
        sec
        ror BarTimeP1
;*******increase bar of eneregy*******
        lda #TRUE
        sta BooleanJoy1 ;joy keeps its direction
        lda #SFX_PISTOL_BEAR_RIGHT
        sta IdSfxP1

        lda #TRUE
        sta ReflectP1
        lda #POSITION_RIGHT_P1 ;right side of the trunk
	sta CoordXBearP1

        jsr deleteTrunkAndBranchTreeTwo
        ;jsr updateScoreAndBranchesPL1 ;this is now after VBLANC is ON
.endJoy
        ENDM

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        MAC FIREBUTTON_MACRO
;********CHECK FIRE WHEN IN 1/2 MODE********
.preCheckFire0
        lda BooleanFire0 ;check if Fire0 was pressed
        beq .checkFire0 ;if released checkFire0

        lda #%10000000 ;fire?
        bit INPT4
        beq .preCheckFire1
        lda #FALSE
        sta BooleanFire0

.checkFire0
        lda TimerFreezeP0 ;check if P0 freezed
        bne .preCheckFire1

        lda GameOver_P0 ;if TRUE means game over
        beq .preCheckFire1 ;if in game not check fire0

        lda TimerForCountDownP0
        bne .preCheckFire1

        lda #%10000000 ;fire?
        bit INPT4
        bne .preCheckFire1

        lda #TRUE
        sta BooleanFire0

        lda #COUNTDOWN_VALUE ;value=200 - 60=1 second
        sta TimerForCountDownP0 ;set timer for countdown
        ;reset score
        lda #0 ;reset here to keep old score
	sta ScorePt1P0
	sta ScorePt2P0
        jsr initVarsBeginGameP0 ;so reset them

.preCheckFire1
        lda BooleanFire1 ;check if Fire1 was pressed
        beq .checkFire1 ;if released checkFire1

        lda #%10000000 ;fire?
        bit INPT5
        beq .endFire
        lda #FALSE
        sta BooleanFire1

.checkFire1
        lda TimerFreezeP1 ;check if P1 freezed
        bne .endFire

        lda GameOver_P1
        beq .endFire

        lda TimerForCountDownP1
        bne .endFire

        lda #%10000000 ;fire?
        bit INPT5
        bne .endFire

        lda #TRUE
        sta BooleanFire1

        lda #COUNTDOWN_VALUE ;value=200 - 60=1 second
        sta TimerForCountDownP1 ;set timer for countdown
        ;reset score
        lda #0 ;reset here to keep old score
        sta ScorePt1P1
	sta ScorePt2P1
        jsr initVarsBeginGameP1 ;so reset them
.endFire
        ENDM