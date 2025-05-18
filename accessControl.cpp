#include "accessControl.h"
#include <iostream>
using namespace std;

AccessControl::AccessControl()
  : userList(nullptr), numUsers(0),
    permissionsList(nullptr), numVertices(0)
{}

AccessControl::~AccessControl() {
    delete[] userList;
    if (permissionsList) {
        for (int i = 0; i < numVertices; ++i) {
            PermissionNode* p = permissionsList[i];
            while (p) {
                PermissionNode* next = p->next;
                delete p;
                p = next;
            }
        }
        delete[] permissionsList;
    }
}

void AccessControl::expandUserList() {
    UserInfo* newList = new UserInfo[numUsers + 1];
    for (int i = 0; i < numUsers; ++i)
        newList[i] = userList[i];
    delete[] userList;
    userList = newList;

    PermissionNode** newPerms = new PermissionNode*[numUsers + 1];
    for (int i = 0; i < numUsers; ++i)
        newPerms[i] = permissionsList ? permissionsList[i] : nullptr;
    newPerms[numUsers] = nullptr;
    delete[] permissionsList;
    permissionsList = newPerms;

    ++numUsers;
    numVertices = numUsers;
}

bool AccessControl::linkUsers(int fromIdx, int toIdx) {
    if (fromIdx < 0 || toIdx < 0 || fromIdx >= numVertices || toIdx >= numVertices)
        return false;

    PermissionNode* p1 = new PermissionNode;
    p1->userIndex     = toIdx;
    p1->next          = permissionsList[fromIdx];
    permissionsList[fromIdx] = p1;

    PermissionNode* p2 = new PermissionNode;
    p2->userIndex     = fromIdx;
    p2->next          = permissionsList[toIdx];
    permissionsList[toIdx] = p2;

    return true;
}

void AccessControl::addUser(const string& username, const string& role) {
    expandUserList();
    userList[numUsers - 1].username = username;
    userList[numUsers - 1].role     = role;
    cout << "Added user: " << username
         << " (Role: " << role << ")\n";
}

int AccessControl::findUser(const string& username) const {
    for (int i = 0; i < numUsers; ++i)
        if (userList[i].username == username)
            return i;
    return -1;
}

bool AccessControl::hasPermission(const string& username, const string& action) const {
    int idx = findUser(username);
    if (idx < 0) return false;

    string role = userList[idx].role;

    while (role != "Admin" && role != "Editor" && role != "Viewer") {
        cout << "Invalid role detected for user \"" << username << "\".\n";
        cout << "Please re-enter the correct role (Admin / Editor / Viewer): ";
        cin >> role;
    }

    if (role == "Admin")  return true;
    if (role == "Editor")
        return action == "Read" || action == "Write";
    if (role == "Viewer")
        return action == "Read";

    return false;
}


bool AccessControl::grantPermission(const string& adminUsername,
                                    const string& targetUsername,
                                    const string& action) {
    int adminIdx  = findUser(adminUsername);
    int targetIdx = findUser(targetUsername);
    if (adminIdx < 0 || targetIdx < 0 ||
        userList[adminIdx].role != "Admin") {
        cout << "Denied: only Admin can grant.\n";
        return false;
    }
    if (linkUsers(adminIdx, targetIdx)) {
        cout << "Granted '" << action << "' from "
             << adminUsername << " to " << targetUsername << "\n";
        return true;
    }
    return false;
}

void AccessControl::listUsers() const {
    for (int i = 0; i < numUsers; ++i) {
        cout << "User[" << i << "]: "
             << userList[i].username
             << " (Role: " << userList[i].role << ")\n";
    }
}
