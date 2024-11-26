#include "autocomplete.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int compare_terms(const void *a, const void *b) {
    const term *term1 = *(const term **)a;
    const term *term2 = *(const term **)b;
    return strcmp(term1->term, term2->term);
}

void read_in_terms(term **terms, int *pnterms, char *filename){
    FILE *file = fopen(filename, "r");
    char count[100];
    fgets(count, sizeof(count), file);
    *pnterms = atoi(count);
    printf("%d\n", *pnterms);
    

    file = fopen(filename, "r"); // Reset file position indicator to the beginning
    // Allocate memory for terms
    (*terms) = malloc(*pnterms * sizeof(term));
    
    // Read terms from file
    for (int i = 1; i < *pnterms + 1; i++) {
        fgets((*terms)[i].term, 256, file);
        printf("%s\n", (*terms)[i]);
        //(*terms).term = malloc(256 * sizeof(char)); // Assuming max term length of 255 characters
        
        // Remove newline character if present
        char *newline = strchr((*terms)[i].term, '\n');
        if (newline != NULL) {
            *newline = '\0';
        }
    }

    fclose(file);

    // Sort terms in lexicographic order
    //qsort(*terms, count, sizeof(term *), compare_terms);

}





int main() {
    term *terms;
    int num_terms;
    //read_in_terms(&terms, &num_terms, "cities_with_wrong_char.txt");
    read_in_terms(&terms, &num_terms, "93827.txt");

    // Print terms
    for (int i = 0; i < num_terms; i++) {
        printf("%s\n", terms[i].term);
    }

    // Free allocated memory
    for (int i = 0; i < num_terms; i++) {
        free(terms[i].term);
    }
    free(terms);

    return 0;
}