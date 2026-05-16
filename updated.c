#include <stdio.h>
#include <stdlib.h>

// structure
struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

// prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);

// NEW FEATURES
void displayAll(FILE *fPtr);
void searchAccount(FILE *fPtr);
void addInterest(FILE *fPtr);

int main()
{
    FILE *cfPtr;
    unsigned int choice;

    // open or create file
    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        cfPtr = fopen("credit.dat", "wb+");
        struct clientData blank = {0, "", "", 0.0};

        for (int i = 0; i < 100; i++)
            fwrite(&blank, sizeof(blank), 1, cfPtr);

        printf("New file created!\n");
    }

    // menu loop
    while ((choice = enterChoice()) != 8)
    {
        switch (choice)
        {
        case 1: textFile(cfPtr); break;
        case 2: updateRecord(cfPtr); break;
        case 3: newRecord(cfPtr); break;
        case 4: deleteRecord(cfPtr); break;

        case 5: displayAll(cfPtr); break;   // NEW
        case 6: searchAccount(cfPtr); break; // NEW
        case 7: addInterest(cfPtr); break;  // NEW

        default: printf("Invalid choice\n");
        }
    }

    fclose(cfPtr);
    return 0;
}

// MENU
unsigned int enterChoice(void)
{
    unsigned int choice;

    printf("\n1. Save to text file");
    printf("\n2. Update account");
    printf("\n3. Add new account");
    printf("\n4. Delete account");
    printf("\n5. Display all accounts");
    printf("\n6. Search account");
    printf("\n7. Add interest");
    printf("\n8. Exit\n");

    printf("Enter choice: ");
    scanf("%u", &choice);

    return choice;
}

// TEXT FILE
void textFile(FILE *readPtr)
{
    FILE *writePtr;
    struct clientData client;

    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        printf("File error\n");
        return;
    }

    rewind(readPtr);

    while (fread(&client, sizeof(client), 1, readPtr))
    {
        if (client.acctNum != 0)
        {
            fprintf(writePtr, "%d %s %s %.2f\n",
                    client.acctNum,
                    client.lastName,
                    client.firstName,
                    client.balance);
        }
    }

    fclose(writePtr);
    printf("Data written to accounts.txt\n");
}

// UPDATE
void updateRecord(FILE *fPtr)
{
    unsigned int account;
    double transaction;
    struct clientData client;

    printf("Enter account number: ");
    scanf("%u", &account);

    fseek(fPtr, (account - 1) * sizeof(client), SEEK_SET);
    fread(&client, sizeof(client), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found\n");
        return;
    }

    printf("Current balance: %.2f\n", client.balance);

    printf("Enter amount (+/-): ");
    scanf("%lf", &transaction);

    client.balance += transaction;

    fseek(fPtr, -(long)sizeof(client), SEEK_CUR);
    fwrite(&client, sizeof(client), 1, fPtr);
}

// NEW RECORD
void newRecord(FILE *fPtr)
{
    struct clientData client;
    unsigned int account;

    printf("Enter account number (1-100): ");
    scanf("%u", &account);

    fseek(fPtr, (account - 1) * sizeof(client), SEEK_SET);
    fread(&client, sizeof(client), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists\n");
        return;
    }

    printf("Enter last name, first name, balance:\n");
    scanf("%s %s %lf", client.lastName, client.firstName, &client.balance);

    client.acctNum = account;

    fseek(fPtr, (account - 1) * sizeof(client), SEEK_SET);
    fwrite(&client, sizeof(client), 1, fPtr);
}

// DELETE
void deleteRecord(FILE *fPtr)
{
    struct clientData blank = {0, "", "", 0};
    unsigned int account;

    printf("Enter account number to delete: ");
    scanf("%u", &account);

    fseek(fPtr, (account - 1) * sizeof(blank), SEEK_SET);
    fwrite(&blank, sizeof(blank), 1, fPtr);

    printf("Account deleted\n");
}

// DISPLAY ALL
void displayAll(FILE *fPtr)
{
    struct clientData client;
    rewind(fPtr);

    printf("\nAll Accounts:\n");

    while (fread(&client, sizeof(client), 1, fPtr))
    {
        if (client.acctNum != 0)
        {
            printf("%d %s %s %.2f",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.balance);

            if (client.balance < 1000)
                printf(" (Low Balance)");

            printf("\n");
        }
    }
}

// SEARCH
void searchAccount(FILE *fPtr)
{
    struct clientData client;
    unsigned int acc;
    int found = 0;

    printf("Enter account number: ");
    scanf("%u", &acc);

    fseek(fPtr, (acc - 1) * sizeof(client), SEEK_SET);
    fread(&client, sizeof(client), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Found: %d %s %s %.2f\n",
               client.acctNum,
               client.lastName,
               client.firstName,
               client.balance);
        found = 1;
    }

    if (!found)
        printf("Account not found\n");
}

// ADD INTEREST
void addInterest(FILE *fPtr)
{
    struct clientData client;
    rewind(fPtr);

    while (fread(&client, sizeof(client), 1, fPtr))
    {
        if (client.acctNum != 0)
        {
            client.balance += client.balance * 0.05;

            fseek(fPtr, -(long)sizeof(client), SEEK_CUR);
            fwrite(&client, sizeof(client), 1, fPtr);
        }
    }

    printf("Interest added to all accounts\n");
}