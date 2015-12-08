#include <iostream>
#include <string>
#include <stdio.h>

#include <opencv2/opencv.hpp>
//#include <opencv2/core/cuda.hpp>


// #include <opencv/cv.h> //Minha versão é outra.
// #include <opencv/ml.h>
// #include <opencv/cxcore.h>
// #include <opencv/highgui.h>


#include <sys/time.h> //Não existe no Windows.


#include <cuda.h> //Original

//#include <CUDA/cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

__global__ void smoothGray (unsigned char *imagem, unsigned char *saida, unsigned int cols, unsigned int linhas)
{
	unsigned int indice = (blockIdx.y * blockDim.x * 65536) + (blockIdx.x * 1024) + threadIdx.x; // calcula o indice do vetor com base nas dimensões de bloco e indice da thread
	if(indice >= cols*linhas)
		return;
	//indices para o campo da imagem que participará do smooth 
	int i_begin = (indice/(int)cols) - 2, i_end = (indice/(int)cols)+2;
	int j_begin = (indice%(int)cols) - 2, j_end = (indice%(int)cols)+2;
	if(i_begin<0) i_begin = 0;
	if(j_begin<0) j_begin = 0;
	if(i_end>=cols) i_end = cols-1;
	if(j_end>=cols) j_end = cols-1;
	
	//calcula o smooth no ponto de indice da thread
	int media = 0;
	int qtd = 0;
	for (int i = i_begin; i<=  i_end; ++i)
	{
		for(int j = j_begin; j<= j_end; ++j)
		{
			media += imagem[(i*cols)+j];
			qtd++;
		}
	}
	saida[indice] = (unsigned char)(media/qtd);
}

void cudaCinza(char *nome_imagem, char *nome_saida)
{
	cv::Mat imagem = cv::imread(nome_imagem, CV_LOAD_IMAGE_GRAYSCALE); //abre a imagem de origem
	cv::Mat saida(imagem.rows, imagem.cols, imagem.type()); //cria a imagem de destino

	struct timeval start,end;
    double tempo=0.0;
    gettimeofday(&start,NULL);
	
	unsigned char * imagem_entrada;
	unsigned char * imagem_saida;
	cudaMalloc((void **)&(imagem_entrada), sizeof(unsigned char) * imagem.rows*imagem.cols);//aloca a imagem de origem na GPU
	cudaMemcpy((void *)(imagem_entrada), (void *)(imagem.data), sizeof(unsigned char) * imagem.rows * imagem.cols, cudaMemcpyHostToDevice); //manda a imagem de origem pra GPU
	
	cudaMalloc((void **)&(imagem_saida), sizeof(unsigned char) * imagem.rows*imagem.cols);//aloca a imagem de destino na GPU
		
	/* Inicio do processo paralelo */
	
//	cuInit(0); 
	dim3 Bloco_dim(1024); //define bloco de 1 linha tamanho 1024
	unsigned int num_Blocos = (unsigned int)ceil(((double)(imagem.rows*imagem.cols))/1024); //verifica quantos blocos serão necessário para a imagem
	smoothGray <<< num_Blocos, Bloco_dim >>> (imagem_entrada, imagem_saida, imagem.cols, imagem.rows);
	cudaDeviceSynchronize(); 
	/* Fim do processo paralelo */
	
	cudaMemcpy((void *)(saida.data), imagem_saida,(size_t)(sizeof(unsigned char) * imagem.rows * imagem.cols), cudaMemcpyDeviceToHost); //copia a imagem resultante para o Host
	
	gettimeofday(&end,NULL);
    tempo =( ((double) ( ((end.tv_sec * 1000000 + end.tv_usec)
                                - (start.tv_sec * 1000000 + start.tv_usec))))/1000000);
    char arquivo[100];

    sprintf(arquivo, "%s.out", nome_saida);
    FILE *fp = NULL;
    if((fp = fopen(arquivo, "a")) == NULL)
		fp = fopen(arquivo,"w");
   
    fprintf(fp, "%lf\n", tempo);
    //apresentar os resultado
    fclose(fp);
	
	cv::imwrite(nome_saida, saida); //Escreve imagem no arquivo
	
	cudaFree(imagem_entrada);
	cudaFree(imagem_saida);
}

__global__ void smoothColor (unsigned char *imagem, unsigned char *saida, unsigned int cols, unsigned int linhas)
{
	unsigned int indice = (blockIdx.y * blockDim.x * 65536) + (blockIdx.x * 1024) + threadIdx.x; // calcula o indice do vetor com base nas dimensões de bloco e indice da thread
	if(indice >= cols*linhas)
		return;
	//indices para o campo da imagem que participará do smooth 
	int i_begin = (indice/(int)cols)-2, i_end = (indice/(int)cols)+2;
	int j_begin = (indice%(int)cols)-2, j_end = (indice%(int)cols)+2;
	if(i_begin<0) i_begin = 0;
	if(j_begin<0) j_begin = 0;
	if(i_end>=cols) i_end = cols-1;
	if(j_end>=cols) j_end = cols-1;
	
	//calcula o smooth no ponto de indice da thread
	int media[3] = {0,0,0};
	int qtd = 0;
	for (int i = i_begin; i<=  i_end; ++i)
	{
		for(int j = j_begin; j<= j_end; ++j)
		{
			media[0] += imagem[((i*cols)+j)*3];
			media[1] += imagem[((i*cols)+j)*3+1];
			media[2] += imagem[((i*cols)+j)*3+2];
			qtd++;
		}
	}

	saida[indice*3] = (unsigned char)(media[0]/qtd);
	saida[indice*3+1] = (unsigned char)(media[1]/qtd);
	saida[indice*3+2] = (unsigned char)(media[2]/qtd);
}

void cudaColorido(char *nome_imagem, char *nome_saida)
{
	cv::Mat imagem = cv::imread(nome_imagem); //abre a imagem de origem
	cv::Mat saida(imagem.rows, imagem.cols, CV_8UC3); //cria a imagem de destino
	
	struct timeval start,end;
    double tempo=0.0;
    gettimeofday(&start,NULL);
	
	unsigned char * imagem_entrada;
	unsigned char * imagem_saida;
	cudaMalloc((void **)&(imagem_entrada), sizeof(unsigned char) * imagem.rows*imagem.cols * 3);//aloca a imagem de origem na GPU
	cudaMemcpy((void *)(imagem_entrada), (void *)(imagem.data), sizeof(unsigned char) * imagem.rows * imagem.cols * 3, cudaMemcpyHostToDevice); //manda a imagem de origem pra GPU

	cudaMalloc((void **)&(imagem_saida), sizeof(unsigned char) * imagem.rows*imagem.cols * 3);//aloca a imagem de destino na GPU
	
	/* Inicio do processo paralelo */
	
	//cuInit(0); 
	dim3 Bloco_dim(1024); //define bloco de 1 linha tamanho 1024

	unsigned int num_Blocos = (unsigned int)ceil(((double)(imagem.rows*imagem.cols)) / 1024); //verifica quantos blocos serão necessário para a imagem

	smoothColor << < num_Blocos, Bloco_dim >> > (imagem_entrada, imagem_saida, imagem.cols, imagem.rows); 
	cudaDeviceSynchronize(); 

	/* Fim do processo paralelo */
	

	cudaMemcpy((void *)(saida.data), (void *)imagem_saida, (size_t)(sizeof(unsigned char) * imagem.rows * imagem.cols * 3), cudaMemcpyDeviceToHost); //copia a imagem resultante para o Host
	
	gettimeofday(&end,NULL);
    tempo =( ((double) ( ((end.tv_sec * 1000000 + end.tv_usec)
                                - (start.tv_sec * 1000000 + start.tv_usec))))/1000000);
	
	char arquivo[100];
   //char *nome = strcat (nome_saida, ".out");
    sprintf(arquivo, "%s.out", nome_saida);
    FILE *fp = NULL;
    if((fp = fopen(arquivo, "a")) == NULL)
		fp = fopen(arquivo,"w");
   
    fprintf(fp, "%lf\n", tempo);
    //apresentar os resultado
    fclose(fp);
	
	
	cv::imwrite(nome_saida, saida); //Escreve imagem no arquivo
	
	cudaFree(imagem_entrada);
	cudaFree(imagem_saida);
}

int main()
{

	int opcao_cor=0;
	
	char nome_imagem[100];
	char nome_saida[100];
    std::cout << "Projeto Final-----Concorrentes-----\n" ;
    std::cout << "Eduardo Brunaldi dos Santos & Igor de Souza Baliza\n";
    std::cout << "\n\n\n";
	do{
		std::cout << "Escolha o tipo de Imagem:\n";
		std::cout << "0 - Sair\n";
		std::cout << "1 - RGB\n";
		std::cout << "2 - Grayscale\n";
		std::cout << "Digite uma das opcoes: ";
		std::cin >> opcao_cor;
		
		if(opcao_cor != 0)
		{
			int indice=0, i;
			char buffer_saida[100];
			std::cout << "Digite o nome do arquivo de entrada: ";
			std::cin >> nome_imagem;
			
			for(i=0; i < strlen(nome_imagem); ++i)
			{
				if(nome_imagem[i] == '/' ) indice = i+1;
			}
			
				int j = 0;
				for(; j <= i-indice; ++j)
				{
					buffer_saida[j] = nome_imagem[indice+j];
				}
			
			sprintf (nome_saida, "saida/%s", buffer_saida);
			if(opcao_cor == 1){
				cudaColorido(nome_imagem, nome_saida);
			}else{
				cudaCinza(nome_imagem, nome_saida);
			}
			break;

		}
		
	}while(opcao_cor != 0);


    return 0;
}
