//Background substraction 

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <cv.h>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int main()
{
  Size t = Size(240,320);
  
  //0. Define variables
  float alpha = 0.9;
  float beta = 30;
  struct dirent **namelist;
  Mat Mean = Mat::zeros(240, 320,CV_64F);
  Mat tmp = Mat::zeros(240, 320,CV_64F);
  Mat Deviation = Mat::zeros(240, 320,CV_64F);

  //1. Data load - MEAN & SD Calculate

  //Load & Accumulate
  int n = scandir("../input", &namelist, 0, alphasort);
  if (n < 0)
    perror("scandir");
  else {
    for (int i = 2; i < 152; i++) {
      std::string buf("../input/");
      buf.append(namelist[i]->d_name);
      tmp = imread(buf,0);
      tmp.convertTo(tmp,CV_64F);
      accumulate(tmp,Mean);
    }   
  }
  //Mean
  free(namelist);
  Mean = Mean / 150;

  //SD
  n = scandir("../input", &namelist, 0, alphasort);
  if (n < 0)
    perror("scandir");
  else {
    for (int i = 2; i < 152; i++) {
      std::string buf("../input/");
      buf.append(namelist[i]->d_name);
      tmp = imread(buf,0);
      tmp.convertTo(tmp,CV_64F);
      tmp -= Mean;
      accumulateSquare(tmp,Deviation);
    }
  }
  free(namelist);
  Deviation = Deviation/(150-1);
  cv::sqrt(Deviation,Deviation);


  //3.Segmentation with threshold
  tmp.convertTo(tmp,CV_8U);
  Mean.convertTo(Mean,CV_8U);
  double threashold=60;
  n = scandir("../input", &namelist, 0, alphasort);
  if (n < 0)
    perror("scandir");
  else {
    for (int i = 153; i < 302; i++) {
      std::string buf("../input/");
      buf.append(namelist[i]->d_name);
      tmp = imread(buf,0);
      threshold( abs(tmp-Mean), tmp, threashold, 255,0 );
    }
  }
  free(namelist);

  //4. Segmentation with gaussian model
  cv::VideoWriter writer;
  writer.open ( "test.avi", CV_FOURCC('P','I','M','1'), 25, cv::Size ( 320,240), 0 );
  tmp.convertTo(tmp,CV_8U);
  Mean.convertTo(Mean,CV_8U);
  Deviation.convertTo(Deviation,CV_8U);
  n = scandir("../input", &namelist, 0, alphasort);
  if (n < 0)
    perror("scandir");
  else {
    for (int i = 153; i < 302; i++) {
      std::string buf("../input/");
      buf.append(namelist[i]->d_name);
      tmp = imread(buf,0);
      tmp = abs(tmp-Mean) > alpha*(Deviation + beta);
      writer.write(tmp);
    }
  }
  free(namelist);
  return 0;
} 
