#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <locale.h>

struct TreeNode {
    char *word;
    char *meaning;
    char *grammar;
    struct TreeNode *left;
    struct TreeNode *right;
};

// Convert a string to lowercase
void toLowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// Remove hyphens from a string
void removeHyphens(char *str) {
    char *src = str;
    char *dst = str;
    while (*src) {
        if (*src != '-') {
            *dst = *src;
            dst++;
        }
        src++;
    }
    *dst = '\0';
}

// Remove punctuation from a string
void removePunctuation(char *str) {
    int i, j;
    for (i = 0, j = 0; str[i] != '\0'; i++) {
        if (ispunct(str[i])) continue;
        str[j++] = str[i];
    }
    str[j] = '\0';
}

struct TreeNode *createNode(char *word, char *meaning, char *grammar) {
    struct TreeNode *newNode = (struct TreeNode *)malloc(sizeof(struct TreeNode));
    newNode->word = (word != NULL) ? strdup(word) : NULL;
    newNode->meaning = (meaning != NULL) ? strdup(meaning) : NULL;
    newNode->grammar = (grammar != NULL) ? strdup(grammar) : NULL;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Insert a node into the tree (case-insensitive)
struct TreeNode *insertCaseInsensitive(struct TreeNode *root, char *word, char *meaning, char *grammar) {
    toLowercase(word);
    removeHyphens(word);
    char *cleanedWord = strdup(word);
    removePunctuation(cleanedWord);

    if (root == NULL) return createNode(cleanedWord, meaning, grammar);

    int cmp = strcasecmp(cleanedWord, root->word);
    if (cmp < 0) root->left = insertCaseInsensitive(root->left, cleanedWord, meaning, grammar);
    else if (cmp > 0) root->right = insertCaseInsensitive(root->right, cleanedWord, meaning, grammar);
    return root;
}

// Read CSV file and build arrays
void readCsvFile(char *filename, char ***words, char ***meanings, char ***grammars, int *index) {
    FILE *csvFile = fopen(filename, "r");
    if (csvFile == NULL) {
        perror("Error opening CSV file");
        return;
    }

    char line[1000];
    int lineCounter = 0;
    while (fgets(line, sizeof(line), csvFile) != NULL) {
        // Remove the newline character at the end
        line[strcspn(line, "\n")] = '\0';
        lineCounter++;

        char *word = strtok(line, ",");
        char *grammar = strtok(NULL, ",");
        char *meaning = strtok(NULL, ",");

        // Check if word is NULL before inserting into the array
        if (word != NULL) {
            toLowercase(word);
            removeHyphens(word);
            char *cleanedWord = strdup(word);
            removePunctuation(cleanedWord);

            (*words)[*index] = cleanedWord;
            (*meanings)[*index] = (meaning != NULL) ? strdup(meaning) : NULL;
            (*grammars)[*index] = (grammar != NULL) ? strdup(grammar) : NULL;

            (*index)++;
        } else {
            // Handle the case where word is NULL or strtok returns NULL unexpectedly
            continue;  // Skip the current line
        }
    }

    fclose(csvFile);
}

// Create a completely balanced BST
struct TreeNode *createBalancedBST(char **words, char **meanings, char **grammars, int start, int end) {
    if (start > end) {
        return NULL;
    }

    int mid = (start + end) / 2;
    struct TreeNode *newNode = (struct TreeNode *)malloc(sizeof(struct TreeNode));
    newNode->word = strdup(words[mid]);
    newNode->meaning = (meanings[mid] != NULL) ? strdup(meanings[mid]) : NULL;
    newNode->grammar = (grammars[mid] != NULL) ? strdup(grammars[mid]) : NULL;

    newNode->left = createBalancedBST(words, meanings, grammars, start, mid - 1);
    newNode->right = createBalancedBST(words, meanings, grammars, mid + 1, end);

    return newNode;
}

void findSmallestAndLargest(struct TreeNode *node, char **smallest, char **largest) {
    if (node == NULL) {
        return;
    }

    if (*smallest == NULL || strcmp(node->word, *smallest) < 0) {
        *smallest = node->word;
    }

    if (*largest == NULL || strcmp(node->word, *largest) > 0) {
        *largest = node->word;
    }

    findSmallestAndLargest(node->left, smallest, largest);
    findSmallestAndLargest(node->right, smallest, largest);
}

// Function to print the smallest and largest words for the left and right children
void printSmallestAndLargest(struct TreeNode *root) {
    if (root == NULL) {
        printf("The tree is empty.\n");
        return;
    }

    char *smallestLeft = NULL;
    char *largestLeft = NULL;
    char *smallestRight = NULL;
    char *largestRight = NULL;
	char *largestLeftRight = NULL;
	char *smallestLeftRight = NULL;
	char *largestRightRight = NULL;
	char *smallestRightLeft = NULL;
	
	printf("\n\t\t\t\t\t     %s\n", root->word);
	
	findSmallestAndLargest(root, &smallestLeft, &largestRight);
	
	printf("\t\t\t\t       (%s---%s)\n", (smallestLeft != NULL) ? smallestLeft : "N/A", (largestRight != NULL) ? largestRight : "N/A");
	printf("\t\t\t\t\t  /\t      \\ \n");
	printf("\t\t\t\t    %s\t     %s\n", root->left->word, root->right->word);
	
	findSmallestAndLargest(root->left, &smallestLeft, &largestLeft);
    findSmallestAndLargest(root->right, &smallestRight, &largestRight);

	printf("\t\t\t      (%s---%s)        (%s---%s)\n", (smallestLeft != NULL) ? smallestLeft : "N/A", (largestLeft != NULL) ? largestLeft : "N/A", (smallestRight != NULL) ? smallestRight : "N/A", (largestRight != NULL) ? largestRight : "N/A");
	printf("\t\t\t       /\t    \\\t       /\t       \\\n");
	printf("\t\t\t   %s\t  %s   %s\t      %s\n", root->left->left->word, root->left->right->word, root->right->left->word, root->right->right->word);

	findSmallestAndLargest(root->left->left, &smallestLeft, &largestLeftRight);
	findSmallestAndLargest(root->left->right, &smallestLeftRight, &largestLeft);
	findSmallestAndLargest(root->right->left, &smallestRight, &largestRightRight);
	findSmallestAndLargest(root->right->right, &smallestRightLeft, &largestRight);
	
    printf("\t\t\t  (%s-        (%s-   (%s-     (%s-\n", (smallestLeft != NULL) ? smallestLeft : "N/A", (smallestLeftRight != NULL) ? smallestLeftRight : "N/A", (smallestRight != NULL) ? smallestRight : "N/A", (smallestRightLeft != NULL) ? smallestRightLeft : "N/A");
	printf("\t\t\t -%s)    -%s)    -%s)   -%s)\n\n", (largestLeftRight != NULL) ? largestLeftRight : "N/A", (largestLeft != NULL) ? largestLeft : "N/A", (largestRightRight != NULL) ? largestRightRight : "N/A", (largestRight != NULL) ? largestRight : "N/A" );
}

// Free memory allocated for the array
void freeArray(char **array, int size) {
    for (int i = 0; i < size; i++) {
        free(array[i]);
    }
    free(array);
}

// Search for a word and get its meaning (case-insensitive)
bool searchAndGetMeaningCaseInsensitive(struct TreeNode *root, char *word, char **meaning, char **grammar) {
    if (root == NULL) {
        return false;
    }

    int cmp = strcasecmp(word, root->word);
    if (cmp < 0) {
        return searchAndGetMeaningCaseInsensitive(root->left, word, meaning, grammar);
    } else if (cmp > 0) {
        return searchAndGetMeaningCaseInsensitive(root->right, word, meaning, grammar);
    } else {
        *meaning = (root->meaning != NULL) ? root->meaning : "The meaning of the word is unknown as the word was added to the database by the user.";
        *grammar = (root->grammar != NULL) ? root->grammar : "The grammar of the word is unknown as the word was added to the database by the user.";
        return true;
    }
}

// Check the spelling of a sentence and prompt to add unknown words
void checkSpellingOfSentence(struct TreeNode *root, char *sentence) {
    char *token = strtok(sentence, " ");
    while (token != NULL) {
        removePunctuation(token);
        char *meaning;
        char *grammar;
        if (searchAndGetMeaningCaseInsensitive(root, token, &meaning, &grammar)) {
            printf("The word '%s' is spelled correctly.\n", token);
            printf("Grammar: %s\n", grammar);
            printf("Meaning: %s\n", meaning);
        } else {
            printf("The word '%s' is not found/spelled correctly.\n", token);
            printf("Do you want to add this word to the database? (y/n): ");
            char addWordChoice;
            scanf(" %c", &addWordChoice);
            if (addWordChoice == 'y' || addWordChoice == 'Y') {
                root = insertCaseInsensitive(root, token, NULL, NULL);
                printf("The word has been added to the database.\n");
            }
        }
        token = strtok(NULL, " ");
    }
}
// In-order traversal to write data to CSV file
void inOrderTraversal(struct TreeNode *root, FILE *csvFile) {
    if (root == NULL) {
        return;
    }
    inOrderTraversal(root->left, csvFile);
    if (root->word != NULL) {
        fprintf(csvFile, "%s,", root->word);
    }
    if (root->grammar != NULL) {
        fprintf(csvFile, "%s,", root->grammar);
    } else {
        fprintf(csvFile, ",");
    }
    if (root->meaning != NULL) {
        char *meaningCopy = strdup(root->meaning);
        char *pos;
        while ((pos = strchr(meaningCopy, ',')) != NULL) {
            *pos = ';';
        }
        fprintf(csvFile, "%s", meaningCopy);
        free(meaningCopy);
    } else {
        fprintf(csvFile, ",");
    }
    fprintf(csvFile, "\n");
    inOrderTraversal(root->right, csvFile);
}

void writeCsvFile(struct TreeNode *root, char *filename) {
    FILE *csvFile = fopen(filename, "w");
    if (csvFile == NULL) {
        perror("Error opening CSV file for writing");
        return;
    }
    inOrderTraversal(root, csvFile);
    fclose(csvFile);
}

// Free memory allocated for the tree
void freeTree(struct TreeNode *root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root->word);
    free(root->meaning);
    free(root->grammar);
    free(root);
}

int main() {

    int index = 0;
    char **words = malloc(sizeof(char *) * 5000);
    char **meanings = malloc(sizeof(char *) * 5000);
    char **grammars = malloc(sizeof(char *) * 5000);

    readCsvFile("word.csv", &words, &meanings, &grammars, &index);

    struct TreeNode *root = createBalancedBST(words, meanings, grammars, 0, index - 1);

    printSmallestAndLargest(root);

    freeArray(words, index);
    freeArray(meanings, index);
    freeArray(grammars, index);

	while (1) {
        char inputSentence[256];
        printf("Enter a word or sentence to check its spelling (or 'q' to quit): ");
        scanf(" %[^\n]%*c", inputSentence); 
        if (strcmp(inputSentence, "q") == 0) {
            break;
        }
        checkSpellingOfSentence(root, inputSentence);
    }
	
    writeCsvFile(root, "word.csv");
    freeTree(root);
    return 0;
}
