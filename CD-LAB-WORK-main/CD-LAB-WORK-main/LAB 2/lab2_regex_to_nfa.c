#include <stdio.h>
#include <string.h>

int state = 0;

/* Structure for NFA state */
typedef struct {
    int start;
    int end;
} NFA;

/* Print transition */
void transition(int from, char symbol, int to) {
    if (symbol == 'e')
        printf("q%d -- ε --> q%d\n", from, to);
    else
        printf("q%d -- %c --> q%d\n", from, symbol, to);
}

/* Create NFA for symbol */
NFA symbolNFA(char c) {
    NFA nfa;
    nfa.start = state++;
    nfa.end = state++;
    transition(nfa.start, c, nfa.end);
    return nfa;
}

/* Concatenation */
NFA concat(NFA n1, NFA n2) {
    transition(n1.end, 'e', n2.start);
    n1.end = n2.end;
    return n1;
}

/* Union */
NFA unionNFA(NFA n1, NFA n2) {
    NFA nfa;
    nfa.start = state++;
    nfa.end = state++;
    transition(nfa.start, 'e', n1.start);
    transition(nfa.start, 'e', n2.start);
    transition(n1.end, 'e', nfa.end);
    transition(n2.end, 'e', nfa.end);
    return nfa;
}

/* Kleene Star */
NFA kleene(NFA n) {
    NFA nfa;
    nfa.start = state++;
    nfa.end = state++;
    transition(nfa.start, 'e', n.end);
    transition(nfa.start, 'e', n.start);
    transition(n.end, 'e', n.start);
    transition(n.end, 'e', nfa.end);
    return nfa;
}

int main() {
    char re[50];
    NFA stack[50];
    int top = -1;

    printf("Enter Regular Expression (postfix): ");
    scanf("%s", re);

    printf("\nNFA Transitions:\n");

    for (int i = 0; i < strlen(re); i++) {
        char c = re[i];

        if (c >= 'a' && c <= 'z') {
            stack[++top] = symbolNFA(c);
        }
        else if (c == '.') {   // Concatenation
            NFA n2 = stack[top--];
            NFA n1 = stack[top--];
            stack[++top] = concat(n1, n2);
        }
        else if (c == '|') {   // Union
            NFA n2 = stack[top--];
            NFA n1 = stack[top--];
            stack[++top] = unionNFA(n1, n2);
        }
        else if (c == '*') {   // Kleene star
            NFA n = stack[top--];
            stack[++top] = kleene(n);
        }
    }

    printf("\nStart State: q%d", stack[top].start);
    printf("\nFinal State: q%d\n", stack[top].end);

    return 0;
}
