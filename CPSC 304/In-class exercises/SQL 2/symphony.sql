drop table performedin;
drop table musician;
drop table purchase;
drop table songsperformed;
drop table song;
drop table show;
drop table person;

create table Person
	(email char(40),
	name varchar(20),
	age int null,
	primary key (email));

create table Show
	(id char(9),
	year int,
	month varchar(9),
	day int,
	showing varchar(20),
	attendanceNumber int,
	primary key (id));

create table Song
	(composer varchar(20),
	title varchar(40),
	primary key (composer, title));

create table SongsPerformed
	(showID char(9),
	composer varchar(20),
	title varchar(40),
	primary key (showID, composer, title),
	foreign key (showID) references show,
	foreign key (composer, title) references song);

create table Purchase
	(email char(40),
	showID char(9),
	price decimal(10,2),
	primary key (email, showID),
	foreign key (email) references person,
	foreign key (showID) references show);

create table Musician
	(id char(9),
	name char(40),
	instrument varchar(20),
	position varchar(20),
	nationality varchar(20),
	primary key (id));

create table PerformedIn
	(id char(9),
	showID char(9),
	primary key (id, showID),
	foreign key (id) references musician,
	foreign key (showID) references show);


insert into Person
values('rickchanning@hmail.com', 'Rick Channing', 24);

insert into Person
values('lik@coldmail.com', 'Livia Karsen', 29);

insert into Person
values('smacfeather@hmail.com', 'Stearns MacFeather', 36);

insert into Person
values('annd@coldmail.com', 'Ann Dull', 38);

insert into Person
values('yokomasa@yohoo.com', 'Masahiro Yokonabe', 26);

insert into Person
values('alicegreene@coldmail.com', 'Alice Greene', 38);

insert into Person
values('yves.belanger@coldmail.com', 'Yves Belanger', 41);

insert into Person
values('jwest@hmail.com', 'Jadyn West', 23);

insert into Person
values('lf96@neteasy.com', 'Fang Li', 26);

insert into Show
values('38-95-716', 2019, 'January', 1, 'morning', 100);

insert into Show
values('68-72-397', 2019, 'January', 15, 'evening', 80);

insert into Show
values('29-86-275', 2019, 'September', 21, 'evening', 100);

insert into Show
values('92-51-504', 2020, 'February', 14, 'afternoon', 50);

insert into Song
values('Mozart', 'Symphony No. 41');

insert into Song
values('Berlioz', 'Symphonie Fantastique');

insert into Song
values('Mahler', 'Resurrection');

insert into Song
values('Beethoven', 'Choral Fantasy');

insert into Song
values('Beethoven', 'Symphony No. 5');

insert into SongsPerformed
values('38-95-716', 'Mozart', 'Symphony No. 41');

insert into SongsPerformed
values('38-95-716', 'Beethoven', 'Symphony No. 5');

insert into SongsPerformed
values('38-95-716', 'Beethoven', 'Choral Fantasy');

insert into SongsPerformed
values('68-72-397', 'Mozart', 'Symphony No. 41');

insert into SongsPerformed
values('68-72-397', 'Berlioz', 'Symphonie Fantastique');

insert into SongsPerformed
values('68-72-397', 'Beethoven', 'Choral Fantasy');

insert into SongsPerformed
values('29-86-275', 'Mahler', 'Resurrection');

insert into SongsPerformed
values('29-86-275', 'Beethoven', 'Symphony No. 5');

insert into SongsPerformed
values('92-51-504', 'Berlioz', 'Symphonie Fantastique');

insert into SongsPerformed
values('92-51-504', 'Mahler', 'Resurrection');

insert into SongsPerformed
values('92-51-504', 'Beethoven', 'Symphony No. 5');

insert into Purchase
values('lik@coldmail.com', '38-95-716', 50.00);

insert into Purchase
values('smacfeather@hmail.com', '38-95-716', 50.00);

insert into Purchase
values('jwest@hmail.com', '38-95-716', 100.00);

insert into Purchase
values('rickchanning@hmail.com', '38-95-716', 9.99);

insert into Purchase
values('yves.belanger@coldmail.com', '68-72-397', 49.99);

insert into Purchase
values('yokomasa@yohoo.com', '68-72-397', 25.00);

insert into Purchase
values('lik@coldmail.com', '68-72-397', 25.00);

insert into Purchase
values('smacfeather@hmail.com', '68-72-397', 25.00);

insert into Purchase
values('lf96@neteasy.com', '68-72-397', 25.00);

insert into Purchase
values('rickchanning@hmail.com', '29-86-275', 24);

insert into Purchase
values('smacfeather@hmail.com', '29-86-275', 36);

insert into Purchase
values('lf96@neteasy.com', '29-86-275', 26);

insert into Purchase
values('yves.belanger@coldmail.com', '29-86-275', 41);

insert into Purchase
values('lik@coldmail.com', '29-86-275', 29);

insert into Purchase
values('annd@coldmail.com', '92-51-504', 120.00);

insert into Purchase
values('lf96@neteasy.com', '92-51-504', 60.00);

insert into Purchase
values('jwest@hmail.com', '92-51-504', 99.99);

insert into Purchase
values('yokomasa@yohoo.com', '92-51-504', 60.00);

insert into Purchase
values('smacfeather@hmail.com', '92-51-504', 85.50);

insert into Musician
values('88-02-916', 'Alice Spring', 'flute', 'wind', 'Canadian');

insert into Musician
values('88-97-734', 'Michael Oatland', 'flute', 'wind', 'Canadian');

insert into Musician
values('88-72-630', 'Martin Bullard', 'clarinet', 'wind', 'French');

insert into Musician
values('88-63-719', 'Greg Jackson', 'violin', 'string', 'American');

insert into Musician
values('88-18-037', 'Emily Ross', 'viola', 'string', 'Canadian');

insert into Musician
values('88-52-138', 'Gilles Gagnon', 'timpani', 'percussion', 'Canadian');

insert into Musician
values('88-05-603', 'Karl Schmidt', 'cymbals', 'percussion', 'Austrian');

insert into Musician
values('88-08-927', 'Edward Atkinson', 'trumpet', 'brass', 'Canadian');

insert into Musician
values('88-37-561', 'Aisling Quinn', 'trombone', 'brass', 'Canadian');

insert into PerformedIn
values('88-02-916', '38-95-716');

insert into PerformedIn
values('88-63-719', '38-95-716');

insert into PerformedIn
values('88-05-603', '38-95-716');

insert into PerformedIn
values('88-37-561', '38-95-716');

insert into PerformedIn
values('88-72-630', '68-72-397');

insert into PerformedIn
values('88-63-719', '68-72-397');

insert into PerformedIn
values('88-05-603', '68-72-397');

insert into PerformedIn
values('88-18-037', '29-86-275');

insert into PerformedIn
values('88-52-138', '29-86-275');

insert into PerformedIn
values('88-08-927', '29-86-275');

insert into PerformedIn
values('88-37-561', '29-86-275');

insert into PerformedIn
values('88-02-916', '92-51-504');

insert into PerformedIn
values('88-63-719', '92-51-504');

insert into PerformedIn
values('88-52-138', '92-51-504');

insert into PerformedIn
values('88-05-603', '92-51-504');

insert into PerformedIn
values('88-37-561', '92-51-504');
