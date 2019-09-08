#define FILE_NAME      "/forth/ascii_xfer_a001.txt"
#define WORKING_DIR "/forth"

#define VERBIAGE_AA
#undef VERBIAGE_AA

#define FBUFF_PRN // echo + char during file read+print op
#undef FBUFF_PRN

#include "SdFat.h"
extern File thisFile;
#define WRITE_FORTH(a) {thisFile.print((a));}
#define WRITELN_FORTH(a) {thisFile.println((a));}
