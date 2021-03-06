#pragma once
#include <assert.h>
#include <complex.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef short sample_t;
ssize_t read_n(int fd, ssize_t n, void* buf);
ssize_t write_n(int fd, ssize_t n, void* buf);
void sample_to_complex(sample_t * s, complex double * X, long n);
void complex_to_sample(complex double * X, sample_t * s, long n);
void fft_r(complex double * x, complex double * y, long n, complex double w);
void fft(complex double * x, complex double * y, long n);
void ifft(complex double * y, complex double * x, long n);
void bandpass(complex double * y, complex double * x, long n, long fmin, long fmax);
void change_frequency(complex double* y, complex double* x, long n, int slide);
void print_complex(FILE* wp, complex double * Y, long n);
int pow2check(long N);
int touch_sound(int s, int n0, sample_t* data, long fmin, long fmax, int slide);

