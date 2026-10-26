
#include <stdio.h>

#define MAX 100
#define MENU_SIZE 10
#define MAX_ITEMS_PER_ORDER 10

struct MenuItem {
    char name[50];
    float price;
};

struct Order {
    int orderNo;
    int itemCount;
    int itemIndex[MAX_ITEMS_PER_ORDER];
    int quantity[MAX_ITEMS_PER_ORDER];
    float totalBill;
    int status;
};

struct MenuItem menu[MENU_SIZE] = {
    {"Dosa", 40},
    {"Idli", 20},
    {"Tea", 15},
    {"Sandwich", 50},
    {"Coffee", 25},
    {"Chicken Biryani", 220},
	{"Chicken Tandoori", 350},
	{"Chicken 65", 200},
	{"Mutton Biryani", 320},
	{"Fish Fry", 220}
   
};

struct Order orders[MAX];
int orderCount = 0;
int nextOrderNumber = 1;

int queue[MAX];
int queueCount = 0;

int stack[MAX];
int top = -1;

void displayMenu()
{
    int i;

    printf("\n========== CAFETERIA MENU ==========\n");

    for (i = 0; i < MENU_SIZE; i++) {
        printf("%2d. %-25s Rs. %.2f\n",
               i + 1, menu[i].name, menu[i].price);
    }

    printf(" 0. Finish selecting items\n");
    printf("====================================\n");
}

int findOrderIndex(int orderNo)
{
    int i;

    for (i = 0; i < orderCount; i++) {
        if (orders[i].orderNo == orderNo)
            return i;
    }

    return -1;
}

void removeFromQueue(int orderNo)
{
    int i, j;

    for (i = 0; i < queueCount; i++) {
        if (queue[i] == orderNo) {
            for (j = i; j < queueCount - 1; j++)
                queue[j] = queue[j + 1];

            queueCount--;
            return;
        }
    }
}

void removeFromStack(int orderNo)
{
    int i;

    for (i = top; i >= 0; i--) {
        if (stack[i] == orderNo) {
            for (; i < top; i++)
                stack[i] = stack[i + 1];

            top--;
            return;
        }
    }
}

void displayOrder(struct Order order)
{
    int i;

    printf("\n========================================\n");
    printf("           COLLEGE CAFETERIA\n");
    printf("----------------------------------------\n");
    printf("Order Number : %d\n", order.orderNo);
    printf("----------------------------------------\n");
    printf("%-23s %5s %10s\n", "Item", "Qty", "Amount");

    for (i = 0; i < order.itemCount; i++) {
        int item = order.itemIndex[i];

        printf("%-23s %5d Rs. %8.2f\n",
               menu[item].name,
               order.quantity[i],
               menu[item].price * order.quantity[i]);
    }

    printf("----------------------------------------\n");
    printf("TOTAL BILL: Rs. %.2f\n", order.totalBill);
    printf("========================================\n");
}

void placeOrder()
{
    struct Order newOrder;
    int choice, quantity;

    if (orderCount >= MAX) {
        printf("\nOrder storage is full!\n");
        return;
    }

    newOrder.itemCount = 0;
    newOrder.totalBill = 0;
    newOrder.status = 1;

    displayMenu();

    while (1) {
        printf("\nEnter item number (0 to finish): ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input!\n");
            return;
        }

        if (choice == 0)
            break;

        if (choice < 1 || choice > MENU_SIZE) {
            printf("Invalid item number!\n");
            continue;
        }

        if (newOrder.itemCount >= MAX_ITEMS_PER_ORDER) {
            printf("Maximum items per order reached!\n");
            break;
        }

        printf("Enter quantity: ");

        if (scanf("%d", &quantity) != 1) {
            printf("Invalid input!\n");
            return;
        }

        if (quantity <= 0) {
            printf("Quantity must be greater than zero!\n");
            continue;
        }

        newOrder.itemIndex[newOrder.itemCount] = choice - 1;
        newOrder.quantity[newOrder.itemCount] = quantity;

        newOrder.totalBill +=
            menu[choice - 1].price * quantity;

        newOrder.itemCount++;
    }

    if (newOrder.itemCount == 0) {
        printf("\nNo items selected. No order was placed.\n");
        return;
    }

    newOrder.orderNo = nextOrderNumber++;

    orders[orderCount] = newOrder;
    orderCount++;

    queue[queueCount++] = newOrder.orderNo;
    stack[++top] = newOrder.orderNo;

    printf("\nOrder placed successfully!\n");
    displayOrder(newOrder);
}

void cancelOrderByNumber()
{
    int orderNo, index;

    printf("\nEnter order number to cancel: ");

    if (scanf("%d", &orderNo) != 1) {
        printf("Invalid input!\n");
        return;
    }

    index = findOrderIndex(orderNo);

    if (index == -1) {
        printf("Order number not found!\n");
        return;
    }

    if (orders[index].status != 1) {
        printf("This order has already been served or cancelled!\n");
        return;
    }

    displayOrder(orders[index]);

    orders[index].status = 3;

    removeFromQueue(orderNo);
    removeFromStack(orderNo);

    printf("\nOrder %d cancelled successfully!\n", orderNo);
}



void cancelLatestOrder()
{
    int orderNo, index;

    if (top == -1) {
        printf("\nNo pending orders to cancel!\n");
        return;
    }

    orderNo = stack[top--];
    index = findOrderIndex(orderNo);

    if (index != -1 && orders[index].status == 1) {
        orders[index].status = 3;
        removeFromQueue(orderNo);

        printf("\nLatest pending order cancelled successfully!\n");
        printf("Cancelled Order Number: %d\n", orderNo);
    }
}

void displayPendingOrders()
{
    int i, index;
    int found = 0;

    printf("\n========== PENDING ORDERS ==========\n");

    for (i = 0; i < queueCount; i++) {
        index = findOrderIndex(queue[i]);

        if (index != -1 && orders[index].status == 1) {
            displayOrder(orders[index]);
            found = 1;
        }
    }

    if (!found)
        printf("No pending orders.\n");
}

void serveNextOrder()
{
    int orderNo, index, i;

    if (queueCount == 0) {
        printf("\nNo pending orders to serve!\n");
        return;
    }

    orderNo = queue[0];

    for (i = 0; i < queueCount - 1; i++)
        queue[i] = queue[i + 1];

    queueCount--;

    index = findOrderIndex(orderNo);

    if (index != -1) {
        orders[index].status = 2;
        removeFromStack(orderNo);

        printf("\nOrder served successfully!\n");
        displayOrder(orders[index]);
    }
}

int main()
{
    int choice;

    do {
        printf("\n====== COLLEGE CAFETERIA SYSTEM ======\n");
        printf("1. Place Order\n");
        printf("2. Cancel Order by Order Number\n");
        printf("3. Cancel Latest Order (Stack)\n");
        printf("4. Display Pending Orders\n");
        printf("5. Serve Next Order (Queue)\n");
        printf("6. Display Menu\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Restart the program.\n");
            return 1;
        }

        switch (choice) {
            case 1:
                placeOrder();
                break;
            case 2:
                cancelOrderByNumber();
                break;
            case 3:
                cancelLatestOrder();
                break;
            case 4:
                displayPendingOrders();
                break;
            case 5:
                serveNextOrder();
                break;
            case 6:
                displayMenu();
                break;
            case 7:
                printf("Thank you for using the cafeteria system!\n");
                break;
            default:
                printf("Invalid choice!\n");
        }

    } while (choice != 7);

    return 0;
}
