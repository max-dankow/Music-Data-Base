create table if not exists artist
(
   id_artist INTEGER PRIMARY KEY AUTOINCREMENT,
   name varchar(255) NOT NULL,
   country varchar(255) 
);

create table if not exists album
(
   id_album INTEGER PRIMARY KEY AUTOINCREMENT,
   album_title varchar(255) NOT NULL,
   album_artist INT,
   FOREIGN KEY (album_artist) REFERENCES artist(id_artist)
);

create table if not exists song
(
   id_song INTEGER PRIMARY KEY AUTOINCREMENT,
   song_title varchar(255) NOT NULL,
   album INT NOT NULL,
   FOREIGN KEY (album) REFERENCES album(id_abum)
);

create table if not exists artist_and_songs
(
   id_song INT NOT NULL,
   id_artist INT NOT NULL,
   FOREIGN KEY (id_song) REFERENCES song(id_song)
   FOREIGN KEY (id_artist) REFERENCES artist(id_artist)
);