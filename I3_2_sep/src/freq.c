#include <assert.h>
#include <complex.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "./../include/phonelib.h"
#include "./../include/freqlib.h"

ssize_t read_n(int fd, ssize_t n, void * buf) {
	ssize_t re = 0;
  	while (re < n) {
    	ssize_t r = read(fd, buf + re, n - re);
    	if (r == -1) die("read");
    	if (r == 0) break;
    	re += r;
	}
	memset(buf + re, 0, n - re);
	return re;
}

ssize_t write_n(int fd, ssize_t n, void * buf) {
	ssize_t wr = 0;
  	while (wr < n) {
    	ssize_t w = write(fd, buf + wr, n - wr);
    	if (w == -1) die("write");
    	wr += w;
  	}
  	return wr;
}

void sample_to_complex(sample_t * s, 
		       complex double * X, 
		       long n) {
  	long i;
  	for (i = 0; i < n; i++) X[i] = s[i];
}

void complex_to_sample(complex double * X, 
		       sample_t * s, 
		       long n) {
  	long i;
  	for (i = 0; i < n; i++) {
    	s[i] = creal(X[i]);
  	}
}

void fft_r(complex double * x, 
	   complex double * y, 
	   long n, 
	   complex double w) {
  	if (n == 1) { y[0] = x[0]; }
  	else {
    	complex double W = 1.0; 
    	long i;
    	for (i = 0; i < n/2; i++) {
      		y[i]     =     (x[i] + x[i+n/2]); /* 偶数行 */
      		y[i+n/2] = W * (x[i] - x[i+n/2]); /* 奇数行 */
      		W *= w;
    	}
    	fft_r(y,     x,     n/2, w * w);
    	fft_r(y+n/2, x+n/2, n/2, w * w);
    	for (i = 0; i < n/2; i++) {
      		y[2*i]   = x[i];
      		y[2*i+1] = x[i+n/2];
    	}
  	}
}

void fft(complex double * x, 
	 complex double * y, 
	 long n) {
  	long i;
  	double arg = 2.0 * M_PI / n;
  	complex double w = cos(arg) - 1.0j * sin(arg);
  	fft_r(x, y, n, w);
  	for (i = 0; i < n; i++) y[i] /= n;
}

void ifft(complex double * y, 
	  complex double * x, 
	  long n) {
  	double arg = 2.0 * M_PI / n;
  	complex double w = cos(arg) + 1.0j * sin(arg);
  	fft_r(y, x, n, w);
}

void bandpass(complex double* y, complex double* x, long n, long fmin, long fmax){
  double f0 = 44100;
  double T = n/f0;
  for(long i=0 ; i<n/2 ; i++){
    double f = i/T;
    if(abs(f)<fmin || fmax<abs(f)){
      y[i] = 0.0;
    }
  }
  for(long i=n/2 ; i<n ; i++){
    double f = (n-i)/T;
    if(abs(f)<fmin || fmax<abs(f)){
      y[i] = 0.0;
    }
  }
}


void change_frequency(complex double* y, complex double* x, long n, int slide){
	//slide分だけ周波数大きい方向へずらす
	//ここどうするかわからん
	if(slide>0){
		for(int i=n/2 ; i>=0 ; i--){
			if(i+slide>=n/2){
				continue;
			}
			y[i+slide] = y[i];
		}
		for(int i=n/2 ; i<n ; i++){
			if(i+slide>=n){
				continue;
			}
			y[i] = y[i+slide];
		}
	}else{
		slide *= -1;
		for(int i=0 ; i<n ; i++){
			if(i+slide>=n){
				continue;
			}
			y[i] = y[i+slide];
		}
	}
}

void print_complex(FILE * wp, 
		   complex double * Y, long n) {
  long i;
  for (i = 0; i < n; i++) {
    fprintf(wp, "%ld %f %f %f %f\n", 
	    i, 
	    creal(Y[i]), cimag(Y[i]),
	    cabs(Y[i]), atan2(cimag(Y[i]), creal(Y[i])));
  }
}


int pow2check(long N) {
  	long n = N;
  	while (n > 1) {
    	if (n % 2) return 0;
    	n = n / 2;
  	}
  	return 1;
}

int touch_sound(int s, int n0, sample_t* data, long fmin, long fmax, int slide){
    long n = n0;
    if(!pow2check(n)){
        fprintf(stderr, "error : in (%ld) not a power of two\n", n);
        exit(1);
    }
    if(n==0){
        return 0;
    }
    sample_t * buf = calloc(sizeof(sample_t), n);
    complex double* X = calloc(sizeof(complex double),n);
    complex double* Y = calloc(sizeof(complex double),n);
    buf = data;
    sample_to_complex(buf, X,n);
    fft(X,Y,n);
    bandpass(Y,X,n, fmin,fmax);
    //change_frequency(Y,X,n,slide);
    ifft(Y,X,n);
    send(s, buf, n*sizeof(short), 0);
    return n;
}


