#include <common.h>

extern word_t expr(char *e, bool *success);

void init_monitor(int, char *[]);
void am_init_monitor();
void engine_start();
int is_exit_status_bad();

int cmd_p(char *args) {
  bool success;
  word_t res = expr(args, &success);
  if (success) {
    printf("%d", res);
    return 0;
  } else {
    printf("invalid expression\n");
    return 0;
  }
}

int test() {
  char* load_file = "/home/PA/ics2022/nemu/tools/gen-expr/input";
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
        printf("  my ans: %u\n", res);
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
	return 0;
}

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
#ifdef CONFIG_TARGET_AM
  am_init_monitor();
#else
  init_monitor(argc, argv);
#endif

  /* Start engine. */
  // engine_start();
  test();
  return 0;
  // return is_exit_status_bad();
}
