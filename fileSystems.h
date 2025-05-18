#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include "dataStructures.h"

using namespace std;

// user info
struct UserInfo {
    string username;
    string passwordHash;
    string securityHash;
    string lastLogout;
};

// file/folder node
struct VersionNode {
    string content;
    VersionNode* prev;
    VersionNode* next;
};
struct FolderNode {
    string name;
    bool isFolder;
    string owner;
    FolderNode* parent;
    FolderNode* child;
    string content;
    FolderNode* nextItem;
    VersionNode* versionHead;
    string fileData;
};

// recycle bin & history
struct BinNode { string item; BinNode* next; };
struct HistNode { string item; HistNode* next; };

// user→ID mapping
struct UserMapNode {
    string username;
    int userId;
    UserMapNode* next;
};

// forward for graph
struct GraphNode;

class UploadTask {
public:
    string fileName, fileContent;
    int priority;
    time_t timestamp;
    UploadTask* next;

    UploadTask(string name, int prio, string content);
};

class UploadQueue {
private:
    UploadTask* head;

public:
    UploadQueue();
    void enqueue(string fileName, int priority, string content);
    void processUploads();
};


// FileSystem class declaration
class fileSystem {
private:
    // users
    UserInfo users[100];
    int userCount;

    // folders
    FolderNode* root;
    FolderNode* current;

    // recycle bin
    BinNode* binTop;

    // access history
    HistNode* histFront;
    HistNode* histRear;

    // file-sharing graph
    UserMapNode* userMapHead;
    int nextUserId;
    GraphNode** adjList;
    int numUsers;

    UploadQueue uploadQueue;

    // helper function for to implement functionalities
    static string secure(const string& input);
    void cleanupTree(FolderNode* node);
    void pushBin(const string& item);
    bool popBin(string& out);
    void enqueueHist(const string& item);

    FolderNode* findChild(const string& name) const;
    void addVersionNode(FolderNode* f, const string& content);
    bool rollbackVersionNode(FolderNode* f);

    int getUserId(const string& username);
    void expandAdjList();
    void preOrderSearch(FolderNode* node,const string& filename,FolderNode*& foundNode);

public:
    fileSystem();
    ~fileSystem();

    // user management
    void registerUser(const string& username, const string& password, const string& securityAnswer);
    bool loginUser(const string& username, const string& password);
    bool recoverPassword(const string& username, const string& securityAnswer);
    void logoutUser(const string& username);
    string getLastLogoutTime(const string& username) const;

    // folder ops
    void create(const string& name, bool isFolder);
    bool changeDirectory(const string& name);
    void goBack();
    void displayContents() const;

    // recycle bin
    void deleteCurrentItem(const string& name);
    void restoreLastDeleted();

    // history
    void accessItem(const string& name);
    void showHistory() const;

    // versioning
    void saveVersion(const string& fileName, const string& content);
    void rollbackVersion(const string& fileName);

    // file sharing
    void addUser(const string& username);
    void shareFileWith(const string& fromUser, const string& toUser, const string& fileName);
    void showUserConnections(const string& username) const;

    // sync upload
    void enqueueFileUpload(const string& fileName, int priority,const string& content);
    void processFileUploads();

    bool userExists(const string& username) const;
    bool existsInCurrentDirectory(const string& name) const;
    bool hasAccessTo(const string& fileName, const string& username) const;
    FolderNode* searchFile(const string& filename);
    void writeToFile(const string& fileName, const string& newContent);
    bool nameExists(const string &name, FolderNode *node);
    bool nameExists(const string &name);

};

#endif
