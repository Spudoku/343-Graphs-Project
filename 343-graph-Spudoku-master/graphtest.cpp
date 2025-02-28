/**
 * Testing BST - Binary Search Tree functions
 *
 * @author Yusuf Pisan
 * @date 19 Oct 2019
 */

#include "graph.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// global value for testing
// NOLINTNEXTLINE
stringstream globalSS;

// need to reset SS before calling this
void vertexPrinter(const string &s) { globalSS << s; }

// need to reset SS before calling this
void edgePrinter(const string &from, const string &to, int weight) {
  globalSS << "[" << from << to << " " << weight << "]";
}

// convert a map to a string so we can compare it
template <typename K, typename L>
static string map2string(const map<K, L> &mp) {
  stringstream out;
  for (auto &p : mp) {
    out << "[" << p.first << ":" << p.second << "]";
  }
  return out.str();
}

void testGraphBasic() {
  cout << "Starting testGraphBasic" << endl;
  Graph g;
  assert(g.add("a") && "add vertex a");
  assert(g.add("b") && "add vertex b");
  assert(g.add("c") && "add vertex c");
  assert(g.add("d") && "add vertex d");
  assert(g.add("e") && "add vertex e");
  assert(!g.add("b") && "b added twice");

  assert(g.connect("a", "b", 10) && "connect a b");
  // //cout << "Edges count after connecting a to b: " << g.edgesSize() << endl;

  assert(!g.connect("a", "b", 50) && "duplicate connect a b");
  // //cout << "Edges count after connecting a to b (duplicate): " <<
  // g.edgesSize()
  //<< endl;

  assert(!g.connect("a", "a", 1) && "connect a to itself");
  // //cout << "Edges count after connecting a itself: " << g.edgesSize() <<
  // endl;

  g.connect("a", "d", 40);
  // //cout << "Edges count after connecting a to d: " << g.edgesSize() << endl;

  g.connect("a", "c", 20);
  // //cout << "Edges count after connecting a to c: " << g.edgesSize() << endl;

  assert((g.verticesSize() == 5) && "graph number of vertices");
  assert((g.edgesSize() == 3) && "graph number of edges");
  assert((g.vertexDegree("a") == 3) && "vertex number of edges");
  // //cout << "g.vertexDegree('c'): " << g.vertexDegree("c") << endl;
  //  ////cout << g.printContents("c") << endl;

  assert((g.vertexDegree("c") == 0) && "no outgoing edges c");
  assert((g.vertexDegree("xxx") == -1) && "no edges for xxx");
  assert(!g.contains("xxx") && "xxx not in graph");
  assert(g.contains("a") && "a in graph");

  // check that they are sorted based on edge end label
  // //cout << "g.getEdgesAsString('a'): " << g.getEdgesAsString("a") << endl;
  assert(g.getEdgesAsString("a") == "b(10),c(20),d(40)");
  // disconnect non-existent edge/vertex
  assert(!g.disconnect("a", "e") && "disconnecting non-existent vertex");
  assert((g.edgesSize() == 3) && "disconnected nonexisting");
  assert(g.disconnect("a", "c") && "a-c disconnect");

  assert((g.edgesSize() == 2) && "number of edges after disconnect");
  // //cout << "g.vertexDegree('a'): " << g.vertexDegree("a") << endl;
  assert((g.vertexDegree("a") == 2) && "a has 2 edges");
  assert(g.getEdgesAsString("a") == "b(10),d(40)" && "removing middle edge");

  cout << "Ending testGraphBasic" << endl;
}

void testGraph0DFS() {
  cout << "Starting testGraph0DFS" << endl;
  Graph g;
  if (!g.readFile("graph0.txt")) {
    return;
  }
  assert(g.contains("A") && "a in graph");
  assert(g.contains("B") && "b in graph");
  assert(g.contains("C") && "c in graph");
  assert(g.getEdgesAsString("A") == "B(1),C(8)");
  assert(g.getEdgesAsString("B") == "C(3)");
  assert(g.getEdgesAsString("C").empty());

  g.dfs("A", vertexPrinter);
  // //cout << "Getting edges of A: " << g.getEdgesAsString("A") << endl;
  // //cout << "globalSS.str(): " << globalSS.str() << endl;
  assert(globalSS.str() == "ABC" && "starting from A");

  globalSS.str("");
  g.dfs("B", vertexPrinter);
  assert(globalSS.str() == "BC" && "starting from B");

  globalSS.str("");
  g.dfs("C", vertexPrinter);
  assert(globalSS.str() == "C" && "starting from C");

  globalSS.str("");
  g.dfs("X", vertexPrinter);
  assert(globalSS.str().empty() && "starting from X");
  cout << "Ending testGraph0DFS" << endl;
}

void testGraph0BFS() {
  cout << "testGraph0BFS" << endl;
  Graph g;
  if (!g.readFile("graph0.txt")) {
    return;
  }

  globalSS.str("");
  g.bfs("A", vertexPrinter);
  // //cout << "Getting edges of A: " << g.getEdgesAsString("A") << endl;
  // //cout << "globalSS.str(): " << globalSS.str() << endl;
  assert(globalSS.str() == "ABC" && "starting from A");

  globalSS.str("");
  g.bfs("B", vertexPrinter);
  assert(globalSS.str() == "BC" && "starting from B");

  globalSS.str("");
  g.bfs("C", vertexPrinter);
  assert(globalSS.str() == "C" && "starting from C");

  globalSS.str("");
  g.bfs("X", vertexPrinter);
  assert(globalSS.str().empty() && "starting from X");

  cout << "Ending testGraph0DFS" << endl;
}

void testGraph0Dijkstra() {
  cout << "Starting testGraph0Dijkstra" << endl;
  Graph g;
  if (!g.readFile("graph0.txt")) {
    return;
  }
  // make sure the file is being read
  // //cout << "[Dijkstra] checking contents of A: " << g.getEdgesAsString("A")
  //      << endl;
  map<string, int> weights;
  map<string, string> previous;
  tie(weights, previous) = g.dijkstra("A");
  // //cout << "[Dijkstra] Dijkstra(A) weights is " << map2string(weights) <<
  // endl; //cout << "[Dijkstra] Dijkstra(A) previous is " <<
  // map2string(previous)
  // << endl;
  assert(map2string(weights) == "[B:1][C:4]" && "Dijkstra(A) weights");

  assert(map2string(previous) == "[B:A][C:B]" && "Dijkstra(A) previous");

  // cout << endl << endl;
  tie(weights, previous) = g.dijkstra("B");
  // //cout << "[Dijkstra] Dijkstra(B) weights is " << map2string(weights) <<
  // endl; //cout << "[Dijkstra] Dijkstra(B) previous is " <<
  // map2string(previous)
  // << endl;
  // cout << endl << endl;
  assert(map2string(weights) == "[C:3]" && "Dijkstra(B) weights");
  assert(map2string(previous) == "[C:B]" && "Dijkstra(B) previous");

  tie(weights, previous) = g.dijkstra("X");
  assert(map2string(weights).empty() && "Dijkstra(C) weights");
  assert(map2string(previous).empty() && "Dijkstra(C) previous");

  cout << "Ending testGraph0Dijkstra" << endl;
}

void testGraph0NotDirected() {
  cout << "Starting testGraph0NotDirected" << endl;
  bool isDirectional = false;
  Graph g(isDirectional);
  if (!g.readFile("graph0.txt")) {
    return;
  }

  globalSS.str("");
  g.bfs("A", vertexPrinter);

  assert(globalSS.str() == "ABC" && "starting from A");

  globalSS.str("");
  g.dfs("B", vertexPrinter);
  assert(globalSS.str() == "BAC" && "starting from B");

  globalSS.str("");
  g.dfs("C", vertexPrinter);
  assert(globalSS.str() == "CAB" && "starting from C");

  globalSS.str("");
  g.dfs("X", vertexPrinter);
  assert(globalSS.str().empty() && "starting from X");

  map<string, int> weights;
  map<string, string> previous;
  tie(weights, previous) = g.dijkstra("A");
  // //cout << "[Dijkstra] Dijkstra(A) weights is " << map2string(weights) <<
  // endl; //cout << "[Dijkstra] Dijkstra(A) previous is " <<
  // map2string(previous)
  // << endl;
  assert(map2string(weights) == "[B:1][C:4]" && "Dijkstra(A) weights");
  assert(map2string(previous) == "[B:A][C:B]" && "Dijkstra(A) previous");
  // //cout << endl << endl;
  // //cout << "[Dijkstra] Dijkstra(A) weights is " << map2string(weights) <<
  // endl; //cout << "[Dijkstra] Dijkstra(A) previous is " <<
  // map2string(previous)
  // << endl;
  tie(weights, previous) = g.dijkstra("B");
  // cout << endl << endl;
  //  //cout << "[Dijkstra] Dijkstra(B) weights is " << map2string(weights) <<
  //  endl; //cout << "[Dijkstra] Dijkstra(B) previous is " <<
  //  map2string(previous)
  //  << endl;
  assert(map2string(weights) == "[A:1][C:3]" && "Dijkstra(B) weights");
  assert(map2string(previous) == "[A:B][C:B]" && "Dijkstra(B) previous");
  // cout << endl << endl;
  tie(weights, previous) = g.dijkstra("X");
  assert(map2string(weights).empty() && "Dijkstra(C) weights");
  assert(map2string(previous).empty() && "Dijkstra(C) previous");

  // cout << "Starting MST Prim tests!" << endl;
  globalSS.str("");
  int mstLength = g.mstPrim("A", edgePrinter);
  // cout << "mstLength: " << mstLength << endl;
  // cout << "globalSS.str(): " << globalSS.str() << endl;
  assert(mstLength == 4 && "mst A is 4");
  assert(globalSS.str() == "[AB 1][BC 3]" && "mst A is [AB 1][BC 3]");

  globalSS.str("");
  mstLength = g.mstPrim("B", edgePrinter);
  // cout << "mstLength: " << mstLength << endl;
  // cout << "globalSS.str(): " << globalSS.str() << endl;
  assert(mstLength == 4 && "mst 4 is 4");
  assert(globalSS.str() == "[BA 1][BC 3]");

  globalSS.str("");
  mstLength = g.mstPrim("C", edgePrinter);
  // cout << "mstLength: " << mstLength << endl;
  // cout << "globalSS.str(): " << globalSS.str() << endl;
  assert(mstLength == 4 && "mst C is 4");
  assert(globalSS.str() == "[CB 3][BA 1]");

  globalSS.str("");
  mstLength = g.mstPrim("X", edgePrinter);
  // cout << "mstLength: " << mstLength << endl;
  // cout << "globalSS.str(): " << globalSS.str() << endl;
  assert(mstLength == -1 && "mst X is -1");
  assert(globalSS.str().empty() && "mst for vertex not found");

  // bonus Kruskal
  // //cout << "Starting Kruskal tests!" << endl;
  // globalSS.str("");
  // mstLength = g.mstKruskal(edgePrinter);
  // assert(mstLength == 4 && "mstKruskal is 4");
  cout << "Ending testGraph0NotDirected" << endl;
}

void testGraph1() {
  // cout << "Starting testGraph1" << endl;
  Graph g;
  if (!g.readFile("graph1.txt")) {
    return;
  }
  globalSS.str("");
  g.dfs("A", vertexPrinter);
  assert(globalSS.str() == "ABCDEFGH" && "dfs starting from A");

  globalSS.str("");
  g.bfs("A", vertexPrinter);
  assert(globalSS.str() == "ABHCGDEF" && "bfs starting from A");

  globalSS.str("");
  g.dfs("B", vertexPrinter);
  assert(globalSS.str() == "BCDEFG" && "dfs starting from B");

  globalSS.str("");
  g.bfs("B", vertexPrinter);
  assert(globalSS.str() == "BCDEFG" && "bfs starting from B");

  map<string, int> weights;
  map<string, string> previous;
  auto p = g.dijkstra("A");
  weights = p.first;
  previous = p.second;
  assert(map2string(weights) == "[B:1][C:2][D:3][E:4][F:5][G:4][H:3]" &&
         "Dijkstra(B) weights");
  assert(map2string(previous) == "[B:A][C:B][D:C][E:D][F:E][G:H][H:A]" &&
         "Dijkstra(B) previous");

  // cout << "Ending testGraph1" << endl;
}

void testAll() {

  testGraphBasic();
  testGraph0DFS();
  testGraph0BFS();
  testGraph0Dijkstra();
  testGraph0NotDirected();
  testGraph1();
}
