#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

#define INVALID -1

enum {
  TK_NOTYPE = 256, TK_EQ,

  /* TODO: Add more token types */
  TK_NUM,
  TK_LP,
  TK_RP,
};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\(", TK_LP},
  {"\\)", TK_RP},
  {"\\*", '*'},         //
  {"/", '/'},         //
  {"\\+", '+'},         // plus
  {"-", '-'},         //
  {"==", TK_EQ},        // equal
  {"[0-9][0-9]*", TK_NUM},
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[1024] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0' && e[position] != '\n') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        // char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        // Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
        //     i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
          case TK_NOTYPE:
            break;
          case TK_NUM: {
            tokens[nr_token].type = TK_NUM;
            memset(tokens[nr_token].str, ' ', 32);
            strncpy(tokens[nr_token].str, e + position - substr_len, substr_len);
            // printf("token str: %s\n", tokens[nr_token].str);
            nr_token++;
            break;
          }
          case '+': {
            tokens[nr_token].type = '+';
            nr_token++;
            break;
          }
          case '*': {
            tokens[nr_token].type = '*';
            nr_token++;
            break;
          }
          case '-': {
            tokens[nr_token].type = '-';
            nr_token++;
            break;
          }
          case '/': {
            tokens[nr_token].type = '/';
            nr_token++;
            break;
          }
          case TK_LP: {
            tokens[nr_token].type = TK_LP;
            nr_token++;
            break;
          }
          case TK_RP: {
            tokens[nr_token].type = TK_RP;
            nr_token++;
            break;
          }
          // default: TODO();
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }
  // for (int i = 0; i < nr_token; i++) {
  //   printf("token %d type is %c\n", i, tokens[i].type);
  // }

  return true;
}

typedef struct stack {
  int data[32];
  int top;
} stack;

stack* stack_init() {
  stack* s;
  s = (stack*)malloc(sizeof(stack));
  s->top = -1;
  return s;
}

void stack_add(stack* s, int val) {
  if (s == NULL)
    panic("Stack is not initialized!");
  if (s->top + 1 >= 31)
    panic("Stack is full!");
  s->top++;
  s->data[s->top] = val;
}

int stack_remove(stack* s) {
  if (s == NULL)
    panic("Stack is not initialized!");
  if (s->top == -1)
    panic("Stack is empty!");
  int val = s->data[s->top];
  s->top--;
  return val;
}

int stack_top(stack* s) {
  if (s == NULL)
    panic("Stack is not initialized!");
  return s->data[s->top];
}

bool stack_empty(stack* s) {
  if (s == NULL)
    panic("Stack is not initialized!");
  return s->top == -1;
}

int check_parentheses(int p, int q) {
  int left = p, right = q;
  while (tokens[left].type == TK_NOTYPE)
    left++;
  while (tokens[right].type == TK_NOTYPE)
    right--;

  if (right <= left)
    return -1;
  if (tokens[left].type != TK_LP || tokens[right].type != TK_RP)
    return 0;
  left++;
  right--;

  // printf("left: %d, right: %d\n", left, right);
  stack* stack = stack_init();
  for (int i = left; i <= right; i++) {
    if (tokens[i].type == TK_LP) {
      stack_add(stack, TK_RP);
    } else if (tokens[i].type == TK_RP) {
      if (!stack_empty(stack))
        stack_remove(stack);
      else
        return 0;
    }
  }
  if (!stack_empty(stack))
    return -1;
  return 1;
}

word_t eval(int p, int q) {
  int res;
  if (p > q) {
    /* Bad expression */
    return INVALID;
  } else if (p == q) {
    /* Single token.
     * For now this token should be a number.
     * Return the value of the number.
     */
    return atoi(tokens[p].str);
  } else if ((res = check_parentheses(p, q)) == 1) {
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
    // printf("p: %d, q: %d is valid\n", p, q);
    return eval(p + 1, q - 1);
  } else {
    if (res == -1) {
      return INVALID;
    }
    word_t op = -1;
    int precedence = 2;
    // printf("calc p: %d, q: %d\n", p, q);
    // int left = p, right = q;
    // int left_paren = -1, right_paren = -1;
    // while (left <= q && tokens[left].type != TK_LP) {
    //   left++;
    // }
    // while (right >= p && tokens[right].type != TK_RP) {
    //   right--;
    // }
    // if (left < right) {
    //   left_paren = left;
    //   right_paren = right;
    // } else if ((left > q && right >= p) || (right < p && left <= q)) {
    //   return INVALID;
    // }

    stack* stack = stack_init();
    for (int i = p; i <= q; i++) {
      if (tokens[i].type == TK_LP) {
        stack_add(stack, TK_RP);
      } else if (tokens[i].type == TK_RP) {
        if (!stack_empty(stack))
          stack_remove(stack);
      }
      if (!stack_empty(stack))
        continue;
      if (tokens[i].type != '*' && tokens[i].type != '/' && tokens[i].type != '+' && tokens[i].type != '-')
        continue;
      switch (tokens[i].type) {
        case '*':
        case '/':
          if (1 <= precedence) {
            precedence = 1;
            op = i;
          }
          break;
        case '+':
        case '-':
          if (0 <= precedence) {
            precedence = 0;
            op = i;
          }
          break;
      }
    }
    // printf("main op is %c\n", tokens[op].type);
    word_t val1 = eval(p, op - 1);
    word_t val2 = eval(op + 1, q);

    switch (tokens[op].type) {
      case '+':
        return val1 + val2;
      case '-':
        return val1 - val2;
      case '*':
        return val1 * val2;
      case '/':
        return val1 / val2;
      default: assert(0); return 0;
    }
  }
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  word_t res = eval(0, nr_token - 1);
  *success = (res != INVALID);

  return res;
}
