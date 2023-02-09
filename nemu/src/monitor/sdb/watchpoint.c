#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  word_t val;
  char* e;
} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* insert(WP **old_head, WP *wp) {
  assert(wp != NULL);
  wp->next = *old_head;
  *old_head = wp;
  return wp;
}

WP* delete(WP **old_head, WP *wp) {
  if (wp == NULL)
    assert(0);
  WP dummy;
  WP *p = &dummy;
  dummy.next = *old_head;
  while (p != NULL) {
    if (p->next == wp) {
      p->next = wp->next;
      wp->next = NULL;
      *old_head = dummy.next;
      return wp;
    }
    p = p->next;
  }
  return NULL;
}

WP* new_wp() {
  WP* d_wp = delete(&free_, free_);
  insert(&head, d_wp);
  return d_wp;
}

void free_wp(WP *wp) {
  WP* d_wp = delete(&head, wp);
  insert(&free_, d_wp);
}

void display_wp() {
  WP* wp = head;
  printf("watch points:");
  while (wp != NULL) {
    printf("[%d]%d ", wp->NO, wp->val);
    wp = wp->next;
  }
  printf(".\nfree points:");
  wp = free_;
  while (wp != NULL) {
    printf("[%d] ", wp->NO);
    wp = wp->next;
  }
  printf(".\n");
}

int check_wp() {
  WP* wp = head;
  while (wp != NULL) {
    bool success;
    word_t new_val = expr(wp->e, &success);
    if (!success)
      return -1;
    if (new_val != wp->val)
      nemu_state.state = NEMU_STOP;
    wp = wp->next;
  }
  return 0;
}

int cmd_w(char *args) {
  if (args == NULL) {
    printf("usage: 'w expression'\n");
    return 0;
  }
  bool success;
  word_t res = expr(args, &success);
  if (success) {
    WP* wp = new_wp();
    wp->val = res;
    int len = strlen(args);
    wp->e = (char *) malloc(len + 1);
    strcpy(wp->e, args);
  }
  display_wp();
  return 0;
}

int cmd_d(char *args) {
  if (args == NULL) {
    printf("usage: 'd N'\n");
    return 0;
  }
  int N = atoi(args);
  WP* wp = wp_pool + N;
  free_wp(wp);
  free(wp->e);
  display_wp();
  return 0;
}
