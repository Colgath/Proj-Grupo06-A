#ifndef __SMOOTH_H__
#define __SMOOTH_H__

#include <iostream>
#include <string>
#include <opencv/cv.h>
#include <opencv/ml.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <sys/time.h>
#include <omp.h>
//#include <ctime>
#include <mpi.h>

#define MASTER_TO_SLAVE_TAG 1
#define SLAVE_TO_MASTER_TAG 10 
#define PAI 0
#define INICIO 0
#define MEIO 1
#define FIM 2
//using namespace std;

void calculaRGB(cv::Mat img, cv::Mat img_saida, int i, int j, int top, int botton, int left, int right);
void sequencialColorido(std::string nome_imagem, std::string imagem_saida);
void calculaCinza(cv::Mat img, cv::Mat img_saida, int i, int j, int top, int botton, int left, int right);
void sequencialCinza(std::string nome_imagem, std::string imagem_saida);
void calculaConCinza(unsigned char *img, unsigned char *img_saida, int i, int j, int top, int botton, int left, int right, int width);
void concorrenteCinza(std::string nome_imagem, std::string imagem_saida);
void concorrenteColorido(std::string nome_imagem, std::string imagem_saida);


#endif