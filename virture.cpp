#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CODE_SIZE 1000
#define MAX_STACK_SIZE 100

// 操作符枚举类型
enum opts {
    LOAD, LOADI, STO, ADD, SUB, MULT, DIV, BR, BRF, EQ, NOTEQ, GT, LES, GE, LE, AND,
    OR, NOT, IN, OUT, CAL, ENTER, RETURN
};

// 中间代码结构体
struct Code {
    char opt[10];  // 操作符
    int operand;   // 操作数
};

struct Code code[MAX_CODE_SIZE];  // 中间代码
int stack[MAX_STACK_SIZE];        // 栈
int top = 0;                     // 栈顶
int base = 0;                     // 基地址
int ip = 0;                       // 指令指针

// 操作符映射
int getOptIndex(char* opt) {
    if (strcmp(opt, "LOAD") == 0) return LOAD;
    if (strcmp(opt, "LOADI") == 0) return LOADI;
    if (strcmp(opt, "STO") == 0) return STO;
    if (strcmp(opt, "ADD") == 0) return ADD;
    if (strcmp(opt, "SUB") == 0) return SUB;
    if (strcmp(opt, "MULT") == 0) return MULT;
    if (strcmp(opt, "DIV") == 0) return DIV;
    if (strcmp(opt, "BR") == 0) return BR;
    if (strcmp(opt, "BRF") == 0) return BRF;
    if (strcmp(opt, "EQ") == 0) return EQ;
    if (strcmp(opt, "NOTEQ") == 0) return NOTEQ;
    if (strcmp(opt, "GT") == 0) return GT;
    if (strcmp(opt, "LES") == 0) return LES;
    if (strcmp(opt, "GE") == 0) return GE;
    if (strcmp(opt, "LE") == 0) return LE;
    if (strcmp(opt, "AND") == 0) return AND;
    if (strcmp(opt, "OR") == 0) return OR;
    if (strcmp(opt, "NOT") == 0) return NOT;
    if (strcmp(opt, "IN") == 0) return IN;
    if (strcmp(opt, "OUT") == 0) return OUT;
    if (strcmp(opt, "CAL") == 0) return CAL;
    if (strcmp(opt, "ENTER") == 0) return ENTER;
    if (strcmp(opt, "RETURN") == 0) return RETURN;
    return -1;  // 未知操作符
}

// 栈操作函数
void push(int value) {
    if (top < MAX_STACK_SIZE - 1) {
        stack[top] = value;
        top++;
    }
    else {
        printf("栈溢出！\n");
        exit(-1);
    }
}

int pop() {
    if (top >= 0) {
        top--;
        return stack[top];
    }
    else {
        printf("栈为空！\n");
        exit(-1);
    }
}


// 虚拟机执行
void TESTmachine() {
    char filename[100];
    int codenum = 0;  // 中间代码条数
    FILE* in;
    printf("请输入目标文件名：");
    scanf("%s", filename);

    // 加载中间代码
    if ((in = fopen(filename, "r")) == NULL) {//打开输入文件
                printf("\n打开%s错误！\n", filename);
                exit(-1);      //出错就运行结束
            }
            while (!feof(in)) {   //读取中间代码到codes数组中
                fscanf(in, "%s %d", &code[codenum].opt, &code[codenum].operand);
                codenum++;
            }
           codenum--;     //最后一次读取会多加1
            fclose(in);
            memset(stack, 0, sizeof(stack));
    do {
        struct Code current_code = code[ip];
        ip++;
        int optIndex = getOptIndex(current_code.opt);  // 获取操作符的索引

        switch (optIndex) {
        case LOAD:
            push(stack[current_code.operand + base]);
            break;
        case LOADI:
            push(current_code.operand);
            break;
        case STO:
            stack[current_code.operand + base] = pop();
            break;
        case ADD: {
            int b = pop();
            int a = pop();
            push(a + b);
            break;
        }
        case SUB: {
            int b = pop();
            int a = pop();
            push(a - b);
            break;
        }
        case MULT: {
            int b = pop();
            int a = pop();
            push(a * b);
            break;
        }
        case DIV: {
            int b = pop();
            int a = pop();
            if (b == 0) {
                printf("除数为零错误！\n");
                exit(-1);
            }
            push(a / b);
            break;
        }
        case BR:
            ip = current_code.operand;
            break;
        case BRF:
            if (pop() == 0) {
                ip = current_code.operand;
            }
            break;
        case EQ: {
            int b = pop();
            int a = pop();
            push(a == b ? 1 : 0);
            break;
        }
        case NOTEQ: {
            int b = pop();
            int a = pop();
            push(a != b ? 1 : 0);
            break;
        }
        case GT: {
            int b = pop();
            int a = pop();
            push(a > b ? 1 : 0);
            break;
        }
        case LES: {
            int b = pop();
            int a = pop();
            push(a < b ? 1 : 0);
            break;
        }
        case GE: {
            int b = pop();
            int a = pop();
            push(a >= b ? 1 : 0);
            break;
        }
        case LE: {
            int b = pop();
            int a = pop();
            push(a <= b ? 1 : 0);
            break;
        }
        case AND: {
            int b = pop();
            int a = pop();
            push(a && b ? 1 : 0);
            break;
        }
        case OR: {
            int b = pop();
            int a = pop();
            push(a || b ? 1 : 0);
            break;
        }
        case NOT: {
            int a = pop();
            push(!a);
            break;
        }
        case IN: {
            int input;
            printf("请输入数据：");
            scanf("%d", &input);
            push(input);
            break;
        }
        case OUT: {
            printf("输出：%d\n", pop());
            break;
        }
        case CAL: {
            push(base);  // 保存当前基址
            push(ip);    // 保存返回地址
            ip = current_code.operand;  // 跳转到函数位置
            top = top - 2;//确保top值的正确
            base = top;
            break;
        }
        case ENTER:
            top += current_code.operand;
            break;
        case RETURN:
            top = base;
            ip = stack[top + 1];  // 恢复返回地址
            base = stack[top];    // 恢复基地址
            break;
        default:
            printf("未知操作符：%s\n", current_code.opt);
            exit(-1);
        }
    }while (ip != 0);
}

int main() {
    TESTmachine();
    return 0;
}

