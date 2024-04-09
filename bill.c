#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ITEMS 50
#define MAX_NAME 50
#define MAX_ITEM_NAME 20

struct Item {
    char name[MAX_ITEM_NAME];
    float price;
    int quantity;
};

struct Order {
    char customer[MAX_NAME];
    char date[30];
    int numOfItems;
    struct Item items[MAX_ITEMS];
};

void generateBillHeader(char name[], char date[]) {
    printf("\n\n");
    printf("\t    Couple Restaurant");
    printf("\n\t   -----------------");
    printf("\nDate: %s", date);
    printf("\nInvoice To: %s", name);
    printf("\n");
    printf("---------------------------------------\n");
    printf("Items\t\t");
    printf("Qty\t\t");
    printf("Total\t\t");
    printf("\n---------------------------------------");
    printf("\n\n");
}

void generateBillBody(char item[], int qty, float price) {
    printf("%s\t\t", item);
    printf("%d\t\t", qty);
    printf("%.2f\t\t", qty * price);
    printf("\n");
}

void generateBillFooter(float total) {
    printf("\n");
    float discount = 0.1 * total;
    float netTotal = total - discount;
    float tax = 0.18 * netTotal;
    float grandTotal = netTotal + tax;
    printf("---------------------------------------\n");
    printf("Sub Total\t\t\t%.2f", total);
    printf("\nDiscount @10%%\t\t\t%.2f", discount);
    printf("\n\t\t\t\t-------");
    printf("\nNet Total\t\t\t%.2f", netTotal);
    printf("\nTax @18%%\t\t\t%.2f", tax);
    printf("\n---------------------------------------");
    printf("\nGrand Total\t\t\t%.2f", grandTotal);
    printf("\n---------------------------------------\n");
}

int main() {
    int choice;
    char contFlag = 'y';
    struct Order order;

    while (contFlag == 'y') {
        system("clear");
        printf("\t============Couple RESTAURANT============");
        printf("\n\nPlease select your preferred operation");
        printf("\n\n1.Generate Invoice");
        printf("\n2.Show all Invoices");
        printf("\n3.Search Invoice");
        printf("\n4.Exit");

        printf("\n\nYour choice:\t");
        scanf("%d", &choice);
        fflush(stdin);
        
        switch (choice) {
            case 1:
                system("clear");
                printf("\nPlease enter the name of the customer:\t");
                scanf("%s", order.customer);
                printf("\nPlease enter the date (dd-mm-yyyy):\t");
                scanf("%s", order.date);
                printf("\nPlease enter the number of items:\t");
                scanf("%d", &order.numOfItems);
                for (int i = 0; i < order.numOfItems; i++) {
                    printf("\nPlease enter the item %d name:\t", i + 1);
                    scanf("%s", order.items[i].name);
                    printf("Please enter the quantity:\t");
                    scanf("%d", &order.items[i].quantity);
                    printf("Please enter the unit price:\t");
                    scanf("%f", &order.items[i].price);
                }

                generateBillHeader(order.customer, order.date);
                float total = 0;
                for (int i = 0; i < order.numOfItems; i++) {
                    generateBillBody(order.items[i].name, order.items[i].quantity, order.items[i].price);
                    total += order.items[i].quantity * order.items[i].price;
                }
                generateBillFooter(total);

                FILE *fp = fopen("RestaurantBill.dat", "a");
                if (fp != NULL) {
                    fwrite(&order, sizeof(struct Order), 1, fp);
                    fclose(fp);
                    printf("\nSuccessfully saved\n");
                } else {
                    printf("\nError saving\n");
                }
                break;

            case 2:
                system("clear");
                fp = fopen("RestaurantBill.dat", "r");
                if (fp != NULL) {
                    printf("\n  *****Your Previous Invoices*****\n");
                    while (fread(&order, sizeof(struct Order), 1, fp)) {
                        float tot = 0;
                        generateBillHeader(order.customer, order.date);
                        for (int i = 0; i < order.numOfItems; i++) {
                            generateBillBody(order.items[i].name, order.items[i].quantity, order.items[i].price);
                            tot += order.items[i].quantity * order.items[i].price;
                        }
                        generateBillFooter(tot);
                    }
                    fclose(fp);
                } else {
                    printf("\nNo invoices found\n");
                }
                break;

            case 3:
                system("clear");
                char name[MAX_NAME];
                printf("Enter the name of the customer:\t");
                scanf("%s", name);
                fp = fopen("RestaurantBill.dat", "r");
                if (fp != NULL) {
                    int invoiceFound = 0;
                    while (fread(&order, sizeof(struct Order), 1, fp)) {
                        if (strcmp(order.customer, name) == 0) {
                            float tot = 0;
                            generateBillHeader(order.customer, order.date);
                            for (int i = 0; i < order.numOfItems; i++) {
                                generateBillBody(order.items[i].name, order.items[i].quantity, order.items[i].price);
                                tot += order.items[i].quantity * order.items[i].price;
                            }
                            generateBillFooter(tot);
                            invoiceFound = 1;
                        }
                    }
                    fclose(fp);
                    if (!invoiceFound) {
                        printf("Sorry, the invoice for %s does not exist\n", name);
                    }
                } else {
                    printf("\nNo invoices found\n");
                }
                break;

            case 4:
                printf("\n\t\t Bye Bye :)\n\n");
                exit(0);
                break;

            default:
                printf("Sorry invalid option\n");
                break;
        }
        printf("\nDo you want to perform another operation? [y/n]:\t");
        scanf(" %c", &contFlag);
    }
    printf("\n\t\t Bye Bye :)\n\n");
    return 0;
}
