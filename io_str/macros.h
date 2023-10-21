#include <assert.h>

#define witerate() while (true)
void handleIf(bool condition, const char *condition_name, int line);
#define mHandleIf(expr, expr_name) (handleIf(expr, expr_name, __LINE__))
#define str_shift_args(argc, argv) newStr(h_shift_args(argc, argv))
#define v_shift_args(argc, argv) (void)h_shift_args(argc, argv)
void *mHandleIfReturn_s(void *expr,int32_t line,char* file);
#define mHandleIfReturn(expr) (mHandleIfReturn_s(expr,__LINE__,__FILE__))
#define UNUSED_ARGS(argc, argv)                                                \
  do {                                                                         \
    (void)argc;                                                                \
    (void)argv;                                                                \
} while (0)
void handleIf(bool condition, const char *condition_name, int line) {
  if (condition) {
    fprintf(stdout, "[INFO] OK=>%s\n", condition_name);
  } else {
    fprintf(stderr, "[ERROR]=> %s fail! At line:%d\n", condition_name, line);
    exit(0);
  }
}
void *mHandleIfRetrn_s(void *expr,int32_t line,char* file){
    if (expr==NULL) {
        fprintf(stderr,"expr was NULL pointer | L:%d | FILE: %s\n",line,file);
        exit(1);
    } 
    return expr;
}
char *h_shift_args(int *argc, char ***argv){
    assert(*argc > 0);
    char *result = **argv;
    (*argv) += 1;
    (*argc) -= 1;
    return result;
}
