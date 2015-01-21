#include <sys/types.h>  // For stat().
#include <sys/stat.h>   // For stat().
#include <iostream>
#include <string>
#include "TString.h"
using namespace std;

int mkfolder(TString strPath){

  //string strPath;
  //cout << "Enter directory to check: ";
  //cin >> strPath;

  if ( access( ((string)strPath).c_str(), 0 ) == 0 )
  {
    struct stat status;
    stat( ((string)strPath).c_str(), &status );

    if ( status.st_mode & S_IFDIR )
    {
      cout << "The directory exists." << endl;
      return 0;
    }
    else
    {
      cout << "The path you entered is a file." << endl; return 1;
    }
  }
  else
  {
    cout << "Directory doesn't exist" << endl;
      mkdir(strPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
      cout<< "Directory created"<<endl; return 0;
  }


}
