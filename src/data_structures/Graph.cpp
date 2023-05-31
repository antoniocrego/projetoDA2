#include <climits>
#include "Graph.h"

int Graph::getNumVertex() const {
    return vertexSet.size();
}

std::vector<Vertex *> Graph::getVertexSet() const {
    return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
Vertex * Graph::findVertex(const int &id) const {
    for (auto v : vertexSet)
        if (v->getId() == id)
            return v;
    return nullptr;
}

Vertex* Graph::indexVertex(const int &id) const{
    return vertexSet.at(id);
}

/*
 * Finds the index of the vertex with a given content.
 */
int Graph::findVertexIdx(const int &id) const {
    for (unsigned i = 0; i < vertexSet.size(); i++)
        if (vertexSet[i]->getId() == id)
            return i;
    return -1;
}
/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
bool Graph::addVertex(const int &id) {
    if (findVertex(id) != nullptr)
        return false;
    vertexSet.push_back(new Vertex(id));
    return true;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
bool Graph::addEdge(const int &sourc, const int &dest, const double &weight) {
    int cost;
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, weight);
    return true;
}

bool Graph::addBidirectionalEdge(const int &sourc, const int &dest, const double &weight) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    auto e1 = v1->addEdge(v2,weight);
    auto e2 = v2->addEdge(v1,weight);
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}

void deleteMatrix(int **m, int n) {
    if (m != nullptr) {
        for (int i = 0; i < n; i++)
            if (m[i] != nullptr)
                delete [] m[i];
        delete [] m;
    }
}

void deleteMatrix(double **m, int n) {
    if (m != nullptr) {
        for (int i = 0; i < n; i++)
            if (m[i] != nullptr)
                delete [] m[i];
        delete [] m;
    }
}

Graph::~Graph() {
    deleteMatrix(distMatrix, vertexSet.size());
    deleteMatrix(pathMatrix, vertexSet.size());
}

bool Graph::removeVertex(const int &id) {
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        if ((*it)->getId() == id) {
            auto v = *it;
            v->removeOutgoingEdges();
            for (auto u : vertexSet) {
                u->removeEdge(v->getId());
            }
            vertexSet.erase(it);
            delete v;
            return true;
        }
    }
    return false;
}

std::pair<int,double> Graph::Dijsktra(int source,int dest) {
    int n = vertexSet.size();
    int parent[n],dis[n];
    std::fill(parent,parent+n,-1);
    std::fill(dis,dis+n,0);
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int> >, std::greater<std::pair<int, int> > > pq;
    std::vector<int> dist(vertexSet.size(),INT_MAX);
    pq.push(std::make_pair(0,source));
    dist[source] = 0;
    while(pq.top().second != dest && !pq.empty()){
        int top = pq.top().second;
        auto top_v = findVertex(top);
        pq.pop();
        int flag = (top_v->getAdj().size()-1);
        for(int i = 0; i <= flag; i++){
            int v = top_v->getAdj().at(i)->getDest()->getId();
            int weight = top_v->getAdj().at(i)->getWeight();
            if(dist[v]  > dist[top] + weight) {
                parent[v] = top;
                dist[v] = dist[top] + weight;
                pq.push(std::make_pair(dist[v],v));
                dis[v] = dist[v];
            }
            else{
                dis[v]= dist[v];
            }
        }
    }

    double maxFlow=INF;
    if (parent[dest]!=-1) {
        Vertex * v = findVertex(dest);
        while (v->getId() != source) {
            for (auto e: v->getAdj()) {
                if (e->getDest()->getId() == parent[v->getId()]) {
                    v = findVertex(parent[v->getId()]);
                    if (e->getWeight() < maxFlow) maxFlow = e->getWeight();
                }
            }
            for (auto e: v->getIncoming()) {
                if (e->getOrig()->getId() == parent[v->getId()]) {
                    v = findVertex(parent[v->getId()]);
                    if (e->getWeight() < maxFlow) maxFlow = e->getWeight();
                }
            }
        }
    }
    else return {-1,-1};
    return {dis[dest],maxFlow};
}


std::vector<int> Graph::dfs(const int & source) const{
    std::vector<int> result;
    auto s = findVertex(source);
    if (s==nullptr) return result;
    for (auto v : vertexSet) v->setVisited(false);

    dfsVisit(s,result);

    return result;
}

void Graph::dfsVisit(Vertex* v, std::vector<int>& traversal) const{
    v->setVisited(true);
    traversal.push_back(v->getId());
    for (auto& e: v->getAdj()){
        auto w = e->getDest();
        if (!w->isVisited()) dfsVisit(w,traversal);
    }
}

std::string Graph::getType() const {
    return type;
}

void Graph::setType(std::string type) {
    this->type = type;
}

std::vector<Edge *> Graph::prim() {

    std::vector<Edge *> mst;
    if (vertexSet.empty()) {
        return mst;
    }

    // Reset auxiliary info
    for (auto v: vertexSet) {
        v->setDist(INF);
        v->setPath(nullptr);
        v->setVisited(false);
    }

    // start with an arbitrary vertex
    Vertex *s = vertexSet.at(0);
    s->setDist(0);

    // initialize priority queue
    MutablePriorityQueue<Vertex> q;
    q.insert(s);
    // process vertices in the priority queue
    while (!q.empty()) {
        auto v = q.extractMin();
        if(v->getId() != 0) mst.push_back(v->getPath());
        v->setVisited(true);
        for (auto &e: v->getAdj()) {
            Vertex *w = e->getDest();
            if (!w->isVisited()) {
                auto oldDist = w->getDist();
                if (e->getWeight() < oldDist) {
                    w->setDist(e->getWeight());
                    w->setPath(e);
                    if (oldDist == INF) {
                        q.insert(w);
                    } else {
                        q.decreaseKey(w);
                    }
                }
            }
        }
    }

    return mst;
}

Edge * Graph::findEdge(int orig, int dest){
    Vertex * origin = indexVertex(orig);
    for(auto edge : origin->getAdj()){
        if(edge->getDest()->getId() == dest) return edge;
    }
    return nullptr;
}
