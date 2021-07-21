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










## Questao 5
**Utilizando o programa exemplos/filtroespacial.cpp como referência, implemente um programa laplgauss.cpp. O programa deverá acrescentar mais uma funcionalidade ao exemplo fornecido, permitindo que seja calculado o laplaciano do gaussiano das imagens capturadas. Compare o resultado desse filtro com a simples aplicação do filtro laplaciano.**




## Questao 6.1
**Utilizando o programa exemplos/addweighted.cpp como referência, implemente um programa tiltshift.cpp. Três ajustes deverão ser providos na tela da interface:**
* **um ajuste para regular a altura da região central que entrará em foco;
* **um ajuste para regular a força de decaimento da região borrada;
* **um ajuste para regular a posição vertical do centro da região que entrará em foco. Finalizado o programa, a imagem produzida deverá ser salva em arquivo.

