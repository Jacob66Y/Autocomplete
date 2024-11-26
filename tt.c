#include <stdio.h>
#include <string.h>

typedef struct {
    char term[200];
    double weight;
} term;

int compare_terms(const void *a, const void *b) {
    const term *term1 = (const term *)a;
    const term *term2 = (const term *)b;
    return strcmp(term1->term, term2->term);
}

int compare(char *a, char *b){
    int i = 0;
    while (a[i] == b[i]){
        i++;
    }
    int l = a[i];
    int r = b[i];
    if (i >=  strlen(b)){
        return 0;
    }
    return l - r;
}

int lowest_match(term *terms, int nterms, char *substr) {
    int low = 0;
    int high = nterms - 1;

    while (low <= high) {
        int mid = (high + low) / 2;
        int cmp = compare(terms[mid].term, substr);
        
        if (cmp < 0) {
            low = mid + 1;
        } else if (cmp > 0) {
            high = mid - 1;
        } else {
            // Match found, update result and continue searching towards lower indices
            return mid;
        }
    }
    return low;

}

int highest_match(term *terms, int nterms, char *substr){
    int low = 0;
    int high = nterms - 1;

    while (low <= high) {
        int mid = (high + low) / 2;
        int cmp = compare(terms[mid].term, substr);
        
        if (cmp < 0) {
            low = mid + 1;
        } else if (cmp > 0) {
            high = mid - 1;
        } else {
            // Match found, update result and continue searching towards lower indices
            return mid;
        }
    }
    return low;
}


int main() {
    int a = strcmp("ab", "ap");
    printf("%d\n", a);
    term terms[] = {
        {"apple", 0.5},
        {"appad", 0.3},
        {"appac", 0.3},
        {"banana", 0.3},
        {"cherry", 0.8},
        {"date", 0.2},
        {"grape", 0.6},
        {"lemon", 0.7},
        {"orange", 0.9}
    };
    int nterms = sizeof(terms) / sizeof(terms[0]);
    
    // Sort the terms array in ascending lexicographic order
    qsort(terms, nterms, sizeof(term), compare_terms);
    
    char substr[] = "appa";

    int index = highest_match(terms, nterms, substr);
    if (index != -1) {
        printf("Lowest match term: %s (at index %d)\n", terms[index].term, index);
    } else {
        printf("No match found.\n");
    }

    return 0;
}
