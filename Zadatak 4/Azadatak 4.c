/* Polynomials are stored as singly linked lists sorted by descending
 * exponent. Terms with the same exponent are merged automatically and
 * terms whose coefficient becomes 0 are removed.
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct Term {
    int coef;
    int exp;
    struct Term *next;
} Term;

static void free_poly(Term *p)
{
    while (p) {
        Term *tmp = p->next;
        free(p);
        p = tmp;
    }
}

/* Insert a term keeping the list sorted (descending exponent).
 * Merges equal exponents, drops zero coefficients.
 * Returns the new head, or sets *err = 1 on allocation failure. */
static Term *insert_term(Term *head, int coef, int exp, int *err)
{
    Term *prev = NULL, *cur = head, *t;

    if (coef == 0)
        return head;

    while (cur && cur->exp > exp) {
        prev = cur;
        cur = cur->next;
    }

    if (cur && cur->exp == exp) {          /* same exponent: combine */
        cur->coef += coef;
        if (cur->coef == 0) {              /* term canceled out */
            if (prev) prev->next = cur->next;
            else      head = cur->next;
            free(cur);
        }
        return head;
    }

    t = malloc(sizeof *t);
    if (!t) {
        *err = 1;
        return head;
    }
    t->coef = coef;
    t->exp = exp;
    t->next = cur;
    if (prev) prev->next = t;
    else      head = t;
    return head;
}

/* Read one polynomial from the file. Returns NULL with *err set on failure
 * (an empty/zero polynomial returns NULL with *err == 0). */
static Term *read_poly(FILE *fp, int *err)
{
    int n, i, c, e;
    Term *head = NULL;

    if (fscanf(fp, "%d", &n) != 1 || n < 0) {
        fprintf(stderr, "Error: invalid term count in file.\n");
        *err = 1;
        return NULL;
    }
    for (i = 0; i < n; i++) {
        if (fscanf(fp, "%d %d", &c, &e) != 2 || e < 0) {
            fprintf(stderr, "Error: invalid term %d in file.\n", i + 1);
            *err = 1;
            free_poly(head);
            return NULL;
        }
        head = insert_term(head, c, e, err);
        if (*err) {
            fprintf(stderr, "Error: out of memory.\n");
            free_poly(head);
            return NULL;
        }
    }
    return head;
}

static Term *add_poly(const Term *a, const Term *b, int *err)
{
    Term *r = NULL;
    for (; a && !*err; a = a->next) r = insert_term(r, a->coef, a->exp, err);
    for (; b && !*err; b = b->next) r = insert_term(r, b->coef, b->exp, err);
    if (*err) { free_poly(r); return NULL; }
    return r;
}

static Term *mul_poly(const Term *a, const Term *b, int *err)
{
    Term *r = NULL;
    const Term *pb;
    for (; a && !*err; a = a->next)
        for (pb = b; pb && !*err; pb = pb->next)
            r = insert_term(r, a->coef * pb->coef, a->exp + pb->exp, err);
    if (*err) { free_poly(r); return NULL; }
    return r;
}

static void print_poly(const char *label, const Term *p)
{
    int first = 1;
    printf("%s = ", label);
    if (!p) {
        printf("0\n");
        return;
    }
    for (; p; p = p->next) {
        int c = p->coef;
        if (first) {
            if (c < 0) printf("-");
        } else {
            printf(c < 0 ? " - " : " + ");
        }
        if (c < 0) c = -c;

        if (p->exp == 0)
            printf("%d", c);
        else {
            if (c != 1) printf("%d", c);
            printf(p->exp == 1 ? "x" : "x^%d", p->exp);
        }
        first = 0;
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    const char *name = (argc > 1) ? argv[1] : "polynomials.txt";
    FILE *fp = fopen(name, "r");
    Term *p1 = NULL, *p2 = NULL, *sum = NULL, *prod = NULL;
    int err = 0;

    if (!fp) {
        fprintf(stderr, "Error: cannot open '%s'.\n", name);
        return 1;
    }

    p1 = read_poly(fp, &err);
    if (!err) p2 = read_poly(fp, &err);
    fclose(fp);

    if (!err) sum  = add_poly(p1, p2, &err);
    if (!err) prod = mul_poly(p1, p2, &err);

    if (err) {
        fprintf(stderr, "Program aborted due to an error.\n");
    } else {
        print_poly("P1       ", p1);
        print_poly("P2       ", p2);
        print_poly("P1 + P2  ", sum);
        print_poly("P1 * P2  ", prod);
    }

    free_poly(p1);
    free_poly(p2);
    free_poly(sum);
    free_poly(prod);
    return err ? 1 : 0;
}
