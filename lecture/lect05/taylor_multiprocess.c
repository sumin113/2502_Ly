#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>

#define N 4
#define TERMS 3

/* 테일러 급수 계산 함수 */
double sinx_taylor(double x, int terms) {
    double value = x;
    double numer = x * x * x;
    double denom = 6.0; // 3!
    int sign = -1;

    for (int j = 1; j <= terms; j++) {
        value += sign * numer / denom;
        numer *= x * x;
        denom *= (2.0 * j + 2) * (2.0 * j + 3);
        sign *= -1;
    }
    return value;
}

int main() {
    double x[N] = {0, M_PI / 6., M_PI / 3., 0.134};
    double result[N];
    int fd[N][2];

    /* 자식마다 파이프 생성 */
    for (int i = 0; i < N; i++) {
        if (pipe(fd[i]) == -1) {
            perror("pipe");
            exit(1);
        }
    }

    /* 자식 프로세스를 생성하고 병렬으로 계산 */
    for (int i = 0; i < N; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            exit(1);
        }

        if (pid == 0) { /* 자식 프로세스 */
            close(fd[i][0]); /* 읽기 닫기 */
	    srand(getpid());
	    sleep(rand() % 3 + 1); /* 병렬 진행 확인하기 위해 잠시 멈춤 */

            double res = sinx_taylor(x[i], TERMS);
            write(fd[i][1], &res, sizeof(double)); /* 부모프로세스에 결과 전송 */
            close(fd[i][1]);

            printf("[Child %d] sin(%.3f) 계산 완료\n", getpid(), x[i]);
            exit(0);
        } else {
            close(fd[i][1]); /* 쓰기 닫기 */
        }
    }

    /* 부모 프로세스는 자식 프로세스 완전 종료까지 대기 */
    for (int i = 0; i < N; i++) {
        wait(NULL);
    }

    /* 부모 프로세스:는 파이프에서  계산 결과 받는다. */
    for (int i = 0; i < N; i++) {
        read(fd[i][0], &result[i], sizeof(double));
        close(fd[i][0]);
    }

    /* 최종결과 출력 */
    printf("\nlect05 계산 결과:\n");
    for (int i = 0; i < N; i++) {
        printf("sin(%.3f) by Taylor series = %.6f\n", x[i], result[i]);
        printf("sin(%.3f) = %.6f\n\n", x[i], sin(x[i]));
    }

    return 0;
}

