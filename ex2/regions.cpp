#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char**){
  cv::Mat image;
  cv::Vec3b val;
  
  int P1x,P1y,P2x,P2y;
  
  image= cv::imread("biel.png",cv::IMREAD_GRAYSCALE);
  if(!image.data)
    std::cout << "nao abriu biel.png" << std::endl;

  cv::namedWindow("janela", cv::WINDOW_AUTOSIZE);

  int width= image.rows;
  int heigth = image.cols;

  std::cout<<"Insira as coordenadas de p1x,p1y,p2x,p2y. Lembrando que Img de tamanho "<<width<<"x"<<heigth<<std::endl;

  std::cin>>P1x>>P1y>>P2x>>P2y;
  //poderia inserir uma condicao para realizar a insercao de pontos novamente caso a largura ou altura seja maior que a original 

  cv::imshow("janela", image);

  cv::waitKey();


   // Le imagem

  image= cv::imread("biel.png",cv::IMREAD_COLOR);

  // a partir dos dois pontos inputados, realiza a iteracao na regiao

  for(int i=P1x;i<P2x;i++){
    for(int j=P1y;j<P2y;j++){
    	
    	//pega valor de cor no pixel
    	
    	Vec3b cor = image.at<Vec3b>(i,j);
    	
    	//inverte pixel
    	cor.val[0] = 255 - cor.val[0];
    	cor.val[1] = 255 - cor.val[1];
    	cor.val[2] = 255 - cor.val[2];
    
        //reescreve ele na posicao 
	image.at<Vec3b>(i,j) = cor;
    }
  }

  cv::imshow("janela", image);
  cv::waitKey();
  return 0;
}
