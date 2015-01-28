
/*CREATE TABLE*/
CREATE TABLE photo (id INTEGER PRIMARY KEY, path TEXT)
CREATE TABLE param (id_whitebalanceadjustb NUMERIC, id_photo INTEGER PRIMARY KEY, id_iso NUMERIC, id_shutterspeed NUMERIC, id_aperture NUMERIC, id_focus NUMERIC, id_whitebalanceadjusta NUMERIC)
CREATE TABLE desc (id_photo INTEGER PRIMARY KEY)
CREATE TABLE iso (id_iso INTEGER PRIMARY KEY, value_iso TEXT)
CREATE TABLE shutterspeed (id_shutterspeed INTEGER PRIMARY KEY, value_shutterspeed TEXT)
CREATE TABLE aperture (id_aperture INTEGER PRIMARY KEY, value_aperture TEXT)
CREATE TABLE focus (id_focus INTEGER PRIMARY KEY, value_focus TEXT)







/*INSERT*/


insert = (char *)"INSERT INTO iso (value_iso) VALUES('400');"\
                  "INSERT INTO iso (value_iso) VALUES('800');"\
                  "INSERT INTO iso (value_iso) VALUES('1600');"\
                  "INSERT INTO iso (value_iso) VALUES('3200');"\
                  "INSERT INTO iso (value_iso) VALUES('6400');";


 insert = (char *) "INSERT INTO aperture (value_aperture) VALUES ('3.5');"\
  "INSERT INTO aperture (value_aperture) VALUES ('4.0');"\
  "INSERT INTO aperture (value_aperture) VALUES ('4.5');"\
"INSERT INTO aperture (value_aperture) VALUES ('  5');"\
  "INSERT INTO aperture (value_aperture) VALUES ('5.6');"\
  "INSERT INTO aperture (value_aperture) VALUES ('6.3');"\
  "INSERT INTO aperture (value_aperture) VALUES ('7.1');"\
"INSERT INTO aperture (value_aperture) VALUES ('  8');"\
"INSERT INTO aperture (value_aperture) VALUES ('  9');"\
 "INSERT INTO aperture (value_aperture) VALUES (' 10');"\
 "INSERT INTO aperture (value_aperture) VALUES (' 11');"\
 "INSERT INTO aperture (value_aperture) VALUES (' 13');"\
 "INSERT INTO aperture (value_aperture) VALUES (' 14');"\
 "INSERT INTO aperture (value_aperture) VALUES (' 16');"\
 "INSERT INTO aperture (value_aperture) VALUES (' 18');"\
 "INSERT INTO aperture (value_aperture) VALUES (' 20');"\

 
insert = (char *) "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('5');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('4');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('3.2');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('2.5');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('2');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1.6');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1.3');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('0.8');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('0.6');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('0.5');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('0.4');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('0.3');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/4');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/5');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/6');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/8');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/10');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/13');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/15');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/20');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/25');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/30');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/40');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/50');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/60');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/80');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/100');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/125');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/160');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/200');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/250');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/320');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/400');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/500');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/640');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/800');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/1000');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/1250');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/1600');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/2000');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/2500');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/3200');"\
  "INSERT INTO shutterspeed (value_shutterspeed) VALUES ('1/400');";



  insert = (char *)"INSERT INTO focus (value_focus) VALUES('Near 1');" \
  "INSERT INTO focus (value_focus) VALUES('Near 2');" \
  "INSERT INTO focus (value_focus) VALUES('Near 3');" \
  "INSERT INTO focus (value_focus) VALUES('None');" \
  "INSERT INTO focus (value_focus) VALUES('Far 1');" \
  "INSERT INTO focus (value_focus) VALUES('Far 2');" \
  "INSERT INTO focus (value_focus) VALUES('Far 3');";













