//--------------------------------------------------------------
// Proyecto 1
//--------------------------------------------------------------
// Ecuación unidimensional de disipación de calor
//--------------------------------------------------------------
// Integrantes:
// Paula Camila Gonzalez Ortega, 18398
// Diana Ximena de León Figueroa, 18607
// Maria Ines Vasquez Figueroa, 18250
// gcc -o paralelo paralelo.c -lm -fopenmp
// ./paralelo


#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <omp.h>

#define c 1e-5
#define L 10
#define C 0.5

int thread_count;

double calcularTj(double TjAnt, double TjAct, double TjSig) {
	return TjAct + C * (TjAnt - 2 * TjAct + TjSig);
}

void parHeatDisipation(double *data, double *dataCopy, int N, double err, int id, int count) {
	double errCalculado = 1e-25;
	double sum;
	int k, n = 0;
	
	int chunkSize = (int) (N / count * 1.0);
	printf("N = %d count = %d chunkSize = %d \n", N, count, chunkSize);
	int inicio, final; 
	inicio = (id != 1) ? (id -1) * chunkSize : 1;
	final = (id != count) ? id * chunkSize : N - 1;
	
	printf("id = %d inicio = %d fin = %d \n", id, inicio, final);
	
	// TODO: Validar el calculo del err
    while (n < 100000 && errCalculado < err) {
    	// Calcular las temperaturas
    	for(int j = inicio; j < final; j++) {
    		dataCopy[j] = calcularTj(data[j - 1], data[j], data[j + 1]);
		}

		// Actualizar array de solucion y calcular el err actual
		sum = 0.0;
		for (k = 1; k < N - 1; k++) {
			sum += dataCopy[k] - dataCopy[k - 1];
			data[k] = dataCopy[k];
		}
		
		errCalculado = sum / N;
		n += 1;
    }
}


int main(int argc, char *argv[]) {
	thread_count = strtol(argv[1], NULL, 10);
    double err, T0, TL, TR, dx, dt;
    int N;
    double *temperatura, *temperaturaCopy;

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
    
    //---- Asignación de memoria para el vector temperatura ----
    if ( (temperatura = (double *)malloc(N * sizeof(double))) == NULL )
    perror("memory allocation for temperatura");

	//---- Asignación de memoria para el vector temperaturaCopy ----
    if ( (temperaturaCopy = (double *)malloc(N * sizeof(double))) == NULL )
    perror("memory allocation for temperaturaCopy");
    
    // Calcular dx, dt
    // dx = (L * 1.0) / N;
    // dt = pow(dx, 2) / c;
    // dt = 0.5 * pow(dx, 2) / c;
    
    // Empezar a medir tiempo
    struct timeval begin, end;
    gettimeofday(&begin, 0);
    
    // Asignar los valores iniciales
    temperatura[0] = TL;
    temperatura[N - 1] = TR;
    
    temperaturaCopy[0] = TL;
    temperaturaCopy[N - 1] = TR;
    
    // Inicializar array
    int i;
    #pragma omp parallel for private(i)\
    schedule(guided, 8)
    for (i = 1; i < N - 1; i++) {
    	temperatura[i] = T0;
    }
    
    int t, count;
    count = thread_count;
    //private(t, N, err, count)\
    //shared(temperatura, temperaturaCopy)
    // Opcion 1 (solo esta linea)
    #pragma omp parallel for schedule(static, 1) num_threads(thread_count)
    // Opcion 2
    //#pragma omp parallel
    //#pragma omp single nowait
	for (t = 1; t <= thread_count; t++) {
		// Opcion 2 tambien
		// #pragma omp task
		parHeatDisipation(temperatura, temperaturaCopy, N, err, t, count);
	}

    // Mostrar resultados
    for (i = 0; i < N; i++) {
		printf("%lf \t", temperatura[i]);
	}

    free(temperatura);
    free(temperaturaCopy);
    
    // Calcular el tiempo transcurrido
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = seconds + microseconds * 1e-6;
    printf("Time: %.3f segundos.\n", elapsed);
}

