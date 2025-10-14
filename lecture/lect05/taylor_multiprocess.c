#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>

#define N 4
#define TERMS 3

/* 테일러 급수를 하나씩 계산한다. */
double sinx_taylor(double x, int terms) {
    double value = x;
    double numer = x*x*x;
    double denom = 6.0; // 3!
    int sign = -1;

    for(int j=1; j<=terms; j++) {
        value += sign * numer / denom;
        numer *= x*x;
        denom *= (2.0*j+2)*(2.0*j+3);
        sign *= -1;
    }
    return value;
}

int main() {
    double x[N] = {0, M_PI/6., M_PI/3., 0.134};
    double result[N];
    int fd[N][2];

    /* 파이프를 생성한다. */
    for(int i=0; i<N; i++) {
        if(pipe(fd[i]) == -1) {
            perror("pipe");
            exit(1);
        }
    }

    /* 자식 프로세스를 생성한다. */
    for(int i=0; i<N; i++) {
        pid_t pid = fork();
        if(pid < 0) {
            perror("fork");
            exit(1);
        }
        if(pid == 0) {
            close(fd[i][0]);
            double res = sinx_taylor(x[i], TERMS);
            write(fd[i][1], &res, sizeof(double));	/* 부모 프로세스에 결과를 전달한다. */
            close(fd[i][1]);
            exit(0);
        } else {
            close(fd[i][1]);
        }
    }

    /* 부모 프로세스는 자식 프로세스가 모두 종료될 때까지 대기한다. */
    for(int i=0; i<N; i++) {
        wait(NULL);
    }

    /* 부모 프로세스가 파이프에서 결과를 받아온다. */
    for(int i=0; i<N; i++) {
        read(fd[i][0], &result[i], sizeof(double));
        close(fd[i][0]);
    }

    /* 결과를 출력한다. */
    for(int i=0; i<N; i++) {
        printf("sin(%.3f) by Taylor series = %.6f\n", x[i], result[i]);
        printf("sin(%.3f) = %.6f\n", x[i], sin(x[i]));
    }

    return 0;
}

