#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <fstream>
#include <stack>

using namespace std;

class CityGraph {
private:
    unordered_map<string, vector<pair<string, int>>> adjList;

public:
    void addConnection(const string &city1, const string &city2, int distance) {
        adjList[city1].push_back({city2, distance});
        adjList[city2].push_back({city1, distance}); // Bidirectional connection
    }

    void removeConnection(const string &city1, const string &city2) {
        auto &neighbors1 = adjList[city1];
        auto &neighbors2 = adjList[city2];

        neighbors1.erase(remove_if(neighbors1.begin(), neighbors1.end(),
                                   [&](const pair<string, int> &p) { return p.first == city2; }),
                         neighbors1.end());

        neighbors2.erase(remove_if(neighbors2.begin(), neighbors2.end(),
                                   [&](const pair<string, int> &p) { return p.first == city1; }),
                         neighbors2.end());

        cout << "Connection removed between " << city1 << " and " << city2 << endl;
    }

    void updateConnection(const string &city1, const string &city2, int newDistance) {
        for (auto &neighbor : adjList[city1]) {
            if (neighbor.first == city2) {
                neighbor.second = newDistance;
                break;
            }
        }
        for (auto &neighbor : adjList[city2]) {
            if (neighbor.first == city1) {
                neighbor.second = newDistance;
                break;
            }
        }
        cout << "Connection updated between " << city1 << " and " << city2 << " to " << newDistance << " km." << endl;
    }

    void printConnections() {
        for (const auto &pair : adjList) {
            cout << pair.first << " -> ";
            for (const auto &neighbor : pair.second) {
                cout << "(" << neighbor.first << ", " << neighbor.second << " km) ";
            }
            cout << endl;
        }
    }

    void findShortestPath(const string &start, const string &destination) {
        unordered_map<string, int> distances;
        unordered_map<string, string> previous;
        priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> pq;

        for (const auto &pair : adjList) {
            distances[pair.first] = numeric_limits<int>::max();
        }
        distances[start] = 0;
        pq.push({0, start});

        while (!pq.empty()) {
            auto [currentDist, currentCity] = pq.top();
            pq.pop();

            if (currentCity == destination) break;

            for (const auto &[neighbor, weight] : adjList[currentCity]) {
                int newDist = currentDist + weight;
                if (newDist < distances[neighbor]) {
                    distances[neighbor] = newDist;
                    previous[neighbor] = currentCity;
                    pq.push({newDist, neighbor});
                }
            }
        }

        if (distances[destination] == numeric_limits<int>::max()) {
            cout << "No path found from " << start << " to " << destination << endl;
            return;
        }

        vector<string> path;
        for (string at = destination; !at.empty(); at = previous[at]) {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());

        cout << "Shortest path from " << start << " to " << destination << ": ";
        for (const auto &city : path) {
            cout << city << " ";
        }
        cout << "\nDistance: " << distances[destination] << " km" << endl;
    }

    void findAllPaths(const string &start, const string &end, vector<string>& path, vector<vector<string>>& allPaths) {
        path.push_back(start);

        if (start == end) {
            allPaths.push_back(path);
        } else {
            for (const auto &neighbor : adjList[start]) {
                if (find(path.begin(), path.end(), neighbor.first) == path.end()) {
                    findAllPaths(neighbor.first, end, path, allPaths);
                }
            }
        }
        path.pop_back();
    }

    void findLongestPath(const string &start, const string &end, vector<string>& path, vector<vector<string>>& allPaths) {
        findAllPaths(start, end, path, allPaths);
        int longest = 0;
        vector<string> longestPath;

        for (const auto &p : allPaths) {
            int length = 0;
            for (size_t i = 0; i < p.size() - 1; ++i) {
                for (const auto &neighbor : adjList[p[i]]) {
                    if (neighbor.first == p[i + 1]) {
                        length += neighbor.second;
                    }
                }
            }
            if (length > longest) {
                longest = length;
                longestPath = p;
            }
        }

        cout << "Longest path from " << start << " to " << end << ": ";
        for (const auto &city : longestPath) {
            cout << city << " ";
        }
        cout << "\nDistance: " << longest << " km" << endl;
    }

    void displayCityNeighbors(const string &city) {
        if (adjList.find(city) == adjList.end()) {
            cout << "City not found!" << endl;
            return;
        }

        cout << "Neighbors of " << city << ": ";
        for (const auto &neighbor : adjList[city]) {
            cout << neighbor.first << " ";
        }
        cout << endl;
    }

    void findIsolatedCities() {
        cout << "Isolated cities (no connections): ";
        bool found = false;
        for (const auto &pair : adjList) {
            if (pair.second.empty()) {
                cout << pair.first << " ";
                found = true;
            }
        }
        if (!found) {
            cout << "None" << endl;
        } else {
            cout << endl;
        }
    }

    void saveGraphToFile(const string &filename) {
        ofstream file(filename);
        for (const auto &pair : adjList) {
            for (const auto &neighbor : pair.second) {
                file << pair.first << " " << neighbor.first << " " << neighbor.second << "\n";
            }
        }
        file.close();
        cout << "Graph saved to " << filename << endl;
    }

    void loadGraphFromFile(const string &filename) {
        ifstream file(filename);
        string city1, city2;
        int distance;
        adjList.clear();
        while (file >> city1 >> city2 >> distance) {
            addConnection(city1, city2, distance);
        }
        file.close();
        cout << "Graph loaded from " << filename << endl;
    }

    void bfs(const string &start) {
        unordered_map<string, bool> visited;
        queue<string> q;
        q.push(start);
        visited[start] = true;

        cout << "BFS Traversal starting from " << start << ": ";
        while (!q.empty()) {
            string city = q.front();
            q.pop();
            cout << city << " ";

            for (const auto &neighbor : adjList[city]) {
                if (!visited[neighbor.first]) {
                    visited[neighbor.first] = true;
                    q.push(neighbor.first);
                }
            }
        }
        cout << endl;
    }

    void dfsUtil(const string &city, unordered_map<string, bool> &visited) {
        visited[city] = true;
        cout << city << " ";

        for (const auto &neighbor : adjList[city]) {
            if (!visited[neighbor.first]) {
                dfsUtil(neighbor.first, visited);
            }
        }
    }

    void dfs(const string &start) {
        unordered_map<string, bool> visited;
        cout << "DFS Traversal starting from " << start << ": ";
        dfsUtil(start, visited);
        cout << endl;
    }
};

int main() {
    CityGraph graph;

    // Example connections for Gondar
    graph.addConnection("Fasil Ghebbi", "Gondar Castle", 2);
    graph.addConnection("Fasil Ghebbi", "Debre Birhan Selassie", 3);
    graph.addConnection("Gondar Castle", "Lake Tana", 5);
    graph.addConnection("Debre Birhan Selassie", "Church of St. Mary", 4);
    graph.addConnection("Lake Tana", "Blue Nile Falls", 10);
    graph.addConnection("Church of St. Mary", "Fasil Ghebbi", 2);

    int choice;
    string city1, city2, filename;
    int distance;

    do {
        cout << "\nMenu:\n";
        cout << "1. Add Connection\n";
        cout << "2. Remove Connection\n";
        cout << "3. Update Connection\n";
        cout << "4. Print Connections\n";
        cout << "5. Find Shortest Path\n";
        cout << "6. Save Graph to File\n";
        cout << "7. Load Graph from File\n";
        cout << "8. Find All Paths Between Two Cities\n";
        cout << "9. Find Longest Path Between Two Cities\n";
        cout << "10. Display Neighbors of a City\n";
        cout << "11. Find Isolated Cities\n";
        cout << "12. BFS Traversal\n";
        cout << "13. DFS Traversal\n";
        cout << "14. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter first city: ";
                cin >> city1;
                cout << "Enter second city: ";
                cin >> city2;
                cout << "Enter distance: ";
                cin >> distance;
                graph.addConnection(city1, city2, distance);
                break;

            case 2:
                cout << "Enter first city: ";
                cin >> city1;
                cout << "Enter second city: ";
                cin >> city2;
                graph.removeConnection(city1, city2);
                break;

            case 3:
                cout << "Enter first city: ";
                cin >> city1;
                cout << "Enter second city: ";
                cin >> city2;
                cout << "Enter new distance: ";
                cin >> distance;
                graph.updateConnection(city1, city2, distance);
                break;

            case 4:
                graph.printConnections();
                break;

            case 5:
                cout << "Enter start city: ";
                cin >> city1;
                cout << "Enter destination city: ";
                cin >> city2;
                graph.findShortestPath(city1, city2);
                break;

            case 6:
                cout << "Enter filename: ";
                cin >> filename;
                graph.saveGraphToFile(filename);
                break;

            case 7:
                cout << "Enter filename: ";
                cin >> filename;
                graph.loadGraphFromFile(filename);
                break;

            case 8:
                cout << "Enter start city: ";
                cin >> city1;
                cout << "Enter destination city: ";
                cin >> city2;
                {
                    vector<string> path;
                    vector<vector<string>> allPaths;
                    graph.findAllPaths(city1, city2, path, allPaths);

                    cout << "All paths between " << city1 << " and " << city2 << ":\n";
                    for (const auto &p : allPaths) {
                        for (const auto &city : p) {
                            cout << city << " ";
                        }
                        cout << endl;
                    }
                }
                break;

            case 9:
                cout << "Enter start city: ";
                cin >> city1;
                cout << "Enter destination city: ";
                cin >> city2;
                {
                    vector<string> path;
                    vector<vector<string>> allPaths;
                    graph.findLongestPath(city1, city2, path, allPaths);
                }
                break;

            case 10:
                cout << "Enter city: ";
                cin >> city1;
                graph.displayCityNeighbors(city1);
                break;

            case 11:
                graph.findIsolatedCities();
                break;

            case 12:
                cout << "Enter start city for BFS: ";
                cin >> city1;
                graph.bfs(city1);
                break;

            case 13:
                cout << "Enter start city for DFS: ";
                cin >> city1;
                graph.dfs(city1);
                break;

            case 14:
                cout << "Exiting...\n";
                break;

            default:
                cout << "Invalid choice, please try again.\n";
        }
    } while (choice != 14);

    return 0;
}
