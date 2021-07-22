#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <cmath>

double x0 = 0;
int slider_x0 = 0;
int slider_x0_max = 100;

double band = 0;
int band_slider = 0;
int band_slider_max = 100;

double decaimento = 1;
int d_slider = 1;
int d_slider_max = 100;

cv::Mat image1, image2, blended, mask, peso1, peso2, blend1,blend2, matriz_alfa1, matriz_alfa2;

char TrackbarName[50];

int altura,largura;
float alfa1,alfa2;

//calculo de alpha, a partir da formula pedida no exercicio

float alpha(float ponto,float x0, float b,float d){

  float a = 0.5*(tanhf((ponto-(x0+(b/2)))/d)-tanhf((ponto-(x0-(b/2)))/d));
  return a;
}


//toda mudanca no slider modifica a imagem

void montagem_de_tiltshift(){

// Inicializa matrizes alfa de tamanho igual a da imagem

  matriz_alfa1 = cv::Mat::zeros(altura,largura,CV_32F);
  matriz_alfa2 = cv::Mat::zeros(altura,largura,CV_32F);
  
  
// preenche valores de alfa1 e alfa2
  for(int i=0;i<altura;i++){ 
      alfa1 = alpha(i,x0,band,decaimento);	
      alfa2 = 1.0 - alfa1;

      for(int j=0;j<largura;j++){
          matriz_alfa1.at<float>(i,j) = alfa1;
          matriz_alfa2.at<float>(i,j) = alfa2;
      }
  }
  
  //replica alfa pra matrizes auxiliares. Caso contrario dah erro em aritmetica
  cv::Mat aux1[] = {matriz_alfa1, matriz_alfa1, matriz_alfa1};
  cv::Mat aux2[] = {matriz_alfa2, matriz_alfa2, matriz_alfa2};
  cv::merge(aux1, 3, matriz_alfa1);
  cv::merge(aux2, 3, matriz_alfa2);
  
  
  //instancia matrizes auxiliares
  cv::Mat copia1, copia2, result_32fc;
  
  //converte para Float3C
  image1.convertTo(copia1, CV_32FC3);
  image2.convertTo(copia2, CV_32FC3);
  
  //multiplica matriz alpha
  cv::multiply(copia1, matriz_alfa1, copia1);
  cv::multiply(copia2, matriz_alfa2, copia2);
  
  //soma os dois
  cv::add(copia1, copia2, result_32fc);
  result_32fc.convertTo(blended, CV_8UC3);
  
  //Mostrar as duas imagens (original e borrada) e a saida)
  cv::imshow("imagem original", image1);
  cv::imshow("borrada", image2);
  cv::imwrite("borrada.jpg",image2);
  cv::imshow("tiltshift", blended);
  cv::imwrite("tiltshift.jpg",blended);

}



// trackbares
void on_trackbar_x0(int, void*){
 x0 = (double) slider_x0*altura/slider_x0_max ;
 std::cout<<"x0"<<x0<<std::endl;
 montagem_de_tiltshift();
 cv::imshow("tiltshift", blended);
}

void on_trackbar_band(int, void*){
 band = (double) band_slider*altura/band_slider_max ;
 std::cout<<"band"<<band<<std::endl;
 montagem_de_tiltshift();
 cv::imshow("tiltshift", blended);
}

void on_trackbar_d(int, void*){
 decaimento = (double) d_slider;
 std::cout<<"decaimento"<<decaimento<<std::endl;
 montagem_de_tiltshift();
 cv::imshow("tiltshift", blended);
}


int main(int argvc, char** argv){


  // Pega imagem e cria copia para borrar
  image1 = cv::imread("blend1.jpg");
  image2 = image1.clone();

  cv::Mat imageAux;

  //blur gaussiano
  cv::GaussianBlur(image1,image2,cv::Size(5,5),5.0,5.0);
  cv::GaussianBlur(image2,imageAux,cv::Size(5,5),5.0,5.0);
  cv::GaussianBlur(imageAux,image2,cv::Size(5,5),5.0,5.0);
  
  //altura e largura auxilia
  altura = image1.rows;
  largura = image1.cols;
  
  
  cv::namedWindow("tiltshift", 1);
  std::sprintf( TrackbarName, "x0 x %d", slider_x0_max );
  cv::createTrackbar( TrackbarName, "tiltshift",
                      &slider_x0,
                      slider_x0_max,
                      on_trackbar_x0 );
  on_trackbar_x0(slider_x0, 0 );
  
  std::sprintf( TrackbarName, "band x %d", band_slider_max );
  cv::createTrackbar( TrackbarName, "tiltshift",
                      &band_slider,
                      band_slider_max,
                      on_trackbar_band );
  on_trackbar_band(band_slider, 0 );
  
  std::sprintf( TrackbarName, "d x %d", d_slider_max );
  cv::createTrackbar( TrackbarName, "tiltshift",
                      &d_slider,
                      d_slider_max,
                      on_trackbar_d );
  on_trackbar_d(d_slider, 0 );
  

  cv::waitKey(0);
  return 0;
}
