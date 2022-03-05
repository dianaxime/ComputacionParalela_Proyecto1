//--------------------------------------------------------------
// Proyecto 1 - Paralela UVG
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
#define C 0.5

double calcularTj(double TjAnt, double TjAct, double TjSig){
	double Tj = TjAct + C * (TjAnt - 2 * TjAct + TjSig);
	
	return Tj;
}


int main(int argc, char *argv[]) {
    double err, T0, TL, TR, dx, dt, sum;
    int N, n = 0;
    double errCalculado = 1e-25;

    printf("--- Ingrese los parámetros para iniciar la simulación --- \n");
    printf("Precisión o diferencia requerida: ");
    scanf("%lf", &err);
    printf("Número de intervalos discretos: ");
    scanf("%d", &N);
    printf("Temperatura inicial de toda la barra: ");
    scanf("%lf", &T0);
    printf("Temperatura en la frontera izquierda (x=0): ");
    scanf("%lf", &TL);
    printf("Temperatura en la frontera derecha (x=L): ");
    scanf("%lf", &TR);
    
    double temperatura[N], temperaturaCopy[N];
    
    // Calcular dx, dt
    // dx = (L * 1.0) / N;
    // dt = pow(dx, 2) / c;
    // dt = 0.5 * pow(dx, 2) / c;
    
    // Se comienza a medir el tiempo
    struct timeval begin, end;
    gettimeofday(&begin, 0);
    
    // Se asignan los valores iniciales
    temperatura[0] = TL;
    temperatura[N - 1] = TR;
    
    temperaturaCopy[0] = TL;
    temperaturaCopy[N - 1] = TR;
    
    // Se inicializa el array
    #pragma omp parallel for private(i)\
    schedule(guided,10)
    for (int i = 1; i < N - 1; i++) {
    	temperatura[i] = T0;
    }
    
    // Validacion del calculo del err
    while (n < 100000 && errCalculado < err) {
    	// Calcular las temperaturas
    	for(int j = 1; j < N - 1; j++) {
			temperaturaCopy[j] = calcularTj(temperatura[j - 1], temperatura[j], temperatura[j + 1]);
			//printf("%lf \t", temperaturaCopy[j]);
		}
		//printf("%d \n", n);
		// Se actualiza el array de solucion y el err actual
		sum = 0.0;
		#pragma omp parallel for private(k)\
    	schedule(guided,10)
		for (int k = 1; k < N - 1; k++) {
			sum += temperaturaCopy[k] - temperaturaCopy[k - 1];
			//printf("%lf \t", (temperaturaCopy[k] - temperaturaCopy[k - 1]));
			temperatura[k] = temperaturaCopy[k];
		}
		
		errCalculado = sum / N;
		n += 1;
    }
    // Se muestran los resultados al user
    for (int i = 0; i < N; i++) {
		printf("%lf \t", temperatura[i]);
	}

    printf("\nError calculado: %lf \n", errCalculado);
    
    // Calcular el tiempo transcurrido en ejecucion
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = seconds + microseconds * 1e-6;
    printf("Time: %.3f segundos.\n", elapsed);
}

