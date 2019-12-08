#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

inline
unsigned int getRecNum() {
  string         dir   = "./recordings";
  vector<string> files = vector<string>();
  unsigned int   max   = 0;

  /* Just to make sure this is in the correct directory */
  cout << "\ngetRecNum> Current working directory: ";
  system("pwd");
  cout << endl << endl;
  /* -- */

  DIR *dp;
  struct dirent *dirp;
  if((dp  = opendir(dir.c_str())) == NULL) {
      cout << "Error(" << errno << ") opening " << dir << endl;
      return errno;
  }

  while ((dirp = readdir(dp)) != NULL) {
    //cout << "Type: " << (to_string(dirp->d_type)).c_str() << ", Name: " << dirp->d_name << endl;
    if (dirp->d_type == 4 && string(dirp->d_name) != "." && string(dirp->d_name) != "..")
      files.push_back(string(dirp->d_name));
  }
  closedir(dp);

  int i_size = files.size();
  for (int i=0; i<i_size; i++) {
    try {
      if (stoi(files[i]) > max)
        max = stoi(files[i]);
    } catch(...) {}
  }

  return max+1;
}
