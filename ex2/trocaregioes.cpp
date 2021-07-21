#include <iostream>
#include <opencv2/opencv.hpp>
#include <algorithm>    // std::min and std::max

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  cv::Mat image;
  cv::Vec3b val;

  image= cv::imread("biel.png",cv::IMREAD_GRAYSCALE);
  if(!image.data)
    std::cout << "nao abriu biel.png" << std::endl;

  cv::namedWindow("janela", cv::WINDOW_AUTOSIZE);

  cv::imshow("janela", image);
  cv::waitKey();


  // Primeiro, coleta valores de altura e largura para 
  //determinar o ponto de corte na metade desses valores

  int width= image.rows/2;
  int heigth = image.cols/2;

  // gerar 4 retangulos, cada um respectivo ao quadrante da 
  // imagem a ser invertido. 
  // Parametros 1 e 2 sao os pontos de origem no canto superior direito
  // parametros 3 e 4 sao largura e altura do retangulo
     
  cv::Rect Q1_rect(0, 0, width, heigth);  
  cv::Rect Q2_rect(width,0 ,width, heigth);
  cv::Rect Q3_rect(0 , heigth, width, heigth);
  cv::Rect Q4_rect(width , heigth, width, heigth);

  // Proxima parte eh usar o Mat para armazenar os quadrantes das imagens
  // a partir do retangulo instanciado
  
  cv::Mat Q1 = image(Q1_rect);
  cv::Mat Q2 = image(Q2_rect);
  cv::Mat Q3 = image(Q3_rect);
  cv::Mat Q4 = image(Q4_rect);
  
  // Para nao ter problemas com a imagem de origem, eh feita
  // uma copia para trabalhar a imagem final
  
  cv::Mat img_trocada = image.clone();
  
  // Por fim, realiza a transferencia dos quadrantes Q's divididos para a nova imagem
  
  Q1.copyTo(img_trocada(Q4_rect));    //Q1->Q4
  Q2.copyTo(img_trocada(Q3_rect));    //Q2->Q3
  Q3.copyTo(img_trocada(Q2_rect));    //Q3->Q2  
  Q4.copyTo(img_trocada(Q1_rect));    //Q4->Q1
  
  // chamada da janela
  
  namedWindow("janela",WINDOW_AUTOSIZE);
  imshow("janela", img_trocada);

  waitKey();


  return 0;
}
