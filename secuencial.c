//--------------------------------------------------------------
// Proyecto 1
//--------------------------------------------------------------
// Ecuación unidimensional de disipación de calor
//--------------------------------------------------------------
// Integrantes:
// Paula Camila Gonzalez Ortega, 18398
// Diana Ximena de León Figueroa, 18607
// Maria Ines Vasquez Figueroa, 18250

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

#define c 1e-5
#define L 10

int main(int argc, char *argv[]) {
    double err, T0, TL, TR, dx, dt, errCalculado;
    int N;

    printf("--- Ingrese los parámetros para iniciar la simulación --- \n");
    printf("Precisión o diferencia requerida: ");
    scanf("%lf", &err);
    printf("Número de intervalos discretos: ");
    scanf("%d", &N);
    printf("Temperatura inicial de toda la barra:");
    scanf("%lf", &T0);
    printf("Temperatura en la frontera izquierda (x=0): ");
    scanf("%lf", &TL);
    printf("Temperatura en la frontera derecha (x=L):");
    scanf("%lf", &TR);
    
    double temperatura[N], temperaturaCopy[N];
    
    // Calcular dx, dt
    dx = (L * 1.0) / N;
    // dt = pow(dx, 2) / c;
    dt = 0.5 * pow(dx, 2) / c;
    
    // Asignar los valores iniciales
    temperatura[0] = TL;
    temperatura[N - 1] = TR;
    
    temperaturaCopy[0] = TL;
    temperaturaCopy[N - 1] = TR;
    
    for (int i = 1; i < N - 1; i++) {
    	temperatura[i] = T0;
    }
}


