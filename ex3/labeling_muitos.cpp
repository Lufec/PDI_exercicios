#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

//Solucao: reseta nobjects, soma 1 em um contador (multiplo de 256) e continua a contar

int main(int argc, char** argv){
  cv::Mat image, realce;
  int width, height;
  int nobjects;
  int cont256 = 0;
  
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

  // busca objetos presentes
  nobjects=0;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 255){
        // achou um objeto
        nobjects++;
        p.x=j;
        p.y=i;
        
        //reset se nobjects = 255
        if(nobjects == 255){
            cont256++;
            nobjects = 1;
        }
  		// preenche o objeto com o contador
		  cv::floodFill(image,p,nobjects);
      }
    }
  }
  std::cout << "a figura tem " << cont256*256 + nobjects << " bolhas\n";
  cv::equalizeHist(image, realce);
  cv::imshow("image", image);
  cv::imshow("realce", realce);
  cv::imwrite("labeling.png", image);
  cv::waitKey();
  return 0;
}
