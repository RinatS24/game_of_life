#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 80
#define M 25

int input(int** field);
int count_alive_cells(int** field);
void print_field(int** field);
int speed(char up_down_speed, int time);
void update_field(int** field1, int** field2);
int life_cell(int neighbors, int cell);
int count_neighbors(int** field, int x, int y);
void swap(int** field1, int** field2);

int main() {
    int** field1 = malloc(M * sizeof(int*));
    for (int i = 0; i < M; i++) field1[i] = malloc(N * sizeof(int));

    int** field2 = malloc(M * sizeof(int*));
    for (int i = 0; i < M; i++) field2[i] = malloc(N * sizeof(int));

    if (input(field1) != 0) {
        printf("Wrong input\n");
    } else {
        if (freopen("/dev/tty", "r", stdin)) initscr();
        noecho();
        curs_set(FALSE);
        nodelay(stdscr, TRUE);

        while (count_alive_cells(field1)) {
            int time = 200;
            char up_down_speed = getch();

            time = speed(up_down_speed, time);

            usleep(time * 1000);

            update_field(field1, field2);

            print_field(field2);
            swap(field1, field2);
        }
        for (int i = 0; i < M; i++) free(field1[i]);
        free(field1);

        for (int i = 0; i < M; i++) free(field2[i]);
        free(field2);
        endwin();
    }
    return 0;
}

int input(int** field) {
    int flag = 0;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (scanf("%d", &field[i][j]) != 1 || field[i][j] > 1 || field[i][j] < 0) {
                flag = 1;
            }
        }
    }
    return flag;
}

void print_field(int** field) {
    clear();
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (field[i][j])
                mvprintw(i, j, "#");
            else
                mvprintw(i, j, ".");
        }
        printw("\n");
    }
    refresh();
}

int count_alive_cells(int** field) {
    int sum = 0;
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++) sum += field[i][j];

    return sum;
}

int speed(char up_down_speed, int time) {
    if (up_down_speed == '-' && time < 1000)
        time += 100;
    else if (up_down_speed == '+' && time > 100)
        time -= 100;
    return time;
}

int life_cell(int neighbors, int cell) {
    int next_life;
    if ((neighbors == 3 || neighbors == 2) && cell == 1)
        next_life = 1;
    else if (neighbors == 3 && cell == 0)
        next_life = 1;
    else
        next_life = 0;
    return next_life;
}

int count_neighbors(int** field, int x, int y) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            int nx = (x + i + M) % M;
            int ny = (y + j + N) % N;
            count += field[nx][ny];
        }
    }
    return count;
}

void update_field(int** field1, int** field2) {
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++) field2[i][j] = life_cell(count_neighbors(field1, i, j), field1[i][j]);
}

void swap(int** field1, int** field2) {
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++) field1[i][j] = field2[i][j];
}
