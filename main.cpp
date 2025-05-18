#include "fileSystems.cpp"
#include "accessControl.cpp"
#include <iostream>
#include <cstdlib> 
#include <unistd.h>

using namespace std;

string runLengthEncode(const string& input) {
    string result = ""; 
    int count = 1;      
    for (int i = 1; i < input.length(); i++) {
        if (i < input.length() && input[i] == input[i - 1]) {
            count++;  
        } else {
            result += to_string(count);
            result += input[i - 1];
            count = 1;
        }
    }

    return result;
}

void loadingBar() {
    const char bar1 = '=';
    const char bar2 = '#';

    cout << "\033[1;33m";  // Bright Yellow
    cout << "\033[40m";    // Black background

    cout << "\033[2J\033[1;1H"; 

    cout << "\n\n\n\t\t\t      LOADING...";
    cout << "\n\n\n\t\t\t\t";

    for (int i = 0; i < 40; i++)
        cout << bar1;
    cout << "\r";
    cout << "\t\t\t\t";
    cout.flush();

    for (int i = 0; i < 40; i++) {
        cout << bar2 << flush;
        usleep(100000);
    }

    cout << endl << endl;
    cout << "\t\t\t\t";
    cout << "\033[0m"; // Reset the background

    cout << "Press Enter to continue...";
    cin.ignore();
    cin.get();

    cout << "\033[2J\033[1;1H";
}



int main() {
    system("clear");
    fileSystem fs;
    char choice;
    AccessControl ac;
    string username, password, securityAnswer;
    string role;
    string name,fromUser, toUser, action;


    cout << "\n\n\n\n\n\n\n\n";
    cout << "\t\t\t\tGOOGLE DRIVE FILE SYSTEM\n";
    cout << "\t\t\t\t==========================\n";
    cout << "\n\nPress any key to continue...";
    cin.get(); 
    system("clear");
    loadingBar();

    bool isLoggedIn = false;
    string currentUser;
    do {
        if (!isLoggedIn) {
            cout << "\n==== Google Drive File System ===="
                 << "\n---- User Authentication ----" 
                 << "\nA. Register User"
                 << "\nB. Login"
                 << "\nC. Recover Password"
                 << "\nD. Register User and role for that user"
                 << "\n0. Exit"
                 << "\nSelect an option: ";
            cin >> choice;
            char inputMenu = choice;
            
            choice = toupper(choice);
            system("clear");

            switch (choice) {
                case 'A': {  // Register
                    cout << "\n[Register]\nUsername: ";
                    cin >> username;
                    cout << "Password: ";
                    cin >> password;
                    cout << "Security answer: ";
                    cin >> securityAnswer;

                    bool validUsername = !username.empty();
                    for (char c : username) {
                        if (!isalpha(c)) { validUsername = false; break; }
                    }
                    if (!validUsername) {
                        cout << "Error: Username must contain alphabets only." << endl;
                        break;
                    }

                    bool hasUpper = false, hasLower = false, hasSpecial = false;
                    if (password.length() >= 7) {
                        for (char c : password) {
                            if (isupper(c))      hasUpper   = true;
                            else if (islower(c)) hasLower   = true;
                            else if (ispunct(c)) hasSpecial = true;
                        }
                    }
                    if (!(password.length() >= 7 && hasUpper && hasLower && hasSpecial)) {
                        cout << "Error: Password must be 7 characters or greater and include at least one uppercase letter, one lowercase letter, and one special character." << endl;
                        break;
                    }

                    try {
                        fs.registerUser(username, password, securityAnswer);
                        // cout << "Registration successful." << endl;
                    } catch (const exception &e) {
                        cout << "Registration failed: " << e.what() << endl;
                    }
                    break;
                    system("clear");
                }

                case 'B': {  // Login
                    cout << "\n[Login]\nUsername: ";
                    cin >> username;
                    cout << "Password: ";
                    cin >> password;

                    if (username.empty() || password.empty()) {
                        cout << "Error: Username and password cannot be empty.\n";
                        break;
                    }

                    try {
                        isLoggedIn = fs.loginUser(username, password);
                        if (isLoggedIn) {
                            currentUser = username;
                            cout << "Welcome, " << currentUser << "!\n";
                        } else {
                            cout << "Login failed: Invalid credentials.\n";
                        }
                    } catch (const exception &e) {
                        cout << "Login error: " << e.what() << "\n";
                    }
                    break;
                    system("clear");
                }

                case 'C': {  // Recover Password
                    cout << "\n[Recover Password]\nUsername: ";
                    cin >> username;
                    cout << "Security answer: ";
                    cin >> securityAnswer;

                    if (username.empty() || securityAnswer.empty()) {
                        cout << "Error: All fields are required.\n";
                        break;
                    }

                    try {
                        fs.recoverPassword(username, securityAnswer);
                    } catch (const exception &e) {
                        cout << "Recovery failed: " << e.what() << "\n";
                    }
                    break;
                    system("clear");
                }
                case 'D': {  // Register with Role
                    cout << "\n[Register w/ Role]\nUsername: ";
                    cin >> username;
                    cout << "Role (Admin/Editor/Viewer): ";
                    cin >> role;
                    cout << "Password: ";
                    cin >> password;
                    cout << "Security answer: ";
                    cin >> securityAnswer;

                    fs.registerUser(username, password, securityAnswer);
                    ac.addUser(username, role);        
                    // cout << "Registration with role complete.\n";
                    break;
                    system("clear");
                }

                case '0':
                    cout << "\nGoodbye!\n";
                    break;
                    system("clear");

                default:
                    cout << "\nInvalid selection. Please choose a valid option.\n";
            }

        } else {
            cout << "\n==== File & Folder Management ===="
                 << "\nD. Logout User"
                 << "\nE. Show Last Logout Time"
                 << "\nF. Create Folder"
                 << "\nG. Create File"
                 << "\nH. Enter Folder"
                 << "\nI. Go Back to Parent Folder"
                 << "\nJ. List Contents"
                 << "\nK. Delete Item (Recycle Bin)"
                 << "\nL. Restore Last Deleted Item"
                 << "\nM. Show Access History"
                 << "\nN. Save a New File Version"
                 << "\nO. Rollback to Previous Version"
                 << "\nP. Share File with Another User"
                 << "\nQ. View Users Shared Connections"
                 << "\nR. Search the File"
                 << "\nS. Upload a File"
                 << "\nT. Process all the Uploads"
                 << "\nU. Check Permission"          
                 << "\nV. Grant Permission"           
                 << "\nW. List All Users"            
                 << "\n0. Exit"
                 << "\nSelect an option: ";
            cin >> choice;
            choice = toupper(choice);
            system("clear");

            switch (choice) {
                case 'D': 
                    try {
                        fs.logoutUser(currentUser);
                    } catch (const exception &e) {
                        cout << "Logout error: " << e.what() << "\n";
                    }
                    isLoggedIn = false;
                    currentUser.clear();
                    break;
                    system("clear");

                case 'E': {  // Show Last Logout Time
                    cout << "\n[Last Logout Time]\nUsername: ";
                    cin >> username;
                    if (username.empty()) {
                        cout << "Error: Username cannot be empty.\n";
                        break;
                    }
                    try {
                        cout << "Last logout: " << fs.getLastLogoutTime(username) << "\n";
                    } catch (const exception &e) {
                        cout << "Error: " << e.what() << "\n";
                    }
                    break;
                    system("clear");
                }

               case 'F': 
               case 'G': {  // Create File or Folder
                    bool isFolder = (choice == 'F');  
                    cout << (isFolder ? "\n[Create Folder]\n" : "\n[Create File]\n")
                        << "Enter name: ";
                    cin >> name;

                    if (name.empty()) {
                        cout << "Error: Name cannot be empty.\n";
                        break;
                    }

                    try {
                        fs.create(name, isFolder);  
                        //cout << (isFolder ? "Folder " : "File ") << "created: " << name << "\n";

                        if (!isFolder) {
                            char choice;
                            cout << "\nDo you want to write into the file?"
                                << "\n1. Yes"
                                << "\n2. No (leave it empty)"
                                << "\nEnter your choice (1/2): ";
                            cin >> choice;

                            if (choice == '1') {
                                cout << "Enter file content (single line): ";
                                cin.ignore();
                                FolderNode* fd;
                                string userInput = fd->content;
                                getline(cin, userInput);
                                fs.writeToFile(name, userInput);
                                cout << "Content written to file: " << name << "\n";
                                
                                char compressChoice;
                                cout << "\nDo you want to compress the data using Run-Length Encoding (RLE)?"
                                    << "\nPress Y to compress or any other key to skip: ";
                                cin >> compressChoice;
                                
                                if (compressChoice == 'Y' || compressChoice == 'y') {
                                    string compressedData = runLengthEncode(userInput);
                                    cout << "\nCompressed Data: \n" << compressedData << endl;
                                }

                                cout << "\nPress any key to return to the main menu...";
                                cin.ignore();
                                cin.get();
                            } else if (choice == '2') {
                                cout << "File left empty.\n";
                            } else {
                                cout << "Invalid input. File left unchanged.\n";
                            }
                        }

                    } catch (const exception &e) {
                        cout << "Creation failed: " << e.what() << "\n";
                    }
                    break;
                    system("clear");
                }

                case 'H': {  // Enter Folder
                    cout << "\n[Enter Folder]\nFolder name: ";
                    cin >> name;
                    if (name.empty()) {
                        cout << "Error: Folder name cannot be empty.\n";
                        break;
                    }
                    try {
                        fs.changeDirectory(name);
                    } catch (const exception &e) {
                        cout << "Error: " << e.what() << "\n";
                    }
                    break;
                    system("clear");
                }

                case 'I':  // Go Back
                    try {
                        fs.goBack();
                    } catch (const exception &e) {
                        cout << "Error: " << e.what() << "\n";
                    }
                    break;
                    system("clear");

                case 'J':  // List Contents
                    try {
                        fs.displayContents();
                    } catch (const exception &e) {
                        cout << "Error: " << e.what() << "\n";
                    }
                    break;
                    system("clear");

                case 'K': {  // Delete Item
                    cout << "\n[Delete Item]\nEnter item name: ";
                    cin >> name;
                    if (name.empty()) {
                        cout << "Error: Item name cannot be empty.\n";
                        break;
                    }
                    try {
                        fs.deleteCurrentItem(name);
                        cout << "Deleted: " << name <<"(moved to Recycle Bin)" << "\n";
                    } catch (const exception &e) {
                        cout << "Deletion failed: " << e.what() << "\n";
                    }
                    break;
                    system("clear");
                }

                case 'L':  // Restore Last Deleted
                    try {
                        fs.restoreLastDeleted();
                    } catch (const exception &e) {
                        cout << "Error: " << e.what() << "\n";
                    }
                    break;
                    system("clear");

                case 'M':  // Show Access History
                    try {
                        fs.showHistory();
                    } catch (const exception &e) {
                        cout << "Error: " << e.what() << "\n";
                    }
                    break;
                    system("clear");

                case 'N': {  // Save a New File Version
                    cout << "\n[Save File Version]\nFile name: ";
                    cin >> name;
                    if (name.empty()) {
                        cout << "Error: File name cannot be empty.\n";
                        break;
                    }
                    cout << "Version description: ";
                    cin.ignore(10000, '\n');
                    FolderNode* fd;
                    string userInput = fd->content;
                    getline(cin, userInput);
                    if (userInput.empty()) {
                        cout << "Error: Description cannot be empty.\n";
                        break;
                    }
                    try {
                        fs.saveVersion(name, userInput);
                        //cout << "Version saved for " << name << "\n";
                    } catch (const exception &e) {
                        cout << "Error: " << e.what() << "\n";
                    }
                    break;
                    system("clear");
                }

                case 'O': {  // Rollback to Previous Version
                    cout << "\n[Rollback Version]\nFile name: ";
                    cin >> name;
                    if (name.empty()) {
                        cout << "Error: File name cannot be empty.\n";
                        break;
                    }
                    try {
                        fs.rollbackVersion(name);
                        cout << "Rolled back: " << name << "\n";
                    } catch (const exception &e) {
                        cout << "Error: " << e.what() << "\n";
                    }
                    break;
                    system("clear");
                }

                case 'P': {  // Share File
                    cout << "\n[Share File]\nYour username: ";
                    cin  >> fromUser;
                    cout << "Recipient username: ";
                    cin  >> toUser;
                    cout << "File to share: ";
                    cin  >> name;

                    if (!fs.userExists(fromUser)) {
                        cout << "Error: sender \"" << fromUser << "\" not registered.\n";
                    }
                    else if (!fs.userExists(toUser)) {
                        cout << "Error: recipient \"" << toUser << "\" not found.\n";
                    }
                    else if (!fs.existsInCurrentDirectory(name)) {
                        cout << "Error: file \"" << name << "\" does not exist here.\n";
                    }
                    else {
                        fs.shareFileWith(fromUser, toUser, name);
                        cout << "Shared \"" << name << "\" from "
                            << fromUser << " to " << toUser << ".\n";
                    }
                    break;
                    system("clear");
                }

                case 'Q': {  // View Users Shared Connections
                    cout << "\n[Shared Connections]\nUsername: ";
                    cin >> username;
                    if (username.empty()) {
                        cout << "Error: Username cannot be empty.\n";
                        break;
                    }
                    try {
                        fs.showUserConnections(username);
                    } catch (const exception &e) {
                        cout << "Error: " << e.what() << "\n";
                    }
                    break;
                    system("clear");
                }

                case 'R': {  // Search File
                    cout << "\n[Search File]\nFile name: ";
                    cin >> name;
                    if (name.empty()) {
                        cout << "Error: File name cannot be empty.\n";
                        break;
                    }
                    FolderNode* found = fs.searchFile(name);
                    if (found) {
                        cout << "Found file: \"" << name << "\"\n";
                    } else {
                        cout << "File not found: \"" << name << "\"\n";
                    }
                    break;
                    system("clear");
                }

                case 'S': {  // Upload File
                    cout << "\n[Upload File]\nEnter file name: ";
                    cin >> name;
                    if (name.empty()) {
                        cout << "Error: File name cannot be empty.\n";
                        break;
                    }

                    if (!fs.existsInCurrentDirectory(name)) {
                        cout << "Error: File \"" << name << "\" does not exist in the current folder.\n";
                        break;
                    }

                    FolderNode* fileNode = fs.searchFile(name);
                    if (fileNode == nullptr || fileNode->isFolder) {
                        cout << "Error: \"" << name << "\" is not a valid file.\n";
                        break;
                    }

                    int priority;
                    cout << "Enter priority (lower = higher priority): ";
                    cin >> priority;

                    string fileContent = fileNode->content;
                    fs.enqueueFileUpload(name, priority, fileContent);  
                    cout << "File queued for upload successfully.\n";
                    break;
                    system("clear");
                }


                case 'T': {  // Process All Uploads
                    cout << "\n[Process Uploads]\n";
                    fs.processFileUploads(); 
                    cout << "All pending file uploads have been processed.\n";
                    break;
                    system("clear");
                }


                case 'U': {  // Check Permission
                    cout << "\n[Check Permission]\nUsername: ";
                    cin >> username;
                    cout << "Action (Read/Write/Execute): ";
                    cin >> action;
                    bool ok = ac.hasPermission(username, action);
                    cout << (ok ? "Permission granted.\n" : "Permission denied.\n");
                    break;
                    system("clear");
                }

                case 'V': {  // Grant Permission
                    cout << "\n[Grant Permission]\nAdmin username: ";
                    cin >> username;
                    cout << "Target username: ";
                    cin >> toUser;
                    cout << "Action to grant: ";
                    cin >> action;
                    ac.grantPermission(username, toUser, action);
                    break;
                    system("clear");
                }

                case 'W': {  // List All Users
                    cout << "\n[All Registered Users which have roles]\n";
                    ac.listUsers();
                    break;
                    system("clear");
                }

                case '0':  // Exit
                    cout << "\nGoodbye!\n";
                    break;
                    system("clear");

                default:
                    cout << "\nInvalid selection. Please choose a valid option.\n";
            }
        }
    } while (choice != '0');
    
    return 0;
}
