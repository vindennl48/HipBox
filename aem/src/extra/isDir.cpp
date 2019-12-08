#include <sys/stat.h>
#include <string>

using namespace std;

bool isDir(const string &s) {
  struct stat buffer;
  return (stat (s.c_str(), &buffer) == 0);
}
