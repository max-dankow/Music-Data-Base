/*************** ARTISTS ******************/

insert into artist
values (NULL, "Muse", "UK");

insert into artist
values (NULL, "Franz Ferdinand", "UK");

insert into artist
values (NULL, "Arctic Monkeys", "UK");

insert into artist
values (NULL, "Prata Vetra", "Latvia");

insert into artist
values (NULL, "Motorama", "Russia");

insert into artist
values (NULL, "Каста", "Russia");

SELECT *
FROM artist;

/*************** ALBUMS ******************/

insert into album
values (NULL, "Drones", 1);

insert into album
values (NULL, "Tonight", 2);

insert into album
values (NULL, "Favorite Worse Nightmare", 3);

insert into album
values (NULL, "You Could Have It So Much Better", 2);

insert into album
values (NULL, "7 Steps of Fresh Air", 4);

insert into album
values (NULL, "Franz Ferdinand", 2);

insert into album
values (NULL, "Шаг", 4);

SELECT *
FROM album;

/*************** SONGS ******************/

insert into song
values (NULL, "Dead Inside", 1);

insert into song
values (NULL, "Psycho", 1);

insert into song
values (NULL, "Live Alone", 2);

insert into song
values (NULL, "Ulysses", 2);

insert into song
values (NULL, "Bite Hard", 2);

insert into song
values (NULL, "Lucid Dream", 2);

insert into song
values (NULL, "Brianstorm", 3);

insert into song
values (NULL, "Outsiders", 4);

insert into song
values (NULL, "I'm Your Villain", 4);

insert into song
values (NULL, "Take Me Out", 7);

insert into song
values (NULL, "Ступени", 6);

/********** SONGS AND ARTISTS ************/


insert into artist_and_songs
values (11, 6);