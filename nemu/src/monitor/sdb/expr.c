#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <memory/paddr.h>

#define INVALID -1

enum {
  TK_NOTYPE = 256, TK_EQ,

  /* TODO: Add more token types */
  TK_NOTEQ,
  TK_LAND,
  TK_NUM,
  TK_LP,
  TK_RP,
  TK_DEREF,
  TK_HEX,
  TK_REG,
  TK_MULT,
  TK_DIVI,
  TK_ADD,
  TK_MINUS
};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {"\\*0[xX][0-9a-fA-F]+", TK_DEREF},
  {" +", TK_NOTYPE},    // spaces
  {"\\(", TK_LP},
  {"\\)", TK_RP},
  {"\\*", TK_MULT},         //
  {"/", TK_DIVI},         //
  {"\\+", TK_ADD},         // plus
  {"-", TK_MINUS},         //
  {"==", TK_EQ},        // equal
  {"!=", TK_NOTEQ},
  {"&&", TK_LAND},
  {"\\$[0-9a-zA-Z]+", TK_REG},
  {"0[xX][0-9a-fA-F]+", TK_HEX},
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
          case TK_HEX: {
            tokens[nr_token].type = TK_HEX;
            memset(tokens[nr_token].str, ' ', 32);
            strncpy(tokens[nr_token].str, e + position - substr_len + 2, substr_len - 2);   // remove "0x" prefix
            nr_token++;
            break;
          }
          case TK_REG: {
            tokens[nr_token].type = TK_REG;
            strncpy(tokens[nr_token].str, e + position - substr_len + 1, substr_len - 1);
            nr_token++;
            break;
          }
          case TK_DEREF: {
            tokens[nr_token].type = TK_DEREF;
            strncpy(tokens[nr_token].str, e + position - substr_len + 3, substr_len - 3);
            nr_token++;
            break;
          }
          case TK_ADD:
          case TK_MULT:
          case TK_MINUS:
          case TK_DIVI:
          case TK_EQ:
          case TK_NOTEQ:
          case TK_LAND:
          case TK_LP:
          case TK_RP: {
            tokens[nr_token].type = rules[i].token_type;
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

  return true;
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
  int stack_count = 0;
  for (int i = left; i <= right; i++) {
    if (tokens[i].type == TK_LP) {
      stack_count++;
    } else if (tokens[i].type == TK_RP) {
      if (stack_count > 0)
        stack_count--;
      else
        return 0;
    }
  }
  if (stack_count > 0)
    return -1;
  return 1;
}

word_t eval(int p, int q) {
  int res;
  if (p > q) {
    /* Bad expression */
    printf("[invalid] p: %d, q: %d\n", p, q);
    return INVALID;
  } else if (p == q) {
    /* Single token.
     * For now this token should be a number.
     * Return the value of the number.
     */
    word_t val = 0;
    bool success;
    switch (tokens[p].type) {
      case TK_NUM:
        val = strtol(tokens[p].str, NULL, 10);
        break;
      case TK_HEX:
        val = strtol(tokens[p].str, NULL, 16);
        break;
      case TK_REG:
        val = isa_reg_str2val(tokens[p].str, &success);
        if (!success) {
          printf("[invalid] cannot find reg\n");
          return INVALID;
        }
        break;
      case TK_DEREF:
        val = paddr_read(strtol(tokens[p].str, NULL, 16), 1);
        break;
      default:
        printf("[invalid]\n");
        return INVALID;
    }
    return val;
  } else if ((res = check_parentheses(p, q)) == 1) {
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
    // printf("p: %d, q: %d is valid\n", p, q);
    return eval(p + 1, q - 1);
  } else {
    if (res == -1) {
      printf("[invalid] parentheses");
      return INVALID;
    }
    word_t op = -1;
    int precedence = 4;
    int stack_count = 0;
    for (int i = p; i <= q; i++) {
      if (tokens[i].type == TK_LP) {
        stack_count++;
      } else if (tokens[i].type == TK_RP) {
        if (stack_count > 0)
          stack_count--;
      }
      if (stack_count > 0)
        continue;
      if (tokens[i].type != TK_MULT && tokens[i].type != TK_DIVI && tokens[i].type != TK_ADD && tokens[i].type != TK_MINUS 
          && tokens[i].type != TK_EQ && tokens[i].type != TK_NOTEQ && tokens[i].type != TK_LAND)
        continue;
      switch (tokens[i].type) {
        case TK_MULT:
        case TK_DIVI:
          if (3 <= precedence) {
            precedence = 3;
            op = i;
          }
          break;
        case TK_ADD:
        case TK_MINUS:
          if (2 <= precedence) {
            precedence = 2;
            op = i;
          }
          break;
        case TK_EQ:
        case TK_NOTEQ:
          if (1 <= precedence) {
            precedence = 1;
            op = i;
          }
          break;
        case TK_LAND:
          if (0 <= precedence) {
            precedence = 0;
            op = i;
          }
          break;
        default: {
          printf("[invalid] op\n");
          return INVALID;
        }
      }
    }
    // printf("main op is %c\n", tokens[op].type);
    word_t val1 = eval(p, op - 1);
    if (val1 == INVALID) {
      printf("[invalid] val1\n");
      return INVALID;
    }
    word_t val2 = eval(op + 1, q);
    if (val2 == INVALID) {
      printf("[invalid] val2\n");
      return INVALID;
    }

    switch (tokens[op].type) {
      case TK_ADD:
        return val1 + val2;
      case TK_MINUS:
        return val1 - val2;
      case TK_MULT:
        return val1 * val2;
      case TK_DIVI: {
        if (val2 == 0) {
          printf("[invalid] divide by 0\n");
          return INVALID;
        }
        return val1 / val2;
      }
      case TK_EQ:
        return (val1 == val2) ? 1 : 0;
      case TK_NOTEQ:
        return (val1 != val2) ? 1 : 0;
      case TK_LAND:
        return val1 && val2;
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
  // for (int i = 0; i < nr_token; i ++) {
  //   if (tokens[i].type == TK_MULT && (i == 0 || tokens[i - 1].type == TK_LP || 
  //       tokens[i - 1].type == TK_ADD || tokens[i - 1].type == TK_MINUS || 
  //       tokens[i - 1].type == TK_MULT || tokens[i - 1].type == TK_DIVI || 
  //       tokens[i - 1].type == TK_EQ || tokens[i - 1].type == TK_NOTEQ || tokens[i - 1].type == TK_LAND)) {
  //     tokens[i].type = TK_DEREF;
  //   }
  // }

  word_t res = eval(0, nr_token - 1);
  *success = (res != INVALID);

  return res;
}
