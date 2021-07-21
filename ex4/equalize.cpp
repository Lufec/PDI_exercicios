#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv){
  cv::Mat image, img_cinza, img_cnz_eq, hist_img, hist_eq;
  int width, height;
  cv::VideoCapture cap;
  int nbins = 64;
  float range[] = {0, 255};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;
  int key;

	cap.open(0);
  
  if(!cap.isOpened()){
    std::cout << "cameras indisponiveis";
    return -1;
  }
  
  cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
  cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);  
  width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
  height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
  
  int hist_w = nbins, hist_h = nbins/2;
  
  cv::Mat display_hist(hist_h, hist_w, CV_8UC1, cv::Scalar(0));
  cv::Mat display_hist_eq(hist_h, hist_w, CV_8UC1, cv::Scalar(0));

  while(1){
    cap >> image;
    cv::cvtColor(image,img_cinza,6); //cinza

    //equalizacao
    cv::equalizeHist(img_cinza,img_cnz_eq);

    //calcula histograma das duas imagens
    cv::calcHist(&img_cinza, 1, 0, cv::Mat(), hist_img, 1, &nbins, &histrange,uniform, acummulate);
    cv::calcHist(&img_cnz_eq, 1, 0, cv::Mat(), hist_eq, 1, &nbins, &histrange,uniform, acummulate);
    
    
    //normaliza ambas
    cv::normalize(hist_img, hist_img, 0, display_hist.rows, cv::NORM_MINMAX, -1, cv::Mat());
    cv::normalize(hist_eq, hist_eq, 0, display_hist_eq.rows, cv::NORM_MINMAX, -1, cv::Mat());
    
    //reseta o escalar pra zero (pra barra se ~movimentar~)
    display_hist.setTo(cv::Scalar(0));
    display_hist_eq.setTo(cv::Scalar(0));
    
    //plot do histograma
    for(int i=0; i<nbins; i++){
      cv::line(display_hist,
      cv::Point(i, hist_h),
      cv::Point(i, hist_h-cvRound(hist_img.at<float>(i))),cv::Scalar(255), 1, 8, 0);
      cv::line(display_hist_eq,
      cv::Point(i, hist_h),
      cv::Point(i, hist_h-cvRound(hist_eq.at<float>(i))),cv::Scalar(255), 1, 8, 0);
    }
    
    display_hist.copyTo(img_cinza(cv::Rect(0,0,nbins,hist_h)));
    display_hist_eq.copyTo(img_cnz_eq(cv::Rect(0,0,nbins,hist_h)));
    
  
    cv::imshow("img_cinza",img_cinza);
    cv::imshow("img_cinza_eq",img_cnz_eq);
    key = cv::waitKey(30);
    if(key == 27) break;
  }
  return 0;
}
