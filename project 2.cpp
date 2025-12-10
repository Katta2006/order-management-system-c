#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_ORDERS 100
#define MAX_ITEMS 10
#define NAME_LEN 40
#define ITEM_LEN 40
#define CANCELED_STACK_SIZE 100
typedef struct {
    int id;
    char customer[NAME_LEN];
    int itemCount;
    char items[MAX_ITEMS][ITEM_LEN];
    float prices[MAX_ITEMS];
    float total;
} Order;
Order queueArr[MAX_ORDERS];
int queueCount = 0;
Order canceledStack[CANCELED_STACK_SIZE];
int top = -1;
int nextOrderID = 1;
void flush_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}
void pushCanceled(Order o) {
    if (top < CANCELED_STACK_SIZE - 1)
        canceledStack[++top] = o;
    else
        printf("Canceled order stack full.\n");
}
void placeOrder() {
    if (queueCount >= MAX_ORDERS) {
        printf("Order queue full. Try later.\n");
        return;
    }
    Order o;
    o.id = nextOrderID++;
    printf("Enter customer name: ");
    fgets(o.customer, NAME_LEN, stdin);
    o.customer[strcspn(o.customer, "\n")] = '\0';
    printf("How many items (max %d): ", MAX_ITEMS);
    if (scanf("%d", &o.itemCount) != 1) {
        flush_stdin();
        printf("Invalid input.\n");
        return;
    }
    if (o.itemCount < 1 || o.itemCount > MAX_ITEMS) {
        printf("Invalid item count.\n");
        flush_stdin();
        return;
    }
    flush_stdin();
    o.total = 0.0f;
    for (int i = 0; i < o.itemCount; ++i) {
        printf("Enter item %d name: ", i + 1);
        fgets(o.items[i], ITEM_LEN, stdin);
        o.items[i][strcspn(o.items[i], "\n")] = '\0';
        printf("Enter price for '%s': ", o.items[i]);
        if (scanf("%f", &o.prices[i]) != 1) {
            flush_stdin();
            printf("Invalid price.\n");
            return;
        }
        o.total += o.prices[i];
        flush_stdin();
    }
    queueArr[queueCount++] = o;
    printf("Order placed successfully! Order ID: %d | Customer: %s | Total: %.2f\n",
           o.id, o.customer, o.total);
}
void printOrder(const Order *o) {
    printf("\nOrder ID: %d\nCustomer: %s\nItems: %d\nTotal: %.2f\n",
           o->id, o->customer, o->itemCount, o->total);
    for (int i = 0; i < o->itemCount; ++i) {
        printf("  %d. %s - %.2f\n", i+1, o->items[i], o->prices[i]);
    }
}
void processNextOrder() {
    if (queueCount == 0) {
        printf("No orders to process.\n");
        return;
    }
    Order o = queueArr[0];
    for (int i = 1; i < queueCount; i++)
        queueArr[i - 1] = queueArr[i];

    queueCount--;
    printf("\nProcessing Order:\n");
    printOrder(&o);
}
void cancelOrder() {
    if (queueCount == 0) {
        printf("No orders to cancel.\n");
        return;
    }
    Order o = queueArr[queueCount - 1];
    queueCount--;
    pushCanceled(o);

    printf("Order ID %d (%s) has been canceled and moved to stack.\n", o.id, o.customer);
}
void viewPendingOrders() {
    if (queueCount == 0) {
        printf("No pending orders.\n");
        return;
    }
    printf("\n--- Pending Orders ---\n");
    for (int i = 0; i < queueCount; i++)
        printOrder(&queueArr[i]);
}
void viewCanceledOrders() {
    if (top == -1) {
        printf("No canceled orders.\n");
        return;
    }
    printf("\n--- Canceled Orders (Stack) ---\n");
    for (int i = top; i >= 0; i--)
        printOrder(&canceledStack[i]);
}
int main() {
    int choice;
    while (1) {
        printf("\n=========== ORDER MANAGEMENT SYSTEM ===========\n");
        printf("1. Place Order\n");
        printf("2. Process Next Order\n");
        printf("3. Cancel Last Order\n");
        printf("4. View Pending Orders\n");
        printf("5. View Canceled Orders\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            flush_stdin();
            printf("Invalid input!\n");
            continue;
        }
        flush_stdin();
        switch (choice) {
            case 1: placeOrder(); break;
            case 2: processNextOrder(); break;
            case 3: cancelOrder(); break;
            case 4: viewPendingOrders(); break;
            case 5: viewCanceledOrders(); break;
            case 6: printf("Exiting. Goodbye!\n"); exit(0);
            default: printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}
