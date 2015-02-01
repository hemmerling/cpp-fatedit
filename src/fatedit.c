/**
 *  @package   fatedit
 *  @file      fatedit.c
 *  @brief     FAT12 and FAT16 Disketten-Editor 
 *             for editing the File Allocation Table ( FAT ) and the directory 
 *             of MSDOS disks with 12-bit FAT ( FAT12 )
 *             and - untested - of harddisks with 16-bit FAT ( FAT16 ).
 *             It is also useful for manual reconstruction of
 *             a FAT after its destruction
 *             It is not useful for harddisks with 32-bit FAT ( FAT 32 )
 *  @author    Rolf Hemmerling <hemmerling@gmx.net>
 *  @version   1.00, 
 *             programming language "C",
 *             development tool chain "Microsoft Visual C++ 1.52" ( Large Model )
 *             and "Microsoft 8086 Assembler",
 *             target: IBM-PC with MS-DOS operating system
 *  @date      2015-01-01
 *  @copyright Apache License, Version 2.0
 *
 *  fatedit.c - FAT12 and FAT16 disk editor
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
 *
 *  Inspired by an Apple II+ program "Bag of Tricks"
 *  by D.Worth and P.Lechner
 *
 *  For operating systems:
 *  - MSDOS msdos 2.x,3.0,3.1,3.2,4.x, pcmos-386 1.02,2.10,
 *      pcditto 3.64/atari with msdos
 *  - CDOS  condos 4.x,5.x,6.x
 *
 *  Compatible with
 *   - all disks (with 12-bit FAT) with complete bootsector
 *     informations
 *   - harddisks (with 12- and 16-bit FAT)
 *   - VDISK (with 128,256,512 byte sectors, with 12-bit FAT)
 *  NOT compatible with
 *   - PLANTRON ramdisk (which emulates drive C: or D:, wrong bootsector
 *     informations
 *   - disks with a media byte in the bootsector only
 *   - ATARI TOS disks
 *
 *  The FAT16 routines are untested, 
 *  as I just had a 10 MByte harddisk at the time of implemention
 *
 *  Major development time 1988-09-27 - 1988-11-01, 1988-12-29 - 1989-01-19
 */

#include "fatedit.h"

/* Globale variables */

/** 
 *  @var      first_log
 *  @brief    After successful reading of bootsector + FAT +
 *            directory, it is set back to 0
 */
int first_log = !0;

/** 
 *  @var      bootlog_ok
 *  @brief    After successful reading of bootsector, 
 *            it is set back to 0
 */
int bootlog_ok  = 0;

/** 
 *  @var      log_ok
 *  @brief    After successful login ( i.e. no reading error ),
 *            it is set to "true"
 */
int log_ok = 0;

/** 
 *  @var      fattyp
 *  @brief    Disk which is logged in, has 12- or 12-bit FAT?
 */
int fattyp = FAT12B;

/** 
 *  @var      buffer
 *  @brief    Jump buffer variable for function "goback" 
 *            This variable is used for aborting the program,
 *            together with the global variable "xxx"
 */
jmp_buf buffer;

/** 
 *  @var      xxx
 *  @brief    This variable is used for aborting the program,
 *            together with the global variable "buffer"
 */
int xxx;

/* Pointers */

/** 
 *  @var      btptr
 *  @brief    Pointer to the bootsector
 */
bootsec_tp *btptr = NULL;

/** 
 *  @var      dirptr
 *  @brief    Pointer to the main directory view-sector
 */
struct direntry_tp *dirptr = NULL;

/** 
 *  @var      fatptr
 *  @brief    Pointer to the FAT
 */
fatsec_tp *fatptr = NULL;

/** 
 *  @var      viptr
 *  @brief    Pointer to the view-sector
 */
unsigned char *viptr = NULL;

/** 
 *  @var      drive
 *  @brief    Default drive, which shall be processed
 */
char drive = 'A';

/** 
 *  @var      lastdrive
 *  @brief    Last drive, which shall be processed
 */
char lastdrive = LASTDR;

/** 
 *  @var      secsize
 *  @brief    Sectorsize = 128,256,512,1024
 */
int secsize = MINSECSIZE;

/** 
 *  @var      dirsecs
 *  @brief    Number of sectors for all directories together
 */
int dirsecs = 0;

/** 
 *  @var      fatsecs
 *  @brief    Number of sectors for all FATs together
 */
int fatsecs = 0;

/** 
 *  @var      offsecs
 *  @brief    Offset for calculation sector->cluster
 */
int offsecs = 0;

/** 
 *  @var      clusters
 *  @brief    Number of clusters on the disk
 */
int clusters = 0;

/** 
 *  @var      dir_entry
 *  @brief    Selected directory entry
 */
unsigned int dir_entry = 0;

/** 
 *  @var      fat_entry
 *  @brief    Selected FAT entry
 */
unsigned int fat_entry = 0;

/** 
 *  @var      st_fat_entry
 *  @brief    Temporary storage for a FAT entry
 */
int st_fat_entry = 0;

/** 
 *  @var      st_fat_value
 *  @brief    Temporary storage for the value of a FAT entry
 */
int st_fat_value = 0;

/* Data */

/** 
 *  @var      errormessages
 *  @brief    2-dimensional list of error messages
 */
char *errormessages[2][16] =
 { {"No error",
    "Can't read bootsector",
    "Can't read FAT",
    "Can't read root directory",
    "Can't read sector",
    "Can't write bootsector",
    "Can't write FAT",
    "Can't write root directory",
    "Login Error",
    "You must first log onto a disk",
    "Only 12- and 16-bit FATs are supported",
    "Wrong FAT entry value - not accepted",
    "Wrong DIR entry value - not accepted",
    "Can't copy to FAT",
    "FAT loop error "
     },
   {"No error",
    "Can't allocate enough memory",
    "Please boot another DOS version, this one (3.3) has fatal errors",
    "Can't set SIGNAL handler",
    "Not yet implemented",
    "Read-only demo version - You can't destroy the disk media structure"
   }
 };

/** 
 *  @var      bootinfomessages
 *  @brief    list of boot information messages
 */
char *bootinfomessages[] =
 { "OEM name and -version",
   "bytes per sector",
   "sectors per cluster",
   "reserved sectors",
   "number of FATs",
   "number of directory entries in the main directory",
   "number of sectors on the disk",
   "number of clusters on the disk",
   "media-flag",
   "sectors per FAT",
   "sectors per track",
   "number of heads",
   "number of hidden sectors"
 };
 
 /* Function specifications */

void errormessage(errtyp4,error4)
 int errtyp4,error4;
 { fprintf(stderr,">>%s<< \n",errormessages[errtyp4][error4]);
 }

int clustosec(cluster,btptr2)
 int cluster;
  bootsec_tp * btptr2;
 { int sector;
   if (offsecs == 0)
    {sector = -1;} /* error condition, no bootinfo available */
   else
    {sector = ((cluster-2) * (*btptr2).sectors_per_cluster) + offsecs;};
   return(sector);
 }

int sectoclus(sector,btptr2)
 int sector;
 bootsec_tp * btptr2;
 { int cluster;
   if (offsecs == 0)
    {cluster = 0xFF6;} /* error condition, no bootinfo available */
   else
    {cluster = ((sector - offsecs)/ (*btptr2).sectors_per_cluster) + 2;};
   return(cluster);
 }

void copy_fat(fatnumber,fatptr2,btptr2)
 int fatnumber; 
 fatsec_tp * fatptr2;
 bootsec_tp * btptr2;
  { fatsec_tp *fatptr3;
    int fatlength; /* of one FAT in number of sectors */
    int i;
    fatlength = (*btptr2).sectors_per_fat;
    if ((fatnumber < (* btptr2).number_of_fats) && (fatnumber > 0))
     { fatptr3 = ( (char *)fatptr2 + fatlength*fatnumber*secsize);
       fatptr2 = ( (char *)fatptr2 + fatlength*WORKFAT*secsize);
       for (i = 0;i < (fatlength * secsize);i++)
    { *(char *)fatptr3 = *(char *)fatptr2;};
     }
    else
    {errormessage(BOOTERR,WFATNUM);};
  }

unsigned int get_fatentry12(index,fatlength,fatnumber,fatptr2)
 unsigned int index;
 int fatlength; /* of one FAT in number of sectors */
 int fatnumber;
 dfatentry12_tp * fatptr2;
  { unsigned int fentry;
#ifdef TEST1
        printf(" fatptr %x : %x \n",FP_SEG(fatptr2),FP_OFF(fatptr2));
#endif
    if (index > ((fatlength * secsize) << 1) / 3 )
     { return(ERRCLUST);
     };
    fatptr2 = (dfatentry12_tp *)
       ( (char *)fatptr2 + fatlength*fatnumber*secsize);
#ifdef TEST1
        printf(" maxindex %d ",((fatlength * secsize) << 1) / 3);
        printf(" fatptr %x : %x \n",FP_SEG(fatptr2),FP_OFF(fatptr2));
        printf(" fatdentry %x %x %x \n",
                        (int)(*(fatptr2+(index>>1))).x[0],
                        (int)(*(fatptr2+(index>>1))).x[1],
                        (int)(*(fatptr2+(index>>1))).x[2]);
#endif
        if (index % 2)
         { /* ungerade */
           fentry = ((( (int)(*(fatptr2+(index>>1))).x[2]) << 8)>>4) +
                    ((( (int)(*(fatptr2+(index>>1))).x[1] & 0xF0))>>4);
         }
        else
         { /* gerade */
           fentry =  (( (int)(*(fatptr2+(index>>1))).x[1] <<12) >>4)
                   +  ( (int)(*(fatptr2 +(index>>1))).x[0] & 0xFF);
         };
        return(fentry);
  }

unsigned int put_fatentry12(value,index,fatlength,fatnumber,fatptr2)
 unsigned int index,value;
 int fatlength; 
 int fatnumber;
 dfatentry12_tp * fatptr2;
  {
#ifdef TEST1
    printf(" fatptr %x : %x \n",FP_SEG(fatptr2),FP_OFF(fatptr2));
    printf("value : %x \n",value);
#endif
    if (value < clusters)
     { if (index > ((fatlength * secsize) << 1) / 3 )
     { return(ERRCLUST);
     };
       if (value > ((fatlength * secsize) << 1) / 3 )
     { return(ERRCLUST);
     };
     }
    else
     { if (value < RESCLUST)
     { return(ERRCLUST);
     };
     };
    fatptr2 = (dfatentry12_tp *)
       ( (char *)fatptr2 + fatlength*fatnumber*secsize);
    if (index % 2)
     { /* uneven */
       (*(fatptr2+(index>>1))).x[2] = (unsigned char)((value>>4) & 0xFF);
           (*(fatptr2+(index>>1))).x[1] =
        ((*(fatptr2+(index>>1))).x[1] & 0x0F) + (unsigned char)((value & 0xF)<<4);
         }
        else
         { /* even */
           (*(fatptr2+(index>>1))).x[1] =
        ((*(fatptr2+(index>>1))).x[1] & 0xF0) +
        ((value >> 8) & 0x0F);
       (*(fatptr2+(index>>1))).x[0] = (unsigned char) (value & 0xFF);
     };
    return(value);
  }

unsigned int get_fatentry16(index,fatlength,fatnumber,fatptr2)
 unsigned int index;
 int fatlength;
 int fatnumber;
 fatentry16_tp * fatptr2;
  { int fentry;
    if (index > ((fatlength * secsize)>>1))
     { return(ERRCLUST);
     };
    fatptr2 = (fatentry16_tp *)
       ( (char *)fatptr2 + fatlength*fatnumber*secsize);
    fentry = *(fatptr2+index);
    return(fentry);
  }

unsigned int put_fatentry16(value,index,fatlength,fatnumber,fatptr2)
 unsigned int value,index;
 int fatlength;
 int fatnumber;
 fatentry16_tp * fatptr2;
  { if (value < clusters)
     { if (index > ((fatlength * secsize)>>1))
     { return(ERRCLUST);
     };
       if (value > ((fatlength * secsize)>>1))
     { return(ERRCLUST);
     };
     }
    else
     { if (value < RESCLUST)
     { return(ERRCLUST);
     };

     };
    fatptr2 = (fatentry16_tp *)
       ( (char *)fatptr2 + fatlength*fatnumber*secsize);
    *(fatptr2+index) = value;
    return(value);
  }

int get_bootinfo(drive,btptr2)
 char drive;
 bootsec_tp * btptr2;
 { int error2;
   if ((error2 = absread((int)(toupper(drive) - 'A'),1,0,btptr2)) == NULL)
      { dirsecs = ((*btptr2).number_of_direntries << 5) >> 9;
    fatsecs = (*btptr2).number_of_fats * (*btptr2).sectors_per_fat;
    offsecs = dirsecs + fatsecs + (*btptr2).reserved_sectors;
    clusters = sectoclus((*btptr2).number_of_sectors,btptr2);
    secsize = (*btptr2).bytes_per_sector;
    if ((*btptr2).number_of_sectors >= 20740)
     { fattyp = FAT16B;
     }
    else
     { fattyp = FAT12B;
     };
      }
   else
         { dirsecs = 0;
           fatsecs = 0;
           offsecs = 0;
         };
   return (error2);
 }

int put_bootinfo(drive,btptr2)
 char drive;
 bootsec_tp * btptr2;
 { int error2;
   error2 = abswrite((int)(toupper(drive) - 'A'),1,0,btptr2);
   return (error2);
 }

int get_maindir(drive,dirptr2)
 char drive;
 struct direntry_tp *dirptr2;
 { int error2;
   /* first logical sector = sector 0 !! */
#ifdef TEST1
   printf("offsecs %d dirsecs %d \n",offsecs,dirsecs);
#endif
   error2 =
      (absread((int)(toupper(drive) - 'A'),dirsecs,offsecs-dirsecs,dirptr2)
      != NULL);
   return (error2);
 }

int put_maindir(drive,dirptr2)
 char drive;
 struct direntry_tp *dirptr2;
 { int error2;
   /* first logical sector = sector 0 !! */
#ifdef TEST1
   printf("offsecs %d dirsecs %d \n",offsecs,dirsecs);
#endif
   error2 =
      (abswrite((int)(toupper(drive) - 'A'),dirsecs,offsecs-dirsecs,dirptr2)
      != NULL);
   return (error2);
 }

int get_fats(drive,fatptr2,btptr2)
 char drive;
 fatsec_tp *fatptr2;
 bootsec_tp *btptr2;
 { int error2;
   /* first logical sector = sector 0 !! */
   error2 = (absread((int)(toupper(drive) - 'A'),
           fatsecs,(*btptr2).reserved_sectors,fatptr2) != NULL);
   return (error2);
 }

int put_fats(drive,fatptr2,btptr2)
 char drive;
 fatsec_tp *fatptr2;
 bootsec_tp *btptr2;
 { int error2;
   /* first logical sector = sector 0 !! */
   error2 = (abswrite((int)(toupper(drive) - 'A'),
                                 fatsecs,(*btptr2).reserved_sectors,fatptr2) != NULL);
   return (error2);
 }

void link_startcluster(selfentry,dirptr2)
 int selfentry;
 struct direntry_tp * dirptr2;
 { int c;
   printf ("Do You really want to modify the directory ? Y/N ");
   c = getch();
   printf("\n");
   if (toupper(c))
    { (* dirptr2).startcluster = selfentry; };
 }

int alloc_sector(btptr2)
 bootsec_tp **btptr2;
 { *btptr2 = malloc(secsize);
   return(*btptr2 == NULL);
 }

int alloc_cluster(viptr2,btptr2)
 unsigned char **viptr2;
 bootsec_tp * btptr2;
 { *viptr2 = malloc(secsize * (*btptr2).sectors_per_cluster);
   return(*viptr2 == NULL);
 }

int realloc_cluster(viptr2,btptr2)
 unsigned char **viptr2;
 bootsec_tp * btptr2;
 { *viptr2 = realloc(*viptr2,secsize * (*btptr2).sectors_per_cluster);
   return(*viptr2 == NULL);
 }

int alloc_maindir(dirlength,dirptr2)
 int dirlength;
 struct direntry_tp **dirptr2;
 { *dirptr2 = calloc(dirlength,sizeof(struct direntry_tp));
   return(*dirptr2 == NULL);
 }

int realloc_maindir(dirlength,dirptr2)
 int dirlength;
 struct direntry_tp **dirptr2;
 { *dirptr2 = realloc(*dirptr2,dirlength * sizeof(struct direntry_tp));
   return(*dirptr2 == NULL);
 }

int alloc_fats(fatlength,fatptr2)
 int fatlength;
 fatsec_tp **fatptr2;
 { *fatptr2 = calloc(fatlength,secsize);
   return(*fatptr2 == NULL);
 }

int realloc_fats(fatlength,fatptr2)
 int fatlength;
 fatsec_tp **fatptr2;
 { *fatptr2 = realloc(*fatptr2,fatlength * secsize);
   return(*fatptr2 == NULL);
 }

void display_bootinfo(btptr2)
 bootsec_tp * btptr2;
 {
 printf("%s :  %s \n",bootinfomessages[0],(*btptr2).oemname);
 printf("%s :  %d \n",bootinfomessages[1],(*btptr2).bytes_per_sector);
 printf("%s :  %d \n",bootinfomessages[2],(*btptr2).sectors_per_cluster);
 printf("%s :  %d \n",bootinfomessages[3],(*btptr2).reserved_sectors);
 printf("%s :  %d \n",bootinfomessages[4],(*btptr2).number_of_fats);
 printf("%s : $%x \n",bootinfomessages[5],(*btptr2).number_of_direntries);
 printf("%s : $%x \n",bootinfomessages[6],(*btptr2).number_of_sectors);
 printf("%s : $%x \n",bootinfomessages[7],clusters);
 printf("%s : %x \n",bootinfomessages[8],(unsigned int)(*btptr2).media_flag);
 printf("%s : %d \n",bootinfomessages[9],(*btptr2).sectors_per_fat);
 printf("%s : %d \n",bootinfomessages[10],(*btptr2).sectors_per_track);
 printf("%s : %d \n",bootinfomessages[11],(*btptr2).number_of_heads);
 printf
  ("%s : %d \n",bootinfomessages[11],(*btptr2).number_of_hiddensectors);
 }

void display_direntry(alldisp,dirptr2)
 struct direntry_tp * dirptr2;
 int alldisp;
 { int k;
   char flags[11];

      if (( (* dirptr2).filename[0] != 0x00) || alldisp )
        { strcpy(flags,"           ");
          if ( (* dirptr2).filename[0] == 0x00)
            {       printf("<empty>");}
          else
            { if ( (* dirptr2).filename[0] == 0xE5)
              { printf("<del>  ");}
              else
              { printf("<used> ");};
            };
              if ((* dirptr2).attribute & READ_ONLY)
                                   { flags[0] = 'R';
                                   }
              if ((* dirptr2).attribute & HIDDEN)
                                   { flags[1] = 'H';
                                   }
              if ((* dirptr2).attribute & SYSTEM)
                                   { flags[2] = 'S';
                                   }
              if ((* dirptr2).attribute & VOLUME)
                                   { flags[3] = 'V';
                                   }
              if ((* dirptr2).attribute & SUBDIR)
                                   { flags[4] = 'D';
                                         flags[9] = '/';
                                   }
              if ((* dirptr2).attribute & ARCHIVE)
                                   { flags[5] = 'A';
                                   }
              if ((* dirptr2).attribute & BIT6)
                   { flags[6] = '6';
                                   }
              if ((* dirptr2).attribute & BIT7)
                   { flags[7] = '7';
                                   }

              printf("%s",flags);
          if ( (* dirptr2).filename[0] == 0xE5)
        { printf("?");
          for (k = 1;k<NLENGTH;k++)
           { printf ("%c",(* dirptr2).filename[k]);
           };
        }
          else
        { /* here empty entries are not yet considered */
          for (k = 0;k<NLENGTH;k++)
           { printf ("%c",(* dirptr2).filename[k]);
           };
        };
          printf(".");
          for (k = 0;k<ELENGTH;k++)
           { printf ("%c",(* dirptr2).extension[k]);
           };
          printf("   $(%8lx)  %2d.%2d.%2d  %2d.%2d.%2d",
          (* dirptr2).filelength,
          (* dirptr2).day,
          (* dirptr2).month,
          (* dirptr2).year + 80,
          (* dirptr2).hour,
          (* dirptr2).minute,
          (* dirptr2).second);
              printf(" \n");
            };
 }

void display_dir(dirlength,dirptr2)
 int dirlength;
 struct direntry_tp * dirptr2;
 { int i;
   for (i = 0;i< dirlength;dirptr2++,i++)
    { if ( (* dirptr2).filename[0] != 0x00)
       { printf("->$(%4x):",i);};
      display_direntry(0,dirptr2);
    };
 }

void display_fats(clusternumber,fatlength,fatnumber,fatptr2)
 int fatlength,fatnumber,clusternumber;
 fatentry_tp * fatptr2;
 { int i,cl,k;;
   k = 0;
   for (i=0;i<clusternumber;i++)
     { switch (fattyp)
        { case FAT12B:
        {cl = get_fatentry12(i,fatlength,fatnumber,fatptr2);break;}
          case FAT16B:
        {cl = get_fatentry16(i,fatlength,fatnumber,
                     (fatentry16_tp *)fatptr2);break;}
          default:
        {errormessage(BOOTERR,WRONGFAT);exit(1);break;};
        };
#ifdef FTEST
       if ((k % 8) == 0)
          { printf("\n->$(%5x):",k); };
       printf ("%5x",cl);
       k++;

#else
           printf ("%5x",cl);
#endif
         };
   printf("\n");
 }

void display_direntries_fatentries(alldisp,fatlength,fatnumber,dirlength,
                   fatptr2,dirptr2)
 int alldisp,fatlength,fatnumber,dirlength;
 fatentry_tp * fatptr2;
 struct direntry_tp * dirptr2;
 { int i;
   for (i=0;i<dirlength;dirptr2++,i++)
    { if (( (* dirptr2).filename[0] != 0x00) || alldisp)
    { int fatvalue;
      printf("->$(%4x) ",i);
      display_direntry(!0,dirptr2);
      fatvalue = (* dirptr2).startcluster;
          while ( (fatvalue != EOFAT) && (fatvalue != NOFAT) )
       { printf("%5x",fatvalue);
        switch (fattyp)
         { case FAT12B:
        {fatvalue =
             get_fatentry12(fatvalue,fatlength,fatnumber,fatptr2);
         break;}
           case FAT16B:
        {fatvalue =
             get_fatentry16(fatvalue,fatlength,
                    fatnumber,(fatentry16_tp *)fatptr2);
         break;}
           default:
        {errormessage(BOOTERR,WRONGFAT);exit(1);break;};
         };
       };
     printf("%5x\n",fatvalue);
    };
    };
   printf("\n");
 }

void display_direntry_fatentries(alldisp,seldentry,fatlength,
                 fatnumber,fatptr2,dirptr2)
 int alldisp,seldentry,fatlength,fatnumber;
 fatentry_tp * fatptr2;
 struct direntry_tp * dirptr2;
 { dirptr2 = dirptr2 + seldentry;
   if (( (* dirptr2).filename[0] != 0x00) || alldisp)
     { int fatvalue;
       display_direntry(!0,dirptr2);
       fatvalue = (* dirptr2).startcluster;
       while ( (fatvalue != EOFAT) && (fatvalue != NOFAT) )
      { printf("%5x",fatvalue);
        switch (fattyp)
         { case FAT12B:
        {fatvalue =
             get_fatentry12(fatvalue,fatlength,fatnumber,fatptr2);
         break;}
           case FAT16B:
        {fatvalue =
             get_fatentry16(fatvalue,fatlength,
                    fatnumber,(fatentry16_tp *)fatptr2);
         break;}
           default:
        {errormessage(BOOTERR,WRONGFAT);exit(1);break;};
         };
      };
       printf("%5x\n",fatvalue);
     }
   else
     { printf("no entry\n");
     };

   printf("\n");
 }

int newlog(fatptr2,btptr2,dirptr2)
 fatsec_tp *fatptr2;
 bootsec_tp *btptr2;
 struct direntry_tp *dirptr2;
 { int error1,error2,error3,noerr;
   noerr = 0;
   /* bootinfo */
   error1 = get_bootinfo(drive,btptr2);
   if (error1 != NULL)
    { errormessage(BOOTERR,BREADERR);
      noerr = !0;};
   error2 = get_fats(drive,fatptr2,btptr2);
   if  (error2 != NULL)
    { errormessage(BOOTERR,FREADERR);
      noerr = !0;}
   error3 = get_maindir(drive,dirptr2);
   if  (error3 != NULL)
    { errormessage(BOOTERR,DREADERR);
      noerr = !0;}
   return(noerr);
 }

int writeback(fatptr2,btptr2,dirptr2)
 fatsec_tp *fatptr2;
 bootsec_tp *btptr2;
 struct direntry_tp *dirptr2;
 { int c,error1,error2,error3,noerr;
   noerr = 0;
   printf("Do You really want to write back ? Y/N ");
   c = getch();  /* ansi specific */
   printf("\n");
   if (toupper(c) == 'Y')
    { /* bootinfo */
      error1 = put_bootinfo(drive,btptr2);
      if (error1 != NULL)
    { errormessage(BOOTERR,BWRITEERR);
      noerr = !0;};
      error2 = put_fats(drive,fatptr2,btptr2);
      if  (error2 != NULL)
    { errormessage(BOOTERR,FWRITEERR);
      noerr = !0;}
      error3 = put_maindir(drive,dirptr2);
      if  (error3 != NULL)
    { errormessage(BOOTERR,DWRITEERR);
      noerr = !0;}
    };
   /* "flush buffer" funktion, read again */
   error3 = get_maindir(drive,dirptr2);
   if  (error3 != NULL)
    { errormessage(BOOTERR,DREADERR);
      noerr = !0;}
 return(noerr);
 }

void alloc_boot()
 { int oldsecsize;
   /* at first, assume the maximum possible sector size */
   oldsecsize = secsize; secsize = MAXSECSIZE;
   if ( alloc_sector(&btptr) != NULL)
       { errormessage(FATALERR,NOMEM); exit(1);}
      else
       { /* zurueck zur "alten" sektorgroesse */
     secsize = oldsecsize;
       };
 }

int alloc_all()
  {int error2;
   /* allocate memory for several different fields */
   /* get bootsector informations, important for the
      memory requirements of the subsequent 
	  memory allocations                             */
   if ((error2 = get_bootinfo(drive,btptr)) != NULL)
       { errormessage(BOOTERR,BREADERR);
         log_ok = 0; bootlog_ok = 0; return(error2);}
   else
       { bootlog_ok = !0; };
   if ( alloc_maindir((*btptr).number_of_direntries,&dirptr) != NULL)
       { errormessage(FATALERR,NOMEM); /*exit(1);*/ };
   if ( alloc_fats(fatsecs,&fatptr) != NULL)
       { errormessage(FATALERR,NOMEM); /*exit(1);*/ };
   if ( alloc_cluster(&viptr,btptr) != NULL)
       { errormessage(FATALERR,NOMEM); exit(1);}
   return(error2);
  }

int realloc_all()
 { int error2;
   /* Modify the memory size for several different fields */
   if ((error2 = get_bootinfo(drive,btptr)) != NULL)
       { errormessage(BOOTERR,BREADERR);
     log_ok = 0; bootlog_ok = 0; return(error2);}
   else
       { bootlog_ok = !0;};
   if ( realloc_maindir((*btptr).number_of_direntries,&dirptr) != NULL)
       { errormessage(FATALERR,NOMEM); /*exit(1);*/ };
   if ( realloc_fats(fatsecs,&fatptr) != NULL)
       { errormessage(FATALERR,NOMEM); /*exit(1);*/ };
   if ( realloc_cluster(&viptr,btptr) != NULL)
       { errormessage(FATALERR,NOMEM); /*exit(1);*/ };
   return(error2);
 }

void check_dos33()
 { printf (" %d  %d \n",_osmajor,_osminor);
   if ((_osmajor == 3) && (_osminor == 30))
        { errormessage(FATALERR,VERERR);
          exit(1);
        }
 }

void title()
 { printf
     ("FATEDIT V1.00 - MSDOS FAT & Directory Recovery Tool\n");
   printf
     ("Copyright 1988-2015 Rolf Hemmerling\n");
   printf
     ("Licensed under the Apache License, Version 2.0\n");
 }

void goback()
 { longjmp(buffer,1);
 }

void aborthandle()
 {
   if (signal(SIGINT,SIG_IGN) == (void (*) ()) -1)
    { errormessage(FATALERR,SIGERR);
      exit(1);};
 }

void backhandle()
 {
   if (signal(SIGINT,goback) == (void (*) ()) -1)
    { errormessage(FATALERR,SIGERR);
      exit(1);};
 }

/*******************/
/* FAT menu item   */
/*******************/

int show_fat_options(selfentry,seldentry,fatlength,fatnumber,fatptr2,dirptr2)
 int selfentry,seldentry,fatlength,fatnumber;
 fatentry_tp * fatptr2;
 struct direntry_tp * dirptr2;
 { int c,cl;
   printf("\n");
   printf("************************************FAT*MENUE*****************************\n");
   /* dirptr2 already points to the right entry */
   switch (fattyp)
        { case FAT12B:
        {cl = get_fatentry12(selfentry,fatlength,fatnumber,
                     fatptr2);break;}
          case FAT16B:
        {cl = get_fatentry16(selfentry,fatlength,fatnumber,
                     (fatentry16_tp *)fatptr2);break;}
          default:
        {errormessage(BOOTERR,WRONGFAT);exit(1);break;};
        };
   printf("fatentry (memory) : $(%5x)            fatentry-value (memory) : $(%5x)\n",
      st_fat_entry,st_fat_value);
   printf("fatentry          : $(%5x)->$(%5x)  direntry->startcluster  : $(%5x)\n",
      selfentry,cl,(*dirptr2).startcluster);
   printf("->$(%4x):",seldentry);
   display_direntry(!0,dirptr2);
 printf("**************************************************************************\n");
   printf("0 = exit this menu\n");
   printf("1 = select fatentry\n");
   printf("N = select next fatentry\n");
   printf("P = select previous fatentry\n");
   printf("+ = select next direntry\n");
   printf("- = select previous direntry\n");
   printf("2 = select direntry\n");
   printf("3 = view cluster(fatentry) in hex \n");
   printf("4 = view cluster(fatentry) in ascii \n");
   printf("5 = enter fatentry value\n");
   printf("6 = link direntry to fatentry\n");
   printf("S = goto startcluster\n");
   printf("A = save fatentry\n");
   printf("V = save fatentry value\n");
   printf("C = (copy) fatentry value := saved fatentry \n");
   printf("R = (restore) fatentry value := saved fatentry value\n");
   printf("F = (follow) fatentry := fatentry value\n");
   printf("**************************************************************************\n");
   c = getch();    /* ansi-c specific */
   c = toupper(c); /* for MSC, getch+toupper are not 
                      allowed in a single combined instruction ! */
   switch (c)
    { case 'A' : { c = 10;break;};
      case 'V' : { c = 11;break;};
      case 'C' : { c = 12;break;};
      case 'R' : { c = 13;break;};
      case '+' : { c = 14;break;};
      case '-' : { c = 15;break;};
      case 'N' : { c = 16;break;};
      case 'P' : { c = 17;break;};
      case 'F' : { c = 18;break;};
      case 'S' : { c = 19;break;};

      default  : {c = c - (int)'0'; break;};
    };
   return(c);
 }

void enter_fatentry(selfentry,fatlength,fatnumber,fatptr2)
 int selfentry, fatlength,fatnumber;
 fatentry_tp * fatptr2;
 { unsigned int entry,xentry;
   unsigned int error1;
   error1 = 0;
   printf("? new entry value : $");
   switch (fattyp)
    { case FAT12B:
       { /* old value */
         entry = get_fatentry12(selfentry,fatlength,fatnumber,fatptr2);
         scanf("%x",&entry);
         error1 =
          put_fatentry12(entry,selfentry,fatlength,fatnumber,fatptr2);
         break;};

      case FAT16B:
       { /* old value */
         entry = get_fatentry16(selfentry,fatlength,fatnumber,
             (fatentry16_tp *)fatptr2);
         scanf("%x",&xentry);
         error1 =
         put_fatentry16(entry,selfentry,fatlength,fatnumber,
                (fatentry16_tp *)fatptr2);
         break;};
      default:
       {errormessage(BOOTERR,WRONGFAT);exit(1);break;};
    };
   if (error1 == ERRCLUST)
    { errormessage(BOOTERR,WRONGFENTRY);
    };
 }

void hex_view(drive,viptr2,btptr2)
 char drive;
 unsigned char *viptr2;
 bootsec_tp * btptr2;
 { int i,fsector,k,first;
   unsigned char *viptr3;
   fsector = clustosec(fat_entry,btptr2);
   if ((absread((int)(toupper(drive) - 'A'),(*btptr2).sectors_per_cluster,
        fsector,viptr2)) != NULL)
      {errormessage(BOOTERR,SREADERR);}
   else
      { viptr3 = viptr2;
    first = !0;
    for (i = 0;i < (secsize * (*btptr2).sectors_per_cluster);i++)
     { if (! (i % 16))
         { if (!first)
        { printf("  ");
          for (k = 0;k < 16;k++)
           { if (*viptr3 < 32)
              { printf(".");viptr3++;}
             else
              { printf("%1c",*(viptr3++));};
           };
        }
           else
        { first = 0;};
           printf("\n->$(%4x) ",i);};
        printf("%3x",(*(viptr2++)));
     };
      };
   printf("  ");
   for (k = 0;k < 16;k++)
    { if (*viptr3 < 32)
    { printf(".");viptr3++;}
      else
    { printf("%1c",*(viptr3++));};
    };
   printf("\n");
 }

void ascii_view(drive,viptr2,btptr2)
 char drive;
 unsigned char *viptr2;
 bootsec_tp * btptr2;
 { int i,fsector;
   fsector = clustosec(fat_entry,btptr2);
   if ((absread((int)(toupper(drive) - 'A'),(*btptr2).sectors_per_cluster,
        fsector,viptr2)) != NULL)
      {errormessage(BOOTERR,SREADERR);}
   else
      { for (i = 0;i < (secsize * (*btptr2).sectors_per_cluster);i++)
     { if (! (i % 64))
         {printf("\n->$(%4x) ",i);};
        if (*viptr2 < 32)
         { printf(".");viptr2++;
         }
        else
         { printf("%1c",*(viptr2++));};
     };
    printf("\n");
      };
 }

int get_fat_value(selfentry,fatlength,fatnumber,fatptr2)
 int selfentry,fatlength,fatnumber;
 fatentry_tp * fatptr2;
 { int cl;
   switch (fattyp)
     { case FAT12B:
     {cl = get_fatentry12(selfentry,fatlength,fatnumber,
        fatptr2);break;}
       case FAT16B:
     {cl = get_fatentry16(selfentry,fatlength,fatnumber,
        (fatentry16_tp *)fatptr2);break;}
       default:
     {errormessage(BOOTERR,WRONGFAT);exit(1);break;};
     };
   return(cl);
 }

int put_fat_value(fvalue,selfentry,fatlength,fatnumber,fatptr2)
 int fvalue,selfentry,fatlength,fatnumber;
 fatentry_tp * fatptr2;
 { int error1;
   int c;
   error1 = 0;
   printf("Do You really want to modify the FAT ? Y/N ");
   c = getch();
   printf("\n");
   if (toupper(c)=='Y')
    { switch (fattyp)
    { case FAT12B:
       { error1 =
          put_fatentry12(fvalue,selfentry,fatlength,fatnumber,fatptr2);
        break;}
      case FAT16B:
       { error1 =
           put_fatentry16(fvalue,selfentry,fatlength,fatnumber,
                (fatentry16_tp *)fatptr2);break;}
      default:
       {errormessage(BOOTERR,WRONGFAT);exit(1);break;};
    };
    };
   return(error1);
 }

/*************************/
/* directory menu punkte */
/*************************/

int show_direntry_options(dirptr2)
 struct direntry_tp * dirptr2;
 { int c;
   printf("\n");
   printf("*******************************DIRENTRY*MENUE*****************************\n");
   printf("->$(%4x):",dir_entry);
   display_direntry(!0,dirptr2);
   printf("**************************************************************************\n");
   printf("0 = exit this menu\n");
   printf("1 = select new entry\n");
   printf("2 = change name\n");
   printf("3 = change time\n");
   printf("4 = change date\n");
   printf("5 = change attributes\n");
   printf("6 = change status\n");
   printf("7 = enter new filelength\n");
   printf("8 = recalculate new filelength\n");
   printf("+ = select next direntry\n");
   printf("- = select previous direntry\n");
   printf("**************************************************************************\n");
   c = getch();    /* ansi-c specific */
   c = toupper(c); /* for MSC, getch+toupper are not 
                      allowed in a single combined instruction ! */  
   switch (c)
    { case '+' : { c = 10;break;};
      case '-' : { c = 11;break;};
      default  : {c = c - (int)'0'; break;};
    };
   return(c);
 }

void get_name(dirptr2)
  struct direntry_tp * dirptr2;
  { char name[9],ext[4];
    int k;
    for (k = 0;k<NLENGTH;k++)   /* old value */
    { name[k] = (* dirptr2).filename[k];
    };
    name[NLENGTH] = (unsigned char)0;
    for (k = 0;k<ELENGTH;k++)   /* old value */
    { ext[k] = (* dirptr2).extension[k];
    };
    ext[ELENGTH] = (unsigned char)0;
    printf("enter a new filename : ");
    scanf(" %8s",name); /* for MSC no safe value-keeping abort with CTRL-Z */
    printf("enter a new exension : ");
    scanf("%3s",ext);   /* for MSC no safe value-keeping abort with CTRL-Z */
    printf("\n");
    for (k = 0;k<strlen(name);k++)
    { (* dirptr2).filename[k] = name[k];
    };
    for (k = strlen(name);k<NLENGTH;k++)
    { (* dirptr2).filename[k] = ' ';
    };
    for (k = 0;k<strlen(ext);k++)
    { (* dirptr2).extension[k] = ext[k];
    };
    for (k = strlen(ext);k<ELENGTH;k++)
    { (* dirptr2).extension[k] = ' ';
    };
  }

void get_time(dirptr2)
  struct direntry_tp * dirptr2;
  { unsigned int minute,hour,second;
    hour = 0;
    minute = 0;
    second = 0;
     /* old values */
    hour = (* dirptr2).hour;
    minute = (* dirptr2).minute;
    second = (* dirptr2).second;
    printf("enter the new time /hour   : ");
    scanf(" %2u",&hour);
    printf("enter the new time /minute : ");
    scanf("%2u",&minute);
    printf("enter the new time /second : ");
    scanf("%2u",&second);
    (* dirptr2).hour =  ((2<<(HOR+1))-1) & hour;
    (* dirptr2).minute = ((2<<(MNU+1))-1) & minute;
    (* dirptr2).second = ((2<<(SEC+1))-1) & second;
  }

void get_date(dirptr2)
  struct direntry_tp * dirptr2;
  { unsigned int year,month,day;
    /* old values */
    year = (* dirptr2).year + 80;
    month = (* dirptr2).month;
    day = (* dirptr2).day;
    printf("enter the new date /year   : ");
    scanf(" %2u",&year);
    printf("enter the new date /month  : ");
    scanf("%2u",&month);
    printf("enter the new date /day    : ");
    scanf("%2u",&day);
    (* dirptr2).year = ((2<<(YER+1))-1) & (year - 80);
    (* dirptr2).month = ((2<<(MON+1))-1) & month;
    (* dirptr2).day = ((2<<(DAY+1))-1) & day;
  }

void change_status(dirptr2)
  struct direntry_tp * dirptr2;
  {  int c;
     switch ((int)(* dirptr2).filename[0])
       { case 0xE5 :
      {printf("entry is <del>eted, for activation enter a new name\n");};
     case 0x00 :
      {printf("entry is <empty>,for activation enter a new name\n");};
     default :
      { printf("entry is <active>, deactivate Y/N ? ");
        c = getch(); /* ansi specific */
        printf("\n");
        if ( (c == 'Y') || (c == 'y'))
          { (* dirptr2).filename[0] = (unsigned char)0xE5;
          };
      };
       }; /* switch */
  }

void change_attributes(dirptr2)
  struct direntry_tp * dirptr2;
  { char bflags[9];
    int i;
    strcpy(bflags,"");
    printf("enter the flags (RHSVDA67) to change : ");
    scanf("%8s",bflags);
    printf("\n");
    strupr(bflags);
    printf("\n");
    for (i = 0;i<strlen(bflags);i++)
     { if (bflags[i] == 'R')
     { if ((* dirptr2).attribute & READ_ONLY)
         { (* dirptr2).attribute = (* dirptr2).attribute ^ READ_ONLY; }
         else
         { (* dirptr2).attribute = (* dirptr2).attribute | READ_ONLY; };
     };
       if (bflags[i] == 'H')
     { if ((* dirptr2).attribute & HIDDEN)
         { (* dirptr2).attribute = (* dirptr2).attribute ^ HIDDEN; }
         else
         { (* dirptr2).attribute = (* dirptr2).attribute | HIDDEN; };
     };
       if (bflags[i] == 'S')
     { if ((* dirptr2).attribute & SYSTEM)
         { (* dirptr2).attribute = (* dirptr2).attribute ^ SYSTEM; }
         else
         { (* dirptr2).attribute = (* dirptr2).attribute | SYSTEM; };
     };
       if (bflags[i] == 'V')
     { if ((* dirptr2).attribute & VOLUME)
         { (* dirptr2).attribute = (* dirptr2).attribute ^ VOLUME; }
         else
         { (* dirptr2).attribute = (* dirptr2).attribute | VOLUME; };
     };
       if (bflags[i] == 'D')
     { if ((* dirptr2).attribute & SUBDIR)
         { (* dirptr2).attribute = (* dirptr2).attribute ^ SUBDIR; }
         else
         { (* dirptr2).attribute = (* dirptr2).attribute | SUBDIR; };
     };
       if (bflags[i] == 'A')
     { if ((* dirptr2).attribute & ARCHIVE)
         { (* dirptr2).attribute = (* dirptr2).attribute ^ ARCHIVE; }
         else
         { (* dirptr2).attribute = (* dirptr2).attribute | ARCHIVE; };
     };
       if (bflags[i] == '6')
     { if ((* dirptr2).attribute & BIT6)
         { (* dirptr2).attribute = (* dirptr2).attribute ^ BIT6; }
         else
         { (* dirptr2).attribute = (* dirptr2).attribute | BIT6; };
     };
       if (bflags[i] == '7')
     { if ((* dirptr2).attribute & BIT7)
         { (* dirptr2).attribute = (* dirptr2).attribute ^ BIT7; }
         else
         { (* dirptr2).attribute = (* dirptr2).attribute | BIT7; };
     };
     };
  }

void enter_filelength(dirptr2)
 struct direntry_tp *dirptr2;
 { long entry;
   /* dirptr2 already points to this entry */
   entry = (* dirptr2).filelength; /* old value */
   printf("? entry number : $");
   scanf("%lx",&entry);
   (* dirptr2).filelength = entry;
 }

void calc_filelength(fatlength,fatnumber,fatptr2,btptr2,dirptr2)
 int fatlength,fatnumber;
 fatsec_tp *fatptr2;
 bootsec_tp *btptr2;
 struct direntry_tp *dirptr2;
 { long laenge;
   int fatvalue,c;
   laenge = 0;
    /* if the upper limit of the filelength is reached, abort */
   fatvalue = (* dirptr2).startcluster;
   while ( (fatvalue != EOFAT) && (fatvalue != NOFAT) &&
       (laenge < (long)clusters))
       { switch (fattyp)
         { case FAT12B:
        {fatvalue =
             get_fatentry12(fatvalue,fatlength,fatnumber,fatptr2);
         break;}
           case FAT16B:
        {fatvalue =
             get_fatentry16(fatvalue,fatlength,
                    fatnumber,(fatentry16_tp *)fatptr2);
         break;}
           default:
        {errormessage(BOOTERR,WRONGFAT);exit(1);break;};
         };
         laenge++;
       };
   if (laenge == (long)clusters)
    { errormessage(BOOTERR,FATLOOP);
    }
   else
    { laenge = laenge * (long)(*btptr2).sectors_per_cluster *
          (long)(*btptr2).bytes_per_sector;
      printf("Do You accept the new length $(%8lx) ? Y/N ",laenge);
      c = getch(); /* ansi specific */
      printf("\n");
      if (toupper(c) == 'Y')
       { (* dirptr2).filelength = laenge;
       }
    };
 }

/********************/
/* main menu punkte */
/********************/

void show_bootinfo(btptr2)
 bootsec_tp *btptr2;
 { display_bootinfo(btptr2);
 }

void show_maindir(dirptr2,btptr2)
 struct direntry_tp *dirptr2;
 bootsec_tp *btptr2;
 { display_dir((*btptr2).number_of_direntries,dirptr2);
 }

void show_direntries_fatentries(fatptr2,btptr2,dirptr2)
 fatsec_tp *fatptr2;
 bootsec_tp *btptr2;
 struct direntry_tp *dirptr2;
 { display_direntries_fatentries(!0,(*btptr2).sectors_per_fat,WORKFAT,
                 (*btptr2).number_of_direntries,
                 (fatentry_tp *)fatptr2,dirptr2);
 }

void modify_direntry(fatptr2,btptr2,dirptr3)
 fatsec_tp *fatptr2;
 bootsec_tp *btptr2;
 struct direntry_tp *dirptr3;
 { int choice;
   struct direntry_tp *dirptr2;
   choice = 0;
   /* the real menu functions */
   do
    { dirptr2 = dirptr3 + dir_entry;
      choice = show_direntry_options(dirptr2);
      switch (choice)
       { case 0  : { break; };
     case 1  : { dir_entry = ask_dentry(dir_entry,btptr2);break;};
     case 2  : { get_name(dirptr2);break;};
     case 3  : { get_time(dirptr2);break;};
     case 4  : { get_date(dirptr2);break;};
     case 5  : { change_attributes(dirptr2);break;};
     case 6  : { change_status(dirptr2);break;};
     case 7  : { enter_filelength(dirptr2);break;};
     case 8  : { calc_filelength((*btptr2).sectors_per_fat,WORKFAT,
                     fatptr2,btptr2,dirptr2);break;};
     case 10 : { if (dir_entry < ((*btptr2).number_of_direntries-1))
               {dir_entry++;break;}
              else
               {errormessage(BOOTERR,WRONGDENTRY);};
             break;};
     case 11 : { if (dir_entry >0)
               {dir_entry--;break;}
              else
               {errormessage(BOOTERR,WRONGDENTRY);};
             break;};
     default : { break;};
       };
    } while (choice != 0);
 }

void modify_fatentry(fatptr2,btptr2,dirptr3)
 fatentry_tp * fatptr2;
 bootsec_tp *btptr2;
 struct direntry_tp * dirptr3;
 { int choice;
   struct direntry_tp * dirptr2;
   choice = 0;
   /* the real menu functions */
   do
    { dirptr2 = dirptr3 + dir_entry;
      choice = show_fat_options(fat_entry,dir_entry,
                (*btptr2).sectors_per_fat,WORKFAT,
                (fatentry_tp *)fatptr2,dirptr2);
      switch (choice)
       { case 0  : { break; };
     case 1  : { fat_entry = ask_fentry(fat_entry);break;};
     case 2  : { dir_entry = ask_dentry(dir_entry,btptr2);break;};
     case 3  : { xxx = setjmp(buffer);
             if (xxx == 0)
            { backhandle();
              hex_view(drive,viptr,btptr);
              aborthandle();
            }
               else
            { aborthandle();
            };
               break;
           }
     case 4  : { xxx = setjmp(buffer);
             if (xxx == 0)
            { backhandle();
              ascii_view(drive,viptr,btptr);
              aborthandle();
            }
               else
            { aborthandle();
            };
               break;
           };
     case 5  : { enter_fatentry(fat_entry,
             (*btptr2).sectors_per_fat,WORKFAT,fatptr2);
             break;};
     case 6  : { link_startcluster(fat_entry,dirptr2); break;};
     case 10 : { st_fat_entry = fat_entry;break;};
     case 11 : { st_fat_value = get_fat_value(fat_entry,
                      (*btptr2).sectors_per_fat,WORKFAT,
                      (fatentry_tp *)fatptr2);break;};
     case 12 : { put_fat_value(st_fat_entry,fat_entry,
                  (*btptr2).sectors_per_fat,WORKFAT,
                  (fatentry_tp *)fatptr2);break;};
     case 13 : { put_fat_value(st_fat_value,fat_entry,
                  (*btptr2).sectors_per_fat,WORKFAT,
                  (fatentry_tp *)fatptr2);break;};
     case 14 : { if (dir_entry < ((*btptr2).number_of_direntries-1))
               {dir_entry++;break;}
              else
               {errormessage(BOOTERR,WRONGDENTRY);};
             break;};
     case 15 : { if (dir_entry >0)
               {dir_entry--;break;}
              else
               {errormessage(BOOTERR,WRONGDENTRY);};
             break;};
     case 16 : { if (fat_entry < (clusters-1))
               {fat_entry++;break;}
              else
               {errormessage(BOOTERR,WRONGFENTRY);};
             break;};
     case 17 : { if (fat_entry >0)
               {fat_entry--;break;}
              else
               {errormessage(BOOTERR,WRONGFENTRY);};
             break;};
     case 18 : { unsigned int x_entry;
             x_entry = get_fat_value(fat_entry,
                      (*btptr2).sectors_per_fat,WORKFAT,
                      (fatentry_tp *)fatptr2);
             if (x_entry < RESCLUST)
              { fat_entry = x_entry;
              };
             break;};
     case 19  : {fat_entry = (* dirptr2).startcluster;
             break;};

     default : { break;};
       };
    } while (choice != 0);
 }

void show_fats(fatptr2,btptr2)
 fatsec_tp *fatptr2;
 bootsec_tp *btptr2;
 { display_fats(clusters, (*btptr2).sectors_per_fat,WORKFAT,
        (fatentry_tp *)fatptr2);
 }

unsigned int ask_fentry(old)
 unsigned int old;
 { unsigned int entry;
   entry = old; /* old value */
   printf("? entry number : $");
   scanf("%x",&entry);
   if (entry <= clusters)
    { return(entry);}
   else
    { errormessage(BOOTERR,WRONGFENTRY);
      return(old);};
 }

unsigned int ask_dentry(old,btptr2)
 unsigned int old;
 bootsec_tp *btptr2;
 { unsigned entry;
   entry = old; /* old value */
   printf("? entry number : $");
   scanf("%x",&entry);
   if ((*btptr2).number_of_direntries > entry)
    { return(entry);}
   else
    { errormessage(BOOTERR,WRONGDENTRY);
      return(old);};
 }

char change_logdrive()
 { int dr;
   char drc;
   printf("? new logdrive <A:>...<Z:> : ");
   dr = getch();  /* ansi specific */
   printf("\n");
   if (((dr >= (int)'A') && (dr <= toupper((int)lastdrive))) ||
       ((dr >= (int)'a') && (dr <= tolower((int)lastdrive))))
    {drc = (char)dr;
     log_ok = 0; bootlog_ok = 0; }
   else
    {drc = drive;};
   return(drc);

 }

int show_main_options()
 { int c;
   printf("\n");
   printf("***********************************MAIN*MENUE*****************************\n");
   printf("log drive <%c:>       ",drive);
   if (bootlog_ok)
    {printf("%d-bit FAT\n",fattyp);}
   else
    {printf("\n");};
#ifdef TEST2
   printf("coreleft : %ld\n",(long)coreleft());
#endif
   printf("**************************************************************************\n");
   printf("0 = exit to dos\n");
   printf("1 = log on new disk (read bootsector & directory & FAT)\n");
   printf("2 = change log drive\n");
   printf("3 = write changes back to disk\n");
   printf("4 = show bootinfo\n");
   printf("5 = show root directory \n");
   printf("6 = show FAT map\n");
   printf("7 = show FAT entries for every directory entry\n");
   printf("8 = modify/show directory entry \n");
   printf("9 = FAT menu\n");
   printf("C = Copy FAT to second FAT\n");
   printf("**************************************************************************\n");
   c = getch();    /* ansi-c specific */
   c = toupper(c); /* for MSC, getch+toupper are not 
                      allowed in a single combined instruction ! */  
   switch (c)
    { case 'C' : { c = 10;break;};
      default  : {c = c - (int)'0'; break;};
    };
   return(c);
 }

void main_menu()
 { int choice;
   /* allocate memory for the bootsector */
   alloc_boot();

   /* ... */

   choice = 0;
   /* the real menu functions */
   do
    { choice = show_main_options();
      switch (choice)
       { case 0 : {exit(0); break ; };
     case 1 : {if (first_log)
            { if (!alloc_all())
               { first_log = 0;
             log_ok = !0;
                         if (newlog(fatptr,btptr,dirptr))
               {errormessage(BOOTERR,LOGERR);
                 log_ok = 0; };
               }
              else
               { log_ok = 0;
             errormessage(BOOTERR,LOGERR);
               };
            }
           else
            { if (realloc_all())
               { log_ok = 0;
             errormessage(BOOTERR,LOGERR);
               }
              else
               { log_ok = !0;
             if (newlog(fatptr,btptr,dirptr))
               {errormessage(BOOTERR,LOGERR);
                 log_ok = 0; };
               };
            };
           break;
          };
     case 2 : {drive = change_logdrive();break;};
#ifdef RTEST
     case 3 : {errormessage(FATALERR,READONLY); break;};
#else
     case 3 : {if (!log_ok)
             { errormessage(BOOTERR,FIRSTLOG);
             }
           else
             {writeback(fatptr,btptr,dirptr); break;};
                   break;
          };
#endif
     case 4 : {if (!bootlog_ok)
             { errormessage(BOOTERR,FIRSTLOG);
             }
           else
             { show_bootinfo(btptr);};
                  break;
          };
     case 5 : {if (!log_ok)
             { errormessage(BOOTERR,FIRSTLOG);
             }
           else
             { xxx = setjmp(buffer);
               if (xxx == 0)
            { backhandle();
              show_maindir(dirptr,btptr);
              aborthandle();
            }
               else
            { aborthandle();
            };
             };
                   break;
          };
     case 6 : {if (!log_ok)
             { errormessage(BOOTERR,FIRSTLOG);
             }
           else
             { xxx = setjmp(buffer);
               if (xxx == 0)
            { backhandle();
              show_fats(fatptr,btptr);
              aborthandle();
            }
               else
            { aborthandle();
            };
             };
                   break;
          };
     case 7 : {if (!log_ok)
             { errormessage(BOOTERR,FIRSTLOG);
             }
           else
             { xxx = setjmp(buffer);
               if (xxx == 0)
            { backhandle();
              show_direntries_fatentries(fatptr,btptr,dirptr);
              aborthandle();
            }
               else
            { aborthandle();
            };
             };
           break;
          };
     case 8 : {if (!log_ok)
             { errormessage(BOOTERR,FIRSTLOG);
             }
           else
             { modify_direntry(fatptr,btptr,dirptr); };
           break;
          };
     case 9 : {if (!log_ok)
             { errormessage(BOOTERR,FIRSTLOG);
             }
           else
             { modify_fatentry(fatptr,btptr,dirptr);};
           break;
          };
     case 10 : {if (!log_ok)
             { errormessage(BOOTERR,FIRSTLOG);
             }
           else
             { copy_fat(RESFAT,fatptr,btptr);};
           break;
          };

     default: {break;}
       };
    } while (choice != 0);
   /* ... */

 }

void main()
 { title();
   aborthandle();
#if OS == MSDOS
   check_dos33();
#endif
   main_menu();
 }
 
