#include <common.h>

extern word_t expr(char *e, bool *success);

int expr_test() {
  char* load_file = "/home/ubuntu/PA/ics2021/nemu/tools/gen-expr/input";
	FILE *fp;
	char line[1024];
	fp = fopen(load_file, "r");
	if(fp == NULL) {
		printf("can not load file!");
		return 1;
	}
	while(!feof(fp)) {
		char* res = fgets(line, 1000, fp);
    res = res;
    // printf("right ans: %s\n", strtok(line, " "));
		word_t n = atoi(strtok(line, " "));
    printf("right ans: %u", n);
    char* args;
    while((args = strtok(NULL, " "))) {
      bool success;
      word_t res = expr(args, &success);
      if (success) {
        printf("\t\tmy ans: %u\n", res);
        if (n != res) {
          printf("wrong answer\n");
          fclose(fp);
          return 0;
        }
      } else {
        printf("invalid expression\n");
        fclose(fp);
        return 0;
      }
    }
	}
	fclose(fp);
  printf("Pass: expr_test\n");
	return 0;
}
