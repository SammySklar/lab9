#include <stdio.h>
#include <stdlib.h>

// RecordType
// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// HashType
struct HashType {
    struct RecordType *data;
    struct HashType *next;
};

// Compute the hash function
int hash(int x, int size) {
    return x % size;
}

// Parses input file to an array of RecordType
int parseData(char *inputFileName, struct RecordType **ppData) {
    FILE *inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile) {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType *)malloc(sizeof(struct RecordType) * dataSz);
        if (*ppData == NULL) {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i) {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }
        fclose(inFile);
    }
    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz) {
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i) {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType **hashTable, int hashTableSize) {
    printf("\nRecords in HashTable:\n");
    for (int i = 0; i < hashTableSize; ++i) {
        printf("Index %d -> ", i);
        struct HashType *current = hashTable[i];
        while (current != NULL) {
            printf("(%d, %c, %d) -> ", current->data->id, current->data->name, current->data->order);
            current = current->next;
        }
        printf("NULL\n");
    }
}

int main(void) {
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input_lab_9.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Initialize hash table with size twice the number of records
    int hashTableSize = 2 * recordSz;
    struct HashType **hashTable = (struct HashType **)malloc(sizeof(struct HashType *) * hashTableSize);
    for (int i = 0; i < hashTableSize; ++i)
        hashTable[i] = NULL;

    // Insert records into hash table
    for (int i = 0; i < recordSz; ++i) {
        int index = hash(pRecords[i].id, hashTableSize);
        struct HashType *newNode = (struct HashType *)malloc(sizeof(struct HashType));
        newNode->data = &pRecords[i];
        newNode->next = NULL;

        if (hashTable[index] == NULL) {
            hashTable[index] = newNode;
        } else {
            // Handle collisions by chaining
            struct HashType *current = hashTable[index];
            while (current->next != NULL)
                current = current->next;
            current->next = newNode;
        }
    }

    // Display records in the hash table
    displayRecordsInHash(hashTable, hashTableSize);

    // Free allocated memory
    for (int i = 0; i < hashTableSize; ++i) {
        struct HashType *current = hashTable[i];
        while (current != NULL) {
            struct HashType *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(hashTable);
    free(pRecords);

    return 0;
}
