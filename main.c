#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sqlite3.h"
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <linux/limits.h>

sqlite3* open_database(char* database_name)
{
    sqlite3* database_ptr;

    if(sqlite3_open(database_name, &database_ptr) != SQLITE_OK)
    {
       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(database_ptr));
       sqlite3_close(database_ptr);
       exit(EXIT_FAILURE);
    }
    return database_ptr;
}


void run_query(struct sqlite3_stmt* st, sqlite3* db)
{
        int code;
        const char *colName;

        for (int i = 0; i < sqlite3_column_count(st); ++i)
        {
            colName = sqlite3_column_name(st, i);
            printf("%-40s\t", colName);
        }

        if (sqlite3_column_count(st) > 0)
            printf("\n");

        while (code = sqlite3_step(st) == SQLITE_ROW)
        {
             const unsigned char *text;

             for (int i = 0; i < sqlite3_column_count(st); ++i)
             {
                 text = sqlite3_column_text(st, i);
                 printf("%-40s\t", text);
             }

             printf("\n");
        }

        if (code == SQLITE_DONE)
        {
            return;
        }
}

void process_query(const char* begin, const char* end, sqlite3* db)
{
    while (end - begin > 0)
    {
        struct sqlite3_stmt* st;
        int code = sqlite3_prepare(db, begin, end - begin, &st,&begin);
        fprintf(stderr, "Operation status: %s\n", sqlite3_errmsg(db));
        if (code != SQLITE_OK)
        {
            return;
        }
        run_query(st, db);
    }
}

void print_discography(char* artist_name, sqlite3* db)
{
    const char* s = "select album.album_title, song.song_title from (artist INNER JOIN album ON album.album_artist = artist.id_artist) LEFT JOIN song ON song.album = album.id_album WHERE artist.name = ? ORDER BY album.album_title, song.song_title;";

    struct sqlite3_stmt* st;
    int code = sqlite3_prepare(db, s, strlen(s), &st, &s);

    if (code != SQLITE_OK)
    {
        fprintf(stderr, "Сan't do prepare: %s\n", sqlite3_errmsg(db));
        return;
    }

    int bind_code = sqlite3_bind_text (st, 1, artist_name, strlen(artist_name), NULL);
    if (bind_code != SQLITE_OK)
    {
        fprintf(stderr, "Bind error. %s\n", sqlite3_errmsg(db));
    }
    run_query(st, db);
}

void execute_script(char* file_name, sqlite3* database_ptr)
{
    int query_file = open(file_name, O_RDONLY);
    if (query_file == -1)
    {
        perror("Can't open sql file.");
        return;
    }
  //получаем размер файла
    struct stat info;
    if (stat(file_name, &info) == -1)
    {
        perror("Can't get file size.");
        return;
    }
    size_t file_size = (int) info.st_size;
    if (file_size == 0)
    {
        return;
    }
  //отображаем содержимое скрипта в файл  
    char* data = (char*) mmap(NULL, file_size, PROT_READ, MAP_SHARED, query_file, 0);
    printf("Query is:\n%s\n", data);
  //запускаем исполнение  
    process_query(data, data + file_size, database_ptr);
    close(query_file);
    munmap(data, file_size);
}

void run_interface(sqlite3* database_ptr)
{
    printf("Hello!\nComands are:\nto execute script -> run <script_name>\nto show discography -> show <artist_name>\nto terminate -> exit\n");
    while (1)
    {
        printf("Enter comand: ");
        char comand[256];
        int code = scanf("%s", comand);
        if (code <= 0)
        {
            break;
        }
        if (strcmp(comand, "run") == 0)
        {
            char script_file_name[PATH_MAX];
            scanf("%s", script_file_name);
            printf("Executing script: %s\n", script_file_name);   
            execute_script(script_file_name, database_ptr);
            continue;
        }
        if (strcmp(comand, "show") == 0)
        {
            char* artist_to_find = (char*) malloc(256);
            int length;
            length = getline(&artist_to_find, &length, stdin);
            artist_to_find[length - 1] = '\0';
            artist_to_find++;
            printf("Showing %s discography:\n", artist_to_find);
            print_discography(artist_to_find, database_ptr);
            continue;
        }
        if (strcmp(comand, "exit") == 0)
        {
            break;
        }
        printf("Wrong comand. Please enter:\nto execute script -> run <script_name>\nto show discography -> show <artist_name>\nto terminate -> exit\n");
    }
    printf("Good bye!\n");
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Wrong arguments\n");
        return EXIT_FAILURE;
    }
    sqlite3* database_ptr = open_database(argv[1]);
    run_interface(database_ptr);
    //execute_script("drop.sql", database_ptr);
/*
    char* artist_to_find = (char*) malloc(256);
    int length;
    length = getline(&artist_to_find, &length, stdin);
    artist_to_find[length - 1] = '\0';
    printf("ARTIST: %s\n", artist_to_find);
    print_discography(artist_to_find, database_ptr);

    //process_query(data, data + size, db);
*/
    sqlite3_close(database_ptr);
    return 0;
}
