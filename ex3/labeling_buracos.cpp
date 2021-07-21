#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv){
  cv::Mat image, realce;
  int width, height;
  int nobjects;
  
  cv::Point p;
  image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
  
  if(!image.data){
    std::cout << "imagem nao carregou corretamente\n";
    return(-1);
  }

  width=image.cols;
  height=image.rows;
  std::cout << width << "x" << height << std::endl;

  p.x=0;
  p.y=0;

  cv::imshow("Imagem Original", image);

 ////////// Apagar objetos tocando a borda  //////////////
 ///// Itera nas 4 bordas. Se cor do pixel = 255, floodfill nele

 // borda superior
  for(int i=0; i < height; i++){
    if(image.at<uchar>(0,i) == 255){
        p.x= i;
        p.y= 0; 
        // Apaga objeto, aplicando floodfill com a cor do fundo
        cv::floodFill(image,p,0);
      }
    }

  //borda inferior
  for(int i=0; i < height; i++){
    if(image.at<uchar>(width-1,i) == 255){
        p.x= i;
        p.y= width-1; 
        // Apaga objeto, aplicando floodfill com a cor do fundo
        cv::floodFill(image,p,0);
      }
    }
    
  //borda esquerda
  for(int i=0; i < width; i++){
    if(image.at<uchar>(i,0) == 255){
        p.x= 0;
        p.y= i; 
        // Apaga objeto, aplicando floodfill com a cor do fundo
        cv::floodFill(image,p,0);
      }
    }
    
  //borda direita
  for(int i=0; i < width; i++){
    if(image.at<uchar>(i,height-1) == 255){
        p.x= height-1;
        p.y= i; 
        // Apaga objeto, aplicando floodfill com a cor do fundo
        cv::floodFill(image,p,0);
      }
    }
       
  //zera posicao para origem novamente
  p.x=0;
  p.y=0;


  // busca todas as bolhas presentes, tal qual labeling.cpp
  // depois subtrai-se as bolhas com furos para obter bolhas sem furos
  nobjects=0;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 255){
        // achou um objeto
        nobjects++;
        p.x=j;
        p.y=i;
  	// preenche o objeto com o contador
	cv::floodFill(image,p,nobjects);
      }
    }
  }
 
  // inverte cor de fundo para 255. Com isso, interior das bolhas furadas sera 0.
  
  //volta posicao pra origem
  p.x = 0; 
  p.y = 0;
  cv::floodFill(image,p,255);
  
  // Contagem de objetos furados
  int nobjects_furos = 0;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 0){  //agora procura cor preta
        nobjects_furos++;
        p.x= j;
        p.y= i;
  	// preenche o objeto com o contador
	cv::floodFill(image,p,255);
    	}
    }
  }

  int nobjects_inteiro = nobjects - nobjects_furos;

  std::cout << " Sao " << nobjects_furos << " bolhas com furos \n";
  std::cout << " Sao " << nobjects_inteiro << " bolhas sem furos \n";
  
  cv::imshow("image", image);
  cv::waitKey();
  return 0;
}
