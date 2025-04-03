#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <limits>
#include <algorithm>
#include <string>

using namespace std;

class Graph {
public:
    struct Edge {
        string destination;
        int weight;
    };

    void addNode(const string& node) {
        if (adjacencyList.find(node) == adjacencyList.end()) {
            adjacencyList[node] = vector<Edge>();
            cout << "Place " << node << " added." << endl;
        } else {
            cout << "Place " << node << " already exists." << endl;
        }
    }

    void addEdge(const string& source, const string& destination, int weight) {
        if (adjacencyList.find(source) != adjacencyList.end() && adjacencyList.find(destination) != adjacencyList.end()) {
            adjacencyList[source].push_back({destination, weight});
            cout << "Road from " << source << " to " << destination << " with distance " << weight << " added." << endl;
        } else {
            cout << "One or both places do not exist." << endl;
        }
    }

    void updateEdge(const string& source, const string& destination, int newWeight) {
        if (adjacencyList.find(source) != adjacencyList.end()) {
            for (auto &edge : adjacencyList[source]) {
                if (edge.destination == destination) {
                    edge.weight = newWeight;
                    cout << "Road from " << source << " to " << destination << " updated to distance " << newWeight << "." << endl;
                    return;
                }
            }
            cout << "Road not found." << endl;
        } else {
            cout << "Source place does not exist." << endl;
        }
    }

    void deleteEdge(const string& source, const string& destination) {
        if (adjacencyList.find(source) != adjacencyList.end()) {
            auto &edges = adjacencyList[source];
            edges.erase(remove_if(edges.begin(), edges.end(),
                                   [destination](const Edge &edge) { return edge.destination == destination; }),
                         edges.end());
            cout << "Road from " << source << " to " << destination << " deleted." << endl;
        } else {
            cout << "Source place does not exist." << endl;
        }
    }

    void deleteNode(const string& node) {
        if (adjacencyList.find(node) != adjacencyList.end()) {
            adjacencyList.erase(node);
            for (auto &pair : adjacencyList) {
                deleteEdge(pair.first, node);
            }
            cout << "Place " << node << " deleted." << endl;
        } else {
            cout << "Place does not exist." << endl;
        }
    }

    void traverse() {
        cout << "Graph traversal:\n";
        for (const auto &pair : adjacencyList) {
            cout << "Place " << pair.first << ": ";
            for (const auto &edge : pair.second) {
                cout << "(" << edge.destination << ", " << edge.weight << ") ";
            }
            cout << endl;
        }
    }

    void searchNode(const string& node) {
        if (adjacencyList.find(node) != adjacencyList.end()) {
            cout << "Place " << node << " exists." << endl;
        } else {
            cout << "Place " << node << " does not exist." << endl;
        }
    }

    void searchEdge(const string& source, const string& destination) {
        if (adjacencyList.find(source) != adjacencyList.end()) {
            for (const auto &edge : adjacencyList[source]) {
                if (edge.destination == destination) {
                    cout << "Road from " << source << " to " << destination << " exists with distance " << edge.weight << "." << endl;
                    return;
                }
            }
            cout << "Road from " << source << " to " << destination << " does not exist." << endl;
        } else {
            cout << "Source place does not exist." << endl;
        }
    }

    void dijkstra(const string& start) {
        unordered_map<string, int> distances;
        for (const auto &pair : adjacencyList) {
            distances[pair.first] = numeric_limits<int>::max();
        }
        distances[start] = 0;

        priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;
        pq.push({0, start});

        while (!pq.empty()) {
            string current = pq.top().second;
            int currentDistance = pq.top().first;
            pq.pop();

            if (currentDistance > distances[current]) continue;

            for (const auto &edge : adjacencyList[current]) {
                int newDistance = currentDistance + edge.weight;
                if (newDistance < distances[edge.destination]) {
                    distances[edge.destination] = newDistance;
                    pq.push({newDistance, edge.destination});
                }
            }
        }

        cout << "Distances from place " << start << ":\n";
        for (const auto &pair : distances) {
            cout << "To " << pair.first << ": " << (pair.second == numeric_limits<int>::max() ? "Infinity" : to_string(pair.second)) << endl;
        }
    }

    void shortestPath(const string& start, const string& end) {
        unordered_map<string, int> distances;
        unordered_map<string, string> previous;
        for (const auto &pair : adjacencyList) {
            distances[pair.first] = numeric_limits<int>::max();
            previous[pair.first] = "";
        }
        distances[start] = 0;

        priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;
        pq.push({0, start});

        while (!pq.empty()) {
            string current = pq.top().second;
            int currentDistance = pq.top().first;
            pq.pop();

            if (currentDistance > distances[current]) continue;

            for (const auto &edge : adjacencyList[current]) {
                int newDistance = currentDistance + edge.weight;
                if (newDistance < distances[edge.destination]) {
                    distances[edge.destination] = newDistance;
                    previous[edge.destination] = current;
                    pq.push({newDistance, edge.destination});
                }
            }
        }

        if (distances[end] == numeric_limits<int>::max()) {
            cout << "No path from " << start << " to " << end << "." << endl;
            return;
        }

        cout << "Shortest path from " << start << " to " << end << ": ";
        vector<string> path;
        for (string at = end; !at.empty(); at = previous[at]) {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());
        for (size_t i = 0; i < path.size(); i++) {
            cout << path[i] << (i == path.size() - 1 ? "" : " -> ");
        }
        cout << " with total distance: " << distances[end] << endl;
    }

    void displayAllEdges() {
        cout << "All roads in the graph:\n";
        for (const auto &pair : adjacencyList) {
            for (const auto &edge : pair.second) {
                cout << "Road from " << pair.first << " to " << edge.destination << " with distance " << edge.weight << endl;
            }
        }
    }

    void findLongestPath() {
        // This is a placeholder function to demonstrate functionality
        cout << "Finding longest path is complex and requires a different approach, not implemented in this version." << endl;
    }

    bool isConnected() {
        if (adjacencyList.empty()) return true;

        unordered_map<string, bool> visited;
        queue<string> q;
        auto startNode = adjacencyList.begin()->first;
        q.push(startNode);
        visited[startNode] = true;

        while (!q.empty()) {
            string current = q.front();
            q.pop();

            for (const auto &edge : adjacencyList[current]) {
                if (!visited[edge.destination]) {
                    visited[edge.destination] = true;
                    q.push(edge.destination);
                }
            }
        }

        for (const auto &pair : adjacencyList) {
            if (!visited[pair.first]) {
                cout << "Graph is not connected. Place " << pair.first << " is unreachable." << endl;
                return false;
            }
        }

        cout << "Graph is connected." << endl;
        return true;
    }

private:
    unordered_map<string, vector<Edge>> adjacencyList;
};

void displayMenu() {
    cout << "\nMenu:\n";
    cout << "1. Add Place\n";
    cout << "2. Add Road\n";
    cout << "3. Update Road\n";
    cout << "4. Delete Road\n";
    cout << "5. Delete Place\n";
    cout << "6. Traverse Graph\n";
    cout << "7. Search Place\n";
    cout << "8. Search Road\n";
    cout << "9. Find Shortest Path (Dijkstra)\n";
    cout << "10. Find Shortest Path between Two Places\n";
    cout << "11. Display All Roads\n";
    cout << "12. Check Connectivity\n";
    cout << "0. Exit\n";
}

int main() {
    Graph g;
    int choice;
    string place1, place2;
    int distance;

    // Sample usage to demonstrate functionality
    cout << "Sample Graph Operations:\n";
    g.addNode("Fasil Ghebbi");
    g.addNode("Debre Birhan Selassie Church");
    g.addNode("Fasilides' Bath");
    g.addNode("Gondar Market");
    g.addEdge("Fasil Ghebbi", "Debre Birhan Selassie Church", 2);
    g.addEdge("Fasil Ghebbi", "Fasilides' Bath", 1);
    g.addEdge("Debre Birhan Selassie Church", "Gondar Market", 3);
    g.addEdge("Fasilides' Bath", "Gondar Market", 2);
    g.traverse();
    g.dijkstra("Fasil Ghebbi");
    g.updateEdge("Fasil Ghebbi", "Debre Birhan Selassie Church", 3);
    g.deleteEdge("Fasil Ghebbi", "Fasilides' Bath");
    g.deleteNode("Gondar Market");
    g.traverse();

    // Interactive menu for user input
    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter place to add: ";
                cin >> place1;
                g.addNode(place1);
                break;
            case 2:
                cout << "Enter source place, destination place, and distance: ";
                cin >> place1 >> place2 >> distance;
                g.addEdge(place1, place2, distance);
                break;
            case 3:
                cout << "Enter source place, destination place, and new distance: ";
                cin >> place1 >> place2 >> distance;
                g.updateEdge(place1, place2, distance);
                break;
            case 4:
                cout << "Enter source place and destination place to delete road: ";
                cin >> place1 >> place2;
                g.deleteEdge(place1, place2);
                break;
            case 5:
                cout << "Enter place to delete: ";
                cin >> place1;
                g.deleteNode(place1);
                break;
            case 6:
                g.traverse();
                break;
            case 7:
                cout << "Enter place to search: ";
                cin >> place1;
                g.searchNode(place1);
                break;
            case 8:
                cout << "Enter source and destination places to search road: ";
                cin >> place1 >> place2;
                g.searchEdge(place1, place2);
                break;
            case 9:
                cout << "Enter starting place for Dijkstra: ";
                cin >> place1;
                g.dijkstra(place1);
                break;
            case 10:
                cout << "Enter starting place and ending place for shortest path: ";
                cin >> place1 >> place2;
                g.shortestPath(place1, place2);
                break;
            case 11:
                g.displayAllEdges();
                break;
            case 12:
                g.isConnected();
                break;
            case 0:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}
