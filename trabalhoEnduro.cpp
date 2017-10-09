//Matheus Carreira Andrade, Mateus Orcese Trevizam, Leonardo Longo
#include <stdio.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>
#include <stdlib.h>
//#include <unistd.h>
#include <string.h>
// Quando compilar em ubuntu trocar o usleep por usleep(sleepTime);
#include <unistd.h>
//Declara��o de Vari�veis Globais
enum CameraType {
	rear,
	top,
	left
};

int cont = 0;
/**CONFIGURA��ES**/
//Camera
CameraType camera;
int posx = 0, posy = 3, posz = 10; //Vari�veis que definem a posi��o da c�mera 0,3,10
int oy = 0, ox = 0, oz = -10;	//Vari�veis que definem para onde a c�mera olha 0,0,-10
int lx = 0, ly = 1, lz = 0;	//Vari�veis que definem qual eixo estar� na vertical do monitor (cima).

//Carro
float km = 0;	//Distancia percorrida
float kmTotal = 0;
float ang = 0;	//Angulo de rota��o da pista
float vel = 0.45; //Velocidade do carro
float vel_ang; //Velocidade na curva - Calculada baseado no raio e velocidade
float pos_car = 0; //Posicao do carro (somente no eixo X)
float pos_car_final = 0;
float vel_x = 0.006; // Velocidade no eixo X (para mover para os lados)
float car_rot = 0;
float mov = 1; //Tamanho do movimento do carro (Esquerda ou direita)
int curva = 0; // Determina se est� se movendo em curva ou n�o

//Pista
float largura = 6; //Largura da Pista
float comprimento = 80; //Comprimento da Pista 78
float raio_curva; //Raio da curva - Calculado com base no comprimento da pista
int atual = 0; //Pista atual 
int prox = 0; //Proxima pista
float fim_pista1x = 0, fim_pista1z = 0, fim_pista2x = 0, fim_pista2z = 0; //vertices que marcam o fim da pista
float numquads = 25; //Numero de quadrados que forma a curva
float cos_ang; //Vari�vel para calculo de vertices da curva
float sen_ang; //Idem anterior

//Cores
float tempo = 0, relogio = 0.001; //Tempo de um per�odo(de 0 a 1) e o incremento no tempo
int periodo = 0, espera = 0; //Indica qual perido est� (0-Dia; 1-Tarde; 2-Noite) e flag para iniciar troca de periodo (1 indica troca)
int prox_periodo = 1;
int cor = 1; //Cor do carro

//Cores rgb(colunas) em cada periodo (linha) para cada objeto
int cor_pista[3][3];
int cor_chao[3][3];
int cor_ceu[3][3][3];
int cor_predios[3][3][3];
int cor_carro[8][3][3];
int cor_adesivo[8][3][3];

int cor_pista_atual[3];
int cor_chao_atual[3];
int cor_predios_atual[3][3];
int cor_ceu_atual[3][3];
int cor_carro_atual[8][3];
int cor_adesivo_atual[8][3];

//Cen�rio
float parallax = 0; //Altera vis�o da cidade ao fundo
float dist_obj = 15; //Dist�ncia entre objetos do cen�rio
#define max_obj 5 //N�mero m�ximo de objetos no cen�rio
float objs[max_obj][2]; //Tabela com as posi��es dos objetos.
int num_obj = 0; //N�mero atual de objetos

				 //Inimigos
float dist = 40; //Dist�ncia entre inimigos (em unidades de velocidade)
float vel_inimigos;
#define max_inimigos 5 //N�mero m�ximo de inimigos de uma vez
float inimigos[max_inimigos][2]; //Tabela com a dist�ncia do inimigo do jogador e onde na pista est�.
int cor_inimigo[max_inimigos];//Cor de cada inimigo
int num_inimigos = 0; //N�mero atual de inimigos
float dist_jogador; //Dist�ncia do Jogador quando passa no mesmo Z que ele
int prct = 15; //Serve para calcular em porcentagem onde na pista, existem prct+1 posi��es em x poss�veis para os inimigos estarem

//Var�aveis para controle de estado do jogo
float vel_p;
float vel_ang_p;
float vel_inimigos_p;
float mov_p;
float relogio_p;
float pn = 5, pn_cont = 0, pn2 = -5;
int inicio = 1;
// Lock framerate
long lastFrameTime = 0;

/****** C�DIGO ******/
void Carro();

void Inimigo(int cor_i);

void Skyline0();
void Skyline1();
void Skyline2();


void Arvore()
{
	glPushMatrix();
	glTranslatef(0, 1.5, 0);
	glPushMatrix();
	glRotatef(90, -1, 0, 0);
	glColor3ub(cor_chao_atual[0]*0.1, cor_chao_atual[1], cor_chao_atual[2]*0.7);
	glutSolidCone(1.75, 3.5, 20, 20);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, -1, 0, 0);
	glColor3ub(100, 80, 60);
	gluCylinder(gluNewQuadric(), 0.5, 0.5, 2, 20, 20);
	glPopMatrix();
}

void Placa() {
	glPushMatrix();
	glScalef(0.5, 0.5, 0.5);
	glPushMatrix();
	glColor3ub(255, 255, 0);
	glBegin(GL_TRIANGLES);
	glVertex3f(0, 4.1, 0);
	glVertex3f(0.9, 2.5, 0);
	glVertex3f(-0.9, 2.5, 0);
	glEnd();
	glPopMatrix();
	
	glPushMatrix();
	glColor3ub(10, 10, 10);
	glBegin(GL_TRIANGLES);
	glVertex3f(0, 4.3, -0.0005);
	glVertex3f(1.05, 2.42, -0.0005);
	glVertex3f(-1.05, 2.42, -0.0005);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3ub(10, 10, 10);
	glVertex3f(-0.08, 3.8, 0.0005);
	glVertex3f(0.08, 3.8, 0.0005);
	glVertex3f(0.08, 2.9, 0.0005);
	glVertex3f(-0.08, 2.9, 0.0005);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(-0.08, 2.8, 0.0005);
	glVertex3f(0.08, 2.8, 0.0005);
	glVertex3f(0.08, 2.65, 0.0005);
	glVertex3f(-0.08, 2.65, 0.0005);
	glEnd();
	glPopMatrix();

	//Haste
	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3ub(110, 110, 110);
	glVertex3f(-0.13, 2.42, 0);
	glVertex3f(0.13, 2.42, 0);
	glVertex3f(0.13, 0, 0);
	glVertex3f(-0.13, 0, 0);
	glEnd();
	glPopMatrix();
	glPopMatrix();
}

void drawBitmapText(const char *string, float x, float y, float z) {
	const char *c;
	//glScalef(3, 3, 3);
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
		//glutSwapBuffers();
	}
}

void Pause() {
	vel = 0;
	vel_ang = 0;
	vel_inimigos = 0;
	mov = 0;
	relogio = 0;
}

void Resume() {
	vel = vel_p;
	vel_ang = vel_ang_p;
	vel_inimigos = vel_inimigos_p;
	mov = mov_p;
	relogio = relogio_p;
}

void Colisao() {
	dist_jogador = pos_car_final - (inimigos[0][1] - largura / 2);
	//printf("%f\n", dist_jogador);
	if (dist_jogador >= -1.1 && dist_jogador <= 1.1) {
		Pause();
		num_inimigos--;
	}
}


void Cenario() {

	int i, j;
	float aux;
	if (num_obj<max_obj) {
		if (num_obj == 0) {
			objs[0][1] = 1;
			objs[0][0] = dist_obj;
			num_obj++;
			for (i = num_obj; i<max_obj; i++) {
				objs[i][1] = objs[i - 1][1] * (-1);
				objs[i][0] = objs[i - 1][0] + dist_obj;
				num_obj++;
			}
		}
		else {
			for (i = 0; i<num_obj; i++) {
				objs[i][0] = objs[i + 1][0];
				objs[i][1] = objs[i + 1][1];
			}
			objs[max_obj - 1][1] = objs[max_obj - 2][1] * -1;
			objs[max_obj - 1][0] = objs[max_obj - 2][0] + dist_obj;
			num_obj++;
		}
	}

	for (i = 0; i<max_obj; i++) {
		if (atual == 0) {//a pista atual (primeira pista) 'e reta
			if (objs[i][0] <= comprimento - km) { //o objeto esta na primeira pista (atual)
				objs[i][0] = objs[i][0] - vel;

				glPushMatrix();
				glTranslatef((largura / 2 + 2)*(objs[i][1]), 0, -objs[i][0]);
				Arvore();
				glPopMatrix();

			}
			else { //o inimigo esta na segunda pista
				if (prox == 0) { //a segunda pista 'e uma reta
					objs[i][0] = objs[i][0] - vel;
					glPushMatrix();
					glTranslatef((largura / 2 + 2)*(objs[i][1]), 0, -objs[i][0]);
					Arvore();
					glPopMatrix();
				}
				else if (prox == 1) { //a segunda pista 'e esquerda
					objs[i][0] = objs[i][0] - vel;
					aux = objs[i][0] - (comprimento - km);
					glPushMatrix();
					glTranslatef(-raio_curva - (largura / 2), 0, km - comprimento);
					glRotatef(vel_ang*aux / vel, 0, 1, 0);
					glTranslatef(raio_curva + largura / 2 + (largura / 2 + 2)*(objs[i][1]), 0, 0);
					Arvore();
					glPopMatrix();
				}
				else if (prox == 2) { //a segunda pista 'e direita
					objs[i][0] = objs[i][0] - vel;
					aux = objs[i][0] - (comprimento - km);
					glPushMatrix();
					glTranslatef(raio_curva + (largura / 2), 0, km - comprimento);
					glRotatef(-vel_ang*aux / vel, 0, 1, 0);
					glTranslatef(-raio_curva - largura / 2 - (largura / 2 + 2)*(objs[i][1]), 0, 0);
					Arvore();
					glPopMatrix();
				}
			}
		}
		else if (atual == 1) { //a pista atual 'e esquerda
			if (objs[i][0] <= comprimento - km) { //o objeto esta na primeira pista
				objs[i][0] = objs[i][0] - vel;
				glPushMatrix();
				glTranslatef(-raio_curva - (largura / 2), 0, 0);
				glRotatef(vel_ang*objs[i][0] / vel, 0, 1, 0);
				glTranslatef(raio_curva + largura / 2 + (largura / 2 + 2)*(objs[i][1]), 0, 0);
				Arvore();
				glPopMatrix();
			}
			else {
				if (prox == 0) { //a segunda pista 'e uma reta
					objs[i][0] = objs[i][0] - vel;
					aux = objs[i][0] - (comprimento - km);
					glPushMatrix();
					glTranslatef(-raio_curva - (largura / 2), 0, 0);
					glRotatef(90 - ang, 0, 1, 0);
					glTranslatef(raio_curva + largura / 2 + (largura / 2 + 2)*(objs[i][1]), 0, -aux);
					Arvore();
					glPopMatrix();
				}
				else if (prox == 1) {//a segunda pista 'e esquerda
					objs[i][0] = objs[i][0] - vel;
					glPushMatrix();
					glTranslatef(-raio_curva - (largura / 2), 0, 0);
					glRotatef(vel_ang*objs[i][0] / vel, 0, 1, 0);
					glTranslatef(raio_curva + largura / 2 + (largura / 2 + 2)*(objs[i][1]), 0, 0);
					Arvore();
					glPopMatrix();
				}
				else if (prox == 2) { //a segunda pista 'e direita
					objs[i][0] = objs[i][0] - vel;
					aux = objs[i][0] - (comprimento - km);
					glPushMatrix();
					//Leva para o centro da curva
					glTranslatef(-raio_curva - (largura / 2), 0, 0);
					//Roda junto com a pista
					glRotatef(-ang, 0, 1, 0);
					//Ir para o centro da curva a direta
					glTranslatef(0, 0, -2 * (raio_curva + (largura / 2)));
					//Roda o angulo necess�rio para colocar no lugar certo respeitando a distancia que falta
					glRotatef(-90 - vel_ang*aux / vel, 0, 1, 0);
					//Posiciona na curva
					glTranslatef(raio_curva + largura / 2 + (largura / 2 + 2)*(objs[i][1]), 0, 0);
					//Desenha o carro
					//glRotatef(180,0,1,0);
					Arvore();
					glPopMatrix();
				}
			}

		}
		else if (atual == 2) {
			if (objs[i][0] <= comprimento - km) {
				objs[i][0] = objs[i][0] - vel;
				glPushMatrix();
				//Leva pro centro da curva
				glTranslatef(raio_curva + (largura / 2), 0, 0);
				//Roda o angulo necess�rio para colocar no lugar certo respeitando a distancia que falta
				glRotatef(180 - vel_ang*objs[i][0] / vel, 0, 1, 0);
				//Posiciona na curva
				glTranslatef(raio_curva + largura / 2 + (largura / 2 + 2)*(objs[i][1]), 0, 0);
				//Desenha o carro
				Arvore();
				glPopMatrix();
			}
			else {
				if (prox == 0) {
					objs[i][0] = objs[i][0] - vel;
					aux = objs[i][0] - (comprimento - km);
					glPushMatrix();
					//Leva pro centro da curva
					glTranslatef(raio_curva + (largura / 2), 0, 0);
					//Roda junto com a pista
					glRotatef(ang, 0, 1, 0);
					//Vai pro fim da pista reta
					glTranslatef(aux, 0, -raio_curva - largura / 2 + (largura / 2 + 2)*(objs[i][1]));
					//Desenha o carro
					Arvore();
					glPopMatrix();
				}
				else if (prox == 1) {
					objs[i][0] = objs[i][0] - vel;
					aux = objs[i][0] - (comprimento - km);
					glPushMatrix();
					//Leva para o centro da curva
					glTranslatef(raio_curva + (largura / 2), 0, 0);
					//Roda junto com a pista
					glRotatef(ang, 0, 1, 0);
					//Ir para o centro da curva a esquerda
					glTranslatef(0, 0, -2 * (raio_curva + (largura / 2)));
					//Roda o angulo necess�rio para colocar no lugar certo respeitando a distancia que falta
					glRotatef(-90 + vel_ang*aux / vel, 0, 1, 0);
					//Posiciona na curva
					glTranslatef(raio_curva + largura / 2 + (largura / 2 + 2)*(objs[i][1]), 0, 0);
					//Desenha o carro
					Arvore();
					glPopMatrix();
				}
				else if (prox == 2) {
					objs[i][0] = objs[i][0] - vel;
					glPushMatrix();
					//Leva pro centro da curva
					glTranslatef(raio_curva + (largura / 2), 0, 0);
					//Roda o angulo necess�rio para colocar no lugar certo respeitando a distancia que falta
					glRotatef(180 - vel_ang*objs[i][0] / vel, 0, 1, 0);
					//Posiciona na curva
					glTranslatef(raio_curva + largura / 2 + (largura / 2 + 2)*(objs[i][1]), 0, 0);
					//Desenha o carro
					Arvore();
					glPopMatrix();
				}
			}
		}
	}

	if (objs[0][0] <= 2) {
		num_obj--;

	}
}

void Spawn() {

	int i, j;
	float aux;
	if (num_inimigos<max_inimigos) {
		if (num_inimigos == 0) {
			aux = rand() % (prct + 1);
			aux = aux / prct;
			cor_inimigo[0] = rand() % 8;
			inimigos[0][1] = 1 + (aux*(largura - 1));
			inimigos[0][0] = dist;
			num_inimigos++;
			for (i = num_inimigos; i<max_inimigos; i++) {
				aux = rand() % (prct + 1);
				aux = aux / prct;
				cor_inimigo[i] = rand() % 8;
				inimigos[i][1] = 1 + (aux*(largura - 1));
				inimigos[i][0] = inimigos[i - 1][0] + dist;
				num_inimigos++;
			}
		}
		else {
			for (i = 0; i<num_inimigos; i++) {
				cor_inimigo[i] = cor_inimigo[i + 1];
				inimigos[i][0] = inimigos[i + 1][0];
				inimigos[i][1] = inimigos[i + 1][1];
			}
			aux = rand() % (prct + 1);
			aux = aux / prct;
			cor_inimigo[max_inimigos - 1] = rand() % 8;
			inimigos[max_inimigos - 1][1] = 0.55 + (aux*(largura - 1));
			inimigos[max_inimigos - 1][0] = inimigos[max_inimigos - 2][0] + dist;
			num_inimigos++;
		}
	}

	for (i = 0; i<max_inimigos; i++) {
		if (atual == 0) {//a pista atual (primeira pista) 'e reta
			if (inimigos[i][0] <= comprimento - km) { //o inimigo esta na primeira pista (atual)
				inimigos[i][0] = inimigos[i][0] - vel_inimigos;
				glPushMatrix();
				glTranslatef(inimigos[i][1] - (largura / 2), 0, -inimigos[i][0]);
				glRotatef(180, 0, 1, 0);
				Inimigo(cor_inimigo[i]);
				glPopMatrix();
			}
			else { //o inimigo esta na segunda pista
				if (prox == 0) { //a segunda pista 'e uma reta
					inimigos[i][0] = inimigos[i][0] - vel_inimigos;
					glPushMatrix();
					glTranslatef(inimigos[i][1] - (largura / 2), 0, -inimigos[i][0]);
					glRotatef(180, 0, 1, 0);
					Inimigo(cor_inimigo[i]);
					glPopMatrix();
				}
				else if (prox == 1) { //a segunda pista 'e esquerda
					inimigos[i][0] = inimigos[i][0] - vel_inimigos;
					aux = inimigos[i][0] - (comprimento - km);
					glPushMatrix();
					glTranslatef(-raio_curva - (largura / 2), 0, km - comprimento);
					glRotatef(vel_ang*aux / vel, 0, 1, 0);
					glTranslatef(raio_curva + inimigos[i][1], 0, 0);
					glRotatef(180, 0, 1, 0);
					Inimigo(cor_inimigo[i]);
					glPopMatrix();
				}
				else if (prox == 2) { //a segunda pista 'e direita
					inimigos[i][0] = inimigos[i][0] - vel_inimigos;
					aux = inimigos[i][0] - (comprimento - km);
					glPushMatrix();
					glTranslatef(raio_curva + (largura / 2), 0, km - comprimento);
					glRotatef(-vel_ang*aux / vel, 0, 1, 0);
					glTranslatef(-raio_curva - largura + inimigos[i][1], 0, 0);
					glRotatef(180, 0, 1, 0);
					Inimigo(cor_inimigo[i]);
					glPopMatrix();
				}
			}
		}
		else if (atual == 1) { //a pista atual 'e esquerda
			if (inimigos[i][0] <= comprimento - km) { //o inimigo esta na primeira pista
				inimigos[i][0] = inimigos[i][0] - vel_inimigos;
				glPushMatrix();
				glTranslatef(-raio_curva - (largura / 2), 0, 0);
				glRotatef(vel_ang*inimigos[i][0] / vel, 0, 1, 0);
				glTranslatef(raio_curva + inimigos[i][1], 0, 0);
				glRotatef(180, 0, 1, 0);
				Inimigo(cor_inimigo[i]);
				glPopMatrix();
			}
			else {
				if (prox == 0) { //a segunda pista 'e uma reta
					inimigos[i][0] = inimigos[i][0] - vel_inimigos;
					aux = inimigos[i][0] - (comprimento - km);
					glPushMatrix();
					glTranslatef(-raio_curva - (largura / 2), 0, 0);
					glRotatef(90 - ang, 0, 1, 0);
					glTranslatef(raio_curva + inimigos[i][1], 0, -aux);
					glRotatef(180, 0, 1, 0);
					Inimigo(cor_inimigo[i]);
					glPopMatrix();
				}
				else if (prox == 1) {//a segunda pista 'e esquerda
					inimigos[i][0] = inimigos[i][0] - vel_inimigos;
					glPushMatrix();
					glTranslatef(-raio_curva - (largura / 2), 0, 0);
					glRotatef(vel_ang*inimigos[i][0] / vel, 0, 1, 0);
					glTranslatef(raio_curva + inimigos[i][1], 0, 0);
					glRotatef(180, 0, 1, 0);
					Inimigo(cor_inimigo[i]);
					glPopMatrix();
				}
				else if (prox == 2) { //a segunda pista 'e direita
					inimigos[i][0] = inimigos[i][0] - vel_inimigos;
					aux = inimigos[i][0] - (comprimento - km);
					glPushMatrix();
					//Leva para o centro da curva
					glTranslatef(-raio_curva - (largura / 2), 0, 0);
					//Roda junto com a pista
					glRotatef(-ang, 0, 1, 0);
					//Ir para o centro da curva a direta
					glTranslatef(0, 0, -2 * (raio_curva + (largura / 2)));
					//Roda o angulo necess�rio para colocar no lugar certo respeitando a distancia que falta
					glRotatef(-90 - vel_ang*aux / vel, 0, 1, 0);
					//Posiciona na curva
					glTranslatef(raio_curva + largura - inimigos[i][1], 0, 0);
					//Desenha o carro
					//glRotatef(180,0,1,0);
					Inimigo(cor_inimigo[i]);
					glPopMatrix();
				}
			}

		}
		else if (atual == 2) {
			if (inimigos[i][0] <= comprimento - km) {
				inimigos[i][0] = inimigos[i][0] - vel_inimigos;
				glPushMatrix();
				//Leva pro centro da curva
				glTranslatef(raio_curva + (largura / 2), 0, 0);
				//Roda o angulo necess�rio para colocar no lugar certo respeitando a distancia que falta
				glRotatef(180 - vel_ang*inimigos[i][0] / vel, 0, 1, 0);
				//Posiciona na curva
				glTranslatef(raio_curva + largura - inimigos[i][1], 0, 0);
				//Desenha o carro
				Inimigo(cor_inimigo[i]);
				glPopMatrix();
			}
			else {
				if (prox == 0) {
					inimigos[i][0] = inimigos[i][0] - vel_inimigos;
					aux = inimigos[i][0] - (comprimento - km);
					glPushMatrix();
					//Leva pro centro da curva
					glTranslatef(raio_curva + (largura / 2), 0, 0);
					//Roda junto com a pista
					glRotatef(ang, 0, 1, 0);
					//Vai pro fim da pista reta
					glTranslatef(aux, 0, -raio_curva - largura + inimigos[i][1]);
					//Desenha o carro
					glRotatef(90, 0, 1, 0);
					Inimigo(cor_inimigo[i]);
					glPopMatrix();
				}
				else if (prox == 1) {
					inimigos[i][0] = inimigos[i][0] - vel_inimigos;
					aux = inimigos[i][0] - (comprimento - km);
					glPushMatrix();
					//Leva para o centro da curva
					glTranslatef(raio_curva + (largura / 2), 0, 0);
					//Roda junto com a pista
					glRotatef(ang, 0, 1, 0);
					//Ir para o centro da curva a esquerda
					glTranslatef(0, 0, -2 * (raio_curva + (largura / 2)));
					//Roda o angulo necess�rio para colocar no lugar certo respeitando a distancia que falta
					glRotatef(-90 + vel_ang*aux / vel, 0, 1, 0);
					//Posiciona na curva
					glTranslatef(raio_curva + inimigos[i][1], 0, 0);
					//Desenha o carro
					glRotatef(180, 0, 1, 0);
					Inimigo(cor_inimigo[i]);
					glPopMatrix();
				}
				else if (prox == 2) {
					inimigos[i][0] = inimigos[i][0] - vel_inimigos;
					glPushMatrix();
					//Leva pro centro da curva
					glTranslatef(raio_curva + (largura / 2), 0, 0);
					//Roda o angulo necess�rio para colocar no lugar certo respeitando a distancia que falta
					glRotatef(180 - vel_ang*inimigos[i][0] / vel, 0, 1, 0);
					//Posiciona na curva
					glTranslatef(raio_curva + largura - inimigos[i][1], 0, 0);
					//Desenha o carro
					//glRotatef(180,0,1,0);
					Inimigo(cor_inimigo[i]);
					glPopMatrix();
				}
			}
		}
	}

	if (inimigos[0][0] <= 2.5) {
		if (inimigos[0][0] >= 0)
			Colisao();
		if (inimigos[0][0] <= -10) {
			num_inimigos--;
		}
	}
}


void Chao() {
	glColor3ub(cor_chao_atual[0], cor_chao_atual[1], cor_chao_atual[2]);
	glBegin(GL_QUADS);
	glVertex3f(-100, -0.01, 10);
	glVertex3f(-100, -0.01, -120);
	glVertex3f(100, -0.01, -120);
	glVertex3f(100, -0.01, 10);
	glEnd();
}

void Reta(int sentido) {
	glColor3ub(cor_pista_atual[0], cor_pista_atual[1], cor_pista_atual[2]);
	glBegin(GL_QUADS);
	if (sentido == 1) {
		glVertex3f(0, 0, largura / 2);
		glVertex3f(0, 0, -largura / 2);
		glVertex3f(-comprimento, 0, -largura / 2);
		glVertex3f(-comprimento, 0, largura / 2);
	}
	else {
		if (atual == 0) {
			glVertex3f(largura / 2, 0, 5);
			glVertex3f(-largura / 2, 0, 5);
		}
		else {
			glVertex3f(largura / 2, 0, 0);
			glVertex3f(-largura / 2, 0, 0);
		}
		glVertex3f(-largura / 2, 0, -comprimento);
		glVertex3f(largura / 2, 0, -comprimento);

	}
	glEnd();
}

void Curva(int proximo) {

	int i;
	float ax, az, bx, bz, cx, cz, dx, dz;

	ax = raio_curva; az = 0;
	bx = raio_curva + largura; 		bz = 0;
	cx = (cos_ang*ax); 		cz = (sen_ang*ax + az*cos_ang);
	dx = (cos_ang*bx); 		dz = (sen_ang*bx + bz*cos_ang);


	if (proximo == 0) {
		az = -5;
		bz = -5;
	}
	glColor3ub(cor_pista_atual[0], cor_pista_atual[1], cor_pista_atual[2]);
	for (i = 0; i<numquads; i++) {
		glBegin(GL_QUADS);
		glVertex3f(ax, 0, az);
		glVertex3f(bx, 0, bz);
		glVertex3f(dx, 0, dz);
		glVertex3f(cx, 0, cz);
		glEnd();
		ax = cx; az = cz;
		bx = dx; 		bz = dz;
		cx = (cos_ang*ax - sen_ang*az); 		cz = (sen_ang*ax + az*cos_ang);
		dx = (cos_ang*bx - sen_ang*bz); 		dz = (sen_ang*bx + bz*cos_ang);

	}
	if (proximo == 0) {
		fim_pista1x = ax;
		fim_pista1z = az;
		fim_pista2x = bx;
		fim_pista2z = bz;
		az = -5;
		bz = -5;
	}


}

void Frente_Frente() {

	glPushMatrix();
	glTranslatef(0, 0, km);
	Reta(0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, km - comprimento);
	Reta(0);
	glPopMatrix();
}

void Frente_Esq() {

	glPushMatrix();
	glTranslatef(0, 0, km);
	Reta(0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-raio_curva - (largura / 2), 0, km - comprimento);
	glScalef(1, 1, -1);
	Curva(1);
	glPopMatrix();
}

void Frente_Dir() {

	glPushMatrix();
	glTranslatef(0, 0, km);
	Reta(0);
	glPopMatrix();

	glPushMatrix();
	glScalef(-1, 1, 1);
	glTranslatef(-raio_curva - (largura / 2), 0, km - comprimento);
	glScalef(1, 1, -1);
	Curva(1);
	glPopMatrix();
}

void Esq_Frente() {
	glPushMatrix();
	glScalef(1, 1, -1);
	glTranslatef(-raio_curva - (largura / 2), 0, 0);
	glRotatef(ang, 0, 1, 0);
	Curva(0);
	glTranslatef((fim_pista1x + fim_pista2x) / 2, 0, (fim_pista1z + fim_pista2z) / 2);
	Reta(1);
	glPopMatrix();
}

void Esq_Esq() {
	glPushMatrix();
	glScalef(1, 1, -1);
	glTranslatef(-raio_curva - largura / 2, 0, 0);
	glRotatef(ang, 0, 1, 0);
	Curva(0);
	glRotatef(-90, 0, 1, 0);
	Curva(1);
	glPopMatrix();
}

void Esq_Dir() {
	glPushMatrix();
	glScalef(1, 1, -1);
	glTranslatef(-raio_curva - (largura / 2), 0, 0);
	glRotatef(ang, 0, 1, 0);
	Curva(0);
	glTranslatef((fim_pista1x + fim_pista2x) / 2, 0, 2 * (fim_pista1z + fim_pista2z) / 2);
	glRotatef(180, 0, 1, 0);
	Curva(1);
	glPopMatrix();
}

void Dir_Frente() {
	glPushMatrix();
	glScalef(-1, 1, -1);
	glTranslatef(-raio_curva - (largura / 2), 0, 0);
	glRotatef(ang, 0, 1, 0);
	Curva(0);
	glTranslatef((fim_pista1x + fim_pista2x) / 2, 0, (fim_pista1z + fim_pista2z) / 2);
	Reta(1);
	glPopMatrix();
}

void Dir_Esq() {
	glPushMatrix();
	glScalef(-1, 1, -1);
	glTranslatef(-raio_curva - (largura / 2), 0, 0);
	glRotatef(ang, 0, 1, 0);
	Curva(0);
	glTranslatef((fim_pista1x + fim_pista2x) / 2, 0, 2 * (fim_pista1z + fim_pista2z) / 2);
	glRotatef(180, 0, 1, 0);
	Curva(1);
	glPopMatrix();
}

void Dir_Dir() {
	glPushMatrix();
	glScalef(-1, 1, -1);
	glTranslatef(-raio_curva - (largura / 2), 0, 0);
	glRotatef(ang, 0, 1, 0);
	Curva(0);
	glScalef(-1, 1, 1);
	Curva(1);
	glPopMatrix();
}


void Gerenciador() {
	tempo += relogio;

	if (atual == 0) {
		switch (prox) {
		case 0: Frente_Frente(); break;
		case 1: Frente_Esq(); break;
		case 2: Frente_Dir(); break;
		}
		curva = 0;
	}
	else if (atual == 1) {
		switch (prox) {
		case 0: Esq_Frente(); break;
		case 1: Esq_Esq(); break;
		case 2: Esq_Dir(); break;
		}
		curva = 1;
	}
	else {
		switch (prox) {
		case 0: Dir_Frente(); break;
		case 1: Dir_Esq(); break;
		case 2: Dir_Dir(); break;
		}
		curva = 1;
	}
	int aux1;
	if (curva == 1) {
		if (ang>90) {
			atual = prox;
			prox = rand() % 3;
			ang = 0;
			km = 0;
		}
		ang += vel_ang;
		km += vel;
		kmTotal += vel;
		if (atual == 1)
			parallax += vel_ang;
		else
			parallax -= vel_ang;
	}
	else {
		if (km>comprimento) {
			aux1 = atual;
			atual = prox;
			prox = aux1;
			prox = rand() % 3;
			km = 0;
		}
		km = km + vel;
		kmTotal += vel;
	}
}

void Cores() {
	if (tempo >= 1) {
		tempo = 0;
		if (espera == 0) {

			espera = 1;
			periodo++;
			if (periodo>2) {
				periodo = 0;
			}
			prox_periodo = periodo + 1;
			if (prox_periodo>2) {
				prox_periodo = 0;
			}
		}
		else {
			espera = 0;
		}
	}
	else {
		if (espera == 0) {
			cor_chao_atual[0] = cor_chao[periodo][0] + tempo*(cor_chao[prox_periodo][0] - cor_chao[periodo][0]);
			cor_chao_atual[1] = cor_chao[periodo][1] + tempo*(cor_chao[prox_periodo][1] - cor_chao[periodo][1]);
			cor_chao_atual[2] = cor_chao[periodo][2] + tempo*(cor_chao[prox_periodo][2] - cor_chao[periodo][2]);

			cor_pista_atual[0] = cor_pista[periodo][0] + tempo*(cor_pista[prox_periodo][0] - cor_pista[periodo][0]);
			cor_pista_atual[1] = cor_pista[periodo][1] + tempo*(cor_pista[prox_periodo][1] - cor_pista[periodo][1]);
			cor_pista_atual[2] = cor_pista[periodo][2] + tempo*(cor_pista[prox_periodo][2] - cor_pista[periodo][2]);

			cor_ceu_atual[0][0] = cor_ceu[periodo][0][0] + tempo*(cor_ceu[prox_periodo][0][0] - cor_ceu[periodo][0][0]);
			cor_ceu_atual[0][1] = cor_ceu[periodo][0][1] + tempo*(cor_ceu[prox_periodo][0][1] - cor_ceu[periodo][0][1]);
			cor_ceu_atual[0][2] = cor_ceu[periodo][0][2] + tempo*(cor_ceu[prox_periodo][0][2] - cor_ceu[periodo][0][2]);

			cor_ceu_atual[1][0] = cor_ceu[periodo][1][0] + tempo*(cor_ceu[prox_periodo][1][0] - cor_ceu[periodo][1][0]);
			cor_ceu_atual[1][1] = cor_ceu[periodo][1][1] + tempo*(cor_ceu[prox_periodo][1][1] - cor_ceu[periodo][1][1]);
			cor_ceu_atual[1][2] = cor_ceu[periodo][1][2] + tempo*(cor_ceu[prox_periodo][1][2] - cor_ceu[periodo][1][2]);

			cor_ceu_atual[2][0] = cor_ceu[periodo][2][0] + tempo*(cor_ceu[prox_periodo][2][0] - cor_ceu[periodo][2][0]);
			cor_ceu_atual[2][1] = cor_ceu[periodo][2][1] + tempo*(cor_ceu[prox_periodo][2][1] - cor_ceu[periodo][2][1]);
			cor_ceu_atual[2][2] = cor_ceu[periodo][2][2] + tempo*(cor_ceu[prox_periodo][2][2] - cor_ceu[periodo][2][2]);

			cor_predios_atual[0][0] = cor_predios[periodo][0][0] + tempo*(cor_predios[prox_periodo][0][0] - cor_predios[periodo][0][0]);
			cor_predios_atual[0][1] = cor_predios[periodo][0][1] + tempo*(cor_predios[prox_periodo][0][1] - cor_predios[periodo][0][1]);
			cor_predios_atual[0][2] = cor_predios[periodo][0][2] + tempo*(cor_predios[prox_periodo][0][2] - cor_predios[periodo][0][2]);

			cor_predios_atual[1][0] = cor_predios[periodo][1][0] + tempo*(cor_predios[prox_periodo][1][0] - cor_predios[periodo][1][0]);
			cor_predios_atual[1][1] = cor_predios[periodo][1][1] + tempo*(cor_predios[prox_periodo][1][1] - cor_predios[periodo][1][1]);
			cor_predios_atual[1][2] = cor_predios[periodo][1][2] + tempo*(cor_predios[prox_periodo][1][2] - cor_predios[periodo][1][2]);

			cor_predios_atual[2][0] = cor_predios[periodo][2][0] + tempo*(cor_predios[prox_periodo][2][0] - cor_predios[periodo][2][0]);
			cor_predios_atual[2][1] = cor_predios[periodo][2][1] + tempo*(cor_predios[prox_periodo][2][1] - cor_predios[periodo][2][1]);
			cor_predios_atual[2][2] = cor_predios[periodo][2][2] + tempo*(cor_predios[prox_periodo][2][2] - cor_predios[periodo][2][2]);

			cor_carro_atual[0][0] = cor_carro[0][periodo][0] + tempo*(cor_carro[0][prox_periodo][0] - cor_carro[0][periodo][0]);
			cor_carro_atual[1][0] = cor_carro[1][periodo][0] + tempo*(cor_carro[1][prox_periodo][0] - cor_carro[1][periodo][0]);
			cor_carro_atual[2][0] = cor_carro[2][periodo][0] + tempo*(cor_carro[2][prox_periodo][0] - cor_carro[2][periodo][0]);
			cor_carro_atual[3][0] = cor_carro[3][periodo][0] + tempo*(cor_carro[3][prox_periodo][0] - cor_carro[3][periodo][0]);
			cor_carro_atual[4][0] = cor_carro[4][periodo][0] + tempo*(cor_carro[4][prox_periodo][0] - cor_carro[4][periodo][0]);
			cor_carro_atual[5][0] = cor_carro[5][periodo][0] + tempo*(cor_carro[5][prox_periodo][0] - cor_carro[5][periodo][0]);
			cor_carro_atual[6][0] = cor_carro[6][periodo][0] + tempo*(cor_carro[6][prox_periodo][0] - cor_carro[6][periodo][0]);
			cor_carro_atual[7][0] = cor_carro[7][periodo][0] + tempo*(cor_carro[7][prox_periodo][0] - cor_carro[7][periodo][0]);

			cor_carro_atual[0][1] = cor_carro[0][periodo][1] + tempo*(cor_carro[0][prox_periodo][1] - cor_carro[0][periodo][1]);
			cor_carro_atual[1][1] = cor_carro[1][periodo][1] + tempo*(cor_carro[1][prox_periodo][1] - cor_carro[1][periodo][1]);
			cor_carro_atual[2][1] = cor_carro[2][periodo][1] + tempo*(cor_carro[2][prox_periodo][1] - cor_carro[2][periodo][1]);
			cor_carro_atual[3][1] = cor_carro[3][periodo][1] + tempo*(cor_carro[3][prox_periodo][1] - cor_carro[3][periodo][1]);
			cor_carro_atual[4][1] = cor_carro[4][periodo][1] + tempo*(cor_carro[4][prox_periodo][1] - cor_carro[4][periodo][1]);
			cor_carro_atual[5][1] = cor_carro[5][periodo][1] + tempo*(cor_carro[5][prox_periodo][1] - cor_carro[5][periodo][1]);
			cor_carro_atual[6][1] = cor_carro[6][periodo][1] + tempo*(cor_carro[6][prox_periodo][1] - cor_carro[6][periodo][1]);
			cor_carro_atual[7][1] = cor_carro[7][periodo][1] + tempo*(cor_carro[7][prox_periodo][1] - cor_carro[7][periodo][1]);

			cor_carro_atual[0][2] = cor_carro[0][periodo][2] + tempo*(cor_carro[0][prox_periodo][2] - cor_carro[0][periodo][2]);
			cor_carro_atual[1][2] = cor_carro[1][periodo][2] + tempo*(cor_carro[1][prox_periodo][2] - cor_carro[1][periodo][2]);
			cor_carro_atual[2][2] = cor_carro[2][periodo][2] + tempo*(cor_carro[2][prox_periodo][2] - cor_carro[2][periodo][2]);
			cor_carro_atual[3][2] = cor_carro[3][periodo][2] + tempo*(cor_carro[3][prox_periodo][2] - cor_carro[3][periodo][2]);
			cor_carro_atual[4][2] = cor_carro[4][periodo][2] + tempo*(cor_carro[4][prox_periodo][2] - cor_carro[4][periodo][2]);
			cor_carro_atual[5][2] = cor_carro[5][periodo][2] + tempo*(cor_carro[5][prox_periodo][2] - cor_carro[5][periodo][2]);
			cor_carro_atual[6][2] = cor_carro[6][periodo][2] + tempo*(cor_carro[6][prox_periodo][2] - cor_carro[6][periodo][2]);
			cor_carro_atual[7][2] = cor_carro[7][periodo][2] + tempo*(cor_carro[7][prox_periodo][2] - cor_carro[7][periodo][2]);

			cor_adesivo_atual[0][0] = cor_adesivo[0][periodo][0] + tempo*(cor_adesivo[0][prox_periodo][0] - cor_adesivo[0][periodo][0]);
			cor_adesivo_atual[1][0] = cor_adesivo[1][periodo][0] + tempo*(cor_adesivo[1][prox_periodo][0] - cor_adesivo[1][periodo][0]);
			cor_adesivo_atual[2][0] = cor_adesivo[2][periodo][0] + tempo*(cor_adesivo[2][prox_periodo][0] - cor_adesivo[2][periodo][0]);
			cor_adesivo_atual[3][0] = cor_adesivo[3][periodo][0] + tempo*(cor_adesivo[3][prox_periodo][0] - cor_adesivo[3][periodo][0]);
			cor_adesivo_atual[4][0] = cor_adesivo[4][periodo][0] + tempo*(cor_adesivo[4][prox_periodo][0] - cor_adesivo[4][periodo][0]);
			cor_adesivo_atual[5][0] = cor_adesivo[5][periodo][0] + tempo*(cor_adesivo[5][prox_periodo][0] - cor_adesivo[5][periodo][0]);
			cor_adesivo_atual[6][0] = cor_adesivo[6][periodo][0] + tempo*(cor_adesivo[6][prox_periodo][0] - cor_adesivo[6][periodo][0]);
			cor_adesivo_atual[7][0] = cor_adesivo[7][periodo][0] + tempo*(cor_adesivo[7][prox_periodo][0] - cor_adesivo[7][periodo][0]);

			cor_adesivo_atual[0][1] = cor_adesivo[0][periodo][1] + tempo*(cor_adesivo[0][prox_periodo][1] - cor_adesivo[0][periodo][1]);
			cor_adesivo_atual[1][1] = cor_adesivo[1][periodo][1] + tempo*(cor_adesivo[1][prox_periodo][1] - cor_adesivo[1][periodo][1]);
			cor_adesivo_atual[2][1] = cor_adesivo[2][periodo][1] + tempo*(cor_adesivo[2][prox_periodo][1] - cor_adesivo[2][periodo][1]);
			cor_adesivo_atual[3][1] = cor_adesivo[3][periodo][1] + tempo*(cor_adesivo[3][prox_periodo][1] - cor_adesivo[3][periodo][1]);
			cor_adesivo_atual[4][1] = cor_adesivo[4][periodo][1] + tempo*(cor_adesivo[4][prox_periodo][1] - cor_adesivo[4][periodo][1]);
			cor_adesivo_atual[5][1] = cor_adesivo[5][periodo][1] + tempo*(cor_adesivo[5][prox_periodo][1] - cor_adesivo[5][periodo][1]);
			cor_adesivo_atual[6][1] = cor_adesivo[6][periodo][1] + tempo*(cor_adesivo[6][prox_periodo][1] - cor_adesivo[6][periodo][1]);
			cor_adesivo_atual[7][1] = cor_adesivo[7][periodo][1] + tempo*(cor_adesivo[7][prox_periodo][1] - cor_adesivo[7][periodo][1]);

			cor_adesivo_atual[0][2] = cor_adesivo[0][periodo][2] + tempo*(cor_adesivo[0][prox_periodo][2] - cor_adesivo[0][periodo][2]);
			cor_adesivo_atual[1][2] = cor_adesivo[1][periodo][2] + tempo*(cor_adesivo[1][prox_periodo][2] - cor_adesivo[1][periodo][2]);
			cor_adesivo_atual[2][2] = cor_adesivo[2][periodo][2] + tempo*(cor_adesivo[2][prox_periodo][2] - cor_adesivo[2][periodo][2]);
			cor_adesivo_atual[3][2] = cor_adesivo[3][periodo][2] + tempo*(cor_adesivo[3][prox_periodo][2] - cor_adesivo[3][periodo][2]);
			cor_adesivo_atual[4][2] = cor_adesivo[4][periodo][2] + tempo*(cor_adesivo[4][prox_periodo][2] - cor_adesivo[4][periodo][2]);
			cor_adesivo_atual[5][2] = cor_adesivo[5][periodo][2] + tempo*(cor_adesivo[5][prox_periodo][2] - cor_adesivo[5][periodo][2]);
			cor_adesivo_atual[6][2] = cor_adesivo[6][periodo][2] + tempo*(cor_adesivo[6][prox_periodo][2] - cor_adesivo[6][periodo][2]);
			cor_adesivo_atual[7][2] = cor_adesivo[7][periodo][2] + tempo*(cor_adesivo[7][prox_periodo][2] - cor_adesivo[7][periodo][2]);


		}
	}
	tempo += relogio;
}

void Ceu() {
	glBegin(GL_QUADS);
	glColor3ub(cor_ceu_atual[0][0], cor_ceu_atual[0][1], cor_ceu_atual[0][2]);
	glVertex3f(-100, -10, -100);
	glVertex3f(100, -10, -100);
	glColor3ub(cor_ceu_atual[1][0], cor_ceu_atual[1][1], cor_ceu_atual[1][2]);
	glVertex3f(100, 20, -100);
	glVertex3f(-100, 20, -100);
	glEnd();
	glBegin(GL_QUADS);
	glColor3ub(cor_ceu_atual[1][0], cor_ceu_atual[1][1], cor_ceu_atual[1][2]);
	glVertex3f(-100, 20, -100);
	glVertex3f(100, 20, -100);
	glColor3ub(cor_ceu_atual[2][0], cor_ceu_atual[2][1], cor_ceu_atual[2][2]);
	glVertex3f(100, 40, -100);
	glVertex3f(-100, 40, -100);
	glEnd();

	glPushMatrix();
	glTranslatef(20 * parallax / 90, 0, -97);
	glScalef(1, 1.9, 1);
	Skyline0();
	glTranslatef(-85, 0, 0);
	Skyline0();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15 * parallax / 90, 0, -98);
	glScalef(1, 1.7, 1);
	Skyline1();
	glTranslatef(-80, 0, 0);
	Skyline1();
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1.5, 1);
	glTranslatef(10 * parallax / 90, 0, -99);
	Skyline2();
	glTranslatef(-70, 0, 0);
	Skyline2();
	glPopMatrix();
}

void Display() {
	// Inicializa par�metros de rendering
	// Define a cor de fundo da janela de visualiza��o como preta
	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_SMOOTH);
	glEnable(GL_BLEND);

	glMatrixMode(GL_PROJECTION);/*glMatrixMode()- define qual matriz ser� alterada. SEMPRE defina o tipo de apresenta��o (Ortogonal ou Perspectiva) na matriz PROJECTION.*/
	glLoadIdentity();//"Limpa" ou "transforma" a matriz em identidade, reduzindo poss�veis erros.

					 //if (projecao == 1)
					 //	glOrtho(-400, 400, -400, 400, -400, 400); //Define a proje��o como ortogonal
					 //else
	gluPerspective(45, 1, 1, 180); //Define a proje��o como perspectiva

	glMatrixMode(GL_MODELVIEW);/*Define qual matriz ser� alterada -> MODELVIEW (onde o desenho ocorrer�).*/
	glLoadIdentity(); //"Limpa" ou "transforma" a matriz em identidade.


	gluLookAt(posx, posy, posz, ox, oy, oz, lx, ly, lz); //Define a pos da c�mera, para onde olha e qual eixo est� na vertical.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* " limpando os buffers para suportarem anima��es */

	if (inicio == 1) {
		glPushMatrix();
		glColor3ub(255, 255, 255);
		drawBitmapText("A - Esquerda", -3, 2.8, -10);
		drawBitmapText("D - Direita", -3, 2.1, -10);
		drawBitmapText("Q e E - Troca a Cor do Carro", -3, 1.4, -10);
		drawBitmapText("C - Troca a Camera", -3, 0.7, -10);
		drawBitmapText("Pressione J para iniciar", -3, 0, -10);
		glPopMatrix();
		Pause();


	}
	if (vel == 0 && inicio != 1) {
		glPushMatrix();
		glColor3ub(255, 255, 255);
		//glScalef(2, 2, 2);
		drawBitmapText("Game     Over    -     Loser!", -3, 3.5, -10);
		drawBitmapText("A - Esquerda", -3, 2.8, -10);
		drawBitmapText("D - Direita", -3, 2.1, -10);
		drawBitmapText("Q e E - Troca a Cor do Carro", -3, 1.4, -10);
		drawBitmapText("C - Troca a Camera", -3, 0.7, -10);
		drawBitmapText("Pressione J para iniciar", -3, 0, -10);
		glPopMatrix();
	}

	char s[20];
	sprintf(s, "%d", (int)kmTotal);
	strcat(s, " Km");
	glColor3ub(255, 255, 255);
	drawBitmapText(s, 0, 5, -10);

	//Chamada para Fun��o  ou fun��es para desenhar o objeto/cena...
	//----------------------------------------------------------------
	Cores();
	Cenario();
	Ceu();
	Chao();
	Carro();
	Spawn();
	Gerenciador();
	//----------------------------------------------------------------   

	glutSwapBuffers(); //Executa a Cena. SwapBuffers d� suporte para mais de um buffer, permitindo execu��o de anima��es sem cintila��es. 
	glutPostRedisplay();
}

void Idle() {

	// Pega o tempo atual desde o come�o da execu��o (em milissegundos)
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = currentTime - lastFrameTime;
	lastFrameTime = currentTime;

	// Anima��o do carro se movendo pros lados
	// Tem que ser antes do usleep para usar o deltaTime correto
	car_rot = 0;
	if (pos_car_final != pos_car)
	{
		float distancia = (pos_car - pos_car_final);
		float direcao = distancia / fabsf(distancia);

		car_rot = -3.0 * direcao;
		pos_car_final += direcao * fminf(fabsf(vel_x * deltaTime), fabsf(distancia));
	}

	// Anima��o da nuvem
	pn_cont += 0.002 * deltaTime;
	if (pn<22) pn += 0.002 * deltaTime;
	else pn = -22;

	if (pn2<22) pn2 += 0.001 * deltaTime;
	else pn2 = -22;

	// deltaTime � o tempo entre 2 frames.
	// 16 milissegundos � o tempo aproximado entre 2 frames de um aplicativo rodando a 60FPS
	if (deltaTime < 16) {
		int sleepTime = 16 - deltaTime;
		usleep(sleepTime);
		//linux usar -> usleep(sleepTime);
	}
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	int aux1;
	if (key == 'd') {
		if (pos_car<(largura / 2) - 1) {
			pos_car += mov;
		}
	}
	else if (key == 'a') {
		if (pos_car>-(largura / 2) + 1) {
			pos_car -= mov;
		}
	}
	else if (key == 'j') {
		vel = vel_p;
		vel_ang = vel_ang_p;
		vel_inimigos = vel_inimigos_p;
		mov = mov_p;
		relogio = relogio_p;
		inicio = 0;
	}
	else if (key == 'c') {
		switch (camera)
		{
		case rear:
			// Muda a camera para top
			posx = 0, posy = 18, posz = 5;
			oy = 0, ox = 0, oz = -5;
			camera = top;
			break;
		case top:
			// Muda a camera pro left
			posx = -2, posy = 2, posz = 7;
			oy = 0, ox = 0, oz = -6;
			camera = left;
			break;
		case left:
			// Muda a camera para rear
			posx = 0, posy = 3, posz = 10;  //Vari�veis que definem a posi��o da c�mera 0,3,10
			oy = 0, ox = 0, oz = -10;		//Vari�veis que definem para onde a c�mera olha 0,0,-10
			camera = rear;
			break;
		}
	}
	else if (key == 'e') {
		cor++;
		if (cor>7)
			cor = 0;
	}
	else if (key == 'q') {
		cor--;
		if (cor<0)
			cor = 7;
	}
	glutPostRedisplay();
}


int main(int argc, char **argv) {
	//Definindo cores
	cor_pista[0][0] = 134; cor_pista[0][1] = 153; cor_pista[0][2] = 134;
	cor_pista[1][0] = 134; cor_pista[1][1] = 112; cor_pista[1][2] = 87;
	cor_pista[2][0] = 55; cor_pista[2][1] = 43; cor_pista[2][2] = 69;

	cor_chao[0][0] = 74; cor_chao[0][1] = 215; cor_chao[0][2] = 0;
	cor_chao[1][0] = 148; cor_chao[1][1] = 200; cor_chao[1][2] = 0;
	cor_chao[2][0] = 25; cor_chao[2][1] = 13; cor_chao[2][2] = 39;

	cor_ceu[0][0][0] = 31; cor_ceu[0][0][1] = 181; cor_ceu[0][0][2] = 144;
	cor_ceu[1][0][0] = 255; cor_ceu[1][0][1] = 121; cor_ceu[1][0][2] = 13;
	cor_ceu[2][0][0] = 53; cor_ceu[2][0][1] = 5; cor_ceu[2][0][2] = 51;

	cor_ceu[0][1][0] = 0; cor_ceu[0][0][1] = 255; cor_ceu[0][1][2] = 255;
	cor_ceu[1][1][0] = 253; cor_ceu[1][1][1] = 58; cor_ceu[1][1][2] = 58;
	cor_ceu[2][1][0] = 73; cor_ceu[2][1][1] = 14; cor_ceu[2][1][2] = 97;

	cor_ceu[0][2][0] = 36; cor_ceu[0][2][1] = 181; cor_ceu[0][2][2] = 144;
	cor_ceu[1][2][0] = 94; cor_ceu[1][2][1] = 0; cor_ceu[1][2][2] = 72;
	cor_ceu[2][2][0] = 13; cor_ceu[2][2][1] = 0; cor_ceu[2][2][2] = 37;

	cor_predios[0][0][0] = 128; cor_predios[0][0][1] = 135; cor_predios[0][0][2] = 135;
	cor_predios[1][0][0] = 126; cor_predios[1][0][1] = 73; cor_predios[1][0][2] = 109;
	cor_predios[2][0][0] = 15; cor_predios[2][0][1] = 0; cor_predios[2][0][2] = 36;

	cor_predios[0][1][0] = 157; cor_predios[0][1][1] = 166; cor_predios[0][1][2] = 165;
	cor_predios[1][1][0] = 191; cor_predios[1][1][1] = 75; cor_predios[1][1][2] = 80;
	cor_predios[2][1][0] = 66; cor_predios[2][1][1] = 32; cor_predios[2][1][2] = 88;

	cor_predios[0][2][0] = 171; cor_predios[0][2][1] = 209; cor_predios[0][2][2] = 205;
	cor_predios[1][2][0] = 218; cor_predios[1][2][1] = 105; cor_predios[1][2][2] = 68;
	cor_predios[2][2][0] = 69; cor_predios[2][2][1] = 33; cor_predios[2][2][2] = 79;

	cor_carro[0][0][0] = 38; cor_carro[0][0][1] = 38; cor_carro[0][0][2] = 38;
	cor_carro[1][0][0] = 255; cor_carro[1][0][1] = 0; cor_carro[1][0][2] = 0;
	cor_carro[2][0][0] = 148; cor_carro[2][0][1] = 201; cor_carro[2][0][2] = 61;
	cor_carro[3][0][0] = 0;	cor_carro[3][0][1] = 0;	cor_carro[3][0][2] = 255;
	cor_carro[4][0][0] = 255;	cor_carro[4][0][1] = 255;	cor_carro[4][0][2] = 0;
	cor_carro[5][0][0] = 166;	cor_carro[5][0][1] = 33;	cor_carro[5][0][2] = 255;
	cor_carro[6][0][0] = 115;	cor_carro[6][0][1] = 147; cor_carro[6][0][2] = 217;
	cor_carro[7][0][0] = 255;	cor_carro[7][0][1] = 255; cor_carro[7][0][2] = 255;


	cor_carro[0][1][0] = 30; cor_carro[0][1][1] = 30; cor_carro[0][1][2] = 20;
	cor_carro[1][1][0] = 189; cor_carro[1][1][1] = 9; cor_carro[1][1][2] = 0;
	cor_carro[2][1][0] = 0; cor_carro[2][1][1] = 95; cor_carro[2][1][2] = 0;
	cor_carro[3][1][0] = 46;	cor_carro[3][1][1] = 0;	cor_carro[3][1][2] = 106;
	cor_carro[4][1][0] = 255;	cor_carro[4][1][1] = 209;	cor_carro[4][1][2] = 46;
	cor_carro[5][1][0] = 143;	cor_carro[5][1][1] = 0;	cor_carro[5][1][2] = 169;
	cor_carro[6][1][0] = 115;	cor_carro[6][1][1] = 126;	cor_carro[6][1][2] = 174;
	cor_carro[7][1][0] = 210;	cor_carro[7][1][1] = 195;	cor_carro[7][1][2] = 179;

	cor_carro[0][2][0] = 17; cor_carro[0][2][1] = 4; cor_carro[0][2][2] = 23;
	cor_carro[1][2][0] = 112; cor_carro[1][2][1] = 28; cor_carro[1][2][2] = 28;
	cor_carro[2][2][0] = 0; cor_carro[2][2][1] = 45; cor_carro[2][2][2] = 32;
	cor_carro[3][2][0] = 14; cor_carro[3][2][1] = 0; cor_carro[3][2][2] = 54;
	cor_carro[4][2][0] = 144; cor_carro[4][2][1] = 163; cor_carro[4][2][2] = 55;
	cor_carro[5][2][0] = 72; cor_carro[5][2][1] = 0; cor_carro[5][2][2] = 122;
	cor_carro[6][2][0] = 66; cor_carro[6][2][1] = 100; cor_carro[6][2][2] = 121;
	cor_carro[7][2][0] = 141; cor_carro[7][2][1] = 141; cor_carro[7][2][2] = 141;


	cor_adesivo[0][0][0] = 0; cor_adesivo[0][0][1] = 255; cor_adesivo[0][0][2] = 0;
	cor_adesivo[1][0][0] = 255; cor_adesivo[1][0][1] = 255; cor_adesivo[1][0][2] = 255;
	cor_adesivo[2][0][0] = 216; cor_adesivo[2][0][1] = 35; cor_adesivo[2][0][2] = 39;
	cor_adesivo[3][0][0] = 255; cor_adesivo[3][0][1] = 255; cor_adesivo[3][0][2] = 0;
	cor_adesivo[4][0][0] = 0; cor_adesivo[4][0][1] = 0; cor_adesivo[4][0][2] = 0;
	cor_adesivo[5][0][0] = 255; cor_adesivo[5][0][1] = 255; cor_adesivo[5][0][2] = 255;
	cor_adesivo[6][0][0] = 200; cor_adesivo[6][0][1] = 47; cor_adesivo[6][0][2] = 47;
	cor_adesivo[7][0][0] = 136; cor_adesivo[7][0][1] = 0; cor_adesivo[7][0][2] = 255;

	cor_adesivo[0][1][0] = 200; cor_adesivo[0][1][1] = 255; cor_adesivo[0][1][2] = 0;
	cor_adesivo[1][1][0] = 247; cor_adesivo[1][1][1] = 195; cor_adesivo[1][1][2] = 179;
	cor_adesivo[2][1][0] = 224; cor_adesivo[2][1][1] = 88; cor_adesivo[2][1][2] = 0;
	cor_adesivo[3][1][0] = 255; cor_adesivo[3][1][1] = 202; cor_adesivo[3][1][2] = 59;
	cor_adesivo[4][1][0] = 42; cor_adesivo[4][1][1] = 32; cor_adesivo[4][1][2] = 13;
	cor_adesivo[5][1][0] = 247; cor_adesivo[5][1][1] = 195; cor_adesivo[5][1][2] = 179;
	cor_adesivo[6][1][0] = 255; cor_adesivo[6][1][1] = 31; cor_adesivo[6][1][2] = 31;
	cor_adesivo[7][1][0] = 150; cor_adesivo[7][1][1] = 0; cor_adesivo[7][1][2] = 210;

	cor_adesivo[0][2][0] = 0; cor_adesivo[0][2][1] = 76; cor_adesivo[0][2][2] = 45;
	cor_adesivo[1][2][0] = 141; cor_adesivo[1][2][1] = 141; cor_adesivo[1][2][2] = 141;
	cor_adesivo[2][2][0] = 136; cor_adesivo[2][2][1] = 86; cor_adesivo[2][2][2] = 44;
	cor_adesivo[3][2][0] = 132; cor_adesivo[3][2][1] = 132; cor_adesivo[3][2][2] = 52;
	cor_adesivo[4][2][0] = 0; cor_adesivo[4][2][1] = 0; cor_adesivo[4][2][2] = 0;
	cor_adesivo[5][2][0] = 141; cor_adesivo[5][2][1] = 141; cor_adesivo[5][2][2] = 141;
	cor_adesivo[6][2][0] = 108; cor_adesivo[6][2][1] = 45; cor_adesivo[6][2][2] = 55;
	cor_adesivo[7][2][0] = 58; cor_adesivo[7][2][1] = 4; cor_adesivo[7][2][2] = 108;



	//Inicializa cores
	cor_chao_atual[0] = cor_chao[periodo][0]; cor_chao_atual[1] = cor_chao[periodo][1]; cor_chao_atual[2] = cor_chao[periodo][2];

	cor_pista_atual[0] = cor_pista[periodo][0]; cor_pista_atual[1] = cor_pista[periodo][1]; cor_pista_atual[2] = cor_pista[periodo][2];

	cor_ceu_atual[0][0] = cor_ceu[periodo][0][0]; cor_ceu_atual[0][1] = cor_ceu[periodo][0][1]; cor_ceu_atual[0][2] = cor_ceu[periodo][0][2];
	cor_ceu_atual[1][0] = cor_ceu[periodo][1][0]; cor_ceu_atual[1][1] = cor_ceu[periodo][1][1]; cor_ceu_atual[1][2] = cor_ceu[periodo][1][2];
	cor_ceu_atual[2][0] = cor_ceu[periodo][2][0]; cor_ceu_atual[2][1] = cor_ceu[periodo][2][1]; cor_ceu_atual[2][2] = cor_ceu[periodo][2][2];

	cor_predios_atual[0][0] = cor_predios[periodo][0][0]; cor_predios_atual[0][1] = cor_predios[periodo][0][1]; cor_predios_atual[0][2] = cor_predios[periodo][0][2];
	cor_predios_atual[1][0] = cor_predios[periodo][1][0]; cor_predios_atual[1][1] = cor_predios[periodo][1][1]; cor_predios_atual[1][2] = cor_predios[periodo][1][2];
	cor_predios_atual[2][0] = cor_predios[periodo][2][0]; cor_predios_atual[2][1] = cor_predios[periodo][2][1]; cor_predios_atual[2][2] = cor_predios[periodo][2][2];

	cor_carro_atual[0][0] = cor_carro[0][periodo][0]; cor_carro_atual[0][1] = cor_carro[0][periodo][1]; cor_carro_atual[0][2] = cor_carro[0][periodo][2];
	cor_carro_atual[1][0] = cor_carro[1][periodo][0]; cor_carro_atual[1][1] = cor_carro[1][periodo][1]; cor_carro_atual[1][2] = cor_carro[1][periodo][2];
	cor_carro_atual[2][0] = cor_carro[2][periodo][0]; cor_carro_atual[2][1] = cor_carro[2][periodo][1]; cor_carro_atual[2][2] = cor_carro[2][periodo][2];
	cor_carro_atual[3][0] = cor_carro[3][periodo][0];
	cor_carro_atual[3][1] = cor_carro[3][periodo][1];
	cor_carro_atual[3][2] = cor_carro[3][periodo][2];
	cor_carro_atual[4][0] = cor_carro[4][periodo][0]; cor_carro_atual[4][1] = cor_carro[4][periodo][1]; cor_carro_atual[4][2] = cor_carro[4][periodo][2];
	cor_carro_atual[5][0] = cor_carro[5][periodo][0]; cor_carro_atual[5][1] = cor_carro[5][periodo][1]; cor_carro_atual[5][2] = cor_carro[5][periodo][2];
	cor_carro_atual[6][0] = cor_carro[6][periodo][0]; cor_carro_atual[6][1] = cor_carro[6][periodo][1]; cor_carro_atual[6][2] = cor_carro[6][periodo][2];
	cor_carro_atual[7][0] = cor_carro[7][periodo][0]; cor_carro_atual[7][1] = cor_carro[7][periodo][1]; cor_carro_atual[7][2] = cor_carro[7][periodo][2];

	cor_adesivo_atual[0][0] = cor_adesivo[0][periodo][0]; cor_adesivo_atual[0][1] = cor_adesivo[0][periodo][1]; cor_adesivo_atual[0][2] = cor_adesivo[0][periodo][2];
	cor_adesivo_atual[1][0] = cor_adesivo[1][periodo][0]; cor_adesivo_atual[1][1] = cor_adesivo[1][periodo][1]; cor_adesivo_atual[1][2] = cor_adesivo[1][periodo][2];
	cor_adesivo_atual[2][0] = cor_adesivo[2][periodo][0]; cor_adesivo_atual[2][1] = cor_adesivo[2][periodo][1]; cor_adesivo_atual[2][2] = cor_adesivo[2][periodo][2];
	cor_adesivo_atual[3][0] = cor_adesivo[3][periodo][0]; cor_adesivo_atual[3][1] = cor_adesivo[3][periodo][1]; cor_adesivo_atual[3][2] = cor_adesivo[3][periodo][2];
	cor_adesivo_atual[4][0] = cor_adesivo[4][periodo][0]; cor_adesivo_atual[4][1] = cor_adesivo[4][periodo][1]; cor_adesivo_atual[4][2] = cor_adesivo[4][periodo][2];
	cor_adesivo_atual[5][0] = cor_adesivo[5][periodo][0]; cor_adesivo_atual[5][1] = cor_adesivo[5][periodo][1]; cor_adesivo_atual[5][2] = cor_adesivo[5][periodo][2];
	cor_adesivo_atual[6][0] = cor_adesivo[6][periodo][0]; cor_adesivo_atual[6][1] = cor_adesivo[6][periodo][1]; cor_adesivo_atual[6][2] = cor_adesivo[6][periodo][2];
	cor_adesivo_atual[7][0] = cor_adesivo[7][periodo][0]; cor_adesivo_atual[7][1] = cor_adesivo[7][periodo][1]; cor_adesivo_atual[7][2] = cor_adesivo[7][periodo][2];

	//Calcula de valores para evitar repeti��o de passos na cria��o da curva
	cos_ang = cos((90 * 3.14) / (numquads * 180));
	sen_ang = sin((90 * 3.14) / (numquads * 180));

	raio_curva = 2 * comprimento / 3.14; //Calculo para que a curva tenha o mesmo tamanho que a reta
	vel_ang = (vel * 180) / (raio_curva*3.14); //Calculo para que a velocidade na curva seja a mesma que na reta

	vel_inimigos = vel*1.2; //Inicializa a velocidade dos inimigos

							//Salvando os valores para pause/resume
	vel_p = vel;
	vel_ang_p = vel_ang;
	vel_inimigos_p = vel_inimigos;
	mov_p = mov;
	relogio_p = relogio;

	glutInit(&argc, argv); // Inicializa glut
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); /*Define as caracter�sticas do espa�o vetorial. Nesse caso, permite anima��es (sem cintila��es)| cores compostas por Verm. Verde e Azul|Buffer que permite trablhar com profundidade e elimina faces escondidas.*/

	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Enduro");
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);
	//glutMouseFunc(Mouse);
	glutKeyboardFunc(keyboard);
	//glutSpecialFunc(TeclasEspeciais);

	lastFrameTime = glutGet(GLUT_ELAPSED_TIME);
	glutMainLoop();
	return 0;
}

void Carro() {
	glPushMatrix();
	glRotatef(car_rot, 0, 1, 0);
	glTranslatef(pos_car_final, 0, 0);
	glRotatef(-90, 0, 1, 0);
	//glTranslatef(-5,0,0);
	glScalef(0.2, 0.2, 0.2);

	/**Tra�ado**/
	glColor3ub(cor_carro_atual[cor][0], cor_carro_atual[cor][1], cor_carro_atual[cor][2]);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 2, 0);
	glVertex3f(2, 2.25, 0);
	glVertex3f(4, 2.25, 0);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(4, 2.25, -2.5);
	glVertex3f(4, 2.25, 2.5);
	glEnd();

	//Portas
	glBegin(GL_LINE_STRIP);
	glVertex3f(4, 2.25, 2.5);
	glVertex3f(3.5, 1, 2.75);
	glVertex3f(3.75, 0.75, 2.75);
	glVertex3f(4.25, 0.5, 2.5);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(7, 3.25, 2);
	glVertex3f(7, 2, 2.5);
	glVertex3f(7, 1, 2.75);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(4, 2.25, -2.5);
	glVertex3f(3.5, 1, -2.75);
	glVertex3f(3.75, 0.75, -2.75);
	glVertex3f(4.25, 0.5, -2.5);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(7, 3.25, -2);
	glVertex3f(7, 2, -2.5);
	glVertex3f(7, 1, -2.75);
	glEnd();

	//Fim portas

	glBegin(GL_LINES);
	glVertex3f(5, 3.25, -2);
	glVertex3f(5, 3.25, 2);

	glVertex3f(8, 3.25, -2);
	glVertex3f(8, 3.25, 2);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(10, 2.25, -2.5);
	glVertex3f(8, 3.25, -2);
	glVertex3f(5, 3.25, -2);
	glVertex3f(4, 2.25, -2.5);
	glVertex3f(2, 2.5, -2.5);
	glVertex3f(0, 2, -2.5);
	glVertex3f(0, 2, 2.5);
	glVertex3f(2, 2.5, 2.5);
	glVertex3f(4, 2.25, 2.5);
	glVertex3f(5, 3.25, 2);
	glVertex3f(8, 3.25, 2);
	glVertex3f(10, 2.25, 2.5);

	glEnd();

	glColor3ub(200, 200, 200);
	glBegin(GL_LINE_LOOP);
	glVertex3f(9.75, 1, -2.75);
	glVertex3f(10, 2.25, -2.5);
	glVertex3f(10, 2.25, 2.5);
	glVertex3f(9.75, 1, 2.75);

	glEnd();

	/**Decalque**/
	glColor3ub(cor_adesivo_atual[cor][0], cor_adesivo_atual[cor][1], cor_adesivo_atual[cor][2]);
	glBegin(GL_QUADS);
	glVertex3f(0, 2.01, 0.2);
	glVertex3f(0, 2.01, 1.4);
	glVertex3f(2, 2.51, 1.4);
	glVertex3f(2, 2.51, 0.2);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(4, 2.26, 0.2);
	glVertex3f(4, 2.26, 1.4);
	glVertex3f(2, 2.51, 1.4);
	glVertex3f(2, 2.51, 0.2);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0, 2.01, -0.2);
	glVertex3f(0, 2.01, -1.4);
	glVertex3f(2, 2.51, -1.4);
	glVertex3f(2, 2.51, -0.2);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(4, 2.26, -0.2);
	glVertex3f(4, 2.26, -1.4);
	glVertex3f(2, 2.51, -1.4);
	glVertex3f(2, 2.51, -0.2);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(5, 3.26, 0.2);
	glVertex3f(5, 3.26, 1.4);
	glVertex3f(6, 3.51, 1.4);
	glVertex3f(6, 3.51, 0.2);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(8, 3.26, 0.2);
	glVertex3f(8, 3.26, 1.4);
	glVertex3f(6, 3.51, 1.4);
	glVertex3f(6, 3.51, 0.2);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(5, 3.26, -0.2);
	glVertex3f(5, 3.26, -1.4);
	glVertex3f(6, 3.51, -1.4);
	glVertex3f(6, 3.51, -0.2);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(8, 3.26, -0.2);
	glVertex3f(8, 3.26, -1.4);
	glVertex3f(6, 3.51, -1.4);
	glVertex3f(6, 3.51, -0.2);
	glEnd();

	/**Vidros**/
	glColor3ub(0, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f(3.99, 2.25, 2.4);
	glVertex3f(4.99, 3.25, 1.9);
	glVertex3f(4.99, 3.25, -1.9);
	glVertex3f(3.99, 2.25, -2.4);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(4.25, 2.25, 2.55);
	glVertex3f(5.1, 3.25, 2.05);
	glVertex3f(6.75, 3.25, 2.05);
	glVertex3f(6.75, 2.25, 2.55);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(8, 2.25, 2.55);
	glVertex3f(7.5, 3, 2.15);
	glVertex3f(7, 3.25, 2.05);
	glVertex3f(7, 2.25, 2.55);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(4.25, 2.25, -2.55);
	glVertex3f(5.1, 3.25, -2.05);
	glVertex3f(6.75, 3.25, -2.05);
	glVertex3f(6.75, 2.25, -2.55);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(8, 2.25, -2.55);
	glVertex3f(7.5, 3, -2.15);
	glVertex3f(7, 3.25, -2.05);
	glVertex3f(7, 2.25, -2.55);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(9.99, 2.26, 2.3);
	glVertex3f(8.01, 3.25, 1.8);
	glVertex3f(8.01, 3.25, -1.8);
	glVertex3f(9.99, 2.26, -2.3);
	glEnd();

	/**Placa**/
	glColor3ub(200, 200, 200);
	glBegin(GL_QUADS);
	glVertex3f(10.01, 2, -2);
	glVertex3f(10.01, 2, 2);
	glVertex3f(9.76, 1.5, 2);
	glVertex3f(9.76, 1.5, -2);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(9.9, 1.5, -1);
	glVertex3f(9.9, 1.5, 1);
	glVertex3f(9.76, 1.25, 1);
	glVertex3f(9.76, 1.25, -1);
	glEnd();


	/**Far�is**/
	glColor3ub(90, 20, 0);
	glBegin(GL_QUADS);
	glVertex3f(10.01, 2.25, 2.5);
	glVertex3f(10.01, 2.25, 1.75);
	glVertex3f(9.76, 1, 2);
	glVertex3f(9.76, 1, 2.75);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(10.01, 2.25, -2.5);
	glVertex3f(10.01, 2.25, -1.75);
	glVertex3f(9.76, 1, -2);
	glVertex3f(9.76, 1, -2.75);
	glEnd();


	glPushMatrix();
	glTranslatef(-0.01, 1.5, 2);
	glRotatef(90, 0, 1, 0);
	glColor3ub(230, 230, 230);
	gluDisk(gluNewQuadric(), 0.25, 0.3, 20, 20);
	glColor3ub(255, 255, 150);
	gluDisk(gluNewQuadric(), 0, 0.25, 20, 20);
	glTranslatef(0.5, 0, 0);
	glColor3ub(230, 230, 230);
	gluDisk(gluNewQuadric(), 0.1, 0.15, 20, 20);
	glColor3ub(255, 255, 150);
	gluDisk(gluNewQuadric(), 0, 0.1, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.01, 1.5, -2);
	glRotatef(90, 0, 1, 0);
	glColor3ub(230, 230, 230);
	gluDisk(gluNewQuadric(), 0.25, 0.3, 20, 20);
	glColor3ub(255, 255, 150);
	gluDisk(gluNewQuadric(), 0, 0.25, 20, 20);
	glTranslatef(-0.5, 0, 0);
	glColor3ub(230, 230, 230);
	gluDisk(gluNewQuadric(), 0.1, 0.15, 20, 20);
	glColor3ub(255, 255, 150);
	gluDisk(gluNewQuadric(), 0, 0.1, 20, 20);
	glPopMatrix();


	/**Base**/
	glColor3ub(cor_carro_atual[cor][0], cor_carro_atual[cor][1], cor_carro_atual[cor][2]);

	//Lado Direitod
	glBegin(GL_QUADS);
	glVertex3f(0, 0.75, 2.75);
	glVertex3f(1, 0.75, 2.75);
	glVertex3f(0.75, 0.5, 2.5);
	glVertex3f(0.25, 0.5, 2.5);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(6.25, 0.75, 2.75);
	glVertex3f(2.75, 0.75, 2.75);
	glVertex3f(3, 0.5, 2.5);
	glVertex3f(6, 0.5, 2.5);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(7.75, 0.75, 2.75);
	glVertex3f(9.5, 0.75, 2.75);
	glVertex3f(9, 0.5, 2.5);
	glVertex3f(8, 0.5, 2.5);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0, 0.75, 2.75);
	glVertex3f(0, 1, 2.75);
	glVertex3f(2, 1, 2.75);
	glVertex3f(1, 0.75, 2.75);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(2, 1, 2.75);
	glVertex3f(2.75, 0.75, 2.75);
	glVertex3f(6.25, 0.75, 2.75);
	glVertex3f(7, 1, 2.75);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(7.75, 0.75, 2.75);
	glVertex3f(9.5, 0.75, 2.75);
	glVertex3f(9.75, 1, 2.75);
	glVertex3f(7, 1, 2.75);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0, 1, 2.75);
	glVertex3f(0, 2, 2.5);
	glVertex3f(9.75, 2, 2.5);
	glVertex3f(9.75, 1, 2.75);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0, 2, 2.5);
	glVertex3f(2, 2.5, 2.5);
	glVertex3f(4, 2.25, 2.5);
	glVertex3f(4, 2, 2.5);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(9.75, 2, 2.5);
	glVertex3f(10, 2.25, 2.5);
	glVertex3f(4, 2.25, 2.5);
	glVertex3f(4, 2, 2.5);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(8, 3.25, 2);
	glVertex3f(5, 3.25, 2);
	glVertex3f(4, 2.25, 2.5);
	glVertex3f(10, 2.25, 2.5);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(5, 3.25, 2);
	glVertex3f(6, 3.5, 1);
	glVertex3f(8, 3.25, 2);
	glVertex3f(7, 3.25, 2);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex3f(10, 2.25, 2.5);
	glVertex3f(9.75, 2, 2.5);
	glVertex3f(9.75, 1, 2.75);
	glEnd();


	//Lado Esquerdo

	glBegin(GL_QUADS);
	glVertex3f(0, 0.75, -2.75);
	glVertex3f(1, 0.75, -2.75);
	glVertex3f(0.75, 0.5, -2.5);
	glVertex3f(0.25, 0.5, -2.5);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(6.25, 0.75, -2.75);
	glVertex3f(2.75, 0.75, -2.75);
	glVertex3f(3, 0.5, -2.5);
	glVertex3f(6, 0.5, -2.5);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(7.75, 0.75, -2.75);
	glVertex3f(9.5, 0.75, -2.75);
	glVertex3f(9, 0.5, -2.5);
	glVertex3f(8, 0.5, -2.5);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0, 0.75, -2.75);
	glVertex3f(0, 1, -2.75);
	glVertex3f(2, 1, -2.75);
	glVertex3f(1, 0.75, -2.75);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(2, 1, -2.75);
	glVertex3f(2.75, 0.75, -2.75);
	glVertex3f(6.25, 0.75, -2.75);
	glVertex3f(7, 1, -2.75);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(7.75, 0.75, -2.75);
	glVertex3f(9.5, 0.75, -2.75);
	glVertex3f(9.75, 1, -2.75);
	glVertex3f(7, 1, -2.75);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0, 1, -2.75);
	glVertex3f(0, 2, -2.5);
	glVertex3f(9.75, 2, -2.5);
	glVertex3f(9.75, 1, -2.75);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0, 2, -2.5);
	glVertex3f(2, 2.5, -2.5);
	glVertex3f(4, 2.25, -2.5);
	glVertex3f(4, 2, -2.5);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(9.75, 2, -2.5);
	glVertex3f(10, 2.25, -2.5);
	glVertex3f(4, 2.25, -2.5);
	glVertex3f(4, 2, -2.5);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(8, 3.25, -2);
	glVertex3f(5, 3.25, -2);
	glVertex3f(4, 2.25, -2.5);
	glVertex3f(10, 2.25, -2.5);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(5, 3.25, -2);
	glVertex3f(6, 3.5, -1);
	glVertex3f(8, 3.25, -2);
	glVertex3f(7, 3.25, -2);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex3f(10, 2.25, -2.5);
	glVertex3f(9.75, 2, -2.5);
	glVertex3f(9.75, 1, -2.75);
	glEnd();




	//Parabrisa
	glBegin(GL_QUADS);
	glVertex3f(4, 2.25, 2.5);
	glVertex3f(5, 3.25, 2);
	glVertex3f(5, 3.25, -2);
	glVertex3f(4, 2.25, -2.5);
	glEnd();


	//Teto
	glBegin(GL_QUADS);
	glVertex3f(6, 3.5, 1);
	glVertex3f(5, 3.25, 2);
	glVertex3f(5, 3.25, -2);
	glVertex3f(6, 3.5, -1);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(6, 3.5, 1);
	glVertex3f(8, 3.25, 2);
	glVertex3f(8, 3.25, -2);
	glVertex3f(6, 3.5, -1);
	glEnd();

	//Parabrisa Traseiro
	glBegin(GL_QUADS);
	glVertex3f(10, 2.25, 2.5);
	glVertex3f(8, 3.25, 2);
	glVertex3f(8, 3.25, -2);
	glVertex3f(10, 2.25, -2.5);
	glEnd();

	//Porta-Malas
	//glColor3ub(200,0,0);
	glBegin(GL_QUADS);
	glVertex3f(10, 2.25, 2.5);
	glVertex3f(9.75, 1, 2.75);
	glVertex3f(9.75, 1, -2.75);
	glVertex3f(10, 2.25, -2.5);
	glEnd();



	glBegin(GL_QUADS);
	glVertex3f(9.5, 0.75, 2.75);
	glVertex3f(9, 0.5, 2.5);
	glVertex3f(9, 0.5, -2.5);
	glVertex3f(9.5, 0.75, -2.75);
	glEnd();

	//Cap�
	glBegin(GL_QUADS);
	glVertex3f(4, 2.25, 2.5);
	glVertex3f(2, 2.5, 2.5);
	glVertex3f(2, 2.5, 0);
	glVertex3f(4, 2.25, 0);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0, 2, 2.5);
	glVertex3f(2, 2.5, 2.5);
	glVertex3f(2, 2.5, 0);
	glVertex3f(0, 2, 0);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(4, 2.25, -2.5);
	glVertex3f(2, 2.5, -2.5);
	glVertex3f(2, 2.5, 0);
	glVertex3f(4, 2.25, 0);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0, 2, -2.5);
	glVertex3f(2, 2.5, -2.5);
	glVertex3f(2, 2.5, 0);
	glVertex3f(0, 2, 0);
	glEnd();

	//Para-choque
	glBegin(GL_QUADS);
	glVertex3f(0, 2, -2.5);
	glVertex3f(0, 1, -2.75);
	glVertex3f(0, 1, 2.75);
	glVertex3f(0, 2, 2.5);
	glEnd();

	glColor3ub(230, 230, 230);
	glBegin(GL_QUADS);
	glVertex3f(0, 0.75, -2.75);
	glVertex3f(0, 1, -2.75);
	glVertex3f(0, 1, 2.75);
	glVertex3f(0, 0.75, 2.75);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0, 0.75, -2.75);
	glVertex3f(0.25, 0.5, -2.5);
	glVertex3f(0.25, 0.5, 2.5);
	glVertex3f(0, 0.75, 2.75);
	glEnd();

	//Paralama
	glColor3ub(220, 220, 220);
	glBegin(GL_QUADS);
	glVertex3f(9.5, 0.75, 2.75);
	glVertex3f(9.75, 1, 2.75);
	glVertex3f(9.75, 1, -2.75);
	glVertex3f(9.5, 0.75, -2.75);
	glEnd();

	//Pneus
	glColor3ub(10, 10, 10);
	glPushMatrix();
	glTranslatef(2, 1, 1.5);
	gluCylinder(gluNewQuadric(), 1, 1, 1, 20, 20);//Pneu
	glTranslatef(0, 0, 1);
	gluDisk(gluNewQuadric(), 0.5, 1, 20, 20);//Pneu
	glColor3ub(200, 200, 200);
	gluDisk(gluNewQuadric(), 0, 0.5, 20, 20);//Calota
	glPopMatrix();

	glColor3ub(10, 10, 10);
	glPushMatrix();
	glTranslatef(2, 1, -2.5);
	gluCylinder(gluNewQuadric(), 1, 1, 1, 20, 20);//Pneu
	gluDisk(gluNewQuadric(), 0.5, 1, 20, 20);//Pneu
	glColor3ub(200, 200, 200);
	gluDisk(gluNewQuadric(), 0, 0.5, 20, 20);//Calota
	glPopMatrix();

	glColor3ub(10, 10, 10);
	glPushMatrix();
	glTranslatef(7, 1, 1.5);
	gluCylinder(gluNewQuadric(), 1, 1, 1, 20, 20);//Pneu
	glTranslatef(0, 0, 1);
	gluDisk(gluNewQuadric(), 0.5, 1, 20, 20);//Pneu
	glColor3ub(200, 200, 200);
	gluDisk(gluNewQuadric(), 0, 0.5, 20, 20);//Calota
	glPopMatrix();

	glColor3ub(10, 10, 10);
	glPushMatrix();
	glTranslatef(7, 1, -2.5);
	gluCylinder(gluNewQuadric(), 1, 1, 1, 20, 20);//Pneu
	gluDisk(gluNewQuadric(), 0.5, 1, 20, 20);//Pneu
	glColor3ub(200, 200, 200);
	gluDisk(gluNewQuadric(), 0, 0.5, 20, 20);//Calota
	glPopMatrix();

	glPopMatrix();
}

void Inimigo(int cor_i) {
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glScalef(0.2, 0.2, 0.2);

	/**Tra�ado**/
	glColor3ub(cor_carro_atual[cor_i][0], cor_carro_atual[cor_i][1], cor_carro_atual[cor_i][2]);

	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 2, 0);
	glVertex3f(2, 2.25, 0);
	glVertex3f(4, 2.25, 0);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(4, 2.25, -2.5);
	glVertex3f(4, 2.25, 2.5);
	glEnd();

	//Portas
	glBegin(GL_LINE_STRIP);
	glVertex3f(4, 2.25, 2.5);
	glVertex3f(3.5, 1, 2.75);
	glVertex3f(3.75, 0.75, 2.75);
	glVertex3f(4.25, 0.5, 2.5);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(7, 3.25, 2);
	glVertex3f(7, 2, 2.5);
	glVertex3f(7, 1, 2.75);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(4, 2.25, -2.5);
	glVertex3f(3.5, 1, -2.75);
	glVertex3f(3.75, 0.75, -2.75);
	glVertex3f(4.25, 0.5, -2.5);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(7, 3.25, -2);
	glVertex3f(7, 2, -2.5);
	glVertex3f(7, 1, -2.75);
	glEnd();

	//Fim portas

	glBegin(GL_LINES);
	glVertex3f(5, 3.25, -2);
	glVertex3f(5, 3.25, 2);

	glVertex3f(8, 3.25, -2);
	glVertex3f(8, 3.25, 2);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(10, 2.25, -2.5);
	glVertex3f(8, 3.25, -2);
	glVertex3f(5, 3.25, -2);
	glVertex3f(4, 2.25, -2.5);
	glVertex3f(2, 2.5, -2.5);
	glVertex3f(0, 2, -2.5);
	glVertex3f(0, 2, 2.5);
	glVertex3f(2, 2.5, 2.5);
	glVertex3f(4, 2.25, 2.5);
	glVertex3f(5, 3.25, 2);
	glVertex3f(8, 3.25, 2);
	glVertex3f(10, 2.25, 2.5);

	glEnd();

	glColor3ub(200, 200, 200);
	glBegin(GL_LINE_LOOP);
	glVertex3f(9.75, 1, -2.75);
	glVertex3f(10, 2.25, -2.5);
	glVertex3f(10, 2.25, 2.5);
	glVertex3f(9.75, 1, 2.75);

	glEnd();

	/**Decalque**/
	glColor3ub(cor_adesivo_atual[cor_i][0], cor_adesivo_atual[cor_i][1], cor_adesivo_atual[cor_i][2]);
	glBegin(GL_QUADS);
	glVertex3f(0, 2.01, 0.2);
	glVertex3f(0, 2.01, 1.4);
	glVertex3f(2, 2.51, 1.4);
	glVertex3f(2, 2.51, 0.2);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(4, 2.26, 0.2);
	glVertex3f(4, 2.26, 1.4);
	glVertex3f(2, 2.51, 1.4);
	glVertex3f(2, 2.51, 0.2);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0, 2.01, -0.2);
	glVertex3f(0, 2.01, -1.4);
	glVertex3f(2, 2.51, -1.4);
	glVertex3f(2, 2.51, -0.2);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(4, 2.26, -0.2);
	glVertex3f(4, 2.26, -1.4);
	glVertex3f(2, 2.51, -1.4);
	glVertex3f(2, 2.51, -0.2);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(5, 3.26, 0.2);
	glVertex3f(5, 3.26, 1.4);
	glVertex3f(6, 3.51, 1.4);
	glVertex3f(6, 3.51, 0.2);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(8, 3.26, 0.2);
	glVertex3f(8, 3.26, 1.4);
	glVertex3f(6, 3.51, 1.4);
	glVertex3f(6, 3.51, 0.2);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(5, 3.26, -0.2);
	glVertex3f(5, 3.26, -1.4);
	glVertex3f(6, 3.51, -1.4);
	glVertex3f(6, 3.51, -0.2);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(8, 3.26, -0.2);
	glVertex3f(8, 3.26, -1.4);
	glVertex3f(6, 3.51, -1.4);
	glVertex3f(6, 3.51, -0.2);
	glEnd();

	/**Vidros**/
	glColor3ub(0, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f(3.99, 2.25, 2.4);
	glVertex3f(4.99, 3.25, 1.9);
	glVertex3f(4.99, 3.25, -1.9);
	glVertex3f(3.99, 2.25, -2.4);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(4.25, 2.25, 2.55);
	glVertex3f(5.1, 3.25, 2.05);
	glVertex3f(6.75, 3.25, 2.05);
	glVertex3f(6.75, 2.25, 2.55);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(8, 2.25, 2.55);
	glVertex3f(7.5, 3, 2.15);
	glVertex3f(7, 3.25, 2.05);
	glVertex3f(7, 2.25, 2.55);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(4.25, 2.25, -2.55);
	glVertex3f(5.1, 3.25, -2.05);
	glVertex3f(6.75, 3.25, -2.05);
	glVertex3f(6.75, 2.25, -2.55);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(8, 2.25, -2.55);
	glVertex3f(7.5, 3, -2.15);
	glVertex3f(7, 3.25, -2.05);
	glVertex3f(7, 2.25, -2.55);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(9.99, 2.26, 2.3);
	glVertex3f(8.01, 3.25, 1.8);
	glVertex3f(8.01, 3.25, -1.8);
	glVertex3f(9.99, 2.26, -2.3);
	glEnd();

	/**Placa**/
	glColor3ub(200, 200, 200);
	glBegin(GL_QUADS);
	glVertex3f(10.01, 2, -2);
	glVertex3f(10.01, 2, 2);
	glVertex3f(9.76, 1.5, 2);
	glVertex3f(9.76, 1.5, -2);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(9.9, 1.5, -1);
	glVertex3f(9.9, 1.5, 1);
	glVertex3f(9.76, 1.25, 1);
	glVertex3f(9.76, 1.25, -1);
	glEnd();


	/**Far�is**/
	glColor3ub(90, 20, 0);
	glBegin(GL_QUADS);
	glVertex3f(10.01, 2.25, 2.5);
	glVertex3f(10.01, 2.25, 1.75);
	glVertex3f(9.76, 1, 2);
	glVertex3f(9.76, 1, 2.75);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(10.01, 2.25, -2.5);
	glVertex3f(10.01, 2.25, -1.75);
	glVertex3f(9.76, 1, -2);
	glVertex3f(9.76, 1, -2.75);
	glEnd();


	glPushMatrix();
	glTranslatef(-0.01, 1.5, 2);
	glRotatef(90, 0, 1, 0);
	glColor3ub(230, 230, 230);
	gluDisk(gluNewQuadric(), 0.25, 0.3, 20, 20);
	glColor3ub(255, 255, 150);
	gluDisk(gluNewQuadric(), 0, 0.25, 20, 20);
	glTranslatef(0.5, 0, 0);
	glColor3ub(230, 230, 230);
	gluDisk(gluNewQuadric(), 0.1, 0.15, 20, 20);
	glColor3ub(255, 255, 150);
	gluDisk(gluNewQuadric(), 0, 0.1, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.01, 1.5, -2);
	glRotatef(90, 0, 1, 0);
	glColor3ub(230, 230, 230);
	gluDisk(gluNewQuadric(), 0.25, 0.3, 20, 20);
	glColor3ub(255, 255, 150);
	gluDisk(gluNewQuadric(), 0, 0.25, 20, 20);
	glTranslatef(-0.5, 0, 0);
	glColor3ub(230, 230, 230);
	gluDisk(gluNewQuadric(), 0.1, 0.15, 20, 20);
	glColor3ub(255, 255, 150);
	gluDisk(gluNewQuadric(), 0, 0.1, 20, 20);
	glPopMatrix();


	/**Base**/
	glColor3ub(cor_carro_atual[cor_i][0], cor_carro_atual[cor_i][1], cor_carro_atual[cor_i][2]);

	//Lado Direitod
	glBegin(GL_QUADS);
	glVertex3f(0, 0.75, 2.75);
	glVertex3f(1, 0.75, 2.75);
	glVertex3f(0.75, 0.5, 2.5);
	glVertex3f(0.25, 0.5, 2.5);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(6.25, 0.75, 2.75);
	glVertex3f(2.75, 0.75, 2.75);
	glVertex3f(3, 0.5, 2.5);
	glVertex3f(6, 0.5, 2.5);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(7.75, 0.75, 2.75);
	glVertex3f(9.5, 0.75, 2.75);
	glVertex3f(9, 0.5, 2.5);
	glVertex3f(8, 0.5, 2.5);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0, 0.75, 2.75);
	glVertex3f(0, 1, 2.75);
	glVertex3f(2, 1, 2.75);
	glVertex3f(1, 0.75, 2.75);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(2, 1, 2.75);
	glVertex3f(2.75, 0.75, 2.75);
	glVertex3f(6.25, 0.75, 2.75);
	glVertex3f(7, 1, 2.75);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(7.75, 0.75, 2.75);
	glVertex3f(9.5, 0.75, 2.75);
	glVertex3f(9.75, 1, 2.75);
	glVertex3f(7, 1, 2.75);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0, 1, 2.75);
	glVertex3f(0, 2, 2.5);
	glVertex3f(9.75, 2, 2.5);
	glVertex3f(9.75, 1, 2.75);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0, 2, 2.5);
	glVertex3f(2, 2.5, 2.5);
	glVertex3f(4, 2.25, 2.5);
	glVertex3f(4, 2, 2.5);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(9.75, 2, 2.5);
	glVertex3f(10, 2.25, 2.5);
	glVertex3f(4, 2.25, 2.5);
	glVertex3f(4, 2, 2.5);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(8, 3.25, 2);
	glVertex3f(5, 3.25, 2);
	glVertex3f(4, 2.25, 2.5);
	glVertex3f(10, 2.25, 2.5);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(5, 3.25, 2);
	glVertex3f(6, 3.5, 1);
	glVertex3f(8, 3.25, 2);
	glVertex3f(7, 3.25, 2);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex3f(10, 2.25, 2.5);
	glVertex3f(9.75, 2, 2.5);
	glVertex3f(9.75, 1, 2.75);
	glEnd();


	//Lado Esquerdo

	glBegin(GL_QUADS);
	glVertex3f(0, 0.75, -2.75);
	glVertex3f(1, 0.75, -2.75);
	glVertex3f(0.75, 0.5, -2.5);
	glVertex3f(0.25, 0.5, -2.5);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(6.25, 0.75, -2.75);
	glVertex3f(2.75, 0.75, -2.75);
	glVertex3f(3, 0.5, -2.5);
	glVertex3f(6, 0.5, -2.5);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(7.75, 0.75, -2.75);
	glVertex3f(9.5, 0.75, -2.75);
	glVertex3f(9, 0.5, -2.5);
	glVertex3f(8, 0.5, -2.5);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0, 0.75, -2.75);
	glVertex3f(0, 1, -2.75);
	glVertex3f(2, 1, -2.75);
	glVertex3f(1, 0.75, -2.75);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(2, 1, -2.75);
	glVertex3f(2.75, 0.75, -2.75);
	glVertex3f(6.25, 0.75, -2.75);
	glVertex3f(7, 1, -2.75);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(7.75, 0.75, -2.75);
	glVertex3f(9.5, 0.75, -2.75);
	glVertex3f(9.75, 1, -2.75);
	glVertex3f(7, 1, -2.75);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0, 1, -2.75);
	glVertex3f(0, 2, -2.5);
	glVertex3f(9.75, 2, -2.5);
	glVertex3f(9.75, 1, -2.75);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0, 2, -2.5);
	glVertex3f(2, 2.5, -2.5);
	glVertex3f(4, 2.25, -2.5);
	glVertex3f(4, 2, -2.5);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(9.75, 2, -2.5);
	glVertex3f(10, 2.25, -2.5);
	glVertex3f(4, 2.25, -2.5);
	glVertex3f(4, 2, -2.5);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(8, 3.25, -2);
	glVertex3f(5, 3.25, -2);
	glVertex3f(4, 2.25, -2.5);
	glVertex3f(10, 2.25, -2.5);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(5, 3.25, -2);
	glVertex3f(6, 3.5, -1);
	glVertex3f(8, 3.25, -2);
	glVertex3f(7, 3.25, -2);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex3f(10, 2.25, -2.5);
	glVertex3f(9.75, 2, -2.5);
	glVertex3f(9.75, 1, -2.75);
	glEnd();




	//Parabrisa
	glBegin(GL_QUADS);
	glVertex3f(4, 2.25, 2.5);
	glVertex3f(5, 3.25, 2);
	glVertex3f(5, 3.25, -2);
	glVertex3f(4, 2.25, -2.5);
	glEnd();


	//Teto
	glBegin(GL_QUADS);
	glVertex3f(6, 3.5, 1);
	glVertex3f(5, 3.25, 2);
	glVertex3f(5, 3.25, -2);
	glVertex3f(6, 3.5, -1);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(6, 3.5, 1);
	glVertex3f(8, 3.25, 2);
	glVertex3f(8, 3.25, -2);
	glVertex3f(6, 3.5, -1);
	glEnd();

	//Parabrisa Traseiro
	glBegin(GL_QUADS);
	glVertex3f(10, 2.25, 2.5);
	glVertex3f(8, 3.25, 2);
	glVertex3f(8, 3.25, -2);
	glVertex3f(10, 2.25, -2.5);
	glEnd();

	//Porta-Malas
	//glColor3ub(200,0,0);
	glBegin(GL_QUADS);
	glVertex3f(10, 2.25, 2.5);
	glVertex3f(9.75, 1, 2.75);
	glVertex3f(9.75, 1, -2.75);
	glVertex3f(10, 2.25, -2.5);
	glEnd();



	glBegin(GL_QUADS);
	glVertex3f(9.5, 0.75, 2.75);
	glVertex3f(9, 0.5, 2.5);
	glVertex3f(9, 0.5, -2.5);
	glVertex3f(9.5, 0.75, -2.75);
	glEnd();

	//Cap�
	glBegin(GL_QUADS);
	glVertex3f(4, 2.25, 2.5);
	glVertex3f(2, 2.5, 2.5);
	glVertex3f(2, 2.5, 0);
	glVertex3f(4, 2.25, 0);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0, 2, 2.5);
	glVertex3f(2, 2.5, 2.5);
	glVertex3f(2, 2.5, 0);
	glVertex3f(0, 2, 0);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(4, 2.25, -2.5);
	glVertex3f(2, 2.5, -2.5);
	glVertex3f(2, 2.5, 0);
	glVertex3f(4, 2.25, 0);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0, 2, -2.5);
	glVertex3f(2, 2.5, -2.5);
	glVertex3f(2, 2.5, 0);
	glVertex3f(0, 2, 0);
	glEnd();

	//Para-choque
	glBegin(GL_QUADS);
	glVertex3f(0, 2, -2.5);
	glVertex3f(0, 1, -2.75);
	glVertex3f(0, 1, 2.75);
	glVertex3f(0, 2, 2.5);
	glEnd();

	glColor3ub(230, 230, 230);
	glBegin(GL_QUADS);
	glVertex3f(0, 0.75, -2.75);
	glVertex3f(0, 1, -2.75);
	glVertex3f(0, 1, 2.75);
	glVertex3f(0, 0.75, 2.75);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0, 0.75, -2.75);
	glVertex3f(0.25, 0.5, -2.5);
	glVertex3f(0.25, 0.5, 2.5);
	glVertex3f(0, 0.75, 2.75);
	glEnd();

	//Paralama
	glColor3ub(220, 220, 220);
	glBegin(GL_QUADS);
	glVertex3f(9.5, 0.75, 2.75);
	glVertex3f(9.75, 1, 2.75);
	glVertex3f(9.75, 1, -2.75);
	glVertex3f(9.5, 0.75, -2.75);
	glEnd();

	//Pneus
	glColor3ub(10, 10, 10);
	glPushMatrix();
	glTranslatef(2, 1, 1.5);
	gluCylinder(gluNewQuadric(), 1, 1, 1, 20, 20);//Pneu
	glTranslatef(0, 0, 1);
	gluDisk(gluNewQuadric(), 0.5, 1, 20, 20);//Pneu
	glColor3ub(200, 200, 200);
	gluDisk(gluNewQuadric(), 0, 0.5, 20, 20);//Calota
	glPopMatrix();

	glColor3ub(10, 10, 10);
	glPushMatrix();
	glTranslatef(2, 1, -2.5);
	gluCylinder(gluNewQuadric(), 1, 1, 1, 20, 20);//Pneu
	gluDisk(gluNewQuadric(), 0.5, 1, 20, 20);//Pneu
	glColor3ub(200, 200, 200);
	gluDisk(gluNewQuadric(), 0, 0.5, 20, 20);//Calota
	glPopMatrix();

	glColor3ub(10, 10, 10);
	glPushMatrix();
	glTranslatef(7, 1, 1.5);
	gluCylinder(gluNewQuadric(), 1, 1, 1, 20, 20);//Pneu
	glTranslatef(0, 0, 1);
	gluDisk(gluNewQuadric(), 0.5, 1, 20, 20);//Pneu
	glColor3ub(200, 200, 200);
	gluDisk(gluNewQuadric(), 0, 0.5, 20, 20);//Calota
	glPopMatrix();

	glColor3ub(10, 10, 10);
	glPushMatrix();
	glTranslatef(7, 1, -2.5);
	gluCylinder(gluNewQuadric(), 1, 1, 1, 20, 20);//Pneu
	gluDisk(gluNewQuadric(), 0.5, 1, 20, 20);//Pneu
	glColor3ub(200, 200, 200);
	gluDisk(gluNewQuadric(), 0, 0.5, 20, 20);//Calota
	glPopMatrix();

	glPopMatrix();
}

void Skyline0() {
	glColor3ub(cor_predios_atual[0][0], cor_predios_atual[0][1], cor_predios_atual[0][2]);

	glBegin(GL_POLYGON);
	glVertex3f(0, 6.5, 0);
	glVertex3f(1, 6.5, 0);
	glVertex3f(1, 6, 0);
	glVertex3f(2, 6, 0);
	glVertex3f(2, 5.5, 0);
	glVertex3f(4, 5.5, 0);
	glVertex3f(4, 0, 0);
	glVertex3f(0, 0, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(4, 0, 0);
	glVertex3f(4, 7, 0);
	glVertex3f(5, 8.5, 0);
	glVertex3f(7, 9.5, 0);
	glVertex3f(9, 10, 0);
	glVertex3f(9, 0, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(9, 0, 0);
	glVertex3f(9, 3.5, 0);
	glVertex3f(10, 3.5, 0);
	glVertex3f(10, 4, 0);
	glVertex3f(13, 4, 0);
	glVertex3f(13, 3.5, 0);
	glVertex3f(15, 3.5, 0);
	glVertex3f(15, 2.5, 0);
	glVertex3f(15.5, 2.5, 0);
	glVertex3f(15.5, 2, 0);
	glVertex3f(16, 2, 0);
	glVertex3f(16, 0, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(16, 0, 0);
	glVertex3f(16, 9.5, 0);
	glVertex3f(21, 9.5, 0);
	glVertex3f(21, 7, 0);
	glVertex3f(24, 7, 0);
	glVertex3f(24, 6.5, 0);
	glVertex3f(25, 6.5, 0);
	glVertex3f(25, 0, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(27, 0, 0);
	glVertex3f(27, 1, 0);
	glVertex3f(28, 1, 0);
	glVertex3f(28, 6, 0);
	glVertex3f(32, 6, 0);
	glVertex3f(32, 5.5, 0);
	glVertex3f(33, 5.5, 0);
	glVertex3f(34, 4.5, 0);
	glVertex3f(34, 1, 0);
	glVertex3f(36, 1, 0);
	glVertex3f(36, 0, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(36, 0, 0);
	glVertex3f(36, 8, 0);
	glVertex3f(38, 9, 0);
	glVertex3f(39, 9, 0);
	glVertex3f(39, 10, 0);
	glVertex3f(42, 10, 0);
	glVertex3f(42, 7, 0);
	glVertex3f(43, 7, 0);
	glVertex3f(43, 5.5, 0);
	glVertex3f(45, 5.5, 0);
	glVertex3f(45, 0, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(45, 0, 0);
	glVertex3f(45, 8.5, 0);
	glVertex3f(46, 8.5, 0);
	glVertex3f(46, 10.5, 0);
	glVertex3f(51, 10.5, 0);
	glVertex3f(51, 6, 0);
	glVertex3f(53, 6, 0);
	glVertex3f(53, 5.5, 0);
	glVertex3f(54, 5.5, 0);
	glVertex3f(54, 0, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(54, 0, 0);
	glVertex3f(54, 7, 0);
	glVertex3f(57, 7, 0);
	glVertex3f(57, 6, 0);
	glVertex3f(58, 6, 0);
	glVertex3f(58, 0, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(61.9, 12, 0);
	glVertex3f(61.9, 14, 0);
	glVertex3f(62.1, 14, 0);
	glVertex3f(62.1, 12, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(58, 0, 0);
	glVertex3f(58, 10, 0);
	glVertex3f(59, 11, 0);
	glVertex3f(61, 12, 0);
	glVertex3f(63, 12, 0);
	glVertex3f(65, 11, 0);
	glVertex3f(66, 10, 0);
	glVertex3f(66, 0, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(66, 0, 0);
	glVertex3f(66, 13, 0);
	glVertex3f(70, 13, 0);
	glVertex3f(70, 11.5, 0);
	glVertex3f(72, 11.5, 0);
	glVertex3f(72, 4.5, 0);
	glVertex3f(73, 4.5, 0);
	glVertex3f(73, 0, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(73, 0, 0);
	glVertex3f(73, 6, 0);
	glVertex3f(74, 6, 0);
	glVertex3f(74, 8, 0);
	glVertex3f(76, 8, 0);
	glVertex3f(76, 6, 0);
	glVertex3f(77, 6, 0);
	glVertex3f(77, 0, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(77, 0, 0);
	glVertex3f(77, 7.5, 0);
	glVertex3f(79, 7.5, 0);
	glVertex3f(79, 8, 0);
	glVertex3f(81, 8, 0);
	glVertex3f(81, 7.5, 0);
	glVertex3f(81, 0, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(83, 0, 0);
	glVertex3f(83, 4, 0);
	glVertex3f(85, 4, 0);
	glVertex3f(85, 5.5, 0);
	glVertex3f(86, 6, 0);
	glVertex3f(87, 5.5, 0);
	glVertex3f(88, 7.5, 0);
	glVertex3f(88, 0, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(88, 0, 0);
	glVertex3f(88, 7, 0);
	glVertex3f(89, 7, 0);
	glVertex3f(89, 8, 0);
	glVertex3f(90, 9, 0);
	glVertex3f(91, 8, 0);
	glVertex3f(91, 7, 0);
	glVertex3f(92, 7, 0);
	glVertex3f(94, 7, 0);
	glVertex3f(94, 0, 0);
	glEnd();
}

void Skyline1() {
	glColor3ub(cor_predios_atual[1][0], cor_predios_atual[1][1], cor_predios_atual[1][2]);

	glBegin(GL_POLYGON);
	glVertex3f(0, 7, 0);
	glVertex3f(2, 7, 0);
	glVertex3f(2, 6.5, 0);
	glVertex3f(3, 6.5, 0);
	glVertex3f(3, 0, 0);
	glVertex3f(0, 0, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(32, 0, 0);
	glVertex3f(32, 13.5, 0);
	glVertex3f(33, 14, 0);
	glVertex3f(35, 14, 0);
	glVertex3f(36, 13.5, 0);
	glVertex3f(36, 13, 0);
	glVertex3f(37.5, 12.5, 0);
	glVertex3f(38, 12.5, 0);
	glVertex3f(38, 0, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(38, 0, 0);
	glVertex3f(38, 14, 0);
	glVertex3f(39, 15.5, 0);
	glVertex3f(40, 16, 0);
	glVertex3f(41, 16, 0);
	glVertex3f(42, 15.5, 0);
	glVertex3f(43, 14, 0);
	glVertex3f(43, 4, 0);
	glVertex3f(46, 4, 0);
	glVertex3f(46, 0, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(46, 0, 0);
	glVertex3f(46, 12, 0);
	glVertex3f(51, 12, 0);
	glVertex3f(51, 0, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(79, 0, 0);
	glVertex3f(79, 19.5, 0);
	glVertex3f(84, 19.5, 0);
	glVertex3f(84, 5, 0);
	glVertex3f(93, 5, 0);
	glVertex3f(93, 0, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(93, 0, 0);
	glVertex3f(93, 10.5, 0);
	glVertex3f(94, 12, 0);
	glVertex3f(95, 12, 0);
	glVertex3f(96, 10.5, 0);
	glVertex3f(96, 7.5, 0);
	glVertex3f(100, 7.5, 0);
	glVertex3f(100, 0, 0);
	glEnd();
}

void Skyline2() {
	glColor3ub(cor_predios_atual[2][0], cor_predios_atual[2][1], cor_predios_atual[2][2]);

	glBegin(GL_POLYGON);
	glVertex3f(3, 0, 0);
	glVertex3f(3, 8.5, 0);
	glVertex3f(7, 8.5, 0);
	glVertex3f(7, 0, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(10, 0, 0);
	glVertex3f(10, 7.5, 0);
	glVertex3f(12, 7.5, 0);
	glVertex3f(12, 11, 0);
	glVertex3f(14, 11, 0);
	glVertex3f(14, 10, 0);
	glVertex3f(15, 10, 0);
	glVertex3f(15, 0, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(18, 0, 0);
	glVertex3f(18, 11.5, 0);
	glVertex3f(19, 12, 0);
	glVertex3f(19, 15, 0);
	glVertex3f(25, 15, 0);
	glVertex3f(25, 0, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(28, 0, 0);
	glVertex3f(28, 15, 0);
	glVertex3f(29, 15, 0);
	glVertex3f(29, 15.5, 0);
	glVertex3f(31, 15.5, 0);
	glVertex3f(31, 13, 0);
	glVertex3f(32, 13, 0);
	glVertex3f(32, 0, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(35, 0, 0);
	glVertex3f(35, 20, 0);
	glVertex3f(37, 20, 0);
	glVertex3f(37, 0, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(48, 0, 0);
	glVertex3f(48, 14.5, 0);
	glVertex3f(53, 14.5, 0);
	glVertex3f(53, 0, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(55.5, 0, 0);
	glVertex3f(55.5, 12, 0);
	glVertex3f(56, 12, 0);
	glVertex3f(56, 15, 0);
	glVertex3f(59, 15, 0);
	glVertex3f(59, 13.5, 0);
	glVertex3f(59.5, 13.5, 0);
	glVertex3f(59.5, 0, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(58, 0, 0);
	glVertex3f(58, 19.5, 0);
	glVertex3f(59, 19.5, 0);
	glVertex3f(59, 20, 0);
	glVertex3f(62, 20, 0);
	glVertex3f(62, 19.5, 0);
	glVertex3f(63, 19.5, 0);
	glVertex3f(63, 0, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(67, 0, 0);
	glVertex3f(67, 14, 0);
	glVertex3f(70, 14, 0);
	glVertex3f(70, 11, 0);
	glVertex3f(71, 11, 0);
	glVertex3f(71, 0, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(73, 0, 0);
	glVertex3f(73, 11, 0);
	glVertex3f(77, 11, 0);
	glVertex3f(77, 0, 0);
	glEnd();
}
