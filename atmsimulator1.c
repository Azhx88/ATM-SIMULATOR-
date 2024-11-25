#include <stdio.h>
#include <string.h>

// Define constants
#define MAX_USERS 100
#define MAX_HISTORY 10

// Structure to represent an account in the linked list 
typedef struct User {
    char cardNumber[20];
    char pin[10];
    float balance;
    char transactionHistory[MAX_HISTORY][50];
    int transactionCount;
} User;

// Array to store users
User users[MAX_USERS];
int userCount = 0;

// Function to add a new user
void addUser(const char *cardNumber, const char *pin, float balance) {
    strcpy(users[userCount].cardNumber, cardNumber);
    strcpy(users[userCount].pin, pin);
    users[userCount].balance = balance;
    users[userCount].transactionCount = 0;
    userCount++;
}

// Function to find a user by card number
User* findUser(const char *cardNumber) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].cardNumber, cardNumber) == 0) {
            return &users[i];
        }
    }
    return NULL;
}

// Function to authenticate user
int authenticateUser(User *user, const char *pin) {
    return strcmp(user->pin, pin) == 0;
}

// Function to add a transaction to history
void addTransaction(User *user, const char *transaction) {
    if (user->transactionCount < MAX_HISTORY) {
        strcpy(user->transactionHistory[user->transactionCount], transaction);
        user->transactionCount++;
    }
}
// Function to display the menu
void displayMenu() {
    printf("\nATM Menu\n");
    printf("1. Balance Inquiry\n");
    printf("2. Cash Withdrawal\n");
    printf("3. Cash Deposit\n");
    printf("4. Fund Transfer\n");
    printf("5. Mini Statement\n");
    printf("6. Change PIN\n");
    printf("7. Exit\n");
    printf("Select an option (1-7): ");
}

// Function to handle balance inquiry
void balanceInquiry(User *user) {
    printf("Your current balance is: $%.2f\n", user->balance);
    addTransaction(user, "Balance Inquiry");
}

// Function to handle cash withdrawal
void cashWithdrawal(User *user) {
    float amount;
    printf("Enter amount to withdraw: ");
    scanf("%f", &amount);
    if (amount > user->balance) {
        printf("Insufficient balance.\n");
    } else {
        user->balance -= amount;
        printf("$%.2f withdrawn successfully.\n", amount);
        char transaction[50];
        sprintf(transaction, "Withdrawal: $%.2f", amount);
        addTransaction(user, transaction);
    }
}
// Function to handle cash deposit
void cashDeposit(User *user) {
    float amount;
    printf("Enter amount to deposit: ");
    scanf("%f", &amount);
    user->balance += amount;
    printf("$%.2f deposited successfully.\n", amount);
    char transaction[50];
    sprintf(transaction, "Deposit: $%.2f", amount);
    addTransaction(user, transaction);
}

// Function to handle fund transfer
void fundTransfer(User *user) {
    char targetCardNumber[20];
    float amount;
    printf("Enter recipient card number: ");
    scanf("%s", targetCardNumber);
    User *targetUser = findUser(targetCardNumber);
    if (targetUser == NULL) {
        printf("Recipient not found.\n");
        return;
    }
    printf("Enter amount to transfer: ");
    scanf("%f", &amount);
    if (amount > user->balance) {
        printf("Insufficient balance.\n");
    } else {
        user->balance -= amount;
        targetUser->balance += amount;
        printf("$%.2f transferred successfully to %s.\n", amount, targetCardNumber);
        char transaction[50];
        sprintf(transaction, "Transfer to %s: $%.2f", targetCardNumber, amount);
        addTransaction(user, transaction);
        sprintf(transaction, "Transfer from %s: $%.2f", user->cardNumber, amount);
        addTransaction(targetUser, transaction);
    }
}
// Function to display mini statement

// Function to change PIN
void changePIN(User *user) {
    char newPIN[10];
    printf("Enter new PIN: ");
    scanf("%s", newPIN);
    strcpy(user->pin, newPIN);
    printf("PIN changed successfully.\n");
    addTransaction(user, "PIN Change");
}

// Main function
int main() {
    // Add some users for testing
    addUser("1234567890123456", "1234", 5000.00);
    addUser("0987654321098765", "5678", 3000.00);

    char cardNumber[20], pin[10];
    printf("Enter card number: ");
    scanf("%s", cardNumber);
    
    if (strlen(cardNumber) != 16) {
        printf("Invalid card number. It must be 16 digits long.\n");
        return 1;
    }
    
    User *currentUser = findUser(cardNumber);
    if (currentUser == NULL) {
        printf("Card not found.\n");
        return 1;
    }
    
    printf("Enter PIN: ");
    scanf("%s", pin);
    if (!authenticateUser(currentUser, pin)) {
        printf("Invalid PIN.\n");
        return 1;
    }

    int choice;
    while (1) {
        displayMenu();
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                balanceInquiry(currentUser);
                break;
            case 2:
                cashWithdrawal(currentUser);
                break;
            case 3:
                cashDeposit(currentUser);
                break;
            case 4:
                fundTransfer(currentUser);
                break;
            case 5:
                miniStatement(currentUser);
                break;
            case 6:
                changePIN(currentUser);
                break;
            case 7:
                printf("Thank you for using the ATM Simulator. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid option, please try again.\n");
        }
    }

    return 0;
}