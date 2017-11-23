//*****************************************************
//
// ExemploQuad2DStrip.cpp 
// Um programa OpenGL simples que abre uma janela GLUT 
// e desenha uma seqüência de quadrados
//
// Marcelo Cohen e Isabel H. Manssour
// Este código acompanha o livro
// "OpenGL - Uma Abordagem Prática e Objetiva" 
// 
//*****************************************************

#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>
#include <time.h>



int matriz23[11][11] = {
{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
{10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
{0, 10, 2, 3, 4, 10, 6, 7, 8, 9, 10},
{0, 10, 2, 3, 4, 10, 6, 7, 8, 9, 10}};

int matriz0[11][11] = {
{1, 2, 3, 4, 3, 2, 1, 0, 1, 2, 1},
{0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

int matriz[5][11][11] = {{
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}},
{
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}},
{
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}},
{
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1}},
{
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1}}
};
int zs = 13;

int ud,lr,r1,r2;

int tFila[5];

float scroll;

int modoScroll;



int aleatorio(){
	float porcentagem =rand()%10; 
	if(porcentagem<=7){
		return 0;
	} else if (porcentagem<=7.5)	{
		return 1;
	} else if (porcentagem<=10)	{
		modoScroll=1;
		return 2;
	}
}

void DesenhaMontanhas(int tipo){
	glColor3f(0.7f, 0.7f, 0.7f); 
	// Desenha uma seqüência de quadrados 
	for(int y = 50; y >-50; y-=10){
	glBegin(GL_QUAD_STRIP);
			for(int x = -50; x<50; x+=10 ){
				int mx = (int)(x+50)/10;
				int my = (int)((y*-1)+50)/10;

				glVertex3f(x,y, matriz[tipo][my][mx]);
				glVertex3f(x,y-10,matriz[tipo][my+1][mx]);       
				glVertex3f(x+10,y,matriz[tipo][my][mx+1]);       
				glVertex3f(x+10,y-10,matriz[tipo][my+1][mx+1]);
			}
	glEnd();
	}

	// Altera a cor do desenho para preta
	glColor3f(0.0f, 0.0f, 0.0f); 

	// Desenha linhas
	for(int y = 50; y >-50; y-=10){
	glBegin(GL_LINES);
			for(int x = -50; x<50; x+=10 ){
				int mx = (int)(x+50)/10;
				int my = (int)((y*-1)+50)/10;

				glVertex3f(x,y, matriz[tipo][my][mx]);
				glVertex3f(x,y-10, matriz[tipo][my+1][mx]);

				glVertex3f(x,y, matriz[tipo][my][mx]);
				glVertex3f(x+10,y, matriz[tipo][my][mx+1]);

				glVertex3f(x+10,y, matriz[tipo][my][mx+1]);       
				glVertex3f(x+10,y-10, matriz[tipo][my+1][mx+1]);

				glVertex3f(x,y-10, matriz[tipo][my+1][mx]);
				glVertex3f(x+10,y-10, matriz[tipo][my+1][mx+1]);

			}
	glEnd();
	}

	glColor3f(1.0f, 0.0f, 0.0f); 

	glBegin(GL_TRIANGLES);
		glVertex2f(-3, 45);
		glVertex2f(0, 50);
		glVertex2f(3, 45);
	glEnd();
}

void Desenha(void)
{	


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	gluLookAt(0,-60,70, 0,40,0, 0,1,0);

	/*
	glTranslatef(lr, ud, 0);
	glRotatef(-r1, 0.0f, 1.0, 0.0f);
	glRotatef(-r2, 1.0f, 0.0f, 0.0f);
	*/
	glTranslatef(0, -scroll, 0);
	
	DesenhaMontanhas(tFila[0]);
	glTranslatef(0, 100, 0);
	DesenhaMontanhas(tFila[1]);
	glTranslatef(0, 100, 0);
	DesenhaMontanhas(tFila[2]);
	glTranslatef(0, 100, 0);
	DesenhaMontanhas(tFila[3]);
	glTranslatef(0, 100, 0);
	DesenhaMontanhas(tFila[4]);

	//Só pra mostrar que o desenho continua o mesmo:
	glLoadIdentity();
	gluLookAt(0,-130,70, 0,0,0, 0,1,0);

	// Executa os comandos OpenGL 
	glFlush();
}

void EspecificaParametrosVisualizacao(void)
{
	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();

	// Especifica a projeção perspectiva(angulo,aspecto,zMin,zMax)
	gluPerspective(45,1,1,500);

	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();

	// Especifica posição do observador e do alvo
	gluLookAt(0,-130,70, 0,0,0, 0,1,0);
}


// Função callback chamada quando o tamanho da janela é alterado 
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{ 
	gluOrtho2D (-50.0f, 50.0f, -50.0f, 50.0f);
	
	EspecificaParametrosVisualizacao();
	/*
	if (w <= h) 
		gluOrtho2D (-50.0f, 50.0f, -50.0f*h/w, 50.0f*h/w);
	else 
		gluOrtho2D (-50.0f*w/h, 50.0f*w/h, -50.0f, 50.0f);
	*/
}

// Função callback chamada para gerenciar eventos de teclas
void Teclado (int key, int x, int y)  {
	if(key == GLUT_KEY_LEFT)
		lr --;
	else if(key == GLUT_KEY_RIGHT)
		lr++;
	else if(key == GLUT_KEY_UP)
		ud ++;
	else if(key == GLUT_KEY_DOWN)
		ud--;

	// Rotaciona braco1
	if(key == GLUT_KEY_F1)
		r1--;
	if(key == GLUT_KEY_F2)
		r1++;
	if(key == GLUT_KEY_F3)
		r2--;
	if(key == GLUT_KEY_F4)
		r2++;

}

// Função responsável por inicializar parâmetros e variáveis
void Inicializa (void)
{   
	// Define a cor de fundo da janela de visualização como branca
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	for( int i=0; i<= 10; i++ ){
		for(int j=0; j<=10; j++){
			for (int tipo=0; tipo<5; tipo++){
				matriz[tipo][i][j]*=zs;
			}
		}
	}

	tFila[0]=0;
	tFila[1]=0;
	tFila[2]=1;
	tFila[4]= aleatorio();

	lr= 0;ud=0;r1=0;r2=0;
	scroll=0;
	modoScroll=0;

	srand(time(NULL)); //Usado para gerar numeros aleatorios

}

// Função callback chamada pela GLUT a cada intervalo de tempo
void Anima(int value)
{

	scroll+=5;

	if (scroll == 100){
		scroll=0;
		//pop()
		tFila[0]=tFila[1];
		tFila[1]=tFila[2];
		tFila[2]=tFila[3];
		tFila[3]=tFila[4];
		//ṕush()
		if (modoScroll==0) {
			tFila[4]= aleatorio();
		} else if(modoScroll==1){
			if(rand()%10<4){
				tFila[4]=3;
			} else{
				tFila[4]=4;
				modoScroll=0;
			}
		}

	}

	// Redesenha o "teapot" em outra posição
	glutPostRedisplay();
	glutTimerFunc(60,Anima, 1);
}

// Programa Principal 
int main(int argc, char** argv){


	glutInit(&argc, argv); // Esse daqui não tava no livro mas sem da erro que nao chamou

	// Define do modo de operação da GLUT
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
 
	// Especifica a posição inicial da janela GLUT
	glutInitWindowPosition(5,5); 
    
	// Especifica o tamanho inicial em pixels da janela GLUT
	glutInitWindowSize(500,500); 
 
	// Cria a janela passando como argumento o título da mesma
	glutCreateWindow("Exemplo com GL_QUAD_STRIP");
 
	// Registra a função callback de redesenho da janela de visualização
	glutDisplayFunc(Desenha);
  
	// Registra a função callback de redimensionamento da janela de visualização
	glutReshapeFunc(AlteraTamanhoJanela);

	// Registra a função callback que será chamada a cada intervalo de tempo
	glutTimerFunc(60, Anima, 1);
  
	// Registra a função callback para tratamento das teclas ASCII
	glutSpecialFunc(Teclado);

	// Chama a função responsável por fazer as inicializações 
	Inicializa();
 
	// Inicia o processamento e aguarda interações do usuário
	glutMainLoop();
 
	return 0;
}
