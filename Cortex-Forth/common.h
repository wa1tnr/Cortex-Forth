#define FILE_NAME      "/forth/ascii_xfer_a001.txt"
#define WORKING_DIR    "/forth"

/* swap these two lines (below) to see (or to suppress) source code listing: */
#undef VERBIAGE_AA
#define VERBIAGE_AA

#define FBUFF_PRN // echo + char during file read+print op
#undef FBUFF_PRN

#include "SdFat.h"
extern File thisFile;
#define WRITE_FORTH(a) {thisFile.print((a));}
#define WRITELN_FORTH(a) {thisFile.println((a));}

#define WRITE_VERT_WSPACE(a) {thisFile.println((a));}
