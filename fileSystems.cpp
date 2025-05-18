#include "fileSystems.h"
#include <iostream>
#include <ctime>

using namespace std;


UploadTask::UploadTask(string name, int prio, string content) {
    FolderNode* fd;
    fileName = name;
    priority = prio;
    fileContent = fd->content;
    timestamp = time(nullptr);
    next = nullptr;
}

UploadQueue::UploadQueue() {
    head = nullptr;
}

void UploadQueue::enqueue(string fileName, int priority, string content) {
    UploadTask* newTask = new UploadTask(fileName, priority, content);

    if (head == nullptr || priority < head->priority || 
        (priority == head->priority && newTask->timestamp < head->timestamp)) {
        newTask->next = head;
        head = newTask;
    } else {
        UploadTask* current = head;
        while (current->next != nullptr && 
               (current->next->priority < priority || 
                (current->next->priority == priority && current->next->timestamp <= newTask->timestamp))) {
            current = current->next;
        }
        newTask->next = current->next;
        current->next = newTask;
    }

    cout << "Queued: " << fileName << " (Priority: " << priority << ")\n";
}

void UploadQueue::processUploads() {
    if (head == nullptr) {
        cout << "No files to upload.\n";
        return;
    }

    cout << "\n--- Uploading Files ---\n";
    while (head != nullptr) {
        cout << "Uploading: " << head->fileName << " [Priority: " << head->priority << "]\n";
        UploadTask* temp = head;
        head = head->next;
        delete temp;
    }
    cout << "All files uploaded.\n";
}

// — helper to hash passwords/security answers —
string fileSystem::secure(const string& input) {
    long long hash = 0;
    const int prime = 37;
    for (int i=0;i<input.length();i++){
        hash = hash * prime + input[i];
    }
    return to_string(hash);
}

// — register a new user —
void fileSystem::registerUser(const string& username,
                              const string& password,
                              const string& securityAnswer){ 
    for (int i = 0; i < userCount; ++i) {
        if (users[i].username == username) {
            cout << "Error: Username '" << username << "' already exists.\n";
            return;
        }
    }

    users[userCount].username     = username;
    users[userCount].passwordHash = secure(password);
    users[userCount].securityHash = secure(securityAnswer);
    users[userCount].lastLogout   = "Never";
    ++userCount;

    cout << "User registered successfully\n";
}


// — verify credentials & show last logout —
bool fileSystem::loginUser(const string& username,
                           const string& password) {
    string h = secure(password);
    for (int i = 0; i < userCount; ++i) {
        if (users[i].username == username &&
            users[i].passwordHash == h) {
            cout << "Login successful. Last logout: "
                 << users[i].lastLogout << "\n";
            return true;
        }
    }
    cout << "Invalid username or password.\n";
    return false;
}

// — recover via security answer or master key —
bool fileSystem::recoverPassword(const string& username,
                                 const string& securityAnswer) {
    string h = secure(securityAnswer);
    for (int i = 0; i < userCount; ++i) {
        if (users[i].username == username &&
            users[i].securityHash == h) {
            cout << "Identity verified. You may reset your password.\n";
            return true;
        }
    }

    char ch;
    do {
        cout << "Security answer incorrect.\n"
             << "Use master key to override? (y/n): ";
        cin >> ch;

        if (ch == 'y' || ch == 'Y') {
            string key;
            cout << "Enter your birthday month: ";
            cin >> key;
            cout << "Master key accepted. You may reset your password.\n";
            return true;
        } else if (ch == 'n' || ch == 'N') {
            cout << "Password recovery cancelled.\n";
            return false;
        } else {
            cout << "Invalid input. Please enter 'y' or 'n'.\n";
        }
    } while (true);
}


// — update last logout timestamp —
void fileSystem::logoutUser(const string& username) {
    time_t now = time(nullptr);
    string   dt  = ctime(&now);
    for (int i = 0; i < userCount; ++i) {
        if (users[i].username == username) {
            users[i].lastLogout = dt;
            cout << "Logout successful.\n";
            return;
        }
    }
    cout << "User not found.\n";
}

// — fetch stored last logout time —
string fileSystem::getLastLogoutTime(const string& username) const {
    for (int i = 0; i < userCount; ++i) {
        if (users[i].username == username)
            return users[i].lastLogout;
    }
    return "User not found.";
}




// Helper function implementations for fileSystems.cpp

bool fileSystem::userExists(const string& username) const {
    for (int i = 0; i < userCount; ++i) {
        if (users[i].username == username) {
            return true;
        }
    }
    return false;
}

bool fileSystem::existsInCurrentDirectory(const string& name) const {
    for (FolderNode* c = current->child; c; c = c->nextItem) {
        if (c->name == name) {
            return true;
        }
    }
    return false;
}

bool fileSystem::hasAccessTo(const string& fileName, const string& username) const {
    // Locate target node in current directory
    FolderNode* target = nullptr;
    for (FolderNode* c = current->child; c; c = c->nextItem) {
        if (c->name == fileName) {
            target = c;
            break;
        }
    }
    if (!target) return false;

    // Owner always has access
    if (target->owner == username) return true;

    // Find recipient ID
    int recipientId = -1;
    for (UserMapNode* m = userMapHead; m; m = m->next) {
        if (m->username == username) {
            recipientId = m->userId;
            break;
        }
    }
    if (recipientId < 0) return false;

    // Find owner ID
    int ownerId = -1;
    for (UserMapNode* m = userMapHead; m; m = m->next) {
        if (m->username == target->owner) {
            ownerId = m->userId;
            break;
        }
    }
    if (ownerId < 0) return false;

    // Check adjacency list for sharing edge
    for (GraphNode* e = adjList[ownerId]; e; e = e->next) {
        if (e->vertex == recipientId) return true;
    }
    return false;
}


// — Constructor / Destructor —————————————————————
fileSystem::fileSystem()
  : userCount(0),
    binTop(nullptr),
    histFront(nullptr),
    histRear(nullptr),
    userMapHead(nullptr),
    nextUserId(0),
    adjList(nullptr),
    numUsers(0)
{
    // build root FolderNode
    root = new FolderNode;
    root->name        = "root";
    root->isFolder    = true;
    root->parent      = nullptr;
    root->child       = nullptr;
    root->nextItem    = nullptr;
    root->versionHead = nullptr;

    current = root;
}

fileSystem::~fileSystem() {
    // 1) directory tree
    cleanupTree(root);

    // 2) recycle-bin stack
    while (binTop) {
        BinNode* t = binTop;
        binTop = binTop->next;
        delete t;
    }

    // 3) history queue
    while (histFront) {
        HistNode* t = histFront;
        histFront = histFront->next;
        delete t;
    }

    // 4) user map
    while (userMapHead) {
        UserMapNode* t = userMapHead;
        userMapHead = userMapHead->next;
        delete t;
    }

    // 5) file-sharing graph
    if (adjList) {
        for (int i = 0; i < numUsers; ++i) {
            GraphNode* e = adjList[i];
            while (e) {
                GraphNode* nx = e->next;
                delete e;
                e = nx;
            }
        }
        delete[] adjList;
    }
}

// — Tree cleanup —————————————————————————————
void fileSystem::cleanupTree(FolderNode* node) {
    if (!node) return;

    // delete children
    FolderNode* c = node->child;
    while (c) {
        FolderNode* nxt = c->nextItem;
        cleanupTree(c);
        c = nxt;
    }

    // delete version list
    VersionNode* v = node->versionHead;
    while (v) {
        VersionNode* nx = v->next;
        delete v;
        v = nx;
    }

    delete node;
}

// — Directory ops —————————————————————————————
bool fileSystem::nameExists(const string &name, FolderNode *node) {
    if (!node) return false;
    if (node->name == name) return true;
    return nameExists(name, node->nextItem) || nameExists(name, node->child);
}

bool fileSystem::nameExists(const string &name) {
    return nameExists(name, root);
}

void fileSystem::create(const string& name, bool isFolder) {
    if (nameExists(name)) {
        cout << "Error: An item named '" << name << "' already exists.\n";
        return;
    }

    FolderNode* node = new FolderNode;
    node->name        = name;
    node->isFolder    = isFolder;
    node->parent      = current;
    node->child       = nullptr;
    node->nextItem    = nullptr;
    node->versionHead = nullptr;

    if (!current->child) {
        current->child = node;
    } else {
        FolderNode* t = current->child;
        while (t->nextItem) t = t->nextItem;
        t->nextItem = node;
    }

    cout << (isFolder ? "Folder " : "File ") << name
              << " created in " << current->name << endl;
    accessItem(name);
}


FolderNode* fileSystem::findChild(const string& name) const {
    for (FolderNode* c = current->child; c; c = c->nextItem) {
        if (!c->isFolder && c->name == name)
            return c;
    }
    return nullptr;
}

bool fileSystem::changeDirectory(const string& name) {
    for (FolderNode* c = current->child; c; c = c->nextItem) {
        if (c->isFolder && c->name == name) {
            current = c;
            cout << "Entered folder: " << name << endl;
            accessItem(name);
            return true;
        }
    }
    cout << "Folder not found: " << name << endl;
    return false;
}

void fileSystem::goBack() {
    if (current->parent) {
        current = current->parent;
        cout << "Returning to folder: " << current->name << endl;
    } else {
        cout << "Already at root directory" << endl;
    }
}

void fileSystem::displayContents() const {
    cout << "Contents of [" << current->name << "]:" << endl;
    for (FolderNode* c = current->child; c; c = c->nextItem) {
        cout << (c->isFolder ? "[Folder] " : "[File]   ") << c->name << endl;
    }
}

// — Recycle Bin (Stack) —————————————————————
void fileSystem::pushBin(const string& item) {
    BinNode* n = new BinNode;
    n->item     = item;
    n->next     = binTop;
    binTop      = n;
}

bool fileSystem::popBin(string& out) {
    if (!binTop) return false;
    BinNode* t = binTop;
    out        = t->item;
    binTop      = t->next;
    delete t;
    return true;
}

void fileSystem::deleteCurrentItem(const string& name) {
    FolderNode* prev = nullptr;
    FolderNode* node = current->child;

    while (node && node->name != name) {
        prev = node;
        node = node->nextItem;
    }
    if (!node) {
        cout << "Error: \"" << name << "\" not found to delete.\n";
        return;
    }
    if (prev) {

        prev->nextItem = node->nextItem;
    } else {

        current->child = node->nextItem;
    }

    pushBin(name);
    delete node;

    cout << "Deleted \"" << name << "\" .\n";
}


void fileSystem::restoreLastDeleted() {
    string n;
    if (popBin(n)) {
        cout << "Restored: " << n << endl;
        create(n, false);
    } else {
        cout << "Recycle Bin empty" << endl;
    }
}

// — Access History (Queue) ————————————————————
void fileSystem::enqueueHist(const string& item) {
    HistNode* n = new HistNode;
    n->item     = item;
    n->next     = nullptr;
    if (!histRear) {
        histFront = histRear = n;
    } else {
        histRear->next = n;
        histRear       = n;
    }
}

void fileSystem::accessItem(const string& name) {
    enqueueHist(name);
}

void fileSystem::showHistory() const {
    cout << "Access History:" << endl;
    for (HistNode* c = histFront; c; c = c->next) {
        cout << "- " << c->item << endl;
    }
}

// — Versioning (Doubly-Linked List) ——————————————————
void fileSystem::addVersionNode(FolderNode* f, const string& content) {
    VersionNode* n = new VersionNode;
    n->content     = content;
    n->prev        = nullptr;
    n->next        = f->versionHead;
    if (f->versionHead) f->versionHead->prev = n;
    f->versionHead = n;
}

bool fileSystem::rollbackVersionNode(FolderNode* f) {
    if (!f->versionHead) return false;
    VersionNode* toRemove = f->versionHead;
    cout << "Rollback to version: " << toRemove->content << endl;
    f->versionHead = toRemove->next;
    if (f->versionHead) f->versionHead->prev = nullptr;
    delete toRemove;
    return true;
}

void fileSystem::saveVersion(const string& fileName, const string& content) {
    FolderNode* f = findChild(fileName);
    if (!f) {
        cout << "File not found: " << fileName << endl;
        return;
    }
    addVersionNode(f, content);
}

void fileSystem::rollbackVersion(const string& fileName) {
    FolderNode* f = findChild(fileName);
    if (!f || !rollbackVersionNode(f))
        cout << "No versions to rollback" << endl;
}

// — File Sharing Graph ———————————————————————
int fileSystem::getUserId(const string& username) {
    for (UserMapNode* m = userMapHead; m; m = m->next) {
        if (m->username == username)
            return m->userId;
    }
    // new user
    UserMapNode* m = new UserMapNode;
    m->username    = username;
    m->userId      = nextUserId;
    m->next        = userMapHead;
    userMapHead    = m;

    expandAdjList();
    return nextUserId++;
}

void fileSystem::expandAdjList() {
    int oldCount = numUsers++;
    GraphNode** newArr = new GraphNode*[numUsers];
    for (int i = 0; i < oldCount; ++i) newArr[i] = adjList[i];
    newArr[oldCount] = nullptr;
    delete[] adjList;
    adjList = newArr;
}

void fileSystem::shareFileWith(
    const string& fromUser,
    const string& toUser,
    const string& fileName)
{
    int u = getUserId(fromUser);
    int v = getUserId(toUser);
    GraphNode* e = new GraphNode;
    e->vertex    = v;
    e->next      = adjList[u];
    adjList[u]   = e;
}

void fileSystem::showUserConnections(const string& username) const {
    int u = -1;
    for (UserMapNode* m = userMapHead; m; m = m->next) {
        if (m->username == username) {
            u = m->userId;
            break;
        }
    }
    if (u < 0) {
        cout << "No such user: " << username << endl;
        return;
    }

    cout << "Connections of " << username << ": ";
    for (GraphNode* e = adjList[u]; e; e = e->next) {  
        for (UserMapNode* m = userMapHead; m; m = m->next) {
            if (m->userId == e->vertex) {
                cout << m->username << " ";
                break;
            }
        }
    }
    cout << endl;
}
void fileSystem::preOrderSearch(FolderNode* node,const string& filename,FolderNode*& foundNode)
{
    if (node == nullptr || foundNode != nullptr) return;

    if (!node->isFolder && node->name == filename) {
        foundNode = node;
        return;
    }
    for (FolderNode* c = node->child; c; c = c->nextItem) {
        preOrderSearch(c, filename, foundNode);
        if (foundNode) return;  
    }
}

FolderNode* fileSystem::searchFile(const string& filename) {
    FolderNode* foundNode = nullptr;
    preOrderSearch(root, filename, foundNode);
    return foundNode;
}

void fileSystem::enqueueFileUpload(const string& name, int priority, const string& content) {
    uploadQueue.enqueue(name, priority,content);
}

void fileSystem::processFileUploads() {
    uploadQueue.processUploads();
}


void fileSystem::writeToFile(const string& name, const string& content) {
    FolderNode* file = findChild(name);

    if (!file || file->isFolder) {
        cout << "Error: File \"" << name << "\" not found or is a folder.\n";
        return;
    }
    file->fileData = content;
    saveVersion(name, content);

    cout << "Content written to file \"" << name << "\" successfully.\n";
}
