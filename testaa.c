#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "autocomplete.h"

int is_digit(char a){
    if(a <= '9' && a >= '0'){
        return 1;
    }else{
        return 0;
    }
}

void seperate(char *line, double *a, int *b){
    double wei = 0;
    int num_p = 0;
    int char_p = 0;
    for (int i = 0; i < strlen(line); i++){
        if (is_digit(line[i]) != 0){
            num_p = i;
            break;
        }
    }
    for (int i = num_p; i < strlen(line); i++){
        if (is_digit(line[i]) == 0){
            char_p = i + 1;
            break;
        }
        wei *= 10;
        wei += (line[i]-48);
    }
    
    *a = wei;
    *b = char_p;
}

int compare_term_byASCII(const void * a, const void * b){
    int i = 0;
    while (((term *)a)->term[i] == ((term *)b)->term[i]){
        i++;
    }
    int l = ((term *)a)->term[i];
    int r = ((term *)b)->term[i];
    return l - r;
}

int compare_terms(const void *a, const void *b) {
    const term *term1 = (const term *)a;
    const term *term2 = (const term *)b;
    return strcmp(term1->term, term2->term);
}

void read_in_terms(term **terms, int *pnterms, char *filename){
    FILE *file = fopen(filename, "r");
    
    char count[100];
    fgets(count, sizeof(count), file);
    *pnterms = atoi(count);
    //printf("%d\n", *pnterms);

    *terms = (term *)malloc((*pnterms) * sizeof(term));

    char line[200];
    double wei;
    int char_p;
    
    for (int i = 0; i < *pnterms; i++) {
        fgets(line, sizeof(char[200]), file);
        seperate(line, &wei, &char_p);
        (*terms)[i].weight = wei;
        strncpy((*terms)[i].term, &line[char_p], strlen(line) - char_p);
        //fgets((*terms)[i].term, sizeof(char[200]), file);

        //printf("%d\n", d);
        //printf("%s\n", (*terms)[i].term);
        char *newline = strchr((*terms)[i].term, '\n');
        if (newline != NULL) {
            *newline = '\0';
        }
    }

    fclose(file);

    qsort(*terms, *pnterms, sizeof(term), compare_terms);
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

int compare_a(char *a, char *b) {
    return strcmp(a, b);
}

int lowest_match(term *terms, int nterms, char *substr) {
    int low = 0;
    int high = nterms - 1;

    while (low <= high) {
        int mid = (high + low) / 2;
        int cmp = compare_a(terms[mid].term, substr);
        
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

int highest_match(term *terms, int nterms, char *substr) {
    int low = 0;
    int high = nterms - 1;

    while (low <= high) {
        int mid = (high + low) / 2;
        int cmp = compare(terms[mid].term, substr);

        if (cmp > 0){
            high = mid - 1;
        } else{
            low = mid + 1;
        }
    }
    return high;
}

int compare_weight(const void *a, const void *b) {
    const term *term1 = (const term *)a;
    const term *term2 = (const term *)b;
    return (term2->weight - term1->weight);
}

void autocomplete(term **answer, int *n_answer, term *terms, int nterms, char *substr){
    int low_index = lowest_match(terms, nterms, substr);
    int high_index = highest_match(terms, nterms, substr);
    printf("%d %d\n", low_index, high_index);
    
    *n_answer = high_index - low_index + 1;
    printf("%d\n", *n_answer);
    *answer = (term *)malloc((*n_answer) * sizeof(term));
    for(int i = 0; i < *n_answer; i++){
        (*answer)[i].weight = (terms)[low_index+i].weight;
        strcpy((*answer)[i].term, (terms)[low_index+i].term);
    }
    qsort(*answer, *n_answer, sizeof(term), compare_weight);
    
}

void write_terms_to_file(term *terms, int num_terms, char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing.\n");
        exit(1);
    }

    // Write each term to the file
    for (int i = 0; i < num_terms; i++) {
        fprintf(file, "%.2f %s\n", terms[i].weight, terms[i].term);
    }

    fclose(file);
}

int main() {
    term *terms;
    int num_terms;
    read_in_terms(&terms, &num_terms, "wiktionary.txt");
    //read_in_terms(&terms, &num_terms, "cities_with_wrong_char.txt");

  
    write_terms_to_file(terms, num_terms, "terms.txt");


    for (int i = 0; i < num_terms; i++) {
        //printf("%s %f\n", terms[i].term, terms[i].weight);
    }
    term *answer;
    int n_answer;
    char substr[] = "sex";
    autocomplete(&answer, &n_answer, terms, num_terms, substr);

    printf("\n");

    for (int i = 0; i < n_answer; i++) {
        printf("%s %lf\n", answer[i].term, answer[i].weight);
    }

    // Free allocated memory
    free(terms);

    return 0;
}
