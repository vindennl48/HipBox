#include "../data/data.h"

void *rec_thread(void *d) {
  int i_size = 0;

  while(true) {
    if (is_recording) {
      i_size = rec_files.size();

      // create the wav file and dir structure
      for (int i=0; i<i_size; i++) {
        RecFile *rec_file = &rec_files[i];
        rec_file->create_file();
      }

      while(is_recording) {
        // create the wav file and dir structure
        for (int i=0; i<i_size; i++) {
          RecFile *rec_file = &rec_files[i];
          rec_file->write_next_sample();
        }
      }

      // close out all recorded files
      for (int i=0; i<i_size; i++) {
        RecFile *rec_file = &rec_files[i];
        rec_file->close_file();
      }
    }

    usleep(10);
  }

  return NULL;
}

