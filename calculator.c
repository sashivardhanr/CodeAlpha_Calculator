/*
 * CodeAlpha C Programming Internship
 * Task 1: Basic Calculator
 *
 * Performs addition, subtraction, multiplication and division.
 * The operation is chosen with a switch-case statement.
 *
 * Features:
 *   - Input validation (rejects text where a number is expected)
 *   - Division-by-zero and invalid-operator handling
 *   - Repeat calculations without restarting the program
 *
 * Compile: gcc -Wall -Wextra -o calculator calculator.c
 * Run    : ./calculator        (Windows: calculator.exe)
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>

/* Result codes returned by calculate() */
typedef enum {
    CALC_OK,
    CALC_DIV_BY_ZERO,
    CALC_BAD_OPERATOR,
    CALC_OVERFLOW
} CalcStatus;

/* Reads one line safely; exits cleanly if input ends (Ctrl+D / Ctrl+Z). */
static void readLine(const char *prompt, char *buf, int size)
{
    printf("%s", prompt);
    if (fgets(buf, size, stdin) == NULL) {
        printf("\nInput ended. Exiting.\n");
        exit(EXIT_SUCCESS);
    }
}

/* Keeps asking until the user types a valid finite number. */
static double readNumber(const char *prompt)
{
    char line[128];
    char *end;
    double value;

    for (;;) {
        readLine(prompt, line, sizeof line);
        errno = 0;
        value = strtod(line, &end);

        if (end == line || errno == ERANGE) {
            printf("  Invalid number. Please try again.\n");
            continue;
        }
        while (isspace((unsigned char)*end)) {
            end++;
        }
        if (*end != '\0' || !isfinite(value)) {
            printf("  Invalid number. Please try again.\n");
            continue;
        }
        return value;
    }
}

/* Reads the operator character (first non-space character typed). */
static char readOperator(void)
{
    char line[64];
    int i = 0;

    for (;;) {
        readLine("Enter operator (+, -, *, /): ", line, sizeof line);
        while (isspace((unsigned char)line[i])) {
            i++;
        }
        if (line[i] != '\0') {
            return line[i];
        }
        i = 0;
        printf("  Please type an operator.\n");
    }
}

/* Asks a yes/no question; returns 1 for yes, 0 for no. */
static int askYesNo(const char *prompt)
{
    char line[64];
    int i;

    for (;;) {
        readLine(prompt, line, sizeof line);
        i = 0;
        while (isspace((unsigned char)line[i])) {
            i++;
        }
        if (line[i] == 'y' || line[i] == 'Y') {
            return 1;
        }
        if (line[i] == 'n' || line[i] == 'N') {
            return 0;
        }
        printf("  Please answer with y or n.\n");
    }
}

/* Performs the calculation. The switch-case selects the operation. */
static CalcStatus calculate(double a, char op, double b, double *result)
{
    switch (op) {
    case '+':
        *result = a + b;
        break;
    case '-':
        *result = a - b;
        break;
    case '*':
        *result = a * b;
        break;
    case '/':
        if (b == 0.0) {
            return CALC_DIV_BY_ZERO;
        }
        *result = a / b;
        break;
    default:
        return CALC_BAD_OPERATOR;
    }

    if (!isfinite(*result)) {
        return CALC_OVERFLOW;
    }
    return CALC_OK;
}

int main(void)
{
    double a, b, result;
    char op;
    CalcStatus status;

    printf("=====================================\n");
    printf("        SIMPLE C CALCULATOR\n");
    printf("   CodeAlpha C Programming - Task 1\n");
    printf("=====================================\n");

    do {
        printf("\n");
        a  = readNumber("Enter first number : ");
        op = readOperator();
        b  = readNumber("Enter second number: ");

        status = calculate(a, op, b, &result);

        switch (status) {
        case CALC_OK:
            printf("\nResult: %g %c %g = %g\n", a, op, b, result);
            break;
        case CALC_DIV_BY_ZERO:
            printf("\nError: Division by zero is not allowed.\n");
            break;
        case CALC_BAD_OPERATOR:
            printf("\nError: '%c' is not a valid operator. Use + - * /\n", op);
            break;
        case CALC_OVERFLOW:
            printf("\nError: The result is too large to represent.\n");
            break;
        }
    } while (askYesNo("\nDo you want to calculate again? (y/n): "));

    printf("\nThank you for using the calculator. Goodbye!\n");
    return 0;
}
