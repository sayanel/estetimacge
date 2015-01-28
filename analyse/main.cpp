#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cassert>
#include <cstring>
#include <vector>
#include <ctime>
#include <fstream>
#include <math.h>

//#include <Eigen/Dense>
#include "ImageRGB.hpp"
#include "ioJPG.hpp"
#include "exif.h"

#include "photo.hpp"
//#include "hdr.hpp"

#include <sqlite3.h> 

using namespace kn;


static int callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

void loadImages(const int argc, char **argv, std::vector<ImageRGB8u> &images, std::string & sql, sqlite3 *db){


  char* zErrMsg = 0;
  const char * query;

  for(int i=1; i<argc; ++i){
    //load the image
    ImageRGB8u imgageTmp;
    std::cout << "loading '" << argv[i] << "' ...";
    loadJPG(imgageTmp,argv[i]);
    images.push_back(imgageTmp);
    std::cout << " done" << std::endl;

    // load the exposure duration from the exif
    EXIFInfo exifReader;
    int parseSuccess = exifReader.parseFrom(fileToString(argv[i]));
    if(parseSuccess != PARSE_EXIF_SUCCESS){
      exifParsingError(parseSuccess);
      exit(0);
    }
    std::cout << " ImageDescription : " << exifReader.ImageDescription << std::endl;
    std::cout << " wxh : " << exifReader.ImageWidth << " x " << exifReader.ImageHeight << std::endl;
    std::cout << " exposure : " << exifReader.ExposureTime << " s" << std::endl;
    std::cout << " flash : " << ((exifReader.Flash==0)?"no":"yes") << std::endl;
    std::cout << " camera : " << exifReader.Model << std::endl;
    std::cout << " ISO : " << exifReader.ISOSpeedRatings << std::endl;
    std::cout << " apperture : " << exifReader.FNumber << std::endl;
    std::cout << " MeteringMode : " << exifReader.MeteringMode << std::endl;
    std::cout << " FocusDistance: " << exifReader.SubjectDistance << " m" << std::endl;
    std::cout << std::endl;

    //char* strcat(char* str1, char* str2); 
    std::stringstream ss_iso;
    std::stringstream ss_id_iso;
    ss_iso << exifReader.ISOSpeedRatings;
    // std::string select = "SELECT id_iso from iso WHERE value_iso = '";
    // select.append(ss_iso.str());
    // select += "'";
    // query = select.c_str();
    // int id_iso = sqlite3_exec(db, query, callback, 0, &zErrMsg);
    //std::cout << "id_iso= " << id_iso << std::endl;

    std::stringstream ss_aperture;
    std::stringstream ss_id_aperture;
    ss_aperture << exifReader.FNumber;
    // select = "SELECT id_aperture from aperture WHERE value_aperture = '";
    // select.append(ss_aperture.str());
    // select += "'";
    // query = select.c_str();
    // int id_aperture = sqlite3_exec(db, query, callback, 0, &zErrMsg);
    //std::cout << "id_aperture= " << id_aperture << std::endl;

    std::stringstream ss_shutterspeed;
    ss_shutterspeed << exifReader.ExposureTime;
    // select = "SELECT id_shutterspeed from shutterspeed WHERE value_shutterspeed = '";
    // select.append(ss_shutterspeed.str());
    // select += "'";
    // query = select.c_str();
    // int id_shutterspeed = sqlite3_exec(db, query, callback, 0, &zErrMsg);
    // std::cout << "id_shutterspeed= " << ss_shutterspeed.str() << std::endl;

    std::stringstream ss_focus;
    ss_focus << exifReader.SubjectDistance;

    // char str_iso[100];
    // snprintf(str_iso, 100, "%d", id_iso);



    sql += "INSERT INTO photo_param (path,id_shutterspeed,id_aperture,id_iso, focus) VALUES ('";
      sql += argv[i];
      sql += "' , '";
      sql += ss_shutterspeed.str();
      sql += "' , '";
      sql += ss_aperture.str();
      sql += "' , '";
      sql += ss_iso.str();
      sql += "' , '";
      sql += ss_focus.str();
    sql+="');";


    //strcat(sql, requete);
    
  }
}


//////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
  if(argc < 2){
    std::cerr << "usage : " << argv[0] << " image_1.jpg ... image_n.jpg" << std::endl;
    std::cerr << "or : " << argv[0] << " dirname/*.jpg" << std::endl;
    exit(0);
  }

  sqlite3 *db;
  char* zErrMsg = 0;
  int  rc;
  std::string sql;

  std::vector<ImageRGB8u> images;

 /* Open database */
   char* db_path = (char*)"../db/database_images"; 

   rc = sqlite3_open(db_path, &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stdout, "Opened database successfully\n");
   }


  loadImages(argc, argv, images, sql, db);
  std::string str;
  const char * query = sql.c_str();
  std::cout << sql << std::endl;


   /* Execute SQL statement */
   rc = sqlite3_exec(db, query, callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{fprintf(stdout, "Table insert successfully\n");}

  sqlite3_close(db);


  //saveJPG(result,"output/result.jpg");

  return 0;
}