create table SpeechIndex( speech text primary key not null, ProcessName text, TableName text);

create table OpenCommand( speech text primary key not null, cmd text, type text);

create table zhIndex( zhcn text primary key not null, pinyin text);

create table gnome_terminal( speech text primary key not null, cmd text, type text);
create table wpp( speech text primary key not null, cmd text, type text);
create table geany( speech text primary key not null, cmd text, type text);
create table nautilus( speech text primary key not null, cmd text, type text);
create table deepin_music_player( speech text primary key not null, cmd text, type text);
create table firefox( speech text primary key not null, cmd text, type text);
