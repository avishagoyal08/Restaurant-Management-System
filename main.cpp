#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include<mmsystem.h>
#include <string.h> //header file to include string
#include <time.h>
#include <conio.h>
#include <fstream>
#include <string>
#include <sstream>
#include <bits/stdc++.h>
const int infinity = INT_MAX;
using namespace std;


// dish Shop Ordering Management System

void gotoxy(int x, int y) // function to manage cursor
{
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
// Defining Data Types

struct customer
{
    int age;
    string name;
    string dishName;
    int quantity;
    double bill;

    customer() {}

    customer(int age, string name, int quantity, string dishName, double bill)
    {
        this->age = age;
        this->name = name;
        this->dishName = dishName;
        this->quantity = quantity;
        this->bill = bill;
    }
};

struct takeAwayCust
{
    customer cust;
    takeAwayCust *next = NULL;

    takeAwayCust(int age, string name, int quantity, string dishName, double bill)
    {
        this->cust = customer(age, name, quantity, dishName, bill);
    }
};

struct dineInCustomer
{
    customer cust;
    dineInCustomer *next = NULL;

    dineInCustomer(int age, string name, int quantity, string dishName, double bill)
    {
        this->cust = customer(age, name, quantity, dishName, bill);
    }
};

struct homeDeliveryCustomer
{

    customer cust;
    string address;
    double deliveryCharges;
    int distanceDelivery;
    struct homeDeliveryCustomer *next = NULL;

    homeDeliveryCustomer(int age, string name, int quantity, string dishName, double bill, string address, double deliveryCharges, int distanceDelivery)
    {
        this->cust = customer(age, name, quantity, dishName, bill);
        this->address = address;
        this->deliveryCharges = deliveryCharges;
        this->distanceDelivery = distanceDelivery;
    }
};

struct Restaurant
{

    string shopName;
    string *menu;
    int *price;
    string address;
    takeAwayCust *nexttakeAwayCust = NULL;
    dineInCustomer *nextDineInCustomer = NULL;
    homeDeliveryCustomer *nextHomeDeliveryCustomer = NULL;
};

// Globally declaring the dish Shop's pointer
Restaurant *myRestaurant = NULL;

// Globally Declaring the Current Customer's Pointers for all three Types
takeAwayCust *currenttakeAwayCust = NULL;
dineInCustomer *currentDineInCustomer = NULL;
homeDeliveryCustomer *currentHomeDeliveryCustomer = NULL;

// Globally declaring the variables for the total of all the orders in queue!
double total, takeAway, dineIn, homeDelivery;
// Globally declaring the variables for the total of all the orders served!
double servedTotal;

// In case of Serving , to keep the record of Customers Served, implementing AVL Tree for efficient Search
// to search the record of Customers by Name
// It can also Display all the customers Served

struct servedCustomer
{

    int age;
    string name;
    int quantity;
    string dishName;
    double bill;
    string customerType;
    servedCustomer *left;
    servedCustomer *right;

    // Constructor
    servedCustomer(int age, string name, int quantity, string dishName, double bill, string customerType)
    {
        // setting customers details

        this->age = age;
        this->name = name;
        this->quantity = quantity;
        this->dishName = dishName;
        this->bill = bill;
        this->customerType = customerType;

        // child to NULL
        this->left = NULL;
        this->right = NULL;
    }
};

servedCustomer *root = NULL; // Global pointer for the root of AVLTree

// isEmpty or not
int isEmpty(servedCustomer *root)
{
    return root == NULL;
}

// display Customers Details

void display(servedCustomer *root)
{
    gotoxy(40, 25);
    cout << "Name :" << root->name << endl;
    gotoxy(40, 26);
    cout << "Age  :" << root->age << endl;
    gotoxy(40, 27);
    cout << "Dish name :" << root->dishName << endl;
    gotoxy(40, 28);
    cout << "Quantity : " << root->quantity << endl;
    gotoxy(40, 29);
    cout << "Bill : " << root->bill << endl;
    gotoxy(40, 30);
    cout << "Customer Type: " << root->customerType << endl;
    gotoxy(40, 30);
    getch();
}

// Height of servedCustomer tree

int height(servedCustomer *root)
{
    if (!root)
        return 0;

    return max(height(root->left), height(root->right)) + 1;
}

// Balance Factor for each ServedCustomer node

int balanceFactor(servedCustomer *root)
{
    if (!root)
        return 0;

    return height(root->left) - height(root->right);
}

// Maximum of two integers as a helper function for height
int max(int a, int b)
{
    return (a > b) ? a : b;
}

// Searching in servedCustomer tree

servedCustomer *search(servedCustomer *root, string keyName)
{
    if (root == NULL)
    {
        return NULL;
    }
    else if (root->name == keyName)
    {
        return root;
    }
    else if (root->name < keyName)
    {
        return search(root->right, keyName);
    }
    else if (root->name > keyName)
    {
        return search(root->left, keyName);
    }

    return root;
}

// Finding Maximum Node of servedCustomer tree

servedCustomer *maxservedCustomer(servedCustomer *root)
{
    // Maximum Node is Present in the most Right Node  of the served Customer Tree

    servedCustomer *p = root;
    servedCustomer *temp = NULL;

    while (p != NULL)
    {
        temp = p;
        p = p->right;
    }

    return temp;
}

// Balancing the ServedCustomer's Tree thorugh AVL Rotations

// LL Rotation
servedCustomer *LLRotation(servedCustomer *root)
// rotate wese right per krna hai!
{
    // saving the new root and the lost element in case of rotation
    servedCustomer *x = root->left;
    servedCustomer *temp = x->right;

    // Performing rotation
    x->right = root;
    root->left = temp;

    // updating the root
    root = x;
    // returning the root
    return x;
}

// RR Rotation
servedCustomer *RRRotation(servedCustomer *root)
{
    // rotate wese left per krna hai!

    // saving the new root and the lost element in case of rotation

    servedCustomer *x = root->right;
    servedCustomer *temp = x->left;

    // Performing rotation
    x->left = root;
    root->right = temp;

    // updating the root
    root = x;

    // returning the root
    return x;
}

// LR Rotation
servedCustomer *LRRotation(servedCustomer *root)
{
    root->left = RRRotation(root->left);
    return LLRotation(root);
}

// RL Rotation
servedCustomer *RLRotation(servedCustomer *root)
{
    root->right = LLRotation(root->right);
    return RRRotation(root);
}

//  INSERTION in servedCustomer Tree

servedCustomer *insertion(int age, string name, int quantity, string dishName, double bill, string customerType, servedCustomer *root)
{
    servedCustomer *newNode = new servedCustomer(age, name, quantity, dishName, bill, customerType);

    if (root == NULL)
    {
        root = newNode;
    }

    else if (root->name > newNode->name)
    {
        root->left = insertion(age, name, quantity, dishName, bill, customerType, root->left);
    }
    else if (root->name < newNode->name)
    {
        root->right = insertion(age, name, quantity, dishName, bill, customerType, root->right);
    }

    else
    {
        cout << "No duplicates Values are Allowed " << endl;
        return root;
    }

    int bf = balanceFactor(root);

    if (bf == 2)
    {
        // LL
        if (root->left->name > newNode->name)
        {
            return LLRotation(root);
        }

        // LR
        if (root->left->name < newNode->name)
        {
            return LRRotation(root);
        }
    }
    else if (bf == -2)
    {
        // RR
        if (root->right->name < newNode->name)
        {
            return RRRotation(root);
        }

        // RL
        if (root->right->name > newNode->name)
        {
            return RLRotation(root);
        }
    }

    return root; // in case there is no need of rotation
}

// Now defining Order Placing and Serving of Take Away Customer
// Based on : Older person will be served first (PRIORITY QUEUE)

void placeOrdertakeAwayCust(int age, string name, string dishName, int quantity, double bill)
{
    // making new Customer
    currenttakeAwayCust = new takeAwayCust(age, name, quantity, dishName, bill);

    if (myRestaurant->nexttakeAwayCust == NULL)
    {
        // if first then insert in front
        myRestaurant->nexttakeAwayCust = currenttakeAwayCust;
    }
    else
    {
        // finding the last Node
        takeAwayCust *temp = myRestaurant->nexttakeAwayCust;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        if (temp->cust.age < currenttakeAwayCust->cust.age)
        {
            // insert at front
            takeAwayCust *firstCustomer = myRestaurant->nexttakeAwayCust;
            myRestaurant->nexttakeAwayCust = currenttakeAwayCust;
            currenttakeAwayCust->next = firstCustomer;
        }
        else
        {
            // insert at end
            temp->next = currenttakeAwayCust;
            currenttakeAwayCust->next = NULL;
        }
    }
    gotoxy(5, 30);
    cout << "Your Order has been Placed MR/MRS " << name << " and your order is " << dishName << " with " << quantity << " quantity and total bill is " << bill << endl;
    gotoxy(40, 33);
    cout << "Enter any key to continue.....";
    getch();
}
void serveOrdertakeAwayCust()
{
    if (myRestaurant->nexttakeAwayCust == NULL)
    {
        gotoxy(40, 25);
        cout << "No Take Away Customer to Serve" << endl;
        gotoxy(40, 27);
        cout << "Enter any key to continue.....";
        getch();
    }
    else
    {
        // serving the first customer
        takeAwayCust *temp = myRestaurant->nexttakeAwayCust;
        // if it has some next element
        if (temp->next != NULL)
        {
            myRestaurant->nexttakeAwayCust = temp->next;
        }
        else
        {
            myRestaurant->nexttakeAwayCust = NULL;
        }
        gotoxy(40, 25);
        cout << "Take Away Customer Served : " << temp->cust.name << endl;

        string customerType = "Take-Away";
        // Now before deleting the node we need to update the servedCustomer Tree
        root = insertion(temp->cust.age, temp->cust.name, temp->cust.quantity, temp->cust.dishName, temp->cust.bill, customerType, root);

        delete temp; // deleting the customer
        gotoxy(40, 27);
        cout << "Enter any key to continue.....";
        getch();
    }
}

// Now defining Order Placing and Serving of Dine-In Customer
// Based on : First Come First Served (FIFO) (QUEUE)

void placeOrderDineInCustomer(int age, string name, string dishName, int quantity, double bill)
{
    // making new Customer
    currentDineInCustomer = new dineInCustomer(age, name, quantity, dishName, bill);

    if (myRestaurant->nextDineInCustomer == NULL)
    {
        // if first insert in front
        myRestaurant->nextDineInCustomer = currentDineInCustomer;
    }
    else
    {
        // finding the last Node
        dineInCustomer *temp = myRestaurant->nextDineInCustomer;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = currentDineInCustomer;
        currentDineInCustomer->next = NULL;
    }
    gotoxy(5, 30);
    cout << "Your Order has been Placed MR/MRS " << name << " and your order is " << dishName << " with " << quantity << " quantity and total bill is " << bill << endl;
    gotoxy(40, 32);
    cout << "Enter any key to continue.....";
    getch();
}
void serveOrderDineInCustomer()
{
    if (myRestaurant->nextDineInCustomer == NULL)
    {
        gotoxy(40, 25);
        cout << "No Dine-In Customer to Serve" << endl;
        gotoxy(40, 27);
        cout << "Enter any key to continue.....";
        getch();
    }
    else
    {
        // serving the first customer
        dineInCustomer *temp = myRestaurant->nextDineInCustomer;
        if (temp->next != NULL)
        {
            myRestaurant->nextDineInCustomer = temp->next;
        }
        else
        {
            myRestaurant->nextDineInCustomer = NULL;
        }
        gotoxy(40, 25);
        cout << "Dine-In Customer Served : " << temp->cust.name << endl;

        string customerType = "Dine-In";
        // Now before deleting the node we need to update the servedCustomer Tree
        root = insertion(temp->cust.age, temp->cust.name, temp->cust.quantity, temp->cust.dishName, temp->cust.bill, customerType, root);

        delete temp; // deleting the customer
        gotoxy(40, 27);
        cout << "Enter any key to continue.....";
        getch();
    }
}

// Now defining Order Placing and Serving of Home Delivery Customer
// Based on : (when all orders are ready)(LIFO)(Stack)

void placeOrderHomeDeliveryCustomer(int age, string name, string dishName, int quantity, double bill, string address, int deliveryCharges, int distanceDelivery)
{
    // making new Customer
    currentHomeDeliveryCustomer = new homeDeliveryCustomer(age, name, quantity, dishName, bill, address, deliveryCharges, distanceDelivery);

    if (myRestaurant->nextHomeDeliveryCustomer == NULL)
    {
        // if first insert in front
        myRestaurant->nextHomeDeliveryCustomer = currentHomeDeliveryCustomer;
    }
    else
    {
        // finding the last Node
        homeDeliveryCustomer *temp = myRestaurant->nextHomeDeliveryCustomer;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = currentHomeDeliveryCustomer;
        currentHomeDeliveryCustomer->next = NULL;
    }
    gotoxy(5, 44);
    cout << "Your Order has been Placed MR/MRS " << name << " and your order is " << dishName << " with " << quantity << " quantity and total bill is " << bill << endl;
    gotoxy(25, 46);
    cout << "Enter any key to continue.....";
    getch();
}

void serveOrderHomeDeliveryCustomer()
{
    if (myRestaurant->nextHomeDeliveryCustomer == NULL)
    {
        gotoxy(40, 25);
        cout << "No Home Delivery Customer to Serve" << endl;
        gotoxy(40, 27);
        cout << "Enter any key to continue.....";
        getch();
    }
    else
    {

        // serving the last customer first
        homeDeliveryCustomer *first = myRestaurant->nextHomeDeliveryCustomer;

        if (first->next == NULL)
        {
            // if it is the only customer

            myRestaurant->nextHomeDeliveryCustomer = NULL;
            gotoxy(40, 25);

            cout << "Home Delivery Customer Served : " << first->cust.name << endl;
            string customerType = "Home-Delivery Customer";
            root = insertion(first->cust.age, first->cust.name, first->cust.quantity, first->cust.dishName, first->cust.bill, customerType, root);

            // now deleting the node
            delete (first);
        }
        else
        {
            homeDeliveryCustomer *s = first->next;
            while (s->next != NULL)
            {
                first = first->next;
                s = s->next;
            }

            first->next = NULL;
            gotoxy(40, 25);
            cout << "Home Delivery Customer Served : " << s->cust.name << endl;
            string customerType = "Home-Delivery Customer";
            root = insertion(s->cust.age, s->cust.name, s->cust.quantity, s->cust.dishName, s->cust.bill, customerType, root);

            // deleting the node

            delete (s);
        }

        // deleting the customer
        gotoxy(40, 27);
        cout << "Enter any key to continue.....";
        getch();
    }
}



void displayAllOrderstakeAwayCusts()
{
    int j;
    if (myRestaurant->nexttakeAwayCust == NULL)
    {
        gotoxy(40, 25);
        cout << "There is no Order for Walking Customer till yet" << endl;
        gotoxy(40, 27);
        cout << "Enter any key to continue.....";
        getch();
    }
    else
    {
        takeAwayCust *traversal = myRestaurant->nexttakeAwayCust;
        j = 25;
        while (traversal != NULL)
        {
            gotoxy(40, j);

            cout << "-----------------------------------------------------" << endl;
            gotoxy(40, j++);
            cout << "Take-Away Customer : " << traversal->cust.name << endl;
            gotoxy(40, j++);
            cout << "Age : " << traversal->cust.age << endl;
            gotoxy(40, j++);
            cout << "Dish Name : " << traversal->cust.dishName << endl;
            gotoxy(40, j++);
            cout << "Quantity : " << traversal->cust.quantity << endl;
            gotoxy(40, j++);
            cout << "Bill : " << traversal->cust.bill << " RS/_" << endl;
            gotoxy(40, j++);
            cout << "-----------------------------------------------------" << endl;
            gotoxy(40, j++);

            traversal = traversal->next;
        }
        gotoxy(40, j++);
        cout << "Enter any key to continue.....";
        getch();
    }
}

void displayAllOrdersHomeDeliveryCustomers()
{
    int j;
    if (myRestaurant->nextHomeDeliveryCustomer == NULL)
    {
        gotoxy(40, 25);
        cout << "There is no Order for Home Delivery Customer till yet" << endl;
        gotoxy(40, 27);
        cout << "Enter any key to continue.....";
        getch();
    }
    else
    {
        homeDeliveryCustomer *traversal = myRestaurant->nextHomeDeliveryCustomer;
        j = 25;
        while (traversal != NULL)
        {
            gotoxy(40, j);

            cout << "-----------------------------------------------------" << endl;
            gotoxy(40, j++);

            cout << "Home Delivery Customer : " << traversal->cust.name << endl;
            gotoxy(40, j++);
            cout << "Age : " << traversal->cust.age << endl;
            gotoxy(40, j++);
            cout << "Dish Name : " << traversal->cust.dishName << endl;
            gotoxy(40, j++);
            cout << "Quantity : " << traversal->cust.quantity << endl;
            gotoxy(40, j++);
            cout << "Delivery Distance : " << traversal->deliveryCharges << "KM" << endl;
            gotoxy(40, j++);
            cout << "Delivery Charges : " << traversal->distanceDelivery << endl;
            gotoxy(40, j++);
            cout << "Bill : " << traversal->cust.bill << " RS/_" << endl;
            gotoxy(40, j++);
            cout << "Delivery Address : " << traversal->address << endl;
            gotoxy(40, j++);
            cout << "-----------------------------------------------------" << endl;
            gotoxy(40, j++);

            traversal = traversal->next;
        }
        gotoxy(40, j++);
        cout << "Enter any key to continue.....";
        getch();
    }
}

void displayAllOrdersDineInCustomers()
{
    int j;
    if (myRestaurant->nextDineInCustomer == NULL)
    {
        gotoxy(40, 25);
        cout << "There is no Order for Dine-In Customer till yet" << endl;
        gotoxy(40, 27);
        cout << "Enter any key to continue.....";
        getch();
    }
    else
    {
        j=25;
        dineInCustomer *traversal = myRestaurant->nextDineInCustomer;
        while (traversal != NULL)
        {
            gotoxy(40, j);
            cout << "-----------------------------------------------------" << endl;
            gotoxy(40, j++);
            cout << "Walking Customer : " << traversal->cust.name << endl;
            gotoxy(40, j++);
            cout << "Age : " << traversal->cust.age << endl;
            gotoxy(40, j++);
            cout << "Dish Name : " << traversal->cust.dishName << endl;
            gotoxy(40, j++);
            cout << "Quantity : " << traversal->cust.quantity << endl;
            gotoxy(40, j++);
            cout << "Bill : " << traversal->cust.bill << " RS/_" << endl;
            gotoxy(40, j++);
            cout << "-----------------------------------------------------" << endl;
            gotoxy(40, j++);

            traversal = traversal->next;
        }
        gotoxy(40, j++);
        cout << "Enter any key to continue.....";
        getch();
    }
}

void totalbillofPendingOrders()
{
    takeAwayCust *p = myRestaurant->nexttakeAwayCust;
    while (p != NULL)
    {
        takeAway += p->cust.bill;
        p = p->next;
    }
    dineInCustomer *q = myRestaurant->nextDineInCustomer;
    while (q != NULL)
    {
        dineIn += q->cust.bill;
        q = q->next;
    }
    homeDeliveryCustomer *r = myRestaurant->nextHomeDeliveryCustomer;
    while (r != NULL)
    {
        homeDelivery += r->cust.bill;
        r = r->next;
    }
    total = takeAway + dineIn + homeDelivery;
    gotoxy(40, 25);
    cout << "The total bill of pending orders for Take-Away customers are : " << takeAway << " RS/_" << endl;
    gotoxy(40, 26);
    cout << "The total bill of pending orders for Dine-In customers are : " << dineIn << " RS/_" << endl;
    gotoxy(40, 27);
    cout << "The total bill of pending orders for Delivery customers are : " << homeDelivery << " RS/_" << endl;
    gotoxy(40, 28);
    cout << "The Total orders pending are : " << total << " RS/_" << endl;
    getch();
}

double totalEarnings(servedCustomer *root)
{

    if (root)
    {
        totalEarnings(root->left);
        servedTotal += root->bill;
        totalEarnings(root->right);
    }

    return servedTotal;
}

// making a graph for the available delivery options

//                           0              1             2        3          4           5
string deliveryPoints[] = {"Restaurant", "Dwarka", "Lajpat Nagar", "Mohanagar", "JoharTown", "Ashok Nagar", "Rajiv Chowk", "Moti Nagar", "Botanical Garden", "Karol Bagh", "Chandini Chowk"};

// first value in the pair is vertex and second is the distance (weight) in KM

vector<vector<pair<int, int>>> deliveryMap = {

    // first value in the pair is vertex and second is the distance (weight) in KM
    {{1, 2}, {2, 3}, {3, 5}, {5, 4}, {6,3}, {10,1}},  //  0  (Restaurant)
    {{0, 2}, {5, 1}, {10,2}},                 //  1  (Dwarka)
    {{0, 3}, {3, 1}},                         //  2  (Lajpat Nagar)
    {{0, 5}, {4, 2}, {5, 2}, {2, 1}},         //  3  (Mohanagar)
    {{3, 2}, {5, 2}},                         //  4  (Rajiv Chowk)
    {{0, 4}, {1, 1}, {3, 2}, {4, 2}, {9,2}},  //  5  (Ashok Nagar)
    {{0,3}, {7,2}, {8,3}, {10,1}},             // 6  (Rajiv Chowk)
    {{6,2}, {8,4}},                            // 7  (Moti Nagar)
    {{6,3}, {7,4}, {9,3}},                     // 8  (Botanical Garden)
    {{5,2}, {8,3}, {10,1}},                    // 9   (Karol Bagh)
    {{0,1}, {1,2}, {6,1}, {9,1}}               // 10  (Chandini Chowk)

};

vector<int> dijkstra(int sourceNode)
{
    vector<int> distance(11, infinity);
    set<pair<int, int>> s;
    distance[sourceNode] = 0; // dish Shop
    s.insert(make_pair(0, sourceNode));

    while (!s.empty())
    {
        auto top = *(s.begin());
        int u = top.first;  //   current weight
        int v = top.second; //  current vertex

        s.erase(s.begin());

        // traversing the adjacency list of v
        for (auto child : deliveryMap[v])
        {
            int childVertex = child.first;
            int childWeight = child.second;

            if (u + childWeight < distance[childVertex])
            {
                distance[childVertex] = u + childWeight;
                s.insert(make_pair(distance[childVertex], childVertex));
            }
        }
    }

    return distance;
}

int main()
{
    // making dish shop
    myRestaurant = new Restaurant;

    // setting the name
    myRestaurant->shopName = "RajMahal Restaurant";

    // setting the address
    myRestaurant->address = "Sector 62, Noida";

    // Setting the menu
    myRestaurant->menu = new string[11]{"",
                                        "French Fries", "Grilled Sandwich",
                                        "Veg. Burger", "Dosa",
                                        "Tomato Soup", "Veg. Noodle Soup",
                                        "Spring Roll", "Chilly Paneer Dry",
                                        "Veg Hakka Noodles", "Pav Bhaji"};

    // setting the price

    myRestaurant->price = new int[11]{0, 200, 320, 130, 400, 350, 400, 270, 500, 290, 310};

    int option = -99;
    PlaySound(TEXT("speech3.wav"),NULL,SND_SYNC);

    // now starting the main program
    do
    {
        system("cls");


        gotoxy(25, 6);
        printf("  **-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**");
        gotoxy(25, 7);
        printf("  **-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**");
        gotoxy(25, 8);
        printf("        =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");
        gotoxy(25, 9);

        printf("        =                RAJMAHAL                  =");
        gotoxy(25, 10);
        printf("        =               RESTAURANT                 =");
        gotoxy(25, 11);
        cout << "        =         (Located at " << myRestaurant->address << ")    =";

        gotoxy(25, 12);
        printf("        =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");
        gotoxy(25, 13);
        printf("  **-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**");
        gotoxy(25, 14);
        printf("  **-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**");
        gotoxy(28, 25);

        getch();
        system("cls");
        gotoxy(8, 8);
        cout << "Our Menu is as follows: " << endl;
        //PlaySound(TEXT("speech4.wav"),NULL,SND_SYNC);
        for (int i = 1; i <= 10; i++)
        {
            gotoxy(8, i + 8);
            cout << i << ". " << myRestaurant->menu[i] << " - " << myRestaurant->price[i] << endl;
        }
        getch();
        // system("cls");
        gotoxy(25, 2);
        cout << "-------------------------------------------------------------------------" << endl;
        gotoxy(25, 3);
        cout << "---------------------------------Operations------------------------------" << endl;
        gotoxy(25, 4);
        cout << "-------------------------------------------------------------------------" << endl;
        gotoxy(25, 5);
        cout << "-------------------------------------------------------------------------" << endl;

        // Order placing
        gotoxy(40, 6);
        cout << "1. Place order for Take-Away Customer" << endl;
        gotoxy(40, 7);
        cout << "2. Place order for Home Delivery Customer" << endl;
        gotoxy(40, 8);
        cout << "3. Place order for Dine-In Customer" << endl;

        // order serving
        gotoxy(40, 9);
        cout << "4. Serve order for Take-Away Customer" << endl;
        gotoxy(40, 10);
        cout << "5. Serve order for Home Delivery Customer" << endl;
        gotoxy(40, 11);
        cout << "6. Serve order for Dine-In Customer" << endl;
        gotoxy(40, 12);
        // cout << "7. Serve All Orders " << endl;

        // Displaying orders
        // gotoxy(40, 13);
        cout << "7. Display all orders of Take-Away Customer" << endl;
        gotoxy(40, 13);
        cout << "8. Display all orders of Home Delivery Customers" << endl;
        gotoxy(40, 14);
        cout << "9. Display all orders of Dine-In Customers" << endl;
        gotoxy(40, 15);
        cout << "10. Search Served Orders " << endl;
        gotoxy(40, 16);
        cout << "11. Display total bill of Pending Orders " << endl;
        gotoxy(40, 17);
        cout << "12. Display the total Earnings of Served Orders " << endl;
        gotoxy(40, 18);
        cout << "0.  EXIT " << endl;
        gotoxy(40, 22);
        cout << "Enter your choice: ";
        // gotoxy(40, 24);

        cin >> option;

        // for taking input of Customer Details
        int age, quantity, dishIndex;
        double bill;
        string address;
        string name;

        switch (option)
        {
        case 1:
        { // placing order for take away customer
            gotoxy(40, 25);
            cout << "Enter the name of the customer: ";
            cin >> name;
            gotoxy(40, 26);
            cout << "Enter the age of the customer: ";
            cin >> age;
            gotoxy(40, 27);
            cout << "Enter the quantity of the dish: ";
            cin >> quantity;
            gotoxy(40, 28);
            cout << "Enter the option for the dish: ";
            cin >> dishIndex;

            bill = quantity * myRestaurant->price[dishIndex];
            placeOrdertakeAwayCust(age, name, myRestaurant->menu[dishIndex], quantity, bill);
            PlaySound(TEXT("speech1.wav"),NULL,SND_SYNC);
        }
        break;

        case 2:

        {                                           // placing order for Home Delivery customer
            vector<int> distFromRest = dijkstra(0); // As Zero is our Source Node

            int optionDelivery = -999, j;

            do
            {
                gotoxy(25, 25);
                cout << "The delivery is available for following Areas : " << endl;
                j = 26;
                for (int i = 1; i <= 10; i++)
                {
                    gotoxy(25, j);
                    cout << i << ". " << deliveryPoints[i] << endl;
                    j++;
                }
                gotoxy(25, 37);

                cout << "Enter your option : ";
                cin >> optionDelivery;

            } while (!(optionDelivery >= 0 && optionDelivery <= 10));

            // setting the delivery address of the Customer
            address = deliveryPoints[optionDelivery];
            gotoxy(25, 39);
            cout << "Enter the name of the customer: ";
            cin >> name;
            gotoxy(25, 40);
            cout << "Enter the age of the customer: ";
            cin >> age;
            gotoxy(25, 41);
            cout << "Enter the quantity of the dish: ";
            cin >> quantity;
            gotoxy(25, 42);
            cout << "Enter the option for the dish: ";
            cin >> dishIndex;

            int deliveryChargesPerKM = 100;
            int deliveryCharges = deliveryChargesPerKM * distFromRest[optionDelivery];
            bill = quantity * myRestaurant->price[dishIndex] + deliveryCharges;

            // distance from the shop
            int distanceFromTheShop = distFromRest[optionDelivery];
            placeOrderHomeDeliveryCustomer(age, name, myRestaurant->menu[dishIndex], quantity, bill, address, distanceFromTheShop, deliveryCharges);
            PlaySound(TEXT("speech1.wav"),NULL,SND_SYNC);
        }
        break;

        case 3:
        { // placing order for Dine-in customer
            gotoxy(40, 25);
            cout << "Enter the name of the customer: ";
            cin >> name;
            gotoxy(40, 26);
            cout << "Enter the age of the customer: ";
            cin >> age;
            gotoxy(40, 27);
            cout << "Enter the quantity of the dish: ";
            cin >> quantity;
            gotoxy(40, 28);
            cout << "Enter the option for the dish: ";
            cin >> dishIndex;

            bill = quantity * myRestaurant->price[dishIndex];
            placeOrderDineInCustomer(age, name, myRestaurant->menu[dishIndex], quantity, bill);
            PlaySound(TEXT("speech1.wav"),NULL,SND_SYNC);
        }
        break;

        case 4:
            // serving order for Take Away customer
            serveOrdertakeAwayCust();
            break;

        case 5:
            // serving order for Home Delivery customer
            serveOrderHomeDeliveryCustomer();
            break;

        case 6:
            // serving order for Dine-in customer
            serveOrderDineInCustomer();
            break;


        case 7:
            // displaying all orders of Take-away customers
            displayAllOrderstakeAwayCusts();
            break;

        case 8:
            // displaying all orders of Home Delivery customers
            displayAllOrdersHomeDeliveryCustomers();
            break;

        case 9:
            // displaying all orders of Dine-in customers
            displayAllOrdersDineInCustomers();
            break;


        case 10:
        { // searching served orders
            gotoxy(40, 24);
            cout << "Enter the name of the customer you want to search: ";
            cin >> name;
            servedCustomer *searchedCustomer = search(root, name);
            gotoxy(40, 25);
            if (searchedCustomer == NULL)
                cout << "No such Customer was Served " << endl;
            else
                display(searchedCustomer);
            getch();
        }
        break;

        case 11:
        { // pending orders bill in queue for all!

            // resetting the state of total orders
            takeAway = 0, dineIn = 0, homeDelivery = 0, total = 0;
            totalbillofPendingOrders();
        }
        break;

        case 12:
        { // total earnings from served orders!

            // resetting the state of total served orders
            servedTotal = 0;
            double totalx = totalEarnings(root);
            gotoxy(40, 25);
            cout << "The Total Earnings are : " << totalx << endl;
            getch();
        }
        break;
        }

    } while (option != 0);

    cout << "Thank you for using our Services " << endl;
    PlaySound(TEXT("speech2.wav"),NULL,SND_SYNC);

    return 0;
}
