#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "sqlite3.h"

int main(int argc, char **argv)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
  
    rc = sqlite3_open("MusicDB", &db);

    if( rc )
    {
       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
       sqlite3_close(db);
       return(1);
    }

    sqlite3_close(db);
    return 0;
}
