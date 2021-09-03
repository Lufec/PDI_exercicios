#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

#define RADIUS 20

  cv::Mat imaginaryInput, complexImage, multsp;
  cv::Mat padded, filter, mag, imFiltrado;
  cv::Mat image, imagegray, tmp, magI;
  cv::Mat_<float> realInput, log_realInput, zeros, exp_idft, ones;
  cv::Mat backgroundImage;
  std::vector<cv::Mat> planos;

  int gammaL_slider = 2, gammaH_slider = 20, C_slider = 1, D0_slider = 5;
  const int gammaL_max = 10, gammaH_max = 50, C_max = 100, D0_max = 200;
  double gammaL, gammaH,C,D0;
  
  int dft_M, dft_N;

// troca os quadrantes da imagem da DFT
void deslocaDFT(cv::Mat& image) {
  cv::Mat tmp, A, B, C, D;

  // se a imagem tiver tamanho impar, recorta a regiao para
  // evitar cópias de tamanho desigual
  image = image(cv::Rect(0, 0, image.cols & -2, image.rows & -2));
  int cx = image.cols / 2;
  int cy = image.rows / 2;

  // reorganiza os quadrantes da transformada
  // A B   ->  D C
  // C D       B A
  A = image(cv::Rect(0, 0, cx, cy));
  B = image(cv::Rect(cx, 0, cx, cy));
  C = image(cv::Rect(0, cy, cx, cy));
  D = image(cv::Rect(cx, cy, cx, cy));

  // A <-> D
  A.copyTo(tmp);
  D.copyTo(A);
  tmp.copyTo(D);

  // C <-> B
  C.copyTo(tmp);
  B.copyTo(C);
  tmp.copyTo(B);
}


void filtragem(){
    // parte imaginaria da matriz complexa (preenchida com zeros)
     zeros = cv::Mat_<float>::zeros(padded.size());
    ones = cv::Mat_<float>::zeros(padded.size());

    // prepara a matriz complexa para ser preenchida
    complexImage = cv::Mat(padded.size(), CV_32FC2, cv::Scalar(0));

    // limpa o array de matrizes que vao compor a
    // imagem complexa
    planos.clear();
    // cria a compoente real
    realInput = cv::Mat_<float>(padded);
    
    
    //log
    realInput += cv::Scalar::all(1);
    cv::log(realInput,realInput);
      
    // insere as duas componentes no array de matrizes
    planos.push_back(realInput);
    planos.push_back(zeros);

    // combina o array de matrizes em uma unica
    // componente complexa
    cv::merge(planos, complexImage);

    // calcula o dft
    cv::dft(complexImage, complexImage);
    // realiza a troca de quadrantes
    deslocaDFT(complexImage);
    cv::resize(complexImage,complexImage,padded.size());
    cv::normalize(complexImage,complexImage,0,1,cv::NORM_MINMAX);

    // a função de transferencia (filtro de frequencia) deve ter o
    // mesmo tamanho e tipo da matriz complexa
    filter = complexImage.clone();

    // cria uma matriz temporária para criar as componentes real
    // e imaginaria do filtro passa alta gaussiano
    tmp = cv::Mat(dft_M, dft_N, CV_32F);
    float D;
    // prepara o filtro passa-alta ideal
    for (int i = 0; i < dft_M; i++) {
      for (int j = 0; j < dft_N; j++) {
        D = (i-dft_M/2)*(i-dft_M/2) + (j-dft_N/2)*(j-dft_N/2);
        tmp.at<float>(i,j) = (gammaH - gammaL)*(1 - exp(-C*( D / (D0*D0) ))) + gammaL; 
      }
    }
 
    // cria a matriz com as componentes do filtro e junta
    // ambas em uma matriz multicanal complexa
    cv::Mat comps[] = {tmp, tmp};
    cv::merge(comps, 2, filter); 
    
    // aplica o filtro de frequencia
    cv::mulSpectrums(complexImage, filter, complexImage, 0);

    // troca novamente os quadrantes
    deslocaDFT(complexImage);

    // calcula a DFT inversa
    cv::idft(complexImage, complexImage);

    // limpa o array de planos
    planos.clear();

    // separa as partes real e imaginaria da
    // imagem filtrada
    cv::split(complexImage, planos);
    
    cv::exp(planos[0],planos[0]);
    // normaliza a parte real para exibicao
    cv::normalize(planos[0], planos[0], 0, 1, cv::NORM_MINMAX);
    imFiltrado = planos[0].clone();
}


void on_trackbar(int, void*){
  gammaL = (double) gammaL_slider/10;
  gammaH = (double) gammaH_slider/10;
  C = (double) C_slider;
  D0 = (double) D0_slider;
  filtragem();
  cv::imshow("Homomorphic",imFiltrado);
}


int main(int argc, char** argv) {
	
  // valores ideais dos tamanhos da imagem
  // para calculo da DFT


  image = cv::imread(argv[1],cv::IMREAD_GRAYSCALE); 
  
  cv::namedWindow("Homomorphic",cv::WINDOW_NORMAL);
  cv::imshow("Original",image);
  cv::waitKey();    
  // identifica os tamanhos otimos para
  // calculo do FFT
  dft_M = cv::getOptimalDFTSize(image.rows);
  dft_N = cv::getOptimalDFTSize(image.cols);

  // realiza o padding da imagem
  cv::copyMakeBorder(image, padded, 0, dft_M - image.rows, 0,
                     dft_N - image.cols, cv::BORDER_CONSTANT,
                     cv::Scalar::all(0));

  char TrackbarName[50]; 
  
  sprintf( TrackbarName, "Gamma L x %d", gammaL_max );
  cv::createTrackbar( TrackbarName, "Homomorphic", &gammaL_slider, gammaL_max, on_trackbar);

  sprintf( TrackbarName, "Gamma H x %d", gammaH_max );
  cv::createTrackbar( TrackbarName, "Homomorphic", &gammaH_slider, gammaH_max, on_trackbar);

  sprintf( TrackbarName, "C x %d", C_max );
  cv::createTrackbar( TrackbarName, "Homomorphic", &C_slider, C_max, on_trackbar);

  sprintf( TrackbarName, "D0 x %d", D0_max );
  cv::createTrackbar( TrackbarName, "Homomorphic", &D0_slider, D0_max, on_trackbar);
  //on_trackbar(0,0);
  cv::waitKey(0);
  return 0;
}
