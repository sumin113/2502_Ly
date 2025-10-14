#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>

#define N 4      // 자식 프로세스 개수 (x 배열 길이)
#define TERMS 3  // 테일러 급수 항 개수

// 테일러 급수로 sin(x) 계산
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
    int fd[N][2]; // 각 자식별 pipe

    // 1. 파이프 생성
    for(int i=0; i<N; i++) {
        if(pipe(fd[i]) == -1) {
            perror("pipe");
            exit(1);
        }
    }

    // 2. 자식 프로세스 생성
    for(int i=0; i<N; i++) {
        pid_t pid = fork();
        if(pid < 0) {
            perror("fork");
            exit(1);
        }
        if(pid == 0) { // 자식 프로세스
            close(fd[i][0]); // 읽기용 닫기
            double res = sinx_taylor(x[i], TERMS);
            write(fd[i][1], &res, sizeof(double)); // 결과 부모에 전달
            close(fd[i][1]);
            exit(0);
        } else { // 부모 프로세스
            close(fd[i][1]); // 쓰기용 닫기
        }
    }

    // 3. 부모: 모든 자식 종료 대기
    for(int i=0; i<N; i++) {
        wait(NULL);
    }

    // 4. 부모: 파이프에서 결과 읽기
    for(int i=0; i<N; i++) {
        read(fd[i][0], &result[i], sizeof(double));
        close(fd[i][0]);
    }

    // 5. 결과 출력
    for(int i=0; i<N; i++) {
        printf("sin(%.3f) by Taylor series = %.6f\n", x[i], result[i]);
        printf("sin(%.3f) = %.6f\n", x[i], sin(x[i]));
    }

    return 0;
}

