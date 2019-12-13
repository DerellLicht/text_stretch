//**************************************************************************************
//  Copyright (c) 2019-2020  Daniel D Miller
//  text_stretch
//  
//  a utility to stretch hard-break text paragraphs into single lines
//  
//  build: g++ -Wall -O2 text_stretch.cpp -o text_stretch.exe
//  
//  Written by:   Daniel D. Miller
//**************************************************************************************

#include <windows.h>
#include <stdio.h>
#include <errno.h>

//lint -e10  Expecting '}'
//lint -e818  Pointer parameter 'argv' (line 88) could be declared as pointing to const

#define  LOOP_FOREVER   1

#define  HTAB     9
#define  CR       13
#define  LF       10
#define  SPC      32

typedef unsigned int  uint ;

#define  MAX_FILE_LEN   1024
char infile[MAX_FILE_LEN+1] = "" ;
char outfile[MAX_FILE_LEN+1] = "" ;

#define  MAX_STR_LEN    128
char inpstr[MAX_STR_LEN+1] = "" ;

#define  MAX_OUT_LEN    16384
char outpstr[MAX_OUT_LEN+1] = "" ;

//**********************************************************************
void strip_newlines(char *rstr)
{
   int slen = (int) strlen(rstr) ;
   while (1) {
      if (slen == 0)
         break;
      if (*(rstr+slen-1) == CR  ||  *(rstr+slen-1) == LF) {
         slen-- ;
         *(rstr+slen) = 0 ;
      } else {
         break;
      }
   }
}

//**********************************************************************
char *strip_leading_spaces(char *str)
{
   if (str == 0)
      return 0;
   char *tptr = str ;
   while (LOOP_FOREVER) {
      if (*tptr == 0)
         return tptr;
      if (*tptr != SPC  &&  *tptr != HTAB)
         return tptr;
      tptr++ ;
   }
}

//**********************************************************************
void strip_duplicate_spaces(char *pstr)
{
   char *hd = pstr ;
   while (*hd != 0) {
      char *tl ;
      //  assume that inter-line spaces are real spaces, not tabs
      if (*hd == SPC) {
         hd++ ;
         tl = hd ;
         if (*tl == SPC) {
            tl = strip_leading_spaces(tl);
            strcpy(hd, tl);
         }
      }
      else {
         hd++ ;
      }
   }
}

//**************************************************************************************
int main(int argc, char** argv)
{
   //  read/parse command line
   int idx ;
   for (idx=1; idx<argc; idx++) {
      char *p = argv[idx] ;
      strncpy(infile, p, MAX_FILE_LEN);
      infile[MAX_FILE_LEN] = 0 ;
   }   

   //  check for input filename
   if (infile[0] == 0) {
      puts("Usage: text_stretch input_filename");
      puts("Output filename will be input_base_name.out");
      return 1;
   }

   strncpy(outfile, infile, MAX_FILE_LEN);
   outfile[MAX_FILE_LEN] = 0 ;
   char *hd = strrchr(outfile, '.') ;
   if (hd == NULL) {
      puts("input_filename must have an extension");
      return 1;
   }
   strcpy(hd, ".out");

   printf("input:  %s\n", infile);
   printf("output: %s\n", outfile);

   //  open input/output files
   FILE *infd = fopen(infile, "rt");
   if (infd == NULL) {
      printf("%s: %s\n", infile, strerror(errno));
      return 1;
   }
   FILE *outfd = fopen(outfile, "wt");
   if (outfd == NULL) {
      printf("%s: %s\n", outfile, strerror(errno));
      return 1;
   }

   //  read lines from input file
   //  Implement state machine to handle input-string parsing
   uint lcount = 0 ;
   uint outlen = 0 ;
   uint outcount = 0 ;
   uint maxoutlen = 0 ;
   while (fgets(inpstr, MAX_STR_LEN, infd) != NULL) {
      lcount++ ;
      strip_newlines(inpstr);
      if (inpstr[0] == 0) {
         //  disregard blank lines
         continue;
      }
      //  see if we are starting a new paragraph
      if (inpstr[0] == SPC  ||  inpstr[0] == HTAB) {
         //  write composite output string to output file
         if (outlen > 0) {
            if (maxoutlen < outlen) {
               maxoutlen = outlen ;
            }
            outcount++ ;
            fprintf(outfd, "%s\n", outpstr);
         }
         //  reset output-string params
         outlen = 0 ;
         outpstr[0] = 0 ;

         hd = strip_leading_spaces(inpstr);
         if (hd == NULL) {
            printf("line %u: blank starting line found!!\n", lcount);
            continue;
         }
      }
      else {
         hd = inpstr ;
      }
      //  append lines as required
      strcat(outpstr, hd);
      //  strip duplicate spaces from line
      strip_duplicate_spaces(outpstr);
      outlen = strlen(outpstr);
   }

   //  at end of file, write final line if required
   if (outlen > 0) {
      if (maxoutlen < outlen) {
         maxoutlen = outlen ;
      }
      outcount++ ;
      fprintf(outfd, "%s\n", outpstr);
   }

   //  close file handles
   fclose(infd);
   fclose(outfd);
   printf("%u lines read from input file\n", lcount);
   printf("%u lines written to output file, max line length: %u bytes\n", outcount, maxoutlen);

   return 0;
}
