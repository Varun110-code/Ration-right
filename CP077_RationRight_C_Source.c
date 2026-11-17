/*
 * CP-077 RationRight - Entitlement-Based Public Distribution Tracker
 * Console application in C.
 *
 * IMPORTANT:
 * The supplied problem statement does not specify card categories or ration
 * rates. This program therefore asks the operator to enter trainer-approved
 * per-member monthly rates for card types A, B and C at startup.
 *
 * Compile: gcc rationright.c -o rationright
 * Run:     ./rationright        (Windows: rationright.exe)
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_FAMILIES 100
#define ID_LEN 20
#define NAME_LEN 60
#define MONTH_LEN 8
#define MAX_COLLECTION_HISTORY 120

typedef struct {
    char id[ID_LEN];
    char headName[NAME_LEN];
    char cardType;              /* A, B or C */
    int members;
    char collectedMonths[MAX_COLLECTION_HISTORY][MONTH_LEN];
    int collectionCount;
} Family;

typedef struct {
    double rice;
    double wheat;
    double sugar;
} Ration;

typedef struct {
    double rice;
    double wheat;
    double sugar;
} Stock;

Family families[MAX_FAMILIES];
int familyCount = 0;
Stock shopStock = {0, 0, 0};

/* Monthly entitlement per member for each card type: A, B and C. */
Ration rates[3];

int readLine(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    if (!fgets(buffer, (int)size, stdin)) return 0;

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {}
    }
    return 1;
}

int readInt(const char *prompt, int min, int max) {
    char line[100], extra;
    int value;
    while (1) {
        if (!readLine(prompt, line, sizeof(line))) return min;
        if (sscanf(line, " %d %c", &value, &extra) == 1 &&
            value >= min && value <= max) return value;
        printf("Invalid input. Enter a whole number from %d to %d.\n", min, max);
    }
}

double readNonNegativeDouble(const char *prompt) {
    char line[100], extra;
    double value;
    while (1) {
        if (!readLine(prompt, line, sizeof(line))) return 0;
        if (sscanf(line, " %lf %c", &value, &extra) == 1 &&
            value >= 0) return value;
        printf("Invalid input. Enter a number greater than or equal to 0.\n");
    }
}

int validMonth(const char *month) {
    if (strlen(month) != 7 || month[4] != '-') return 0;
    for (int i = 0; i < 7; i++) {
        if (i == 4) continue;
        if (!isdigit((unsigned char)month[i])) return 0;
    }
    int m = (month[5] - '0') * 10 + (month[6] - '0');
    return m >= 1 && m <= 12;
}

void readMonth(char month[MONTH_LEN]) {
    while (1) {
        if (!readLine("Enter month (YYYY-MM): ", month, MONTH_LEN)) {
            strcpy(month, "2000-01");
            return;
        }
        if (validMonth(month)) return;
        printf("Invalid month. Example: 2026-11\n");
    }
}

int cardIndex(char cardType) {
    cardType = (char)toupper((unsigned char)cardType);
    if (cardType == 'A') return 0;
    if (cardType == 'B') return 1;
    if (cardType == 'C') return 2;
    return -1;
}

int findFamilyById(const char *id) {
    for (int i = 0; i < familyCount; i++) {
        if (strcmp(families[i].id, id) == 0) return i;
    }
    return -1;
}

int hasCollectedInMonth(const Family *family, const char *month) {
    for (int i = 0; i < family->collectionCount; i++) {
        if (strcmp(family->collectedMonths[i], month) == 0) return 1;
    }
    return 0;
}

Ration calculateEntitlement(const Family *family) {
    Ration result = {0, 0, 0};
    int index = cardIndex(family->cardType);
    if (index < 0) return result;

    result.rice = rates[index].rice * family->members;
    result.wheat = rates[index].wheat * family->members;
    result.sugar = rates[index].sugar * family->members;
    return result;
}

void configureRates(void) {
    const char cardTypes[] = {'A', 'B', 'C'};
    printf("\n--- Configure monthly entitlement rates ---\n");
    printf("Enter the trainer-approved quantity PER MEMBER for each card type.\n");
    printf("Use the same unit throughout (for example, kg).\n");

    for (int i = 0; i < 3; i++) {
        char prompt[120];
        printf("\nCard type %c:\n", cardTypes[i]);
        snprintf(prompt, sizeof(prompt), "  Rice per member: ");
        rates[i].rice = readNonNegativeDouble(prompt);
        rates[i].wheat = readNonNegativeDouble("  Wheat per member: ");
        rates[i].sugar = readNonNegativeDouble("  Sugar per member: ");
    }
}

void registerFamily(void) {
    if (familyCount >= MAX_FAMILIES) {
        printf("Family storage is full.\n");
        return;
    }

    Family f;
    char card[20];

    printf("\n--- Register Family ---\n");
    if (!readLine("Family ID: ", f.id, sizeof(f.id)) || strlen(f.id) == 0) {
        printf("Family ID cannot be empty.\n");
        return;
    }
    if (findFamilyById(f.id) != -1) {
        printf("That family ID already exists.\n");
        return;
    }
    if (!readLine("Family head name: ", f.headName, sizeof(f.headName)) ||
        strlen(f.headName) == 0) {
        printf("Name cannot be empty.\n");
        return;
    }

    while (1) {
        readLine("Card type (A/B/C): ", card, sizeof(card));
        if (strlen(card) == 1 && cardIndex(card[0]) != -1) break;
        printf("Choose A, B or C.\n");
    }

    f.cardType = (char)toupper((unsigned char)card[0]);
    f.members = readInt("Number of family members: ", 1, 50);
    f.collectionCount = 0;

    families[familyCount++] = f;
    printf("Family registered successfully. Total families: %d\n", familyCount);
}

void showEntitlement(void) {
    char id[ID_LEN];
    printf("\n--- Calculate Monthly Entitlement ---\n");
    readLine("Enter family ID: ", id, sizeof(id));

    int index = findFamilyById(id);
    if (index == -1) {
        printf("Family not found.\n");
        return;
    }

    Family *f = &families[index];
    Ration r = calculateEntitlement(f);
    printf("\nFamily: %s | ID: %s | Card: %c | Members: %d\n",
           f->headName, f->id, f->cardType, f->members);
    printf("Monthly entitlement:\n");
    printf("Rice : %.2f\nWheat: %.2f\nSugar: %.2f\n",
           r.rice, r.wheat, r.sugar);
}

void updateStock(void) {
    printf("\n--- Shop Stock Management ---\n");
    printf("Current stock - Rice: %.2f | Wheat: %.2f | Sugar: %.2f\n",
           shopStock.rice, shopStock.wheat, shopStock.sugar);
    printf("1. Add stock\n2. Return to main menu\n");

    int choice = readInt("Choose: ", 1, 2);
    if (choice == 2) return;

    printf("Enter quantity to add (same unit used for entitlement):\n");
    shopStock.rice += readNonNegativeDouble("Rice: ");
    shopStock.wheat += readNonNegativeDouble("Wheat: ");
    shopStock.sugar += readNonNegativeDouble("Sugar: ");

    printf("Stock updated successfully.\n");
    printf("New stock - Rice: %.2f | Wheat: %.2f | Sugar: %.2f\n",
           shopStock.rice, shopStock.wheat, shopStock.sugar);
}

void distributeRation(void) {
    char id[ID_LEN], month[MONTH_LEN];
    printf("\n--- Distribute Ration ---\n");
    readLine("Enter family ID: ", id, sizeof(id));

    int index = findFamilyById(id);
    if (index == -1) {
        printf("Family not found. Register the family first.\n");
        return;
    }

    readMonth(month);
    Family *f = &families[index];

    if (hasCollectedInMonth(f, month)) {
        printf("Distribution blocked: this family has already collected in %s.\n",
               month);
        return;
    }

    Ration r = calculateEntitlement(f);
    printf("Entitlement for %s (%s): Rice %.2f, Wheat %.2f, Sugar %.2f\n",
           f->headName, month, r.rice, r.wheat, r.sugar);

    if (shopStock.rice < r.rice ||
        shopStock.wheat < r.wheat ||
        shopStock.sugar < r.sugar) {
        printf("Distribution refused: shop stock is insufficient.\n");
        printf("Available - Rice %.2f, Wheat %.2f, Sugar %.2f\n",
               shopStock.rice, shopStock.wheat, shopStock.sugar);
        return;
    }

    shopStock.rice -= r.rice;
    shopStock.wheat -= r.wheat;
    shopStock.sugar -= r.sugar;
    if (f->collectionCount >= MAX_COLLECTION_HISTORY) {
        printf("Distribution cannot be recorded: collection history is full.\n");
        /* Restore stock because the transaction cannot be stored. */
        shopStock.rice += r.rice;
        shopStock.wheat += r.wheat;
        shopStock.sugar += r.sugar;
        return;
    }
    strcpy(f->collectedMonths[f->collectionCount], month);
    f->collectionCount++;

    printf("Distribution recorded successfully.\n");
    printf("Remaining stock - Rice %.2f, Wheat %.2f, Sugar %.2f\n",
           shopStock.rice, shopStock.wheat, shopStock.sugar);
}

void generateReports(void) {
    char month[MONTH_LEN];
    printf("\n--- Reports ---\n");
    readMonth(month);

    int pending = 0;
    printf("\nFamilies yet to collect in %s:\n", month);
    for (int i = 0; i < familyCount; i++) {
        if (!hasCollectedInMonth(&families[i], month)) {
            printf("ID: %-12s Name: %-20s Card: %c Members: %d\n",
                   families[i].id, families[i].headName,
                   families[i].cardType, families[i].members);
            pending++;
        }
    }
    if (pending == 0) printf("All registered families have collected.\n");

    printf("\nFamilies pending: %d\n", pending);
    printf("Current remaining stock:\n");
    printf("Rice : %.2f\nWheat: %.2f\nSugar: %.2f\n",
           shopStock.rice, shopStock.wheat, shopStock.sugar);
}

void listFamilies(void) {
    printf("\n--- Registered Families ---\n");
    if (familyCount == 0) {
        printf("No families registered yet.\n");
        return;
    }
    for (int i = 0; i < familyCount; i++) {
        printf("%d. ID: %-12s Name: %-20s Card: %c Members: %d\n",
               i + 1, families[i].id, families[i].headName,
               families[i].cardType, families[i].members);
    }
}

void showMenu(void) {
    printf("\n========== RATIONRIGHT ==========\n");
    printf("1. Register Family\n");
    printf("2. Calculate Monthly Entitlement\n");
    printf("3. Record Ration Distribution\n");
    printf("4. Manage / View Shop Stock\n");
    printf("5. Reports (Pending Families & Remaining Stock)\n");
    printf("6. View Registered Families\n");
    printf("7. Exit\n");
    printf("=================================\n");
}

int main(void) {
    printf("Welcome to CP-077 RationRight!\n");
    configureRates();

    int choice;
    do {
        showMenu();
        choice = readInt("Enter your choice (1-7): ", 1, 7);

        switch (choice) {
            case 1: registerFamily(); break;
            case 2: showEntitlement(); break;
            case 3: distributeRation(); break;
            case 4: updateStock(); break;
            case 5: generateReports(); break;
            case 6: listFamilies(); break;
            case 7: printf("Thank you for using RationRight. Goodbye!\n"); break;
        }
    } while (choice != 7);

    return 0;
}
