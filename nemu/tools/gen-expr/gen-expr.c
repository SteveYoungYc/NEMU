#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static int pos = 0;
int recur_count = 0;
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

int choose(int n) {
  return rand() % n;
}

void gen_num() {
  buf[pos] = choose(9) + '1';
  pos++;
}

static char ops[3] = {'+', '*', '/'};

void gen_rand_op() {
  buf[pos] = ops[choose(2)];
  pos++;
}

void gen(char c) {
  buf[pos] = c;
  pos++;
}

static void gen_rand_expr() {
  recur_count++;
  int n;
  if (recur_count < 30) {
    n = choose(3);
  } else {
    n = choose(2);
  }
  switch (n) {
    case 0: gen_num(); break;
    case 1: gen('('); gen_rand_expr(); gen(')'); break;
    default: gen_rand_expr(); gen_rand_op(); gen_rand_expr(); break;
  }
  buf[pos] = '\0';
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i++) {
    pos = 0;
    recur_count = 0;
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    int r = fscanf(fp, "%d", &result);
    r++;
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
