#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sqlite3.h"
#include "fcntl.h"

void process_query(const char* begin, const char* end, sqlite3* db)
{
    while (end - begin)
    {
        printf("%s\n", begin);
        struct sqlite3_stmt* st;

        int code = sqlite3_prepare(db,
                               begin,
                               end - begin,
                               &st,
                               &begin);

        if (code != SQLITE_OK)
        {
            fprintf(stderr, "Сan't do prepare: %s\n", sqlite3_errmsg(db));
        }

        const char *colName;

        for (int i = 0; i < sqlite3_column_count(st); ++i)
        {
            colName = sqlite3_column_name(st, i);
            printf("%s\t", colName);
        }

        printf("\n");

        while (code = sqlite3_step(st) == SQLITE_ROW)
        {
             const unsigned char *text;

             for (int i = 0; i < sqlite3_column_count(st); ++i)
             {
                 text = sqlite3_column_text(st, i);
                 printf("%s\t", text);
             }

             printf("\n");
        }

        if (code == SQLITE_DONE)
        {
            return;
        }

        printf("**********************************\n");
        sleep(2);
    }
}

int main(int argc, char **argv)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    if (argc != 3)
    {
        fprintf(stderr, "Wrong arguments\n");
    }
  
    rc = sqlite3_open("MusicDB", &db);

    if(rc != SQLITE_OK)
    {
       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
       sqlite3_close(db);
       return(1);
    }
    
    printf("DB opened.\n");

    FILE* query_file = fopen("query.txt", "r");

    if (query_file == NULL)
    {
        perror("Can't open sql file.");
        return 1;
    }

    struct stat info;
    
    if (stat("query.txt", &info) == -1)
    {
        perror("Can't get file size.");
        return 1;
    }

    size_t size = (int) info.st_size;

    printf("File size == %d\n", (int) size);
    char* data = (char*) malloc(size + 1);
    data[size] = '\0';
    fread(data, size, 1, query_file);
    printf("Query is:\n%s\n", data);

    struct sqlite3_stmt* st;

    process_query(data, data + size, db);

    free(data);
    fclose(query_file);
    sqlite3_close(db);
    return 0;
}
