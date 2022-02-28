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

int main(int argc, char *argv[]) {
    double err, T0, TL, TR;
    int N;

    printf("--- Ingrese los parámetros para iniciar la simulación ---");
    scanf("Precisión o diferencia requerida: %f", &err);
    scanf("Número de intervalos discretos: %d", &N);
    scanf("Temperatura inicial de toda la barra: %f", &T0);
    scanf("Temperatura en la frontera izquierda (x=0): %f", &TL);
    scanf("Temperatura en la frontera derecha: %f", &TR);
}