#include <stdio.h>
#include <stdlib.h>
#include "autocomplete.h"
#include <string.h>

int compare_weight(const void *a, const void *b)
{
    term *term1 = (term *)a;
    term *term2 = (term *)b;
    if (term1->weight > term2->weight)
    {
        return -1;
    }
    else if (term1->weight < term2->weight)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int compare_terms(const void *a, const void *b)
{
    term *term1 = (term *)a;
    term *term2 = (term *)b;
    return strcmp(term1->term, term2->term);
}

void read_in_terms(struct term **terms, int *pnterms, char *filename)
{
    // getting the number of terms in the file
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("unsucessful");
        exit(1);
    }
    char buffer[200];
    int nterms = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        nterms++;
    }
    *pnterms = nterms;
    *terms = (term *)malloc(nterms * sizeof(term));

    // putting each term 0into *terms
    rewind(file);
    for (int i = 0; i < nterms; i++)
    {
        fscanf(file, "%lf", &(*terms)[i].weight);
        fscanf(file, "%99s", (*terms)[i].term);
    }
    fclose(file);

    qsort(*terms, nterms, sizeof(term), compare_terms);
}

int lowest_match(term *terms, int nterms, char *substr)
{
    int substr_len = strlen(substr);
    int low = 0;
    int high = nterms - 1;
    int mid;
    int match = -1; // Default to -1, indicating no match

    while (low <= high)
    {
        mid = low + (high - low) / 2; // To prevent potential overflow
        int cmp = strncmp(terms[mid].term, substr, substr_len);

        if (cmp < 0)
        {
            low = mid + 1;
        }
        else if (cmp > 0)
        {
            high = mid - 1;
        }
        else
        {
            match = mid; // Found a match, but continue to find the first occurrence
            high = mid - 1;
        }
    }

    return match; // Will return -1 if no match is found
}

int highest_match(term *terms, int nterms, char *substr)
{
    int substr_len = strlen(substr);
    int low = 0;
    int high = nterms - 1;
    int mid;
    int match = -1; // Default to -1, indicating no match

    while (low <= high)
    {
        mid = low + (high - low) / 2; // To prevent potential overflow
        int cmp = strncmp(terms[mid].term, substr, substr_len);

        if (cmp > 0)
        {
            high = mid - 1;
        }
        else if (cmp < 0)
        {
            low = mid + 1;
        }
        else
        {
            match = mid; // Found a match, continue to find the highest occurrence
            low = mid + 1;
        }
    }

    return match; // Will return -1 if no match is found
}

void autocomplete(term **answer, int *n_answer, term *terms, int nterms, char *substr)
{
    /*The function takes terms (assume it is sorted lexicographically in increasing order), the number
    of terms nterms, and the query string substr, and places the answers in answer, with *n_answer
    being the number of answers. The answers should be sorted by weight in non-increasing order.
    Sorting with qsort*/
    int low = lowest_match(terms, nterms, substr);
    int high = highest_match(terms, nterms, substr);
    *n_answer = high - low + 1;
    *answer = malloc(*n_answer * sizeof(term));
    if (*answer == NULL)
    {
        perror("Memory Allocation Failure");
        exit(1);
    }
    for (int i = 0; i < *n_answer; i++)
    {
        (*answer)[i] = terms[low + i];
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
