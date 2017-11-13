//
// Created by pedrug19 on 13/11/17.
//

#ifndef TRABCG_ESTRUTURAS_H
#define TRABCG_ESTRUTURAS_H
#endif //TRABCG_ESTRUTURAS_H

/*DEFININDO PLATAFORMAS OPERACIONAIS*/
#ifdef _WIN32
    #include <GL/glut.h>
    #include <GL/glu.h>
    #include <GL/gl.h>
#endif

#ifdef _WIN64
    #include <GL/glut.h>
    #include <GL/glu.h>
    #include <GL/gl.h>
#endif

#ifdef __linux__
    #include <GL/freeglut.h>
    #include <unistd.h>
    #include <ncurses.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>