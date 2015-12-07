#include "smooth.h"

/*
    Parametros:
                Mat img = imagem de entrada
                Mat saida = imagem de saida
                int i = linha da imagem
                int j = coluna da imagem
                int top = limite superior
                int botton = limite inferior
                int left = limite a esquerda
                int right = limite a direita
*/
void calculaRGB(cv::Mat img, cv::Mat img_saida, int i, int j, int top, int botton, int left, int right)
{
    unsigned int media[3] = {0,0,0};
    for(int rows = top; rows < botton; ++rows)
    {
        for(int cols = left; cols < right; ++cols)
        {
            media[0] += img.at<cv::Vec3b>(rows,cols)[0];
            media[1] += img.at<cv::Vec3b>(rows,cols)[1];
            media[2] += img.at<cv::Vec3b>(rows,cols)[2];
        }
    }
    int qtd = (botton-top)*(right-left);
    img_saida.at<cv::Vec3b>(i, j)[0] = (unsigned char)(media[0]/qtd);
    img_saida.at<cv::Vec3b>(i, j)[1] = (unsigned char)(media[1]/qtd);
    img_saida.at<cv::Vec3b>(i, j)[2] = (unsigned char)(media[2]/qtd);
}

void sequencialColorido(std::string nome_imagem, std::string imagem_saida){//char *nome_imagem, char *imagem_saida){
    //cout << "nome da imagem: " << nome_imagem << "\n";
    cv::Mat img = cv::imread(nome_imagem);
    cv::Mat img_saida(img.rows, img.cols, CV_8UC3);

    struct timeval start,end;
    double tempo=0.0;
    gettimeofday(&start,NULL);

    //canto superior esquerdo
    calculaRGB(img, img_saida, 0, 0, 0, 3, 0, 3);
    calculaRGB(img, img_saida, 0, 1, 0, 3, 0, 4);
    calculaRGB(img, img_saida, 1, 0, 0, 4, 0, 3);
    calculaRGB(img, img_saida, 1, 1, 0, 4, 0, 4);
    //canto superior direito
    calculaRGB(img, img_saida, 0, img.cols-1, 0, 3, img.cols-3, img.cols);
    calculaRGB(img, img_saida, 0, img.cols-2, 0, 3, img.cols-4, img.cols);
    calculaRGB(img, img_saida, 1, img.cols-1, 0, 4, img.cols-3, img.cols);
    calculaRGB(img, img_saida, 1, img.cols-2, 0, 4, img.cols-4, img.cols);
    //canto inferior esquerdo
    calculaRGB(img, img_saida, img.rows-1, 0, img.rows-3, img.rows, 0, 3);
    calculaRGB(img, img_saida, img.rows-1, 1, img.rows-3, img.rows, 0, 4);
    calculaRGB(img, img_saida, img.rows-2, 0, img.rows-4, img.rows, 0, 3);
    calculaRGB(img, img_saida, img.rows-2, 1, img.rows-4, img.rows, 0, 4);
    //canto inferior direito
    calculaRGB(img, img_saida, img.rows-1, img.cols-1, img.rows-3, img.rows, img.cols-3, img.cols);
    calculaRGB(img, img_saida, img.rows-1, img.cols-2, img.rows-3, img.rows, img.cols-4, img.cols);
    calculaRGB(img, img_saida, img.rows-2, img.cols-1, img.rows-4, img.rows, img.cols-3, img.cols);
    calculaRGB(img, img_saida, img.rows-2, img.cols-2, img.rows-4, img.rows, img.cols-4, img.cols);
    
    for(int j = 2; j < img.cols-2; ++j)
    {
        calculaRGB(img, img_saida, 0, j, 0, 2, j-2, j+2);//primeira linha
        calculaRGB(img, img_saida, 1, j, 0, 3, j-2, j+2);//segunda linha
        calculaRGB(img, img_saida, img.rows-2, j, img.rows-4, img.rows, j-2, j+2);//penultima linha
        calculaRGB(img, img_saida, img.rows-1, j, img.rows-3, img.rows, j-2, j+2);//ultima linha
    }
    for(int i=2 ; i<img.rows-2 ; ++i){
        //executar o codigo
        calculaRGB(img,img_saida, i, 0, i-2,i+2,0,3);//primeira coluna
        calculaRGB(img,img_saida, i, 1, i-2,i+2,0,4);//segunda coluna
        for(int j=2; j<img.cols-2; ++j)//miolo da imagem
        {
            calculaRGB(img,img_saida, i, j, i-2,i+2,j-2,j+2);
        }
        calculaRGB(img,img_saida, i, img.cols-2, i-2,i+2,img.cols-4,img.cols);//penultima coluna
        calculaRGB(img,img_saida, i, img.cols-1, i-2,i+2,img.cols-3,img.cols);//ultima coluna
    }

    gettimeofday(&end,NULL);
    tempo =( ((double) ( ((end.tv_sec * 1000000 + end.tv_usec)
                                - (start.tv_sec * 1000000 + start.tv_usec))))/1000000);

    char nome[100];
    sprintf(nome, "%s.out", imagem_saida.c_str());
    FILE *fp = NULL;
    fp = fopen(nome, "w");
   
    fprintf(fp, "%lf", tempo);
    //apresentar os resultado
    fclose(fp);
    cv::imwrite(imagem_saida, img_saida);
}



void calculaCinza(cv::Mat img, cv::Mat img_saida, int i, int j, int top, int botton, int left, int right)
{
    unsigned int media = 0;
    for(int rows = top; rows < botton; ++rows)
    {
        for(int cols = left; cols < right; ++cols)
        {
            media += img.at<cv::Vec3b>(rows,cols)[0];
        }
    }
    int qtd = (botton-top)*(right-left);
    img_saida.at<uchar>(i, j) = (unsigned char)(media/qtd);
}

void sequencialCinza(std::string nome_imagem, std::string imagem_saida){
    //cout << "nome da imagem: " << nome_imagem << "\n";
    cv::Mat img = cv::imread(nome_imagem);
    cv::Mat img_saida(img.rows, img.cols, CV_8UC1);

    struct timeval start,end;
    double tempo=0.0;
    gettimeofday(&start,NULL);


    //canto superior esquerdo
    calculaCinza(img, img_saida, 0, 0, 0, 3, 0, 3);
    calculaCinza(img, img_saida, 0, 1, 0, 3, 0, 4);
    calculaCinza(img, img_saida, 1, 0, 0, 4, 0, 3);
    calculaCinza(img, img_saida, 1, 1, 0, 4, 0, 4);
    //canto superior direito
    calculaCinza(img, img_saida, 0, img.cols-1, 0, 3, img.cols-3, img.cols);
    calculaCinza(img, img_saida, 0, img.cols-2, 0, 3, img.cols-4, img.cols);
    calculaCinza(img, img_saida, 1, img.cols-1, 0, 4, img.cols-3, img.cols);
    calculaCinza(img, img_saida, 1, img.cols-2, 0, 4, img.cols-4, img.cols);
    //canto inferior esquerdo
    calculaCinza(img, img_saida, img.rows-1, 0, img.rows-3, img.rows, 0, 3);
    calculaCinza(img, img_saida, img.rows-1, 1, img.rows-3, img.rows, 0, 4);
    calculaCinza(img, img_saida, img.rows-2, 0, img.rows-4, img.rows, 0, 3);
    calculaCinza(img, img_saida, img.rows-2, 1, img.rows-4, img.rows, 0, 4);
    //canto inferior direito
    calculaCinza(img, img_saida, img.rows-1, img.cols-1, img.rows-3, img.rows, img.cols-3, img.cols);
    calculaCinza(img, img_saida, img.rows-1, img.cols-2, img.rows-3, img.rows, img.cols-4, img.cols);
    calculaCinza(img, img_saida, img.rows-2, img.cols-1, img.rows-4, img.rows, img.cols-3, img.cols);
    calculaCinza(img, img_saida, img.rows-2, img.cols-2, img.rows-4, img.rows, img.cols-4, img.cols);
    
    for(int j = 2; j < img.cols-2; ++j)
    {
        calculaCinza(img, img_saida, 0, j, 0, 2, j-2, j+2);//primeira linha
        calculaCinza(img, img_saida, 1, j, 0, 3, j-2, j+2);//segunda linha
        calculaCinza(img, img_saida, img.rows-2, j, img.rows-4, img.rows, j-2, j+2);//penultima linha
        calculaCinza(img, img_saida, img.rows-1, j, img.rows-3, img.rows, j-2, j+2);//ultima linha
    }
    for(int i=2 ; i<img.rows-2 ; ++i){
        //executar o codigo
        calculaCinza(img,img_saida, i, 0, i-2,i+2,0,3);//primeira coluna
        calculaCinza(img,img_saida, i, 1, i-2,i+2,0,4);//segunda coluna
        for(int j=2; j<img.cols-2; ++j)//miolo da imagem
        {
            calculaCinza(img,img_saida, i, j, i-2,i+2,j-2,j+2);
        }
        calculaCinza(img,img_saida, i, img.cols-2, i-2,i+2,img.cols-4,img.cols);//penultima coluna
        calculaCinza(img,img_saida, i, img.cols-1, i-2,i+2,img.cols-3,img.cols);//ultima coluna
    }

    gettimeofday(&end,NULL);
    tempo =( ((double) ( ((end.tv_sec * 1000000 + end.tv_usec)
                                - (start.tv_sec * 1000000 + start.tv_usec))))/1000000);

    char nome[100];
    sprintf(nome, "%s.out", imagem_saida.c_str());
    FILE *fp = fopen(nome, "w");
    fprintf(fp, "%lf", tempo);
    fclose(fp);
    //apresentar os resultado

    cv::imwrite(imagem_saida, img_saida);
}


void calculaConCinza(unsigned char *img, unsigned char *img_saida, int i, int j, int top, int botton, int left, int right, int width)
{
    unsigned int media = 0;
    for(int rows = top; rows < botton; ++rows)
    {
        for(int cols = left; cols < right; ++cols)
        {
            media += img[(rows*width)+cols];
        }
    }
    int qtd = (botton-top)*(right-left);
    img_saida[(i*width)+j] = (unsigned char)(media/qtd);
}

void concorrenteCinza(std::string nome_imagem, std::string imagem_saida){//char *nome_imagem, int n_threads){
    int node;
    int num_slaves;

    /*comeco de calcular tempo*/
    struct timeval start,end;
    double tempo=0.0;
    gettimeofday(&start,NULL);


    MPI_Init(NULL, NULL); 
    MPI_Comm_rank(MPI_COMM_WORLD, &node); 
    MPI_Comm_size(MPI_COMM_WORLD, &num_slaves);
    
    if (node == PAI)
    {
        cv::Mat img = cv::imread(nome_imagem);
        cv::Mat img_smooth(img.rows, img.cols, CV_8UC1);

        int rows_to_slave = (img.rows-4)/num_slaves; 
        int limite_inferior = 0;
        int limite_superior = rows_to_slave + 4;
		int num_linhas = 0;
		int i;
        for (i = 1; i<num_slaves-1 ; ++i)
        {
            num_linhas = limite_superior - limite_inferior;

            MPI_Send(&num_linhas, 1, MPI_INT, i, MASTER_TO_SLAVE_TAG, MPI_COMM_WORLD);
            MPI_Send(&img.cols, 1, MPI_INT, i, MASTER_TO_SLAVE_TAG + 1, MPI_COMM_WORLD);
            MPI_Send(&(img.data[limite_inferior * img.cols]), num_linhas * img.cols, MPI_UNSIGNED_CHAR, i, MASTER_TO_SLAVE_TAG + 2, MPI_COMM_WORLD);
            MPI_Send(&limite_inferior, 1, MPI_INT, i, MASTER_TO_SLAVE_TAG + 3, MPI_COMM_WORLD);

            limite_inferior += rows_to_slave;
            limite_superior += rows_to_slave;
        }
        limite_superior = img.rows;
        num_linhas = limite_superior - limite_inferior;
        MPI_Send(&num_linhas, 1, MPI_INT, i, MASTER_TO_SLAVE_TAG, MPI_COMM_WORLD);
        MPI_Send(&img.cols, 1, MPI_INT, i, MASTER_TO_SLAVE_TAG + 1, MPI_COMM_WORLD);
        MPI_Send(&(img.data[limite_inferior * img.cols]), num_linhas * img.cols, MPI_UNSIGNED_CHAR, i, MASTER_TO_SLAVE_TAG + 2, MPI_COMM_WORLD);
        MPI_Send(&limite_inferior, 1, MPI_INT, i, MASTER_TO_SLAVE_TAG + 3, MPI_COMM_WORLD);

        //canto superior esquerdo
        calculaCinza(img, img_smooth, 0, 0, 0, 3, 0, 3);
        calculaCinza(img, img_smooth, 0, 1, 0, 3, 0, 4);
        calculaCinza(img, img_smooth, 1, 0, 0, 4, 0, 3);
        calculaCinza(img, img_smooth, 1, 1, 0, 4, 0, 4);
        //canto superior direito
        calculaCinza(img, img_smooth, 0, img.cols-1, 0, 3, img.cols-3, img.cols);
        calculaCinza(img, img_smooth, 0, img.cols-2, 0, 3, img.cols-4, img.cols);
        calculaCinza(img, img_smooth, 1, img.cols-1, 0, 4, img.cols-3, img.cols);
        calculaCinza(img, img_smooth, 1, img.cols-2, 0, 4, img.cols-4, img.cols);
        //canto inferior esquerdo
        calculaCinza(img, img_smooth, img.rows-1, 0, img.rows-3, img.rows, 0, 3);
        calculaCinza(img, img_smooth, img.rows-1, 1, img.rows-3, img.rows, 0, 4);
        calculaCinza(img, img_smooth, img.rows-2, 0, img.rows-4, img.rows, 0, 3);
        calculaCinza(img, img_smooth, img.rows-2, 1, img.rows-4, img.rows, 0, 4);
        //canto inferior direito
        calculaCinza(img, img_smooth, img.rows-1, img.cols-1, img.rows-3, img.rows, img.cols-3, img.cols);
        calculaCinza(img, img_smooth, img.rows-1, img.cols-2, img.rows-3, img.rows, img.cols-4, img.cols);
        calculaCinza(img, img_smooth, img.rows-2, img.cols-1, img.rows-4, img.rows, img.cols-3, img.cols);
        calculaCinza(img, img_smooth, img.rows-2, img.cols-2, img.rows-4, img.rows, img.cols-4, img.cols);
    
        for(int j = 2; j < img.cols-2; ++j)
        {
            calculaCinza(img, img_smooth, 0, j, 0, 2, j-2, j+2);//primeira linha
            calculaCinza(img, img_smooth, 1, j, 0, 3, j-2, j+2);//segunda linha
            calculaCinza(img, img_smooth, img.rows-2, j, img.rows-4, img.rows, j-2, j+2);//penultima linha
            calculaCinza(img, img_smooth, img.rows-1, j, img.rows-3, img.rows, j-2, j+2);//ultima linha
        }

        for (i = 1; i < num_slaves; i++)
        {
            int posicao;
            int linhas;

            MPI_Recv(&linhas, 1, MPI_INT, i, SLAVE_TO_MASTER_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&posicao, 1, MPI_INT, i, SLAVE_TO_MASTER_TAG + 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&(img_smooth.data[posicao * img_smooth.cols]), linhas * img_smooth.cols, MPI_UNSIGNED_CHAR, i, SLAVE_TO_MASTER_TAG + 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        cv::imwrite(imagem_saida, img_smooth);
    }else
    {
        unsigned char *img_entrada;
        unsigned char *img_saida;
        int linhas, colunas, posicao;

        MPI_Recv(&linhas, 1, MPI_INT, 0, MASTER_TO_SLAVE_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&colunas, 1, MPI_INT, 0, MASTER_TO_SLAVE_TAG + 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        img_entrada = new unsigned char[linhas * colunas];
        img_saida = new unsigned char[(linhas-4) * colunas];
        MPI_Recv(img_entrada, linhas * colunas, MPI_UNSIGNED_CHAR, 0, MASTER_TO_SLAVE_TAG + 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&posicao, 1, MPI_INT, 0, MASTER_TO_SLAVE_TAG + 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        #pragma omp for
        for (int i = 2; i < linhas-2; ++i)
        {
            //executar o codigo
            calculaConCinza(img_entrada,img_saida, i-2, 0, i-2,i+2,0,3, colunas);//primeira coluna
            calculaConCinza(img_entrada,img_saida, i-2, 1, i-2,i+2,0,4, colunas);//segunda coluna
            for(int j=2; j<colunas-2; ++j)//miolo da imagem
            {
                calculaConCinza(img_entrada,img_saida, i-2, j, i-2,i+2,j-2,j+2,colunas);
            }
            calculaConCinza(img_entrada,img_saida, i-2, colunas-2, i-2,i+2,colunas-4,colunas, colunas);//penultima coluna
            calculaConCinza(img_entrada,img_saida, i-2, colunas-1, i-2,i+2,colunas-3,colunas, colunas);//ultima coluna
        }

        linhas -= 4;
        MPI_Send(&linhas, 1, MPI_INT, 0, SLAVE_TO_MASTER_TAG, MPI_COMM_WORLD);
        MPI_Send(&posicao, 1, MPI_INT, 0, SLAVE_TO_MASTER_TAG + 1, MPI_COMM_WORLD);
        MPI_Send(img_saida, linhas * colunas, MPI_UNSIGNED_CHAR, 0, SLAVE_TO_MASTER_TAG + 2, MPI_COMM_WORLD);
        posicao+=2;

        delete img_entrada;
        delete img_saida;
    }

    MPI_Finalize();

    gettimeofday(&end,NULL);
    tempo =( ((double) ( ((end.tv_sec * 1000000 + end.tv_usec)
                                - (start.tv_sec * 1000000 + start.tv_usec))))/1000000);

    char nome[100];
    sprintf(nome, "%s.out", imagem_saida.c_str());
    FILE *fp = fopen(nome, "w");
    fprintf(fp, "%lf", tempo);
    fclose(fp);
    

}

void calculaConRGB(unsigned char *img, unsigned char *img_saida, int i, int j, int top, int botton, int left, int right, int width)
{
    unsigned int media [3] ={0, 0 , 0};
    for(int rows = top; rows < botton; ++rows)
    {
        for(int cols = left; cols < right; ++cols)
        {
            media[0] += img[(rows*width)+cols];
            media[1] += img[(rows*width)+cols+1];
            media[2] += img[(rows*width)+cols+2];
        }
    }
    int qtd = (botton-top)*(right-left);
    img_saida[(i*width)+j] = (unsigned char)(media[0]/qtd);
    img_saida[(i*width)+j+1] = (unsigned char)(media[1]/qtd);
    img_saida[(i*width)+j+2] = (unsigned char)(media[2]/qtd);
}

void concorrenteColorido(std::string nome_imagem, std::string imagem_saida){
    int node;
    int num_slaves;

    /*comeco de calcular tempo*/
    struct timeval start,end;
    double tempo=0.0;
    gettimeofday(&start,NULL);

    MPI_Init(NULL, NULL); 
    MPI_Comm_rank(MPI_COMM_WORLD, &node); 
    MPI_Comm_size(MPI_COMM_WORLD, &num_slaves);
    
    if (node == PAI)
    {
		printf("node: %d\n", node);
		printf("node: %d\n", num_slaves);
        cv::Mat img = cv::imread(nome_imagem);
        cv::Mat img_smooth(img.rows, img.cols, CV_8UC3);

        int rows_to_slave = (img.rows-4)/num_slaves; 
        int limite_inferior = 0;
        int limite_superior = rows_to_slave + 4;
		int num_linhas;
		int i;
		printf("Foi ate aqui 1\n");
        for (i = 1; i<num_slaves-1 ; ++i)
        {
            num_linhas = limite_superior - limite_inferior;

            MPI_Send(&num_linhas, 1, MPI_INT, i, MASTER_TO_SLAVE_TAG, MPI_COMM_WORLD);
			printf("Foi ate aqui 2.%d\n", i);
            MPI_Send(&img.cols, 1, MPI_INT, i, MASTER_TO_SLAVE_TAG + 1, MPI_COMM_WORLD);
			printf("Foi ate aqui 3.%d\n", i);
            MPI_Send(&(img.data[limite_inferior * img.cols * 3]), num_linhas * img.cols * 3, MPI_UNSIGNED_CHAR, i, MASTER_TO_SLAVE_TAG + 2, MPI_COMM_WORLD);
			printf("Foi ate aqui 4.%d\n", i);
            MPI_Send(&limite_inferior, 1, MPI_INT, i, MASTER_TO_SLAVE_TAG + 3, MPI_COMM_WORLD);
			printf("Foi ate aqui 5.%d\n", i);
            limite_inferior += rows_to_slave;
            limite_superior += rows_to_slave;
        }
        limite_superior = img.rows;
        num_linhas = limite_superior - limite_inferior;
        MPI_Send(&num_linhas, 1, MPI_INT, i, MASTER_TO_SLAVE_TAG, MPI_COMM_WORLD);
        MPI_Send(&img.cols, 1, MPI_INT, i, MASTER_TO_SLAVE_TAG + 1, MPI_COMM_WORLD);
        MPI_Send(&(img.data[limite_inferior * img.cols]), num_linhas * img.cols, MPI_UNSIGNED_CHAR, i, MASTER_TO_SLAVE_TAG + 2, MPI_COMM_WORLD);
        MPI_Send(&limite_inferior, 1, MPI_INT, i, MASTER_TO_SLAVE_TAG + 3, MPI_COMM_WORLD);

        //canto superior esquerdo
        calculaRGB(img, img_smooth, 0, 0, 0, 3, 0, 3);
        calculaRGB(img, img_smooth, 0, 1, 0, 3, 0, 4);
        calculaRGB(img, img_smooth, 1, 0, 0, 4, 0, 3);
        calculaRGB(img, img_smooth, 1, 1, 0, 4, 0, 4);
        //canto superior direito
        calculaRGB(img, img_smooth, 0, img.cols-1, 0, 3, img.cols-3, img.cols);
        calculaRGB(img, img_smooth, 0, img.cols-2, 0, 3, img.cols-4, img.cols);
        calculaRGB(img, img_smooth, 1, img.cols-1, 0, 4, img.cols-3, img.cols);
        calculaRGB(img, img_smooth, 1, img.cols-2, 0, 4, img.cols-4, img.cols);
        //canto inferior esquerdo
        calculaRGB(img, img_smooth, img.rows-1, 0, img.rows-3, img.rows, 0, 3);
        calculaRGB(img, img_smooth, img.rows-1, 1, img.rows-3, img.rows, 0, 4);
        calculaRGB(img, img_smooth, img.rows-2, 0, img.rows-4, img.rows, 0, 3);
        calculaRGB(img, img_smooth, img.rows-2, 1, img.rows-4, img.rows, 0, 4);
        //canto inferior direito
        calculaRGB(img, img_smooth, img.rows-1, img.cols-1, img.rows-3, img.rows, img.cols-3, img.cols);
        calculaRGB(img, img_smooth, img.rows-1, img.cols-2, img.rows-3, img.rows, img.cols-4, img.cols);
        calculaRGB(img, img_smooth, img.rows-2, img.cols-1, img.rows-4, img.rows, img.cols-3, img.cols);
        calculaRGB(img, img_smooth, img.rows-2, img.cols-2, img.rows-4, img.rows, img.cols-4, img.cols);
    
        for(int j = 2; j < img.cols-2; ++j)
        {
            calculaRGB(img, img_smooth, 0, j, 0, 2, j-2, j+2);//primeira linha
            calculaRGB(img, img_smooth, 1, j, 0, 3, j-2, j+2);//segunda linha
            calculaRGB(img, img_smooth, img.rows-2, j, img.rows-4, img.rows, j-2, j+2);//penultima linha
            calculaRGB(img, img_smooth, img.rows-1, j, img.rows-3, img.rows, j-2, j+2);//ultima linha
        }

        for (i = 1; i < num_slaves; i++)
        {
            int posicao;
            int linhas;

            MPI_Recv(&linhas, 1, MPI_INT, i, SLAVE_TO_MASTER_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&posicao, 1, MPI_INT, i, SLAVE_TO_MASTER_TAG + 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&(img_smooth.data[posicao * img_smooth.cols * 3]), linhas * img_smooth.cols, MPI_UNSIGNED_CHAR, i, SLAVE_TO_MASTER_TAG + 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        cv::imwrite(imagem_saida, img_smooth);
    }else
    {
        unsigned char *img_entrada;
        unsigned char *img_saida;
        int linhas, colunas, posicao;

        MPI_Recv(&linhas, 1, MPI_INT, 0, MASTER_TO_SLAVE_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&colunas, 1, MPI_INT, 0, MASTER_TO_SLAVE_TAG + 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        img_entrada = new unsigned char[linhas * colunas * 3];
        img_saida = new unsigned char[(linhas-4) * colunas * 3];
        MPI_Recv(img_entrada, linhas * colunas * 3, MPI_UNSIGNED_CHAR, 0, MASTER_TO_SLAVE_TAG + 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&posicao, 1, MPI_INT, 0, MASTER_TO_SLAVE_TAG + 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        #pragma omp for
        for (int i = 2; i < linhas-2; ++i)
        {
            //executar o codigo
            calculaConRGB(img_entrada,img_saida, i-2, 0, i-2,i+2,0,3, colunas);//primeira coluna
            calculaConRGB(img_entrada,img_saida, i-2, 1, i-2,i+2,0,4, colunas);//segunda coluna
            for(int j=2; j<colunas-2; ++j)//miolo da imagem
            {
                calculaConRGB(img_entrada,img_saida, i-2, j, i-2,i+2,j-2,j+2, colunas);
            }
            calculaConRGB(img_entrada,img_saida, i-2, colunas-2, i-2,i+2,colunas-4,colunas, colunas);//penultima coluna
            calculaConRGB(img_entrada,img_saida, i-2, colunas-1, i-2,i+2,colunas-3,colunas, colunas);//ultima coluna
        }

        linhas -= 4;
        MPI_Send(&linhas, 1, MPI_INT, 0, SLAVE_TO_MASTER_TAG, MPI_COMM_WORLD);
        posicao+=2;
        MPI_Send(&posicao, 1, MPI_INT, 0, SLAVE_TO_MASTER_TAG + 1, MPI_COMM_WORLD);
        MPI_Send(img_saida, linhas * colunas * 3, MPI_UNSIGNED_CHAR, 0, SLAVE_TO_MASTER_TAG + 2, MPI_COMM_WORLD);
        
        delete img_entrada;
        delete img_saida;
    }

    MPI_Finalize();


    gettimeofday(&end,NULL);
    tempo =( ((double) ( ((end.tv_sec * 1000000 + end.tv_usec)
                                - (start.tv_sec * 1000000 + start.tv_usec))))/1000000);

    char nome[100];
    sprintf(nome, "%s.out", imagem_saida.c_str());
    FILE *fp = fopen(nome, "w");
    fprintf(fp, "%lf", tempo);
    fclose(fp);
}