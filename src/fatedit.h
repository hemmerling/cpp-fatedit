#ifndef FATEDIT_H
#define FATEDIT_H
/**
 *  @package   fatedit
 *  @file      fatedit.h
 *  @brief     Header file for fatedit.c, the FAT12 and FAT16 disk editor
 *  @author    Rolf Hemmerling <hemmerling@gmx.net>
 *  @version   1.00, 
 *             programming language "C",
 *             development tool chain "Microsoft Visual C++ 1.52" ( Large Model )
 *             and "Microsoft 8086 Assembler"
 *             target: IBM-PC with MS-DOS operating system
 *  @date      2015-01-01
 *  @copyright Apache License, Version 2.0
 *
 *  fatedit.h - Header file for fatedit.c, the FAT12 and FAT16 disk editor
 *
 *  Copyright 1988-2015 Rolf Hemmerling
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing,
 *  software distributed under the License is distributed on an
 *  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 *  either express or implied.
 *  See the License for the specific language governing permissions
 *  and limitations under the License.
 */

/* Activate MISRA-C checks with TI CCS */
#ifdef __TI_COMPILER_VERSION__
/**
 *  @def      CHECK_MISRA
 *  @brief    CHECK_MISRA
 */
#pragma CHECK_MISRA("all")

/**
 *  @def      _osmajor
 *  @brief    Dummy value for the major operating system version
 */
#define _osmajor 0
/**
 *  @def      _osminor
 *  @brief    Dummy value for the minor operating system version
 */
#define _osminor 0

extern char getch(void);
extern void strupr(char *);

#endif

#include <stdlib.h>
#ifdef _DOS_MODE
/* _DOS_MODE is just defined for MSC, MSVCPP */

/**
 *  @def      DOXYGEN
 *  @brief    DOXYGEN is not running
 */
#undef DOXYGEN
#else

/**
 *  @def      DOXYGEN
 *  @brief    DOXYGEN is running
 */
#define DOXYGEN
/* Defines to check conditional code */
#endif

/* Compiler selection: Just 1 compler may be defined */

/** 
 *  @def      MSVCPP
 *  @brief    Microsoft Visual C++ 1.52 ( large modell ) 
 *  
 *  For compilation with C/C++ compilers: 
 *  Do not rename the file from *.c to .cpp.
 *  Just with the file extension .c, 
 *  the file is compiled properly as "C" file according to "C" rules.
 */
#define MSVCPP

/** 
 *  @def      MSC
 *  @brief    Microsoft C Compiler 5.x ( large modell )
 */
#undef  MSC     

/** 
 *   def      QC
 *  @brief    Microsoft Quick C Compiler 1.x ( large modell )
 */
#undef  QC

/** 
 *   def      TURBPC
 *  @brief    Borland Turboc 2.x ( large modell )
 */
#undef  TURBOC

/** 
 *   def      MIXPC
 *  @brief    Mix power-c 1.2.0 ( medium modell )
 */
#undef  MIXPC

/** 
 *   def      RTEST
 *  @brief    Safe mode: If defined, then just reading, no writing back to disk
 */
#undef RTEST

/** 
 *   def      FTEST
 *  @brief    If defined, then the FAT entry number is displayed
 */
#define FTEST

/** 
 *   def      TEST1
 *  @brief    Just for testing, to do additional output
 */
#undef TEST1
/** 
 *   def      TEST2
 *  @brief    Just for testing, to do additional output
 */
#undef TEST2

/* Operating System */

/** 
 *  @def      CONDOS
 *  @brief    Any MSDOS or MSDOS clone than MSDOS 3.3
 */
#define CONDOS 0

/** 
 *  @def      MSDOS
 *  @brief    MSDOS 3.3 or PCDOS 3.3
 */
#define MSDOS  1

/** 
 *  @def      OS
 *  @brief    Selected operating system
 *
 *  A bug in PCDOS 3.3 of 1987-03-18 and MSDOS 3.3 of 1987-11-11
 *  make it necessary that before using the absread MsDos-bios
 *  function You must log on the disk in the target
 *  drive (A:), e.g "DIR A:" or "A:", and so You can't change
 *  disks from within a program like this.
 *
 *  Condos XM 6.X emulates PCDOS 3.3 without this bug
 */
#define OS CONDOS

/** 
 *  @def      LASTDR
 *  @brief    Last drive of MSDOS system
 *
 *  Please enter her the last drive, which may be accessed.
 *  This is intended as protection of harddisks, which bootsector,
 *  directory, FAT.. usually should not be manipulated, 
 *  due to lack of backup.
 *  By this, access to MSDOS 4.x drives with a capacity of more
 *  than 32 MBytes ( FAT 32 ) can be prevented too.
 */
#define LASTDR 'F'

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __TI_COMPILER_VERSION__
#include <process.h>
#include <dos.h>
#include <conio.h>
#endif

#include <signal.h>
#include <setjmp.h>
          
#ifdef MSVCPP
/** 
 *  @def      MSC
 *  @brief    Microsoft C Compiler 5.x 
 */
#define MSC
#endif

#ifdef QC
/* bei QC einstweilen alles so wie bei MSC */
/** 
 *  @def      MSC
 *  @brief    Microsoft C Compiler 5.x 
 */
#define MSC
#endif

#ifdef MIXPC
#include <malloc.h>
#include <mem.h>
/** 
 *  @def      MMODELL
 *  @brief    Medium model
 */
#define MMODELL
/** 
 *  @def      MSC_MIXPC
 *  @brief    MSC_MIXPC
 */
#define MSC_MIXPC
/** 
 *  @def      FP_XSEG(fp)
 *  @brief    FP_XSEG
 *
 *  Keep the original MIXPC definitions
 */
#define FP_XSEG(fp) (*((unsigned *)&(fp) + 1))
/** 
 *  @def      FP_XOFF(fp)
 *  @brief    FP_XOFF
 *
 *  Keep the original MIXPC definitions
 */
#define FP_XOFF(fp) (*((unsigned *)&(fp)))
/** 
 *  @def      FAR_MALLOC(LSIZE)
 *  @brief    FAR_MALLOC
 *
 *  Keep the original MIXPC definitions
 */
#define FAR_MALLOC(LSIZE) farmalloc(unsigned long(LSIZE))
#endif

#ifdef TURBOC
#include <alloc.h>
#include <mem.h>
/** 
 *   undef    MMODELL
 *  @brief    No Medium model with TURBOC
 */
#undef MMODELL
/** 
 *  @def      FP_XSEG
 *  @brief    FP_XSEG
 *
 *  Use always the TURBOC definitions
 */
#define FP_XSEG FP_SEG
/** 
 *  @def      FP_XOFF
 *  @brief    FP_XOFF
 *
 *  Use always the TURBOC definitions
 */
#define FP_XOFF FP_OFF
/** 
 *  @def      FAR_MALLOC(LSIZE)
 *  @brief    FAR_MALLOC
 *
 *  Use always the TURBOC definitions
 */
#define FAR_MALLOC(LSIZE) malloc(LSIZE)
#endif

#ifdef MSC
#ifndef __TI_COMPILER_VERSION__
#include <malloc.h>
#include <memory.h>
#endif

/** 
 *   undef    MMODELL
 *  @brief    No Medium model with MSC
 */
#undef MMODELL
#define MSC_MIXPC
/* erhalten der original MSC definitionen */
/** 
 *  @def      FP_XSEG
 *  @brief    FP_XSEG
 *
 *  Use always the MSC definitions
 */
#define FP_XSEG FP_SEG
/** 
 *  @def      FP_XOFF
 *  @brief    FP_XOFF
 *
 *  Use always the MSC definitions
 */
#define FP_XOFF FP_OFF   

#ifndef MSVCPP
/** 
 *  @def      MK_FP(seg,ofs)
 *  @brief    MK_FP
 *
 *  Make far pointer, this definition is missing in the
 *  Microsoft header files
 */
#define MK_FP(seg,ofs)  ((void far *)(((unsigned long)(seg) << 16) | (ofs)))
#endif

/** 
 *  @fn       absread (int drive, int nsects, int lsect, void *buffer)
 *  @brief    Specification of the external function "absread".
 *            This function is missing in the Microsoft C libraries
 */
int absread (int drive, int nsects, int lsect, void *buffer);

/** 
 *  @fn       abswrite (int drive, int nsects, int lsect, void *buffer)
 *  @brief    Specification of the external function "abswrite".
 *            This function is missing in the Microsoft C libraries
 */
int abswrite(int drive, int nsects, int lsect, void *buffer);
#endif

#ifdef MSC_MIXPC
#undef FP_OFF
#undef FP_SEG
/** 
 *  @def      FP_OFF(fp)
 *  @brief    FP_XOFF
 *
 *  Turboc definition, works always with MSC, works mostly with MIXPC
 */
#define FP_OFF(fp)      ((unsigned)(fp))
/** 
 *  @def      FP_SEG(fp)
 *  @brief    FP_SEG
 *
 *  Turboc definition, works always with MSC, works mostly with MIXPC
 */
#define FP_SEG(fp)      ((unsigned)((unsigned long)(fp) >> 16))
#endif

#ifdef MMODELL
/** 
 *  @typedef  FAR_PTR
 *  @brief    FAR_PTR
 *
 *  In general, (far) pointers must be "unsigned int", 
 *  not "int", 
 *  else the macro MK_FP does not work
 */
typedef void far * FAR_PTR;
#else
/** 
 *  @typedef  FAR_PTR
 *  @brief    FAR_PTR
 *
 *  In general, (far) pointers must be "unsigned int", 
 *  not "int", 
 *  else the macro MK_FP does not work
 */
typedef void * FAR_PTR;
#endif

/** 
 *  def       MAXSECSIZE
 *  @brief    Maximum size of an MSDOS sector
 */
#define MAXSECSIZE 1024

/** 
 *  @def      NORMSECSIZE
 *  @brief    Standard ( = normal ) size of an MSDOS sector
 */
#define NORMSECSIZE 512

/** 
 *  @def      MINSECSIZE
 *  @brief    Minimum size of an MSDOS sector
 */
#define MINSECSIZE 128

/** 
 *  @def      FAT16B
 *  @brief    The FAT is of type FAT16
 *
 *  The FAT16 routines are untested, 
 *  as I just had a 10 MByte harddisk at the time of implemention
 */
#define FAT16B 16

/** 
 *  @def      FAT12B
 *  @brief    The FAT is of type FAT12
 */
#define FAT12B 12

/** 
 *  @def      ERRCLUST
 *  @brief    Error cluster return value ( it is a reserved cluster value )
 */
#define ERRCLUST 0xFFF6

/** 
 *  @def      RESCLUST
 *  @brief    First reserved cluster value
 */
#define RESCLUST 0xFFF0

/** 
 *  @def      EOFAT
 *  @brief    Last value of a FAT link
 */
#define EOFAT -1

/** 
 *  @def      NOFAT
 *  @brief    Startcluster entry for volume entry
 */
#define NOFAT 0

/** 
 *  @def      WORKFAT
 *  @brief    Work FAT - the FAT which is regularly modified = 0
 */
#define WORKFAT 0

/** 
 *  @def      RESFAT
 *  @brief    Reserve FAT
 */
#define RESFAT 1

/** 
 *  @def      NLENGTH
 *  @brief    Maximum length of a classical 8+3 MSDOS filename 
 *            ( not Windows 9x, W2k, WinXP.. filename !!! )
 */
#define NLENGTH 8

/** 
 *  @def      ELENGTH
 *  @brief    Maximum length of a 
 *            classical 8+3 MSDOS filename extension
 */
#define ELENGTH 3

/** 
 *  @def      MAXERR
 *  @brief    Maximum number of error messages per error type 
 */
#define MAXERR 3

/** 
 *  @def      MAXBOOTMES
 *  @brief    Maximum number of messages for bootinfo + show
 */
#define MAXBOOTMES 12

/* Some different error types */

/** 
 *  @def      BOOTERR
 *  @brief    BOOTERR
 */
#define BOOTERR  0

/** 
 *  @def      FATALERR
 *  @brief    FATALERR
 */
#define FATALERR 1

/* Some different boot errors */

/** 
 *  @def      BREADERR
 *  @brief    BREADERR
 */
#define BREADERR     1

/** 
 *  @def      FREADERR
 *  @brief    FREADERR
 */
#define FREADERR     2

/** 
 *  @def      DREADERR
 *  @brief    DREADERR
 */
#define DREADERR     3

/** 
 *  @def      SREADERR
 *  @brief    SREADERR
 */
#define SREADERR     4

/** 
 *  @def      BWRITEERR
 *  @brief    BWRITEERR
 */
#define BWRITEERR    5

/** 
 *  @def      FWRITEERR
 *  @brief    FWRITEERR
 */
#define FWRITEERR    6

/** 
 *  @def      DWRITEERR
 *  @brief    DWRITEERR
 */
#define DWRITEERR    7

/** 
 *  @def      LOGERR
 *  @brief    LOGERR
 */
#define LOGERR       8

/** 
 *  @def      FIRSTLOG
 *  @brief    FIRSTLOG
 */
#define FIRSTLOG     9

/** 
 *  @def      WRONGFAT
 *  @brief    WRONGFAT
 */
#define WRONGFAT    10

/** 
 *  @def      WRONGFENTRY
 *  @brief    WRONGFENTRY
 */
#define WRONGFENTRY 11

/** 
 *  @def      WRONGDENTRY
 *  @brief    WRONGDENTRY
 */
#define WRONGDENTRY 12

/** 
 *  @def      WFATNUM
 *  @brief    WFATNUM
 */
#define WFATNUM     13

/** 
 *  @def      FATLOOP
 *  @brief    FATLOOP
 */
#define FATLOOP     14

/* Some different fatal errors */

/** 
 *  @def      NOMEM
 *  @brief    NOMEM
 */
#define NOMEM    1

/** 
 *  @def      VERERR
 *  @brief    VERERR
 */
#define VERERR   2

/** 
 *  @def      SIGERR
 *  @brief    SIGERR
 */
#define SIGERR   3

/** 
 *  @def      NOTIMPL
 *  @brief    NOTIMPL
 */
#define NOTIMPL  4

/** 
 *  @def      READONLY
 *  @brief    READONLY
 */
#define READONLY 5

/* Media_flag values */

/** 
 *  @def      READ_ONLY
 *  @brief    READ_ONLY
 */
#define READ_ONLY  1

/** 
 *  @def      HIDDEN
 *  @brief    HIDDEN
 */
#define HIDDEN     2

/** 
 *  @def      SYSTEM
 *  @brief    SYSTEM
 */
#define SYSTEM     4

/** 
 *  @def      VOLUME
 *  @brief    VOLUME
 */
#define VOLUME     8

/** 
 *  @def      SUBDIR
 *  @brief    SUBDIR
 */
#define SUBDIR    16

/** 
 *  @def      ARCHIVE
 *  @brief    ARCHIVE
 */
#define ARCHIVE   32

/** 
 *  @def      BIT6
 *  @brief    BIT6
 */
#define BIT6      64

/** 
 *  @def      BIT7
 *  @brief    BIT7
 */
#define BIT7     128

/* direntry_tp bit - field values */

/** 
 *  @def      YER
 *  @brief    Year
 */
#define YER 7

/** 
 *  @def      MON
 *  @brief    Month
 */
#define MON 4

/** 
 *  @def      DAY
 *  @brief    Day
 */
#define DAY 5

/** 
 *  @def      HOR
 *  @brief    Hour
 */
#define HOR 5

/** 
 *  @def      MNU
 *  @brief    Minute
 */
#define MNU 6

/** 
 *  @def      SEC
 *  @brief    Second
 */
#define SEC 5

/* Structure types */

/** 
 *  @struct   bootinfo_tp
 *  @brief    Structure of the bootsector infoblock
 */
struct bootinfo_tp
 { 
   /*@{*/
   char jmpcode; /**< jmpcode */
   int  nearjmp; /**< nearjmp */
   char oemname[NLENGTH]; /**< OEM name */
   unsigned int  bytes_per_sector; /**< bytes per sector */
   unsigned char sectors_per_cluster; /**< sectors per cluster */ 
   unsigned int  reserved_sectors; /**< reserved sectors */
   unsigned char number_of_fats; /**< number of FATs */ 
   unsigned int  number_of_direntries; /**< number of direntries */
   unsigned int  number_of_sectors; /**< number of sectors */
   unsigned char media_flag; /**< media flag */
   unsigned int  sectors_per_fat; /**< sectors per FAT */
   unsigned int  sectors_per_track; /**< sectors per track */
   unsigned int  number_of_heads; /**< number of heads */
   unsigned int  number_of_hiddensectors; /**< number of hiddensectors */
   /*@}*/
 };

/** 
 *  @struct   direntry_tp
 *  @brief    Structure of a 32-byte directory entry
 */
struct direntry_tp
  { 
    /*@{*/
    unsigned char filename[NLENGTH]; /**< filename */
    unsigned char extension[ELENGTH]; /**< extension */
    unsigned char attribute; /**< attribute */
    unsigned char reserved[10]; /**< reserved */
    unsigned int second : SEC; /**< second */
    unsigned int minute : MNU; /**< minute */
    unsigned int hour : HOR; /**< hour */
    unsigned int day :DAY; /**< day */
    unsigned int month : MON; /**< month */
    unsigned int year : YER; /**< year */
    unsigned int startcluster; /**< startcluster */
    unsigned long filelength; /**< filelength */
   /*@}*/
  };

/** 
 *  @typedef  bootsec_tp
 *  @brief    Type definition of a bootsector
 */
typedef struct bootinfo_tp bootsec_tp;

/** 
 *   typedef  fatsec_tp
 *  @brief    Type definition of a FAT sector
 *            ( QC doesn't like typedef here )
 */
#define fatsec_tp void

#ifdef  DOXYGEN
/** 
 *  @struct   dfatentry12_struct
 *  @brief    Structure definition of a double FAT entry for 12-bit FAT
 */
struct dfatentry12_struct 
{ 
  /*@{*/
  unsigned char x [3]; /**< x */
  /*@}*/
};

/** 
 *  @typedef  dfatentry12_tp
 *  @brief    Type definition of a double FAT entry for 12-bit FAT
 */
typedef struct dfatentry12_struct dfatentry12_tp;
#else
/** 
 *  @typedef  dfatentry12_tp
 *  @brief    Type definition of a double FAT entry for 12-bit FAT
 */
typedef struct 
{ 
  /*@{*/
  unsigned char x [3]; /**< x */
  /*@}*/
} dfatentry12_tp;
#endif

/** 
 *  @typedef  fatentry16_tp
 *  @brief    Type definition of a FAT entry for 16-bit FAT
 */
typedef int fatentry16_tp;

/** 
 *  @typedef  fatentry_tp
 *  @brief    More simple type definition of a FAT entry in general,
 *            of course for fatentry16_tp 
 *            there would be a type adaption necessary
 */
typedef dfatentry12_tp fatentry_tp;


/* Function declarations */

/**
 *  @fn       errormessage(int,int)
 *  @param    errtyp4
 *  @param    error4
 *	@brief    General error message 
 */
void errormessage(int,int);

/**
 *  @fn       clustosec(int,bootsec_tp *)
 *  @param    cluster
 *  @param    btptr2
 *  @return   int
 *	@brief    Conversion cluster -> sector 
 */
int clustosec(int,bootsec_tp *);

/**
 *  @fn       sectoclus(int,bootsec_tp *)
 *  @param    sector
 *  @param    btptr2
 *  @return   int
 *	@brief    Conversion sector -> cluster 
 */
int sectoclus(int,bootsec_tp *);

/**
 *  @fn       copy_fat(int,fatsec_tp *,bootsec_tp *);
 *  @param    fatnumber - nth FAT, which is the destionation FAT
 *  @param    fatptr2
 *  @param    btptr2
 *	@brief    Conversion from first to nth FAT 
 */
void copy_fat(int,fatsec_tp *,bootsec_tp *);

/**
 *  @fn       get_fatentry12(unsigned int,int,int,dfatentry12_tp *)
 *  @param    index
 *  @param    fatlength
 *  @param    fatnumber
 *  @param    fatptr2
 *  @return   unsigned int
 *	@brief    Calculate the 12-bit FAT entry, error = ERRCLUST 
 */
unsigned int get_fatentry12(unsigned int,int,int,dfatentry12_tp *);

/**
 *  @fn       put_fatentry12(unsigned int,unsigned int,int,int,
                             dfatentry12_tp *)
 *  @param    value
 *  @param    index
 *  @param    fatlength of one FAT in number of sectors 
 *  @param    fatnumber
 *  @param    fatptr2
 *  @return   unsigned int
 *	@brief    Register / enter the 12-bit FAT entry, error = ERRCLUST 
 */
unsigned int put_fatentry12(unsigned int,unsigned int,int,int,
                dfatentry12_tp *);

/**
 *  @fn       get_fatentry16(unsigned int,int,int,fatentry16_tp *)
 *  @param    index
 *  @param    fatlength of one FAT in naumber of sectors
 *  @param    fatnumber
 *  @param    fatptr2
 *  @return   unsigned int
 *	@brief    Calculate the 16-bit FAT entry, error = ERRCLUST 
 */
unsigned int get_fatentry16(unsigned int,int,int,fatentry16_tp *);

/**
 *  @fn       put_fatentry16(unsigned int,unsigned int,int,int,
                             fatentry16_tp *)
 *  @param    value
 *  @param    index
 *  @param    fatlength of one FAT in number of sectors 
 *  @param    fatnumber
 *  @param    fatptr2
 *  @return   unsigned int
 *	@brief    Register / enter the 16-bit FAT entry, error = 0xFF6 (reserved cluster) 
 */
unsigned int put_fatentry16(unsigned int,unsigned int,int,int,
                fatentry16_tp *);

/**
 *  @fn       get_bootinfo(char,bootsec_tp *)
 *  @param    drive
 *  @param    btptr2
 *  @return   int
 *	@brief    Importing / reding of the boot sector, calculate the offsets
 */
int get_bootinfo(char,bootsec_tp *);

/**
 *  @fn       put_bootinfo(char,bootsec_tp *)
 *  @param    drive
 *  @param    btptr2
 *  @return   int
 *	@brief    Exporting / writing of the boot sector
 */
int put_bootinfo(char,bootsec_tp *);

/**
 *  @fn       get_maindir(char,struct direntry_tp *)
 *  @param    drive
 *  @param    dirptr2
 *  @return   int
 *	@brief    Importing / reading of the main directory
 */
int get_maindir(char,struct direntry_tp *);

/**
 *  @fn       put_maindir(char,struct direntry_tp *)
 *  @param    drive
 *  @param    dirptr2
 *  @return   int
 *	@brief    Exporting /writing of the main directory
 */
int put_maindir(char,struct direntry_tp *);

/**
 *  @fn       get_fats(char,fatsec_tp *,bootsec_tp *)
 *  @param    drive
 *  @param    fatptr2
 *  @param    btptr2
 *  @return   int
 *	@brief    Importing / reading of all FATs
 */
int get_fats(char,fatsec_tp *,bootsec_tp *);

/**
 *  @fn       put_fats(char,fatsec_tp *,bootsec_tp *)
 *  @param    drive
 *  @param    fatptr2
 *  @param    btptr2
 *  @return   int
 *	@brief    Exporting / writing of all FATs
 */
int put_fats(char,fatsec_tp *,bootsec_tp *);

/**
 *  @fn       link_startcluster(int,struct direntry_tp *)
 *  @param    selfentry
 *  @param    dirptr2
 *	@brief    Set startcluster for directory := fatentry value
 */
void link_startcluster(int,struct direntry_tp *);

/**
 *  @fn       alloc_sector(bootsec_tp **)
 *  @param    btptr2
 *  @return   int
 *	@brief    Allocate memory for sector
 */
int alloc_sector(bootsec_tp **);

/**
 *  @fn       alloc_cluster(unsigned char **,bootsec_tp *)
 *  @param    viptr2
 *  @param    btptr2
 *  @return   int
 *	@brief    Allocate memory for cluster ( which consists of several sectors )
 */
int alloc_cluster(unsigned char **,bootsec_tp *);

/**
 *  @fn       alloc_maindir(int,struct direntry_tp **)
 *  @param    dirlength
 *  @param    dirptr2
 *  @return   int
 *	@brief    Allocate memory for main directory
 */
int alloc_maindir(int,struct direntry_tp **);

/**
 *  @fn       alloc_fats(int,fatsec_tp **)
 *  @param    fatlength
 *  @param    fatptr2
 *  @return   int
 *	@brief    Allocate memory for FATs
 */
int alloc_fats(int,fatsec_tp **);

/**
 *  @fn       realloc_maindir(int,struct direntry_tp **)
 *  @param    dirlength
 *  @param    dirptr2
 *  @return   int
 *	@brief    Reallocate memory for main directory
 */
int realloc_maindir(int,struct direntry_tp **);

/**
 *  @fn       realloc_fats(int,fatsec_tp **)
 *  @param    fatlength
 *  @param    fatptr2
 *  @return   int
 *	@brief    Reallocate memory for FATs
 */
int realloc_fats(int,fatsec_tp **);

/**
 *  @fn       realloc_cluster(unsigned char **,bootsec_tp *)
 *  @param    viptr2
 *  @param    btptr2
 *  @return   int
 *	@brief    Reallocate memory for cluster ( which consists of several sectors )
 */
int realloc_cluster(unsigned char **,bootsec_tp *);

/**
 *  @fn       display_bootinfo(bootsec_tp *)
 *  @param    btptr2
 *	@brief    Display of the bootsector informations on stdout
 */
void display_bootinfo(bootsec_tp *);

/**
 *  @fn       display_dir(int,struct direntry_tp *)
 *  @param    dirlength
 *  @param    dirptr2
 *	@brief    Display of the directory informations on stdout
 */
void display_dir(int,struct direntry_tp *);

/**
 *  @fn       display_direntry(int,struct direntry_tp *)
 *  @param    alldisp
 *  @param    dirptr2
 *	@brief    Display of a directory entry on stdout
 */
void display_direntry(int,struct direntry_tp *);

/**
 *  @fn       display_fats(int,int,int,fatentry_tp *)
 *  @param    clusternumber
 *  @param    fatlength
 *  @param    fatnumber
 *  @param    fatptr2
 *	@brief    Display of the FATs on stdout
 */
void display_fats(int,int,int,fatentry_tp *);

/**
 *  @fn       display_direntries_fatentries(int,int,int,int,fatentry_tp *,
 *                                          struct direntry_tp *)
 *  @param    alldisp
 *  @param    fatlength
 *  @param    fatnumber
 *  @param    dirlength
 *  @param    fatptr2
 *  @param    dirptr2
 *	@brief    Display of a FAT entry for each directory entry on stdout
 */
void display_direntries_fatentries(int,int,int,int,fatentry_tp *,
                struct direntry_tp *);

/**
 *  @fn       display_direntry_fatentries(int,int,int,int,fatentry_tp *,
 *                                        struct direntry_tp *)
 *  @param    alldisp
 *  @param    seldentry
 *  @param    fatlength
 *  @param    fatnumber
 *  @param    fatptr2
 *  @param    dirptr2
 *	@brief    Display of a FAT entry for one directory entry on stdout
 *            ( this function is unused in this software version )
 */
void display_direntry_fatentries(int,int,int,int,fatentry_tp *,
                struct direntry_tp *);

/**
 *  @fn       modify_fatentry(fatentry_tp *, bootsec_tp *,struct direntry_tp *)
 *  @param    fatptr2
 *  @param    btptr2
 *  @param    dirptr3
 *	@brief    Modification of FAT entry of a directory entry
 */
void modify_fatentry(fatentry_tp *, bootsec_tp *,struct direntry_tp *);

/**
 *  @fn       newlog(fatsec_tp *,bootsec_tp *,struct direntry_tp *)
 *  @param    fatptr2
 *  @param    btptr2
 *  @param    dirptr2
 *  @return   int
 *	@brief    Log onto a new disk
 */
int newlog(fatsec_tp *,bootsec_tp *,struct direntry_tp *);

/**
 *  @fn       writeback(fatsec_tp *,bootsec_tp *,struct direntry_tp *)
 *  @param    fatptr2
 *  @param    btptr2
 *  @param    dirptr2
 *  @return   int
 *	@brief    Write back to disk
 */
int writeback(fatsec_tp *,bootsec_tp *,struct direntry_tp *);

/**
 *  @fn       alloc_all
 *  @return   int
 *	@brief    Allocate memory for all buffers
 */
int alloc_all(void);

/**
 *  @fn       realloc_all
 *  @return   int
 *	@brief    Realloc memory for all buffers
 */
int realloc_all(void);

/**
 *  @fn       alloc_boot
 *	@brief    Allocate memory for bootsector, 
 *            just 1x directly after start of program
 */
void alloc_boot(void);

/**
 *  @fn       show_fat_options(int,int,int,int,fatentry_tp *,struct direntry_tp *)
 *  @param    selfentry
 *  @param    seldentry
 *  @param    fatlength
 *  @param    fatnumber
 *  @param    fatptr2
 *  @param    dirptr2
 *  @return   int
 *	@brief    Show FAT options
 */
int show_fat_options(int,int,int,int,fatentry_tp *,struct direntry_tp *);

/**
 *  @fn       enter_fatentry(int,int,int,fatentry_tp *)
 *  @param    selfentry
 *  @param    fatlength
 *  @param    fatnumber
 *  @param    fatptr2
 *  @return   int
 *	@brief    Enter a new FAT entry and store it
 */
void enter_fatentry(int,int,int,fatentry_tp *);

/**
 *  @fn       hex_view(char,unsigned char *,bootsec_tp *)
 *  @param    drive
 *  @param    viptr2
 *  @param    btptr2
 *	@brief    Read sector and display it ( hexdump + ascii ) on stdout
 *            Assumption: Sector length = 16 x ??
 */
void hex_view(char,unsigned char *,bootsec_tp *);

/**
 *  @fn       ascii_view(char,unsigned char *,bootsec_tp *)
 *  @param    drive
 *  @param    viptr2
 *  @param    btptr2
 *	@brief    Read sector and display it ( ascii ) on stdout
 *            Assumption: Sector length = 16 x ??
 */
void ascii_view(char,unsigned char *,bootsec_tp *);

/**
 *  @fn       get_fat_value(int,int,int,fatentry_tp *)
 *  @param    selfentry
 *  @param    fatlength
 *  @param    fatnumber
 *  @param    fatptr2
 *  @return   int
 *	@brief    Import / read / get the contents, which is stored in a FAT entry
 */
int get_fat_value(int,int,int,fatentry_tp *);

/**
 *  @fn       put_fat_value(int,int,int,int,fatentry_tp *)
 *  @param    fvalue
 *  @param    selfentry
 *  @param    fatlength
 *  @param    fatnumber
 *  @param    fatptr2
 *  @return   int
 *	@brief    Export / write / put the contents to a FAT entry
 */
int put_fat_value(int,int,int,int,fatentry_tp *);

/**
 *  @fn       show_direntry_options(struct direntry_tp *)
 *  @param    dirptr2
 *  @return   int
 *	@brief    Show directory entry options
 */
int show_direntry_options(struct direntry_tp *);

/**
 *  @fn       get_name(struct direntry_tp *)
 *  @param    dirptr2
 *	@brief    Show directory entry options
 */
void get_name(struct direntry_tp *);

/**
 *  @fn       get_time(struct direntry_tp *)
 *  @param    dirptr2
 *	@brief    Enter the new time for a directory entry
 */
void get_time(struct direntry_tp *);

/**
 *  @fn       get_date(struct direntry_tp *)
 *  @param    dirptr2
 *	@brief    Enter the new date for a directory entry
 */
void get_date(struct direntry_tp *);

/**
 *  @fn       change_status(struct direntry_tp *)
 *  @param    dirptr2
 *	@brief    Change the status of a directory entry
 */
void change_status(struct direntry_tp *);

/**
 *  @fn       change_attributes(struct direntry_tp *)
 *  @param    dirptr2
 *	@brief    Change the attributes of a directory entry
 */
void change_attributes(struct direntry_tp *);

/**
 *  @fn       enter_filelength(struct direntry_tp *)
 *  @param    dirptr2
 *	@brief    Enter the new file length of a directory entry
 */
void enter_filelength(struct direntry_tp *);

/**
 *  @fn       calc_filelength(int,int,fatsec_tp *,bootsec_tp *,struct direntry_tp *)
 *  @param    fatlength
 *  @param    fatnumber
 *  @param    fatptr2
 *  @param    btptr2
 *  @param    dirptr2
 *	@brief    Calculation of the maximum filelength
 */
void calc_filelength(int,int,fatsec_tp *,bootsec_tp *,struct direntry_tp *);

/**
 *  @fn       show_bootinfo(bootsec_tp *)
 *  @param    btptr2
 *	@brief    Show all bootsector values
 */
void show_bootinfo(bootsec_tp *);

/**
 *  @fn       show_maindir(struct direntry_tp *,bootsec_tp *)
 *  @param    dirptr2
 *  @param    btptr2
 *	@brief    Show the main directory
 */
void show_maindir(struct direntry_tp *,bootsec_tp *);

/**
 *  @fn       show_fats(fatsec_tp *,bootsec_tp *)
 *  @param    fatptr2
 *  @param    btptr2
 *	@brief    Show of the FAT
 */
void show_fats(fatsec_tp *,bootsec_tp *);

/**
 *  @fn       show_direntries_fatentries(fatsec_tp *,bootsec_tp *,
 *                                      struct direntry_tp *)
 *  @param    fatptr2
 *  @param    btptr2
 *  @param    dirptr2
 *	@brief    Show the FAT entries of all directory entries
 */
void show_direntries_fatentries(fatsec_tp *,bootsec_tp *,
                struct direntry_tp *);

/**
 *  @fn       modify_direntry(fatsec_tp *,bootsec_tp *,struct direntry_tp *)
 *  @param    fatptr2
 *  @param    btptr2
 *  @param    dirptr3
 *	@brief    Modification of a directory entry
 */
void modify_direntry(fatsec_tp *,bootsec_tp *,struct direntry_tp *);

/**
 *  @fn       ask_fentry(unsigned int)
 *  @param    old
 *  @return   unsigned int
 *	@brief    Enter a FAT entry
 */
unsigned int ask_fentry(unsigned int);

/**
 *  @fn       ask_dentry(unsigned int,bootsec_tp *)
 *  @param    old
 *  @param    btptr2
 *  @return   unsigned int
 *	@brief    Enter a directory entry
 */
unsigned int ask_dentry(unsigned int,bootsec_tp *);

/**
 *  @fn       change_logdrive
 *  @return   char
 *	@brief    Change a logdrive by entering a new drive letter
 */
char change_logdrive(void);

/**
 *  @fn       show_main_options
 *  @return   int
 *	@brief    Show all selectable menu items
 */
int show_main_options(void);

/**
 *  @fn       main_menu
 *	@brief    Process and execute the main menu
 */
void main_menu(void);

/**
 *  @fn       check_dos33
 *	@brief    Check if the program runs on MSDOS 3.3 or PCDOS 3.3
 *            If so, abort with an error message
 *
 *  Check if the program runs on MSDOS 3.3 and PCDOS 3.3,
 *  as there the operating system function "absread" 
 *  doesnÃ‚Â´t work without a prior execution of the "dir" 
 *  command, by COMMAND.COM, 
 *  due to a bug in this operating system
 */
void check_dos33(void);

/**
 *  @fn       title
 *	@brief    This title message is displayed at program start, one time only
 */
void title(void);

/**
 *  @fn       aborthandle
 *	@brief    Don´t care, if the user input is CTRL-C
 */
void aborthandle(void);

/**
 *  @fn       backhandle
 *	@brief    Don´t care, if the user input is CTRL-C
 */
void backhandle(void);

/**
 *  @fn       goback
 *	@brief    Jump back
 */
void goback(void);

/**
 *  @fn       main
 *	@brief    Process and execute the main menu
 */
void main(void);
#endif

