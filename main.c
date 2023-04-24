/*
*/
//TODO: exactly XXX bytes file size when splitting produces 0-size redundant file
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define VERSION "v1.0"

#define SPLIT_SIZE 3*1024*1024/2
#define CHUNK_SIZE 64*1024
#define SPLIT_EXT ".zip"
#define COMBINE_EXT ".zip.001"

void usage(int argc, char *argv[]);
int split (int argc, char *argv[]);
int combine (int argc, char *argv[]);


int main(int argc, char *argv[])
{
    char *filename, *split_file, *combine_file;
    int filename_len, split_len, combine_len;

    if (argc != 2) usage(argc, argv);

    filename = argv[1];
    filename_len = strlen(filename);

    split_file = SPLIT_EXT;
    split_len = strlen(split_file);

    if (filename_len > split_len
        && strncmp(filename + filename_len - split_len, split_file, split_len) == 0)
    {
        printf("split\n");
        split(argc, argv); //Debug
    }

    combine_file = COMBINE_EXT;
    combine_len = strlen(combine_file);

    if (filename_len > combine_len
        && strncmp(filename + filename_len - combine_len, combine_file, combine_len) == 0)
    {
        printf("combine\n");
        filename[filename_len - 4] = '\0'; //crop .XXX suffix
        combine(argc, argv); //Debug
    }
}

void usage(int argc, char *argv[])
{
    fprintf(stderr, "%s - 7zip/peazip compatible splitting and combining large zip files. Version " VERSION "\n", argv[0]);
    fprintf(stderr, "    Split file.zip into file.zip.001+file.zip.002+... etc.\n");
    fprintf(stderr, "    Combine file.zip.001+file.zip.002+... etc. into file.zip\n");
    fprintf(stderr, "    Size of each file is 1.5Mb\n");
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "    %s <filename>\n", argv[0]);
    fprintf(stderr, "    if filename ends with .zip then file is splitted into *.zip.XXX.\n");
    fprintf(stderr, "    if filename ends with .zip.001 then files are combined into original zip.\n");
    fprintf(stderr, "Examples:\n");
    fprintf(stderr, "    %s myfile.zip\n", argv[0]);
    fprintf(stderr, "    %s myfile.zip.001\n", argv[0]);

    exit(0);
}
#if 1
int combine(int argc, char *argv[])
{
  FILE *pFile_in, *pFile_out;
  long lSize, oSize;
  char buffer[CHUNK_SIZE];
  size_t result;
  int file_counter = 1;
  char filename[FILENAME_MAX];

  if (argc != 2) {fputs ("Invalid invocation.", stderr); exit (1);}

  snprintf(filename, FILENAME_MAX, "%s.%03d", argv[1], file_counter);


  pFile_out = fopen ( argv[1] , "wb+" );
  if (pFile_out==NULL) {fputs ("File error",stderr); exit (1);}

while (access(filename, F_OK) == 0) {
  pFile_in = fopen ( filename , "rb" );
  if (pFile_in==NULL) {fputs ("File error",stderr); exit (1);}

  // obtain file size:
  fseek (pFile_in , 0 , SEEK_END);
  lSize = ftell (pFile_in);
  rewind (pFile_in);
  oSize = SPLIT_SIZE;

  // copy the file into the buffer:
  while (lSize > 0 /* or ferror or feof*/) {
      result = fread(buffer,1,CHUNK_SIZE,pFile_in);
      fwrite(buffer,1,result,pFile_out);
      lSize -= result;
      }
        fclose (pFile_in);
        file_counter++;
        snprintf(filename, FILENAME_MAX, "%s.%03d", argv[1], file_counter);

        printf("new file %s\n", filename);

  }
//  while(result != lSize) {fputs ("Reading error",stderr); exit (3);}

  /* the whole file is now loaded in the memory buffer. */

  // terminate
  fclose (pFile_out);
  fclose (pFile_in); //FIXME warning: pFile_in may be unitialized
  return 0;
}
#endif
#if 1
int split (int argc, char *argv[])
{
  FILE *pFile_in, *pFile_out;
  long lSize, oSize;
  char buffer[CHUNK_SIZE];
  size_t result;
  int file_counter = 1;
  char filename[FILENAME_MAX];

  if (argc != 2) {fputs ("Invalid invocation.", stderr); exit (1);}

  pFile_in = fopen ( argv[1] , "rb" );
  if (pFile_in==NULL) {fputs ("File error",stderr); exit (1);}

  snprintf(filename, FILENAME_MAX, "%s.%03d", argv[1], file_counter);

  pFile_out = fopen ( filename , "wb+" );
  if (pFile_out==NULL) {fputs ("File error",stderr); exit (1);}

  // obtain file size:
  fseek (pFile_in , 0 , SEEK_END);
  lSize = ftell (pFile_in);
  rewind (pFile_in);
  oSize = SPLIT_SIZE;


  // copy the file into the buffer:
  while (lSize > 0 /* or ferror or feof*/) {
      result = fread(buffer,1,CHUNK_SIZE,pFile_in);
      fwrite(buffer,1,result,pFile_out);
      lSize -= result;
      oSize -= result;
      if (lSize > 0 && !oSize) {
        fclose (pFile_out);
        file_counter++;
        snprintf(filename, FILENAME_MAX, "%s.%03d", argv[1], file_counter);
        pFile_out = fopen ( filename , "wb+" );
        //if (pFile_out==NULL) {fputs ("File error",stderr); exit (1);}

        printf("new file %s\n", filename);
        oSize = SPLIT_SIZE;
      }
  }
//  while(result != lSize) {fputs ("Reading error",stderr); exit (3);}

  /* the whole file is now loaded in the memory buffer. */

  // terminate
  fclose (pFile_out);
  fclose (pFile_in);
  return 0;
}
#endif
