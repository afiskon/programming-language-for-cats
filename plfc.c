#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

const char* commands[] = {
  "Stroke me",
  "Feed me",
  "Clean my toilet",
  "Play with me",
};

void turn_off_stdin_buffering() {
  struct termios term, term_orig;

  if(tcgetattr(0, &term_orig)) {
    printf("tcgetattr failed\n");
    exit(1); 
  }

  term = term_orig;

  term.c_lflag &= ~(ICANON | ECHO); // clear ICANON and ECHO
  term.c_cc[VMIN] = 0;
  term.c_cc[VTIME] = 0;

  if(tcsetattr(0, TCSANOW, &term)) {
    printf("tcsetattr failed\n");
    exit(1);
  }
}

int main() {
  turn_off_stdin_buffering();

  for(;;) {
    int c = fgetc(stdin);
    if(c == EOF) { // no input yet
      usleep(100*1000); // 100 ms
      continue;
    }

    printf("%s\n", commands[c % (sizeof(commands) / sizeof(commands[0]))]);
  }
  return 0;
}
