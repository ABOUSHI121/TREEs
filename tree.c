#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int studentID;
    char name[50];
    char city[50];
    int classID;
    char enrollmentDate[10];
} student;

typedef struct node {
    student student;
    struct node *left;
    struct node *right;
} Node;

// Create a new node with the given student data
Node *newNode(student student) {
    Node *node = (Node *) malloc(sizeof(Node));
    node->student = student;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Insert a student into the binary search tree rooted at 'node'
Node *insertStudent(Node *node, student student) {
    // If the node is NULL, create a new node with the given student
    if (node == NULL) {
        return newNode(student);
    }

    // Compare the names of the current node and the new student
    if (strcmp(node->student.name, student.name) > 0) {
        // If the name is greater, recursively insert the student into the left subtree
        node->left = insertStudent(node->left, student);
    } else if (strcmp(node->student.name, student.name) < 0) {
        // If the name is smaller, recursively insert the student into the right subtree
        node->right = insertStudent(node->right, student);
    } else {
        // If the name is equal, update the student data in the current node
        node->student = student;
    }

    return node;
}

Node *searchingNode;  // Global variable to store the found node during traversal

// Helper function for searching a student node by studentID
Node *traverseInOrder(Node *node, int studentID) {
    if (node) {
        // Traverse the left subtree
        traverseInOrder(node->left, studentID);

        // Check if the current node's studentID matches the search studentID
        if (node->student.studentID == studentID)
            searchingNode = node;  // Update the global variable to store the found node

        // Traverse the right subtree
        traverseInOrder(node->right, studentID);
    }
}

// Find a student with the given studentID in the binary search tree rooted at 'node'
Node *findStudent(Node *node, int studentID) {
    searchingNode = NULL;  // Initialize the global variable
    traverseInOrder(node, studentID);  // Start the traversal
    return searchingNode;  // Return the found node or NULL if not found
}

// Update the details of a student with the given studentID
void updateStudent(Node *root, int studentID) {
    // Find the node with the given studentID
    Node *node = findStudent(root, studentID);

    if (node != NULL) {
        printf("enter updated name: \n");
        scanf(" %[^\n]", node->student.name);
        printf("enter updated city: \n");
        scanf(" %[^\n]", node->student.city);
        printf("enter updated class ID: \n");
        scanf(" %d", &(node->student.classID));
        printf("enter updated enrollment date (in format DD-MM-YYYY): \n");
        scanf(" %s", node->student.enrollmentDate);
    } else {
        printf("student with ID %d not found.\n", studentID);
    }
}

// List all the students in the binary search tree rooted at 'root' in ascending order of names
void listStudentsByName(Node *root) {
    if (root != NULL) {
        // Traverse the left subtree
        listStudentsByName(root->left);

        // Print the details of the current node's student
        printf("student ID: %d\n", root->student.studentID);
        printf("name: %s\n", root->student.name);
        printf("city: %s\n", root->student.city);
        printf("class ID: %d\n", root->student.classID);
        printf("enrollment Date: %s\n\n", root->student.enrollmentDate);

        // Traverse the right subtree
        listStudentsByName(root->right);
    }
}

// List all the students in the binary search tree rooted at 'root' with the given city
void listStudentsByCity(Node *root, const char *city) {
    if (root != NULL) {
        // Traverse the left subtree
        listStudentsByCity(root->left, city);

        // Check if the current node's city matches the given city
        if (strcmp(root->student.city, city) == 0) {
            // Print the details of the current node's student
            printf("student ID: %d\n", root->student.studentID);
            printf("name: %s\n", root->student.name);
            printf("city: %s\n", root->student.city);
            printf("class ID: %d\n", root->student.classID);
            printf("enrollment Date: %s\n\n", root->student.enrollmentDate);
        }

        // Traverse the right subtree
        listStudentsByCity(root->right, city);
    }
}

// List all the students in the binary search tree rooted at 'root' with the given classID
void listStudentsByClass(Node *root, int classID) {
    if (root != NULL) {
        // Traverse the left subtree
        listStudentsByClass(root->left, classID);

        // Check if the current node's classID matches the given classID
        if (root->student.classID == classID) {
            // Print the details of the current node's student
            printf("student ID: %d\n", root->student.studentID);
            printf("name: %s\n", root->student.name);
            printf("city: %s\n", root->student.city);
            printf("class ID: %d\n", root->student.classID);
            printf("enrollment Date: %s\n\n", root->student.enrollmentDate);
        }

        // Traverse the right subtree
        listStudentsByClass(root->right, classID);
    }
}

// Find the node with the minimum value in the binary search tree rooted at 'node'
Node *minValueNode(Node *node) {
    Node *current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

// Delete a student from the binary search tree rooted at 'root'
Node *deleteStudent(Node *root, Node *toBeDeleted) {
    if (root == NULL) return root;

    if (strcmp(toBeDeleted->student.name, root->student.name) < 0)
        root->left = deleteStudent(root->left, toBeDeleted);
    else if (strcmp(toBeDeleted->student.name, root->student.name) > 0)
        root->right = deleteStudent(root->right, toBeDeleted);
    else {
        if (root->left == NULL) {
            Node *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node *temp = root->left;
            free(root);
            return temp;
        }

        Node *temp = minValueNode(root->right);

        root->student = temp->student;

        root->right = deleteStudent(root->right, temp);
    }
    return root;
}

// Save the details of all students in the binary search tree rooted at 'root' to a file
void saveStudents(Node *root, FILE *file) {
    if (root != NULL) {
        // Traverse the left subtree
        saveStudents(root->left, file);

        // Write the details of the current node's student to the file
        fprintf(file, "%d,%s,%s,%d,%s\n", root->student.studentID, root->student.name, root->student.city,
                root->student.classID, root->student.enrollmentDate);

        // Traverse the right subtree
        saveStudents(root->right, file);
    }
}

// Load student details from a file and insert them into the binary search tree rooted at 'root'
void loadStudents(Node **root, FILE *file) {
    student temp;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%d,%[^,],%[^,],%d,%s",
               &temp.studentID, temp.name, temp.city, &temp.classID, temp.enrollmentDate);
        *root = insertStudent(*root, temp);
    }
}

void freeTree(Node *node) {
    if (node != NULL) {
        freeTree(node->left);
        freeTree(node->right);
        free(node);
    }
}

int main() {
    // root for the tree
    Node *root = NULL;
    FILE *file = fopen("C:\\Users\\hp\\CLionProjects\\untitled106\\students.txt", "r");
    if (file) {
        puts("students.data exists");
        loadStudents(&root, file);
        puts("file is loaded into the tree successfully");
        fclose(file);
    }
    puts("students.data DNE or is empty!\ntree is empty you must fill it!");

    int choice;
    int studentID;
    int classID;
    char city[50];
    student student;
    Node *temp;
    while (1) {
        printf("1. Insert student\n");
        printf("2. Find student\n");
        printf("3. Update student\n");
        printf("4. List students by name\n");
        printf("5. List students by city\n");
        printf("6. List students by class\n");
        printf("7. Delete student\n");
        printf("8. Save students\n");
        printf("9. Exit\n");
        printf("enter your choice: \n");
        scanf(" %d", &choice);

        switch (choice) {
            case 1:
                printf("enter student ID: \n");
                scanf(" %d", &(student.studentID));
                temp = findStudent(root, student.studentID);
                if (temp){
                    puts("This student already exists!");
                    break;
                }
                printf("enter name: \n");
                scanf(" %s", student.name);
                printf("enter city: \n");
                scanf(" %s", student.city);
                printf("enter class ID: \n");
                scanf(" %d", &(student.classID));
                printf("enter enrollment date (in format DD-MM-YYYY): \n");
                scanf(" %s", student.enrollmentDate);
                root = insertStudent(root, student);
                break;
            case 2:
                printf("enter student ID: \n");
                scanf(" %d", &studentID);
                Node *node = findStudent(root, studentID);
                if (node != NULL) {
                    printf("student found:\n");
                    printf("student ID: %d\n", node->student.studentID);
                    printf("name: %s\n", node->student.name);
                    printf("city: %s\n", node->student.city);
                    printf("class ID: %d\n", node->student.classID);
                    printf("enrollment Date: %s\n\n", node->student.enrollmentDate);
                } else {
                    printf("student not found.\n");
                }
                break;
            case 3:
                printf("enter student ID: \n");
                scanf(" %d", &studentID);
                updateStudent(root, studentID);
                break;
            case 4:
                listStudentsByName(root);
                break;
            case 5:
                printf("enter city: \n");
                scanf(" %[^\n]", city);
                listStudentsByCity(root, city);
                break;
            case 6:
                printf("enter class ID: \n");
                scanf(" %d", &classID);
                listStudentsByClass(root, classID);
                break;
            case 7:
                printf("enter student ID: \n");
                scanf(" %d", &studentID);
                Node *toBeDeleted = findStudent(root, studentID);
                if(!toBeDeleted){
                    printf("student not found.\n");
                    break;
                }
                root = deleteStudent(root, toBeDeleted);
                puts("student is deleted successfully");
                break;
            case 8:
                file = fopen("students.data", "w");
                saveStudents(root, file);
                fclose(file);
                break;
            case 9:
                freeTree(root);
                puts("tree is freed successfully\nterminating...");
                return 0;
            default:
                printf("invalid choice. please enter a number between 1 and 9.\n");
                }
    }
}
