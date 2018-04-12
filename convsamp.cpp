/**************************************************************************
*
*   Copyright (C) 2000-2013, International Business Machines
*   Corporation and others.  All Rights Reserved.
*
***************************************************************************
*   file name:  convsamp.c
*   encoding:   ASCII (7-bit)
*
*   created on: 2000may30
*   created by: Steven R. Loomis
*
*   Sample code for the ICU conversion routines.
*
* Note: Nothing special is needed to build this sample. Link with
*       the icu UC and icu I18N libraries.
*
*       I use 'assert' for error checking, you probably will want
*       something more flexible.  '***BEGIN SAMPLE***' and
*       '***END SAMPLE***' mark pieces suitable for stand alone
*       code snippets.
*
*
*  Each test can define it's own BUFFERSIZE
*
*/

#define DEBUG_TMI 0  /* define to 1 to enable Too Much Information */

#include <stdio.h>
#include <ctype.h>            /* for isspace, etc.    */
#include <assert.h>
#include <string.h>
#include <stdlib.h>  /* malloc */

#include "unicode/utypes.h"   /* Basic ICU data types */
#include "unicode/ucnv.h"     /* C   Converter API    */
#include "unicode/ustring.h"  /* some more string fcns*/
#include "unicode/uchar.h"    /* char names           */
#include "unicode/uloc.h"
#include "unicode/unistr.h"

#include "flagcb.h"

/* Some utility functions */

static const UChar kNone[] = { 0x0000 };

/* Print a UChar if possible, in seven characters. */
void prettyPrintUChar(UChar c)
{
  if(  (c <= 0x007F) &&
       (isgraph(c))  ) {
    printf(" '%c'   ", (char)(0x00FF&c));
  } else if ( c > 0x007F ) {
    char buf[1000];
    UErrorCode status = U_ZERO_ERROR;
    int32_t o;

    o = u_charName(c, U_EXTENDED_CHAR_NAME, buf, 1000, &status);
    if(U_SUCCESS(status) && (o>0) ) {
      buf[6] = 0;
      printf("%7s", buf);
    } else {
      printf(" ??????");
    }
  } else {
    switch((char)(c & 0x007F)) {
    case ' ':
      printf(" ' '   ");
      break;
    case '\t':
      printf(" \\t    ");
      break;
    case '\n':
      printf(" \\n    ");
      break;
    default:
      printf("  _    ");
      break;
    }
  }
}


void printUChars(const char  *name = "?",
                 const UChar *uch  = kNone,
                 int32_t     len   = -1 )
{
  int32_t i;

  if( (len == -1) && (uch) ) {
    len = u_strlen(uch);
  }

  printf("%5s: ", name);
  for( i = 0; i <len; i++) {
    printf("%-6d ", i);
  }
  printf("\n");

  printf("%5s: ", "uni");
  for( i = 0; i <len; i++) {
    printf("\\u%04X ", (int)uch[i]);
  }
  printf("\n");

  printf("%5s:", "ch");
  for( i = 0; i <len; i++) {
    prettyPrintUChar(uch[i]);
  }
  printf("\n");
}

void printBytes(const char  *name = "?",
                 const char *uch  = "",
                 int32_t     len   = -1 )
{
  int32_t i;

  if( (len == -1) && (uch) ) {
    len = strlen(uch);
  }

  printf("%5s: ", name);
  for( i = 0; i <len; i++) {
    printf("%-4d ", i);
  }
  printf("\n");

  printf("%5s: ", "uni");
  for( i = 0; i <len; i++) {
    printf("\\x%02X ", 0x00FF & (int)uch[i]);
  }
  printf("\n");

  printf("%5s:", "ch");
  for( i = 0; i <len; i++) {
    if(isgraph(0x00FF & (int)uch[i])) {
      printf(" '%c' ", (char)uch[i]);
    } else {
      printf("     ");
    }
  }
  printf("\n");
}

/*******************************************************************
  Very simple C sample to convert the word 'Moscow' in Russian in Unicode,
  followed by an exclamation mark (!) into the KOI8-R Russian code page.

  This example first creates a UChar String out of the Unicode chars.

  targetSize must be set to the amount of space available in the target
  buffer. After fromUChars is called,
  len will contain the number of bytes in target[] which were
  used in the resulting codepage.  In this case, there is a 1:1 mapping
  between the input and output characters. The exclamation mark has the
  same value in both KOI8-R and Unicode.

  src: 0      1      2      3      4      5      6
  uni: \u041C \u043E \u0441 \u043A \u0432 \u0430 \u0021
   ch: CYRILL CYRILL CYRILL CYRILL CYRILL CYRILL   '!'

 targ:  0    1    2    3    4    5    6
  uni: \xED \xCF \xD3 \xCB \xD7 \xC1 \x21
   ch:                                '!'


Converting FROM unicode
  to koi8-r.
  You must call ucnv_close to clean up the memory used by the
  converter.

  'len' returns the number of OUTPUT bytes resulting from the
  conversion.
 */

UErrorCode convsample_02()
{
  printf("\n\n==============================================\n"
         "Sample 02: C: simple Unicode -> koi8-r conversion\n");


  // **************************** START SAMPLE *******************
  // "cat<cat>OK"
  UChar source[] = { 0x041C, 0x043E, 0x0441, 0x043A, 0x0432,
                     0x0430, 0x0021, 0x0000 };
  char target[100];
  UErrorCode status = U_ZERO_ERROR;
  UConverter *conv;
  int32_t     len;

  // set up the converter
  //! [ucnv_open]
  conv = ucnv_open("koi8-r", &status);
  //! [ucnv_open]
  assert(U_SUCCESS(status));

  // convert to koi8-r
  len = ucnv_fromUChars(conv, target, 100, source, -1, &status);
  assert(U_SUCCESS(status));

  // close the converter
  ucnv_close(conv);

  // ***************************** END SAMPLE ********************

  // Print it out
  printUChars("src", source);
  printf("\n");
  printBytes("targ", target, len);

  return U_ZERO_ERROR;
}

int main()
{

  printf("Default Converter=%s\n", ucnv_getDefaultName() );

  convsample_02();  // C  , u->koi8r, conv

  printf("End of converter samples.\n");

  fflush(stdout);
  fflush(stderr);

  return 0;
}
