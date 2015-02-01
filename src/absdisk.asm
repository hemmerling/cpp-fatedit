;
;  @package   fatedit
;  @file      absdisk.asm
;  @brief     Access to MSDOS sectors by system functions of MSDOS
;             ( by assembly routines ) for a FAT12 and FAT16 disk editor
;  @author    Rolf Hemmerling <hemmerling@gmx.net>
;  @version   1.00, 
;             programming language "8086 Assembler",
;             development tool chain "Microsoft Visual C++ 1.52" ( Large Model )
;             and "Microsoft 8086 Assembler"
;  @date      2015-01-01
;  @copyright Apache License, Version 2.0
;
;  absdisk.asm - Access to MSDOS sectors by system functions of MSDOS
;                ( by assembly routines ) for a FAT12 and FAT16 disk editor
;
;  Copyright 1988-2015 Rolf Hemmerling
;
;  Licensed under the Apache License, Version 2.0 (the "License");
;  you may not use this file except in compliance with the License.
;  You may obtain a copy of the License at
;
;  http://www.apache.org/licenses/LICENSE-2.0
;
;  Unless required by applicable law or agreed to in writing,
;  software distributed under the License is distributed on an
;  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
;  either express or implied.
;  See the License for the specific language governing permissions
;  and limitations under the License.
;
;  Originally supported assembler: 
;  - Microsoft 8086 Assembler 5.x
;  Originally supported c-compilers: 
;  - MSC    msc 5.X large modell
;  - QC     quick-c 1.X large modell
;
;  Major development time ended 1988-12-27

;
; -------------------------------------------------------
;
;  @fn     absread(drive, nsects, sectno, buffer)
;
;  @param  int drive  - Disk drive number (a=0, b=1, etc)
;  @param  int nsects - Number of sectors
;  @param  int sectno - Logical sector number
;  @param  void *buffer  !OR!  char *buffer - Databuffer
;  @return zero if successful, -1 on error
;
; -------------------------------------------------------
;
PARM1    EQU    8
PARM2    EQU    10
PARM3    EQU    12
PARM4O   EQU    14
PARM4S   EQU    16
;

         .MODEL LARGE
         .DATA
         EXTRN _errno : word
         .CODE
         PUBLIC _absread
;
;
_absread PROC FAR
         PUSH SI
         PUSH BP
         MOV  BP,SP
         MOV  AX,[BP + PARM1]
         MOV  CX,[BP + PARM2]
         MOV  DX,[BP + PARM3]
         PUSH DS
         LDS  BX,[BP + PARM4O]
         INT  25H
         POP  BX
         POP  DS
         JB   RERROR1
         XOR  AX,AX
         JMP  SHORT RERROR2
RERROR1: MOV  [_errno],AX
         MOV  AX,-1
RERROR2: POP  BP
         POP  SI
         RET
_absread ENDP
;
; -------------------------------------------------------
;
;  @fn     abswrite(drive, nsects, sectno, buffer)
;
;  @param  int drive  - Disk drive number (a=0, b=1, etc)
;  @param  int nsects - Number of sectors
;  @param  int sectno - Logical sector number
;  @param  void *buffer  !OR!  char *buffer - Databuffer
;  @return zero if successful, -1 on error
;
; -------------------------------------------------------
;
         .MODEL LARGE
         .CODE
         PUBLIC _abswrite
;
_abswrite PROC FAR
         PUSH SI
         PUSH BP
         MOV  BP,SP
         MOV  AX,[BP + PARM1]
         MOV  CX,[BP + PARM2]
         MOV  DX,[BP + PARM3]
         PUSH DS
         LDS  BX,[BP + PARM4O]
         INT  26H
         POP  BX
         POP  DS
         JB   WERROR1
         XOR  AX,AX
         JMP  SHORT WERROR2
WERROR1: MOV  [_errno],AX
         MOV  AX,-1
WERROR2: POP  BP
         POP  SI
         RET
_abswrite ENDP
;
         END
;
