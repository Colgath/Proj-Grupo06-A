#include <stdio.h>
#include "smooth.h"



int main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cerr << "Numero invalido de parametros. Por favor, entre com o tipo de execucao" << std::endl << "0 - Sequencial\n1 - Paralelo" << std::endl;
	}
    int opcao= atoi(argv[1]);
	int opcao_cor=0;
	
	char nome_imagem[100];
	char nome_saida[100];
	if (opcao ==1)
	{
		opcao_cor = 2;
		strcpy(nome_imagem, "imagens/lion.jpg");	//altere aqui o nome da imagem grayscale a ser processada pelo openmpi
	}
	do{
		if(opcao == 0)
		{
			std::cout << "V2.0-----Concorrentes-----\n" ;
			std::cout << "Eduardo Brunaldi dos Santos & Igor de Souza Baliza\n";
			std::cout << "\n\n\n";
			std::cout << "Escolha o tipo de Imagem:\n";
			std::cout << "0 - Sair\n";
			std::cout << "1 - RGB\n";
			std::cout << "2 - Grayscale\n";
			std::cout << "Digite uma das opcoes: ";
			std::cin>> opcao_cor;
			
	//scanf("%d", &opcao_cor);
		}
		if(opcao_cor != 0)
		{
			int indice=0, i;
			char buffer_saida[100];
			if (opcao ==0)
			{
				std::cout << "Digite o nome do arquivo de entrada: ";
				std::cin >> nome_imagem;
			}
		
			for(i=0; i < strlen(nome_imagem); ++i)
			{
				if(nome_imagem[i] == '/' ) indice = i+1;
			}
			
			int j = 0;
			for(; j <= i-indice; ++j)
			{
				buffer_saida[j] = nome_imagem[indice+j];
			}
			
			
			switch (opcao){
			case 0:
				sprintf (nome_saida, "saida/sequencial/%s", buffer_saida);
			   if(opcao_cor == 1){
					sequencialColorido(nome_imagem, nome_saida);
				}else if(opcao_cor ==2){
					sequencialCinza(nome_imagem, nome_saida);
				}

				break;
			case 1:
			printf("Execucao concorrente");
				sprintf (nome_saida, "saida/concorrente/%s", buffer_saida);
				if(opcao_cor == 1){
					concorrenteColorido(nome_imagem, nome_saida);
					opcao_cor = 0;
				}else if(opcao_cor ==2){
					concorrenteCinza(nome_imagem, nome_saida);
					opcao_cor = 1;
					strcpy(nome_imagem, "imagens/rio.jpg");//altera aqui o nome da imagem rgb a ser processada em MPI
				}
				break;
			default:
				std::cout << "Opcao invalida, selecione outra opcao.\n"; 
			}
		}
		
	}while(opcao_cor != 0);

    return 0;
}