#include "graph.h"
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits.h>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

// constructor, empty graph
// directionalEdges defaults to true
Graph::Graph(bool directionalEdges) {
  verticesCount = 0;
  edgesCount = 0;
  dirEdges = directionalEdges;
}

// destructor
Graph::~Graph() {}

// @return total number of vertices
int Graph::verticesSize() const { return verticesCount; }

// return edgesSize
// @return total number of edges
int Graph::edgesSize() const { return edgesCount; }

// @return number of edges from given vertex, -1 if vertex not found
int Graph::vertexDegree(const string &label) const {
  if (contains(label)) {
    // Make sure that the exception is never thrown!

    return vertices.at(label).size();
  }

  return -1;
}

// @return true if vertex added, false if it already is in the graph
bool Graph::add(const string &label) {
  if (contains(label)) {
    return false;
  }

  vertices[label] = {};
  verticesCount++;
  return true;
}

/** return true if vertex already in graph */
bool Graph::contains(const string &label) const {
  // search vertices for key label
  return vertices.find(label) != vertices.end();
}

// @return string representing edges and weights, "" if vertex not found
// A-3->B, A-5->C should return B(3),C(5)
string Graph::getEdgesAsString(const string &label) const {
  stringstream s("");
  auto it = vertices.find(label);
  if (it != vertices.end()) {
    bool first = true;

    for (const auto &p : it->second) {

      if (first) {
        s << p.first << "(" << p.second << ")";
        first = false;
      } else {
        s << "," << p.first << "(" << p.second << ")";
      }
    }
  }
  return s.str();
}

// @return true if successfully connected
bool Graph::connect(const string &from, const string &to, int weight) {
  // if one or both nodes do not exist, add them!
  add(to); // add already checks if they're contained
  add(from);
  // check if a connection exists already or if attemmpting to connect to self
  if (areConnected(from, to) || to == from) {
    return false;
  }
  // forming the connection

  vertices[from][to] = weight;
  if (!dirEdges) {
    vertices[to][from] = weight;
  }

  edgesCount++;
  return true;
}

// returns true if successfully disconnected
// returns false if one or both vertices are not found
bool Graph::disconnect(const string &from, const string &to) {
  if (!contains(from) || !contains(to)) {
    return false;
  }

  // attempt to find the connection between from and to
  bool connected = areConnected(from, to);
  if (connected) {
    vertices[from].erase(to);
    if (!dirEdges) {
      vertices[to].erase(from);
    }

    edgesCount--;
  }

  return connected;
}

// depth-first traversal starting from given startLabel
void Graph::dfs(const string &startLabel, void visit(const string &label)) {
  // cout << "[Depth-First-Search]: beginning search" << endl;
  //  cout << "\t[Depth-First-Search]: " << endl;

  if (!contains(startLabel)) {
    return;
  }
  visited.clear();
  // cout << "\t[Depth-First-Search]: startLabel " << startLabel
  //      << " is in the graph. Starting search!" << endl;
  dfsHelper(startLabel, visit);
}

// recursive helper for DFS
void Graph::dfsHelper(const string &startLabel,
                      void visit(const string &label)) {

  // mark startLabel as visited
  visited.insert(startLabel);
  visit(startLabel);

  for (const auto p : vertices[startLabel]) {

    if (visited.find(p.first) == visited.end()) {
      dfsHelper(p.first, visit);
    }
  }
}

// breadth-first traversal starting from startLabel
void Graph::bfs(const string &startLabel, void visit(const string &label)) {
  if (!contains(startLabel)) {
    return;
  }

  visited.clear();
  queue<string> q;
  q.push(startLabel);
  visited.insert(startLabel);
  visit(startLabel);

  while (!q.empty()) {
    auto neighbors = vertices[q.front()];
    q.pop();
    for (const auto p : neighbors) {
      if (visited.find(p.first) == visited.end()) {
        visit(p.first);
        visited.insert(p.first);
        q.push(p.first);
      }
    }
  }
}

// store the weights in a map
// store the previous label in a map
pair<map<string, int>, map<string, string>>
Graph::dijkstra(const string &startLabel) const {
  map<string, int> weights;
  map<string, string> previous;

  if (contains(startLabel)) {
    unordered_set<string> vertexSet;
    // initialize weights as INT_MAX
    for (const auto &p : vertices) {
      weights[p.first] = INT_MAX;
      previous[p.first] = "";
    }
    weights[startLabel] = 0;

    string cur = startLabel;
    while (vertexSet.size() < vertices.size()) {
      // pick smallest vertex that's not in vertexSet
      int n = INT_MAX;
      string prev = cur;
      cur = "";
      for (auto &v : weights) {
        if (vertexSet.find(v.first) == vertexSet.end() && v.second < n) {
          cur = v.first;
          n = v.second;
        }
      }

      vertexSet.insert(cur);

      if (cur.empty() || n == INT_MAX) {
        break;
      }

      // attempt to update cur's neighbors
      for (const auto &neighbor : vertices.at(cur)) {

        if (vertexSet.find(neighbor.first) == vertexSet.end()) {

          int newWeight = n + vertices.at(cur).at(neighbor.first);
          if (newWeight < weights[neighbor.first]) {

            weights[neighbor.first] = newWeight;
            previous[neighbor.first] = cur;
          }
        } else {
        }
      }
    }

    weights.erase(startLabel);
    previous.erase(startLabel);

    // Collect keys of unreachable nodes
    vector<string> unreachableNodes;
    for (const auto &w : weights) {
      if (w.second == INT_MAX) {
        unreachableNodes.push_back(w.first);
      }
    }

    // Remove unreachable nodes
    for (const string &node : unreachableNodes) {
      weights.erase(node);
      previous.erase(node);
    }
  }
  // cout << endl;
  return make_pair(weights, previous);
}

// minimum spanning tree using Prim's algorithm
int Graph::mstPrim(const string &startLabel,
                   void visit(const string &from, const string &to,
                              int weight)) {
  if (!contains(startLabel)) {
    return -1;
  }

  int total = 0;
  visited.clear();

  string cur = startLabel;

  // tuple format: (weight, from, to)
  priority_queue<tuple<int, string, string>, vector<tuple<int, string, string>>,
                 greater<tuple<int, string, string>>>
      weights;

  while (visited.size() < vertices.size()) {
    visited.insert(cur);

    // insert neighbors of cur to weights
    for (auto &neighbor : vertices.at(cur)) {
      weights.push(make_tuple(neighbor.second, cur, neighbor.first));
    }

    string next;
    while (next.empty() && !weights.empty()) {
      // pop(?) the top off the queue until the best unvisited node edge is
      // found
      string from = get<1>(weights.top());
      string to = get<2>(weights.top());
      int weight = get<0>(weights.top());
      if (visited.find(to) == visited.end()) {

        total += weight;
        next = to;
        visit(from, to, weight);

      } else {

        weights.pop();
      }
    }
    cur = next;
  }
  return total;
}

// read a text file and create the graph
bool Graph::readFile(const string &filename) {
  ifstream myfile(filename);
  if (!myfile.is_open()) {
    cerr << "Failed to open " << filename << endl;
    return false;
  }
  int edges = 0;
  int weight = 0;
  string fromVertex;
  string toVertex;
  myfile >> edges;
  for (int i = 0; i < edges; ++i) {
    myfile >> fromVertex >> toVertex >> weight;
    connect(fromVertex, toVertex, weight);
  }
  myfile.close();
  return true;
}

// returns true if the two vertices are connected
// Doesn't return any references
bool Graph::areConnected(const string &from, const string &to) {
  // directional graph; only checks if 'from' has 'to' in its map
  if (dirEdges && vertices[from].find(to) != vertices[from].end()) {
    return true;
  }
  if (vertices[from].find(to) != vertices[from].end() ||
      vertices[to].find(from) != vertices[to].end()) {
    return true;
  }
  return false;
}

// produces a human-readable version of the given map
std::string Graph::mapToString(const std::map<std::string, int> &data) {
  std::ostringstream oss;
  oss << "{";
  for (auto it = data.begin(); it != data.end(); ++it) {
    if (it != data.begin()) {
      oss << ", ";
    }
    oss << "'" << it->first << " : " << it->second << "'";
  }
  oss << "}";
  return oss.str();
}
