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
    std::cout << "V2.0-----Concorrentes-----\n" ;
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
			std::cout << "Digite o nome do arquivo de entrada: ";
			std::cin >> nome_imagem;
			
			int indice;
			int i = 0;
			for(; nome_imagem[i] != '\0'; ++i)
			{
				if(nome_imagem[i] == '/' ) indice = i;
			}
			indice++;
			int j = 0;
			for(; j < i; ++j)
			{
				nome_saida[j] = nome_imagem[indice+j];
			}
			sprintf (nome_saida, "saida/%s", nome_saida);
			
			switch (opcao){
			case 0:
			   if(opcao_cor == 1){
					sequencialColorido(nome_imagem, nome_saida);
				}else{
					sequencialCinza(nome_imagem, nome_saida);
				}
				break;
			case 1:
				if(opcao_cor == 1){
					concorrenteColorido(nome_imagem, nome_saida);
				}else{
					concorrenteCinza(nome_imagem, nome_saida);
				}
				break;
			default:
				std::cout << "Opcao invalida, selecione outra opcao.\n"; 
			}
		}
		
	}while(opcao_cor != 0);

    return 0;
}