
#include <iostream>
#include <string>
using namespace std;

class TravelRoute {
public:
    float travel_cost;

    TravelRoute() {
        travel_cost = 0;
    }
};

class StarSystem {
public:
    int system_id;
    string name;
    int num_planets;
    float x, y, z;
    bool is_habitable;

    StarSystem** children;
    TravelRoute* routes;
    int child_count;
    int max_children;

    StarSystem(int m, int id, string systemName, int planets,
               float px, float py, float pz, bool habitable) {

        system_id = id;
        name = systemName;
        num_planets = planets;

        x = px;
        y = py;
        z = pz;

        is_habitable = habitable;

        max_children = m;
        child_count = 0;

        children = new StarSystem*[m];
        routes = new TravelRoute[m];

        for (int i = 0; i < m; i++)
            children[i] = NULL;
    }

    ~StarSystem() {
        for (int i = 0; i < max_children; i++)
            delete children[i];

        delete[] children;
        delete[] routes;
    }
};

class Galaxy {
private:
    int M;
    StarSystem* root;

    int checkSubtree(StarSystem* x, bool& balanced) const {
        if (x == NULL)
            return 0;

        int totalSize = 1;
        int minSize = 0, maxSize = 0;
        int childCount = 0;

        for (int i = 0; i < M; i++) {
            if (x->children[i] != NULL) {
                int size = checkSubtree(x->children[i], balanced);

                if (!balanced)
                    return 0;

                if (childCount == 0) {
                    minSize = size;
                    maxSize = size;
                }
                else {
                    if (size < minSize)
                        minSize = size;

                    if (size > maxSize)
                        maxSize = size;
                }

                childCount++;
                totalSize += size;
            }
        }

        if (childCount > 1 && maxSize - minSize > 1)
            balanced = false;

        return totalSize;
    }

public:
    Galaxy(int m) {
        M = m;
        root = NULL;
    }

    Galaxy(int m, StarSystem* rootNode) {
        M = m;
        root = rootNode;
    }

    ~Galaxy() {
        delete root;
    }

    bool is_balanced(StarSystem* x) const {
        bool balanced = true;
        checkSubtree(x, balanced);
        return balanced;
    }

    int count_leaves(StarSystem* x) const {
        if (x == NULL)
            return 0;

        if (x->child_count == 0)
            return 1;

        int leaves = 0;

        for (int i = 0; i < M; i++)
            leaves += count_leaves(x->children[i]);

        return leaves;
    }
};

StarSystem* createTree(int M) {
    int id, planets, habitat;
    float x, y, z;
    string name;

    cout << "\nEnter system ID: ";
    cin >> id;

    cout << "Enter system name (one word): ";
    cin >> name;

    cout << "Enter number of planets: ";
    cin >> planets;

    cout << "Enter coordinates (x y z): ";
    cin >> x >> y >> z;

    cout << "Is it habitable? (1 = Yes, 0 = No): ";
    cin >> habitat;

    StarSystem* node = new StarSystem(
        M, id, name, planets, x, y, z, habitat == 1
    );

    int childCount;

    cout << "How many children does " << name
         << " have? (0-" << M << "): ";

    cin >> childCount;

    while (childCount < 0 || childCount > M) {
        cout << "Enter a number between 0 and " << M << ": ";
        cin >> childCount;
    }

    for (int i = 0; i < childCount; i++) {
        float cost;

        cout << "Enter travel cost to child " << i + 1 << ": ";
        cin >> cost;

        node->children[i] = createTree(M);
        node->routes[i].travel_cost = cost;
        node->child_count++;
    }

    return node;
}

int main() {
    int M;

    cout << "Enter maximum number of children (M): ";
    cin >> M;

    while (M < 1) {
        cout << "M must be at least 1. Enter again: ";
        cin >> M;
    }

    StarSystem* root = createTree(M);

    Galaxy galaxy(M, root);

    cout << "\nIs the galaxy tree balanced? ";

    if (galaxy.is_balanced(root))
        cout << "Yes";
    else
        cout << "No";

    cout << "\nNumber of leaf nodes: "
         << galaxy.count_leaves(root) << endl;

    return 0;
}