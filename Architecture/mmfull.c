/*
 * Square matrix multiplication
 * A * B = C
 *
 * Make sure you provide PAPI header file location and lib location. 
 * If you use system installed PAPI, which is normally located
 * in /usr, you should compile with:
 *    gcc mm.c -o mm -I/usr/include -L/usr/lib/x86_64-linux-gnu -lpapi
 * 
 * If you use own installed PAPI in a path pointed to env PAPI_ROOT, 
 * you should set LD_LIBRARY_PATH env to include your PAPI lib path:
 *     export LD_LIBRARY_PATH=${PAPI_ROOT}/lib:$LD_LIBRARY_PATH 
 * then compile with:
 * gcc mm.c -o mm -I${PAPI_ROOT}/include -L${PAPI_ROOT}/lib -lpapi 
 *
 *  To execute
 *  ./mm 1024 (or other number)
 *
 */


//LIANG ZHAO 
//HOMEWORK 3

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/timeb.h>
#include <string.h>
#include <papi.h>

/* read timer in second */
double read_timer() {
    struct timeb tm;
    ftime(&tm);
    return (double) tm.time + (double) tm.millitm / 1000.0;
}

/* read timer in ms */
double read_timer_ms() {
    struct timeb tm;
    ftime(&tm);
    return (double) tm.time * 1000.0 + (double) tm.millitm;
}

#define REAL float
int N = 2048;
int bsize = 32;

#define NUM_PAPI_EVENTS 4 
int PAPI_EventSet = PAPI_NULL;

void init(int N, REAL *A) {
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            A[i*N+j] = (REAL) drand48();
        }
    }
}

void mm_ijk(int N, REAL * A, REAL * B, REAL *C);
void mm_jik(int N, REAL * A, REAL * B, REAL *C);
void mm_kij(int N, REAL * A, REAL * B, REAL *C);
void mm_ikj(int N, REAL * A, REAL * B, REAL *C);
void mm_jki(int N, REAL * A, REAL * B, REAL *C);
void mm_kji(int N, REAL * A, REAL * B, REAL *C);
void mm_ijk_blocking(int N, REAL * A, REAL * B, REAL * C, int bsize);
void mm_cb(int N, REAL * A, REAL * B, REAL * C, int bsize); /* cache oblivious algorithms */

int main(int argc, char *argv[]) {
    double elapsed_ijk;
    double elapsed_jik;
    double elapsed_kij;
    double elapsed_ikj;
    double elapsed_jki;
    double elapsed_kji;
    double elapsed_ijk_blocking;
    double elapsed_cb;

    if (argc < 2) {
        fprintf(stderr, "Usage: mm <N> <bsize>, default N: %d, bsize: %d\n", N, bsize);
    } else if (argc == 2) { 
    	N = atoi(argv[1]);
    } else if (argc == 3) {
    	bsize = atoi(argv[2]);
    }

    REAL * A = (REAL*)malloc(sizeof(REAL)*N*N);
    REAL * B = (REAL*)malloc(sizeof(REAL)*N*N);
    REAL * C = (REAL*)malloc(sizeof(REAL)*N*N);

    srand48((1 << 12));
    init(N, A);
    init(N, B);

    /* initialize PAPI event for collecting cache misses */
    PAPI_library_init(PAPI_VER_CURRENT);
    PAPI_create_eventset(&PAPI_EventSet);
    PAPI_add_event( PAPI_EventSet, PAPI_LST_INS); /* total load/store instruction completed */
    PAPI_add_event( PAPI_EventSet, PAPI_L1_DCM);  /* L1 D-cache misses */
    PAPI_add_event( PAPI_EventSet, PAPI_L2_DCM);  /* L2 D-cache misses */
    PAPI_add_event( PAPI_EventSet, PAPI_L3_DCM);  /* L3 D-cache misses */

    /* L1 miss rate: L1_DCM/LST_INS 
     * L2 miss rate: L2_DCM/L1_DCM since L1 misses cause L2 access 
     * L3 miss rate: L3_DCM/L2_DCM since L2 misses cause L3 access
     */
    PAPI_start(PAPI_EventSet);

    /* execution and measurement */
    /* for mm_ijk */
    __builtin___clear_cache(A, A+N*N); /* flush cache so we have cold start */
    __builtin___clear_cache(B, B+N*N); 
    __builtin___clear_cache(C, C+N*N); 
    long long PAPI_Values_ijk[NUM_PAPI_EVENTS];
    elapsed_ijk = read_timer();
    PAPI_reset(PAPI_EventSet);
    mm_ijk(N, A, B, C);
    PAPI_read(PAPI_EventSet, PAPI_Values_ijk);
    elapsed_ijk = (read_timer() - elapsed_ijk);
   
    /* for mm_jik */ 
    __builtin___clear_cache(A, A+N*N); /* flush cache so we have cold start */
    __builtin___clear_cache(B, B+N*N); 
    __builtin___clear_cache(C, C+N*N); 
    long long PAPI_Values_jik[NUM_PAPI_EVENTS];
    elapsed_jik = read_timer();
    PAPI_reset(PAPI_EventSet);
    mm_jik(N, A, B, C);
    PAPI_read(PAPI_EventSet, PAPI_Values_jik);
    elapsed_jik = (read_timer() - elapsed_jik);
	
	/* for mm_kij */ 
    __builtin___clear_cache(A, A+N*N); /* flush cache so we have cold start */
    __builtin___clear_cache(B, B+N*N); 
    __builtin___clear_cache(C, C+N*N); 
    long long PAPI_Values_kij[NUM_PAPI_EVENTS];
    elapsed_kij = read_timer();
    PAPI_reset(PAPI_EventSet);
    mm_kij(N, A, B, C);
    PAPI_read(PAPI_EventSet, PAPI_Values_kij);
    elapsed_kij = (read_timer() - elapsed_kij);
	
	/* for mm_ikj */ 
    __builtin___clear_cache(A, A+N*N); /* flush cache so we have cold start */
    __builtin___clear_cache(B, B+N*N); 
    __builtin___clear_cache(C, C+N*N); 
    long long PAPI_Values_ikj[NUM_PAPI_EVENTS];
    elapsed_ikj = read_timer();
    PAPI_reset(PAPI_EventSet);
    mm_ikj(N, A, B, C);
    PAPI_read(PAPI_EventSet, PAPI_Values_ikj);
    elapsed_ikj = (read_timer() - elapsed_ikj);
	
	/* for mm_jki */ 
    __builtin___clear_cache(A, A+N*N); /* flush cache so we have cold start */
    __builtin___clear_cache(B, B+N*N); 
    __builtin___clear_cache(C, C+N*N); 
    long long PAPI_Values_jki[NUM_PAPI_EVENTS];
    elapsed_jki = read_timer();
    PAPI_reset(PAPI_EventSet);
    mm_jki(N, A, B, C);
    PAPI_read(PAPI_EventSet, PAPI_Values_jki);
    elapsed_jki = (read_timer() - elapsed_jki);
	
	/* for mm_kji */ 
    __builtin___clear_cache(A, A+N*N); /* flush cache so we have cold start */
    __builtin___clear_cache(B, B+N*N); 
    __builtin___clear_cache(C, C+N*N); 
    long long PAPI_Values_kji[NUM_PAPI_EVENTS];
    elapsed_kji = read_timer();
    PAPI_reset(PAPI_EventSet);
    mm_kji(N, A, B, C);
    PAPI_read(PAPI_EventSet, PAPI_Values_kji);
    elapsed_kji = (read_timer() - elapsed_kji);
	
	/* for mm_ijk_blocking */ 
    __builtin___clear_cache(A, A+N*N); /* flush cache so we have cold start */
    __builtin___clear_cache(B, B+N*N); 
    __builtin___clear_cache(C, C+N*N); 
    long long PAPI_Values_ijk_blocking[NUM_PAPI_EVENTS];
    elapsed_ijk_blocking = read_timer();
    PAPI_reset(PAPI_EventSet);
    mm_ijk_blocking(N, A, B, C, bsize);
    PAPI_read(PAPI_EventSet, PAPI_Values_ijk_blocking);
    elapsed_ijk_blocking = (read_timer() - elapsed_ijk_blocking);
	
	/* for mm_cb  */
    __builtin___clear_cache(A, A+N*N); /* flush cache so we have cold start*/ 
    __builtin___clear_cache(B, B+N*N); 
    __builtin___clear_cache(C, C+N*N); 
    long long PAPI_Values_cb[NUM_PAPI_EVENTS];
    elapsed_cb = read_timer();
    PAPI_reset(PAPI_EventSet);
    mm_cb(N, A, B, C, bsize);
    PAPI_read(PAPI_EventSet, PAPI_Values_cb);
    elapsed_cb = (read_timer() - elapsed_cb);
	
	
    
    printf("======================================================================================================\n");
    printf("\tMatrix Multiplication: A[%d][%d] * B[%d][%d] = C[%d][%d], bsize: %d\n", N, N, N, N, N, N, bsize);
    printf("------------------------------------------------------------------------------------------------------\n");
    printf("Performance:\t\tRuntime(ms)\tMFLOPS\t\tL1_DMissRate\tL2_DMissRate\tL3_DMissRate\n");
    printf("------------------------------------------------------------------------------------------------------\n");
    printf("mm_ijk:\t\t\t%4f\t%4f\t\t%.2f\t\t%.2f\t\t%.2f\n", elapsed_ijk * 1.0e3, ((((2.0 * N) * N) * N) / (1.0e6 * elapsed_ijk)), (double)PAPI_Values_ijk[1]/(double)PAPI_Values_ijk[0], (double)PAPI_Values_ijk[2]/(double)PAPI_Values_ijk[1], (double)PAPI_Values_ijk[3]/(double)PAPI_Values_ijk[2]);
    printf("mm_jik:\t\t\t%4f\t%4f\t\t%.2f\t\t%.2f\t\t%.2f\n", elapsed_jik * 1.0e3, ((((2.0 * N) * N) * N) / (1.0e6 * elapsed_jik)), (double)PAPI_Values_jik[1]/(double)PAPI_Values_jik[0], (double)PAPI_Values_jik[2]/(double)PAPI_Values_jik[1], (double)PAPI_Values_jik[3]/(double)PAPI_Values_jik[2]);
    printf("mm_kij:\t\t\t%4f\t%4f\t\t%.2f\t\t%.2f\t\t%.2f\n", elapsed_kij * 1.0e3, ((((2.0 * N) * N) * N) / (1.0e6 * elapsed_kij)), (double)PAPI_Values_kij[1]/(double)PAPI_Values_kij[0], (double)PAPI_Values_kij[2]/(double)PAPI_Values_kij[1], (double)PAPI_Values_kij[3]/(double)PAPI_Values_kij[2]);
    printf("mm_ikj:\t\t\t%4f\t%4f\t\t%.2f\t\t%.2f\t\t%.2f\n", elapsed_ikj * 1.0e3, ((((2.0 * N) * N) * N) / (1.0e6 * elapsed_ikj)), (double)PAPI_Values_ikj[1]/(double)PAPI_Values_ikj[0], (double)PAPI_Values_ikj[2]/(double)PAPI_Values_ikj[1], (double)PAPI_Values_ikj[3]/(double)PAPI_Values_ikj[2]);
    printf("mm_jki:\t\t\t%4f\t%4f\t\t%.2f\t\t%.2f\t\t%.2f\n", elapsed_jki * 1.0e3, ((((2.0 * N) * N) * N) / (1.0e6 * elapsed_jki)), (double)PAPI_Values_jki[1]/(double)PAPI_Values_jki[0], (double)PAPI_Values_jki[2]/(double)PAPI_Values_jki[1], (double)PAPI_Values_jki[3]/(double)PAPI_Values_jki[2]);
    printf("mm_kji:\t\t\t%4f\t%4f\t\t%.2f\t\t%.2f\t\t%.2f\n", elapsed_kji * 1.0e3, ((((2.0 * N) * N) * N) / (1.0e6 * elapsed_kji)), (double)PAPI_Values_kji[1]/(double)PAPI_Values_kji[0], (double)PAPI_Values_kji[2]/(double)PAPI_Values_kji[1], (double)PAPI_Values_kji[3]/(double)PAPI_Values_kji[2]);
	printf("mm_ijk_blocking:\t%4f\t%4f\t\t%.2f\t\t%.2f\t\t%.2f\n", elapsed_ijk_blocking * 1.0e3, ((((2.0 * N) * N) * N) / (1.0e6 * elapsed_ijk_blocking)), (double)PAPI_Values_ijk_blocking[1]/(double)PAPI_Values_ijk_blocking[0], (double)PAPI_Values_ijk_blocking[2]/(double)PAPI_Values_ijk_blocking[1], (double)PAPI_Values_ijk_blocking[3]/(double)PAPI_Values_ijk_blocking[2]);
    printf("mm_cb:\t\t\t%4f\t%4f\t\t%.2f\t\t%.2f\t\t%.2f\n", elapsed_cb * 1.0e3, ((((2.0 * N) * N) * N) / (1.0e6 * elapsed_cb)), (double)PAPI_Values_cb[1]/(double)PAPI_Values_cb[0], (double)PAPI_Values_cb[2]/(double)PAPI_Values_cb[1], (double)PAPI_Values_cb[3]/(double)PAPI_Values_cb[2]);
    
	
    PAPI_stop(PAPI_EventSet, NULL);
    free(A); free(B); free(C);
}

void mm_ijk(int N, REAL *A, REAL *B, REAL *C) {
    int i, j, k;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
	    REAL sum = 0.0;
            for (k = 0; k < N; k++)
                sum += A[i*N+k]*B[k*N+j];
            C[i*N+j] = sum;
        }
    }
}


void mm_jik(int N, REAL * A, REAL * B, REAL *C) {
	int j,i,k;
	for (j=0;j<N;j++){
		for (i=0;i<N;i++){
			REAL sum=0.0;
			for (k=0; k<N;k++)
				sum += A[i*N+k]*B[k*N+j];
			C[i*N+j]= sum;
		}
	}
}

void mm_kij(int N, REAL * A, REAL * B, REAL *C) {
	int k,i,j;
	for (k=0;k<N;k++){
		for (i=0;i<N;i++){
			REAL r=A[i*N+k];
			for (j=0; j<N;j++)
			C[i*N+j] += r*B[k*N+j];
		}
	}
}

void mm_ikj(int N, REAL * A, REAL * B, REAL *C) {
	int i,k,j;
	for (i=0;i<N;i++){
		for (k=0;k<N;k++){
			REAL r=A[i*N+k];
			for (j=0; j<N;j++)
			C[i*N+j] += r*B[k*N+j];
		}
	}
}

void mm_jki(int N, REAL * A, REAL * B, REAL *C) {
	int j,k,i;
	for (j=0;j<N;j++){
		for (k=0;k<N;k++){
			REAL r=B[k*N+j];
			for (i=0; i<N;i++)
			C[i*N+j] += A[i*N+k]*r;
		}
	}
}

void mm_kji(int N, REAL * A, REAL * B, REAL *C) {
	int k,j,i;
	for (k=0;k<N;k++){
		for (j=0;j<N;j++){
			REAL r=B[k*N+j];
			for (i=0; i<N;i++)
			C[i*N+j] += A[i*N+k]*r;
		}
	}
}


int min(int a, int b){
	if (a<b)
		return a;
	else return b;
}


void mm_ijk_blocking(int N, REAL * A, REAL * B, REAL * C, int bsize) {
	int jj, kk, i, j, k;
	for (jj=0;jj<N;jj=jj+bsize){
		for (kk=0;kk<N;kk=kk+bsize){
            for (i = 0; i < N; i=i+1) {
				for (j = jj; j < min(jj+bsize-1, N); j=j+1) {
	              REAL r = 0.0;
                  for (k = kk; k < min(kk+bsize-1, N); k=k+1){
				  r += A[i*N+k]*B[k*N+j];}
                      C[i*N+j] += r;
				    
				}
			}
        }
    }
}




void add_matmul_rec(REAL *A, REAL *B, REAL *C, int m, int n, int p, int fdA, int fdB, int fdC) {
	if (m+n+p<=48){
		int i,j,k;
		for (i=0;i<m;++i)
			for(k=0;k<p;++k){
				REAL sum=0;
				for (j=0;j<n;++j)
					sum += A[i*fdA+j]*B[j*fdB+k];
				C[i*fdC+k] += sum;
			}
        }
	else{				
    int m2=m/2, n2=n/2, p2=p/2;
	add_matmul_rec(A, B, C, m2, n2, p2, fdA, fdB, fdC);
	add_matmul_rec(A+n2, B+n2*fdB, C, m2, n-n2, p2, fdA, fdB, fdC);
	
	add_matmul_rec(A, B+p2, C+p2, m2, n2, p-p2, fdA, fdB, fdC);
	add_matmul_rec(A+n2, B+p2+n2*fdB, C, m2, n-n2, p-p2, fdA, fdB, fdC);
	
	add_matmul_rec(A+m2*fdA, B, C+m2*fdC, m-m2, n2, p2, fdA, fdB, fdC);
	add_matmul_rec(A+m2*fdA+n2, B+n2*fdB, C+m2*fdC, m-m2, n-n2, p2, fdA, fdB, fdC);
	
	add_matmul_rec(A+m2*fdA, B+p2, C+m2*fdC+p2, m-m2, n2, p-p2, fdA, fdB, fdC);
	add_matmul_rec(A+m2*fdA+n2, B+p2+n2*fdB, C+m2*fdC, m-m2, n-n2, p-p2, fdA, fdB, fdC);
	}
}

void mm_cb(int N, REAL * A, REAL * B, REAL * C, int bsize ){
	memset(C, 0, sizeof(REAL)*N*N);
	add_matmul_rec(A, B, C, N, N, N, N, N, N);
}


