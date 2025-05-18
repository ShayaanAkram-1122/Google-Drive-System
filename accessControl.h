#ifndef ACCESSCONTROL_H
#define ACCESSCONTROL_H

#include <string>
using namespace std;


struct PermissionNode {
    int               userIndex;
    PermissionNode*   next;
};

struct UserIdMap {
    string        username;
    int           userId;
    UserIdMap*    next;
};

class AccessControl {
public:
    AccessControl();
    ~AccessControl();

    void addUser(const string& username, const string& role);
    bool hasPermission(const string& username, const string& action) const;
    bool grantPermission(const string& adminUsername,
                         const string& targetUsername,
                         const string& action);
    void listUsers() const;

private:
    struct UserInfo { string username, role; };

    UserInfo*        userList;
    int              numUsers;
    PermissionNode** permissionsList;
    int              numVertices;

    void expandUserList();
    bool linkUsers(int fromIdx, int toIdx);
    int  findUser(const string& username) const;
};

#endif 
