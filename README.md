# PDI - Exercicios

# Luiz Fernando Carbonera Filho

## Questao 2.1

**Utilizando o programa exemplos/pixels.cpp como referência, implemente um programa regions.cpp. Esse programa deverá solicitar ao usuário as coordenadas de dois pontos P1 e P2 localizados dentro dos limites do tamanho da imagem e exibir que lhe for fornecida. Entretanto, a região definida pelo retângulo de vértices opostos definidos pelos pontos P1 e P2 será exibida com o negativo da imagem na região correspondente.**

Usando como refer"encia o pixels.cpp, foram realizados os seguintes acrescimos:
* Codigo cin para o usuario inserir as coordenadas dos pontos P1 e P2, tais que eles nao ultrapassem os limites da imagem;
```
  std::cout<<"Insira as coordenadas de p1x,p1y,p2x,p2y. Lembrando que Img de tamanho "<<width<<"x"<<heigth<<std::endl;

  std::cin>>P1x>>P1y>>P2x>>P2y;

```
* Itera em cada pixel dentro do retangulo delimitado por P1 e P2 para realizar a inversao de valores. Essa inversao foi realizada pegando a cor atual do pixel lido, trocando a cor por "cor = 255 - cor", e por fim recolocando-o na imagem final.

```
  for(int i=P1x;i<P2x;i++){
    for(int j=P1y;j<P2y;j++){
    	//pega valor de cor no pixel 	
    	cv::Vec3b cor = image.at<Vec3b>(i,j);   	
    	//inverte pixel
    	cor.val[0] = 255 - cor.val[0];
    	cor.val[1] = 255 - cor.val[1];
    	cor.val[2] = 255 - cor.val[2];  
        //reescreve ele na posicao 
	image.at<Vec3b>(i,j) = cor;
    }
  }
```
* Apos modificar os pixels, a imagem foi salva usando imwrite.

(inserir as duas imagens)


## Questao 2.2
**Utilizando o programa exemplos/pixels.cpp como referência, implemente um programa trocaregioes.cpp. Seu programa deverá trocar os quadrantes em diagonal na imagem. Explore o uso da classe Mat e seus construtores para criar as regiões que serão trocadas.**

* Primeiro, encontra-se o ponto central da imagem
```
  int width= image.rows/2;
  int heigth = image.cols/2;
```
* Em seguida, delimita quatro retangulos, sendo cada um referente ao quadrantes a ser trocados;

```
  cv::Rect Q1_rect(0, 0, width, heigth);  
  cv::Rect Q2_rect(width,0 ,width, heigth);
  cv::Rect Q3_rect(0 , heigth, width, heigth);
  cv::Rect Q4_rect(width , heigth, width, heigth);
```
* A etapa seguinte consiste no uso de Mat. Nesse caso, ela ira armazenar as partes das imagens cujos parametros sao os retangulos instanciados anteriormente;
```
  cv::Mat Q1 = image(Q1_rect);
  cv::Mat Q2 = image(Q2_rect);
  cv::Mat Q3 = image(Q3_rect);
  cv::Mat Q4 = image(Q4_rect);
```
* Para nao ocorrer conflitos com a imagem original, foi gerada uma copia da imagem original, onde essa copia sera a imagem de saida. A area delimitada pelo retangulo i sera inserida na regiao da imagem delimitada pelo retangulo j respectivo da troca.

```
  Q1.copyTo(img_trocada(Q4_rect));    //Q1->Q4
  Q2.copyTo(img_trocada(Q3_rect));    //Q2->Q3
  Q3.copyTo(img_trocada(Q2_rect));    //Q3->Q2  
  Q4.copyTo(img_trocada(Q1_rect));    //Q4->Q1
```
Resultado:

(imagens)

## Questao 3.1
**Observando-se o programa labeling.cpp como exemplo, é possível verificar que caso existam mais de 255 objetos na cena, o processo de rotulação poderá ficar comprometido. Identifique a situação em que isso ocorre e proponha uma solução para este problema.**

* O problema aparecera se houver mais de 255 objetos devido ao tamanho da variavel que as armazenam (char). Para resolver isso sem modificar o tipo, foi criada uma variavel de contador, que agira como "multiplo de 256". A cada 256 objetos, a variavel char sera resetada para 1 (nao 0, pois essa eh a cor de fundo) e o contador eh incrementado em 1.

```
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
```

## Questao 3.2 
**Aprimore o algoritmo de contagem apresentado para identificar regiões com ou sem buracos internos que existam na cena. Assuma que objetos com mais de um buraco podem existir. Inclua suporte no seu algoritmo para não contar bolhas que tocam as bordas da imagem. Não se pode presumir, a priori, que elas tenham buracos ou não.**

* Para resolver o problema dos objetos que tocam as bordas, foi realizada 4 iteracoes, sendo cada uma varrendo uma borda da imagem. 
.* Se um pixel for "0", eh indicativo de que o objeto toca a borda. Portanto ele sera apagado usando floodfill com a cor do fundo.

```
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
```
* A contagem de objetos com e sem buraco seguira a logica: 
.* Conta todos os objetos existentes;
.* Conta os objetos com buracos;
.* Subtrai o primeiro do segundo para obter os objetos sem buraco.

* Para contar todos os objetos existentes, a logica implementada por labeling foi utilizada.

```
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
```
* Para contar os objetos com furos, a cor de fundo foi modificada. Isso fara com que os furos possuam cor diferente da cor de fundo atual. A partir disso, conta-se os objetos furados da imagem e pinta esses furos com a cor de fundo atual. Nao serao pintados os objetos em si pois eles ja o foram na etapa anterior, portanto nao influenciarao na contagem de furos existentes.

```
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
```
* Por fim, subtrai-se o valor de numero de objetos total pelo numero de objetos furados

```
 int nobjects_inteiro = nobjects - nobjects_furos;

  std::cout << " Sao " << nobjects_furos << " bolhas com furos \n";
  std::cout << " Sao " << nobjects_inteiro << " bolhas sem furos \n";
```
Resultado:

(imagens)

## Questao 4
**Utilizando o programa exemplos/histogram.cpp como referência, implemente um programa equalize.cpp. Este deverá, para cada imagem capturada, realizar a equalização do histogram antes de exibir a imagem. Teste sua implementação apontando a câmera para ambientes com iluminações variadas e observando o efeito gerado. Assuma que as imagens processadas serão em tons de cinza.**

* A resolucao dessa questao consistiu no uso da funcao _equalize_. Ela ira receber como argumentos a variavel que possui a imagem original e uma variavel que sera armazenada a imagem equalizada

```
cv::equalizeHist(img_cinza,img_cnz_eq);
```

* Para visualizar ambos histogramas, foi utilizada a funcao _calcHist_, que ira gerar os histogramas das imagens a partir de seus argumentos recebidos. O resultado desses histogramas serao armazenados em objetos da classe _Mat_ de nome hist_img e hist_eq.

```
    cv::calcHist(&img_cinza, 1, 0, cv::Mat(), hist_img, 1, &nbins, &histrange,uniform, acummulate);
    cv::calcHist(&img_cnz_eq, 1, 0, cv::Mat(), hist_eq, 1, &nbins, &histrange,uniform, acummulate);
```
* Foi realizada a normalizacao desses histogramas;

```
    cv::normalize(hist_img, hist_img, 0, display_hist.rows, cv::NORM_MINMAX, -1, cv::Mat());
    cv::normalize(hist_eq, hist_eq, 0, display_hist_eq.rows, cv::NORM_MINMAX, -1, cv::Mat());
```
* Para as imagens dos histogramas atualizarem, eh necessario resetar-los usando _setTo_.

```
    display_hist.setTo(cv::Scalar(0));
    display_hist_eq.setTo(cv::Scalar(0));
```
* Apos gerar-los basta seguir as etapas presentes em _histograma.cpp_ para mostrar-los na imagem coletada da camera.
* Resultado:

(imagens..)

## Questao 4.2
**Utilizando o programa exemplos/histogram.cpp como referência, implemente um programa motiondetector.cpp. Este deverá continuamente calcular o histograma da imagem (apenas uma componente de cor é suficiente) e compará-lo com o último histograma calculado. Quando a diferença entre estes ultrapassar um limiar pré-estabelecido, ative um alarme. Utilize uma função de comparação que julgar conveniente.**

Logica utilizada:
1. Antes de iniciar o laco while(true), captura o valor dos histogramas em R,G,B em uma variavel _hist_Anterior. Foi decidido por capturar dos tres pois soh precisa um deles modificar.  
2. Dentro do laco while(true), captura o histograma RBG atual;
3. Realiza a comparacao entre os dois histogramas calculados anteriormente usando a funcao _compareHist_.
4. Se o resultado dessa comparacao ultrapassar um certo limiar(threshold), ativa o alarme;
5. No fim da iteracao, atualiza o _hist_Anterior_ para o valor atual. Com isso, a proxima iteracao sera entre o frame anterior (hist_Anterior) e o frame atual (hist_Atual).

* Primeiro, a captura inicial do valor de hist_Anterior

```
  cv::calcHist(&planes[0], 1, 0, cv::Mat(), histR_anterior, 1,
                 &nbins, &histrange,
                 uniform, acummulate);
  cv::calcHist(&planes[1], 1, 0, cv::Mat(), histG_anterior, 1,
                 &nbins, &histrange,
                 uniform, acummulate);
  cv::calcHist(&planes[2], 1, 0, cv::Mat(), histB_anterior, 1,
                 &nbins, &histrange,
                 uniform, acummulate);
```

* Em seguida, dentro do la;o while(true), Captura o hist_atual;

```
    cv::calcHist(&planes[0], 1, 0, cv::Mat(), histR, 1,
                 &nbins, &histrange,
                 uniform, acummulate);
    cv::calcHist(&planes[1], 1, 0, cv::Mat(), histG, 1,
                 &nbins, &histrange,
                 uniform, acummulate);
    cv::calcHist(&planes[2], 1, 0, cv::Mat(), histB, 1,
                 &nbins, &histrange,
                 uniform, acummulate);
```

* Realiza-se a comparacao entre os histogramas RGB capturados;

```
    correlacaoR = cv::compareHist(histR,histR_anterior,cv::HISTCMP_CORREL);
    correlacaoG = cv::compareHist(histG,histG_anterior,cv::HISTCMP_CORREL);
    correlacaoB = cv::compareHist(histB,histB_anterior,cv::HISTCMP_CORREL);
```
* Realiza a avaliacao se essa correlacao ultrapassou o limiar;

```
if(correlacaoR <= threshold || correlacaoG <= threshold || correlacaoB <= threshold){
      std::cout << "Movimento!" << std::endl;
}
```

* Por fim, recalcula o hist_Anterior no fim do laco;

```
    cv::calcHist(&planes[0], 1, 0, cv::Mat(), histR_anterior, 1,
                 &nbins, &histrange,
                 uniform, acummulate);
    cv::calcHist(&planes[1], 1, 0, cv::Mat(), histG_anterior, 1,
                 &nbins, &histrange,
                 uniform, acummulate);
    cv::calcHist(&planes[2], 1, 0, cv::Mat(), histB_anterior, 1,
                 &nbins, &histrange,
                 uniform, acummulate);

```
* Resultado:

(imagens)


## Questao 5
**Utilizando o programa exemplos/filtroespacial.cpp como referência, implemente um programa laplgauss.cpp. O programa deverá acrescentar mais uma funcionalidade ao exemplo fornecido, permitindo que seja calculado o laplaciano do gaussiano das imagens capturadas. Compare o resultado desse filtro com a simples aplicação do filtro laplaciano.**

* Filtro Laplaciano do Gaussiano consiste em aplicar o filtro Gaussiano e , em seguida, o filtro Laplaciano. Para adaptar o codigo, foi necessario acrescentar dois fatores:

1. Uma interacao if-else caso seja Laplaciano do Gaussiano;
2. Alterar a variavel que ativa esse if-else em todos os casos possiveis.

* Primeiro, a interacao if-else. Ela deve ser adicionada pois sao realizados dois processos de _Filter2D_, enquanto que os demais processos soh precisara de 1. 
* Para que esse processo de dupla filtragem funcione corretamente, um novo _Mat_ foi gerado para armazenar o resultado intermediario entre as filtragens. Ou seja, ele aramzenara o resultado do filtro Gaussiano;
* Em seguida, esse resultado sera filtrado pelo filtro Laplaciano, onde ele sera armazenado na variavel Mat _frame32f_ jah existente.

```
if(laplgauss == 1){  //se for o laplgauss, ira realizar o gaussiano e depois o laplaciano
        mask = cv::Mat(3,3,CV_32F,gauss);
        //aplica gaussiano
        cv::filter2D(frame32f, frameFiltered1, frame32f.depth(), mask, cv::Point(1, 1), 0);
        mask = cv::Mat(3, 3, CV_32F, laplacian);
        //aplica laplaciano
        cv::filter2D(frameFiltered1, frameFiltered, frame32f.depth(), mask,cv::Point(1, 1), 0);    	  
    }
    else{
        cv::filter2D(frame32f, frameFiltered, frame32f.depth(), mask,
                 cv::Point(1, 1), 0);
    }
```

* A segunda mudanca realizada foi no switch-case do codigo original. Bastou-se acrescentar um booleano _laplgauss_ que ira ligar e desligar a depender do filtro selecionado;

```
 switch (key) {
      case 'a':
        absolut = !absolut;
        laplgauss = 0;
        break;
      case 'm':
        mask = cv::Mat(3, 3, CV_32F, media);
        printmask(mask);
        laplgauss = 0;
        break;
      case 'g':
        mask = cv::Mat(3, 3, CV_32F, gauss);
        printmask(mask);
        laplgauss = 0;
        break;
      case 'h':
        mask = cv::Mat(3, 3, CV_32F, horizontal);
        printmask(mask);
        laplgauss = 0;
        break;
      case 'v':
        mask = cv::Mat(3, 3, CV_32F, vertical);
        printmask(mask);
        laplgauss = 0;
        break;
      case 'l':
        mask = cv::Mat(3, 3, CV_32F, laplacian);
        laplgauss = 0;
        printmask(mask);
        break;
      case 'b':  
        laplgauss = 0;
        mask = cv::Mat(3, 3, CV_32F, boost);
        break;
      case 'p':  //adicionado laplgauss
        laplgauss = 1;
        break;       
      default:
        break;
    }
```

* Resultado :

(imagens)


## Questao 6.1
**Utilizando o programa exemplos/addweighted.cpp como referência, implemente um programa tiltshift.cpp. Três ajustes deverão ser providos na tela da interface:**
* **um ajuste para regular a altura da região central que entrará em foco;**
* **um ajuste para regular a força de decaimento da região borrada;**
* **um ajuste para regular a posição vertical do centro da região que entrará em foco. Finalizado o programa, a imagem produzida deverá ser salva em arquivo.**

Essa questao foi dividida em diversos problemas:
1. Gerar copia da imagem original, porem borrada;
2. Calcular Valor de alpha usando a funcao α(x)=12(tanh((x−l1)/d)−tanh(x−l2)/d)), onde os parametros l1, l2 e d devem ser calculados a partir da altura da regiao central (banda), da forca de decaimento (d) e da posicao do centro da imagem (x0)
3. Gerar as Matrizes de alpha1 e alpha2 respectivas para ambas imagens;
4. Gerar a imagem de tiltshift usando essas matrizes alphas e as imagens original e borrada;
5. Adaptar as ferramentas de slider para atender as exigencias do problema.

* Para gerar o tiltshift, eh preciso produzir uma copia da imagem original, porem borrada. Para isso, um metodo possivel seria usar o filtro da media por uma serie de vezes seguidas. O metodo utilizado foi chamando a funcao de _GaussianBlur_ para borrar a imagem.

```
cv::GaussianBlur(image1,image2,cv::Size(5,5),5.0,5.0);
```
Comparacao das duas imagens:
(imagens..)

* O calculo de alpha foi codificado separadamente, seguindo as exigencias da funcao desejada.

```
float alpha(float ponto,float x0, float b,float d){

  float a = 0.5*(tanhf((ponto-(x0+(b/2)))/d)-tanhf((ponto-(x0-(b/2)))/d));
  return a;
}
```
Onde _ponto_ eh a posicao atual, _x0_ eh o centro da posicao vertical escolhida pelo slider, _b_ eh o tamanho da regiao de banda e _d_ eh o decaimento.

* Geracao das matrizes alpha1 e alpha2, onde alpha 2 eh o complemento de alpha1. 
```
  matriz_alfa1 = cv::Mat::zeros(altura,largura,CV_32F);
  matriz_alfa2 = cv::Mat::zeros(altura,largura,CV_32F);
  
  for(int i=0;i<altura;i++){ 
      alfa1 = alpha(i,x0,band,decaimento);	
      alfa2 = 1.0 - alfa1;

      for(int j=0;j<largura;j++){
          matriz_alfa1.at<float>(i,j) = alfa1;
          matriz_alfa2.at<float>(i,j) = alfa2;
      }
  }
```
* Em seguida, essas matrizes foram replicadas e foram unidas, uma vez que torna-se necessario para nao ocorrer erros do tipo "arithmetic" na execucao.

```
  cv::Mat aux1[] = {matriz_alfa1, matriz_alfa1, matriz_alfa1};
  cv::Mat aux2[] = {matriz_alfa2, matriz_alfa2, matriz_alfa2};
  cv::merge(aux1, 3, matriz_alfa1);
  cv::merge(aux2, 3, matriz_alfa2);
```
* Sao geradas matrizes auxiliares para realizar a operacao de tiltshift e elas receberao as imagens original (1) e borrada (2) no formato Float de 3 canais (RGB).

```
  cv::Mat copia1, copia2, result_32fc;
  image1.convertTo(copia1, CV_32FC3);
  image2.convertTo(copia2, CV_32FC3);
```

* Multiplica as matrizes alphas pelas imagens original e borrada do tipo float

```
  cv::multiply(copia1, matriz_alfa1, copia1);
  cv::multiply(copia2, matriz_alfa2, copia2);
  
```
* Soma os resultados

```
  cv::add(copia1, copia2, result_32fc);
  result_32fc.convertTo(blended, CV_8UC3);
  
```
* E realiza o display e escrita das imagens original, borrada e tiltshift. Essa ultima sera salva em arquivo jpg.

```
  cv::imshow("imagem original", image1);
  cv::imshow("borrada", image2);
  cv::imshow("tiltshift", blended);
  cv::imwrite("tiltshift.jpg",blended);
```

* A funcao final que realiza o tiltshift eh a uniao de todos esses trechos ilustrados.

```
  matriz_alfa1 = cv::Mat::zeros(altura,largura,CV_32F);
  matriz_alfa2 = cv::Mat::zeros(altura,largura,CV_32F);
  for(int i=0;i<altura;i++){ 
      alfa1 = alpha(i,x0,band,decaimento);	
      alfa2 = 1.0 - alfa1;
      for(int j=0;j<largura;j++){
          matriz_alfa1.at<float>(i,j) = alfa1;
          matriz_alfa2.at<float>(i,j) = alfa2;
      }
  }
  cv::Mat aux1[] = {matriz_alfa1, matriz_alfa1, matriz_alfa1};
  cv::Mat aux2[] = {matriz_alfa2, matriz_alfa2, matriz_alfa2};
  cv::merge(aux1, 3, matriz_alfa1);
  cv::merge(aux2, 3, matriz_alfa2);

  cv::Mat copia1, copia2, result_32fc;
  image1.convertTo(copia1, CV_32FC3);
  image2.convertTo(copia2, CV_32FC3);
  cv::multiply(copia1, matriz_alfa1, copia1);
  cv::multiply(copia2, matriz_alfa2, copia2);
  cv::add(copia1, copia2, result_32fc);
  result_32fc.convertTo(blended, CV_8UC3);
  cv::imshow("imagem original", image1);
  cv::imshow("borrada", image2);
  cv::imshow("tiltshift", blended);
  cv::imwrite("tiltshift.jpg",blended);
```

* Resta agora adaptar o visual dos sliders para eles modificarem os valores de x0,band e decaimento, alem de chamarem a funcao de tiltshift.

```
void on_trackbar_x0(int, void*){
 x0 = (double) slider_x0*altura/slider_x0_max ;
 std::cout<<"x0"<<x0<<std::endl;
 montagem_de_tiltshift();
 cv::imshow("addweighted", blended);
}

void on_trackbar_band(int, void*){
 band = (double) band_slider*altura/band_slider_max ;
 std::cout<<"band"<<band<<std::endl;
 montagem_de_tiltshift();
 cv::imshow("addweighted", blended);
}

void on_trackbar_d(int, void*){
 decaimento = (double) d_slider;
 std::cout<<"decaimento"<<decaimento<<std::endl;
 montagem_de_tiltshift();
 cv::imshow("addweighted", blended);
}

```
* Com isso, a mudanca final foi no codigo main() para receber essas trackbars.

```
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
```

* Resultado final:

(imagens...)


## Questao 6.2 
**Utilizando o programa exemplos/addweighted.cpp como referência, implemente um programa tiltshiftvideo.cpp. Tal programa deverá ser capaz de processar um arquivo de vídeo, produzir o efeito de tilt-shift nos quadros presentes e escrever o resultado em outro arquivo de vídeo. A ideia é criar um efeito de miniaturização de cenas. Descarte quadros em uma taxa que julgar conveniente para evidenciar o efeito de stop motion, comum em vídeos desse tipo.

Nao realizada.
