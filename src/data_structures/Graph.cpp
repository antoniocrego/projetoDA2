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
bool Graph::addEdge(const int &sourc, const int &dest, double w, std::string service) {
    int cost;
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, w, service);
    return true;
}

bool Graph::addBidirectionalEdge(const int &sourc, const int &dest, double w, std::string service) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    auto e1 = v1->addEdge(v2, w, service);
    auto e2 = v2->addEdge(v1, w, service);
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

void Graph::testAndVisit(std::queue< Vertex*> &q, Edge *e, Vertex *w, double residual) {
    if (! w->isVisited() && residual > 0) {
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
}

bool Graph::findAugmentingPath(Vertex *s, Vertex *t) {
    for(auto v : vertexSet) {
        v->setVisited(false);
    }
    s->setVisited(true);
    std::queue<Vertex *> q;
    q.push(s);
    while( !q.empty() && !t->isVisited()) {
        auto v = q.front();
        q.pop();
        for(auto e: v->getAdj()) {
            testAndVisit(q, e, e->getDest(), e->getWeight() - e->getFlow());
        }
        for(auto e: v->getIncoming()) {
            testAndVisit(q, e, e->getOrig(), e->getFlow());
        }
    }
    return t->isVisited();
}

double Graph::findMinResidualAlongPath(Vertex *s, Vertex *t) {
    double f = INF;
    for (auto v = t; v != s; ) {
        auto e = v->getPath();
        if (e->getDest() == v) {
            f = std::min(f, e->getWeight() - e->getFlow());
            v = e->getOrig();
        }
        else {
            f = std::min(f, e->getFlow());
            v = e->getDest();
        }
    }
    return f;
}

void Graph::augmentFlowAlongPath(Vertex *s, Vertex *t, double f) {
    for (auto v = t; v != s;) {
        auto e = v->getPath();
        double flow = e->getFlow();
        if (e->getDest() == v) {
            e->setFlow(flow + f);
            v = e->getOrig();
        } else {
            e->setFlow(flow - f);
            v = e->getDest();
        }
    }
}

double Graph::edmondsKarp(int source, int target) {
    double maxFlow = 0;
    Vertex *s = findVertex(source);// O(v)
    Vertex *t = findVertex(target);
    if (s == nullptr || t == nullptr || s == t)
        return -1;

    // Reset the flows
    for (auto v: vertexSet) {
        for (auto e: v->getAdj()) { //O(E)
            e->setFlow(0);
        }
    }
    // Loop to find augmentation paths
    while (findAugmentingPath(s, t)) {
        double f = findMinResidualAlongPath(s, t);
        augmentFlowAlongPath(s, t, f);
        maxFlow += f;
    }
    return maxFlow;
}

int Graph::megaSink(const std::vector<int>& sinks) {
    this->addVertex(this->getNumVertex());
    for(auto e: sinks) {
        addEdge(e,this->getNumVertex()-1,INF,"");
    }
    return this->getNumVertex()-1;
}

int Graph::megaSource(const std::vector<int>& sources) {
    this->addVertex(this->getNumVertex());
    for(int i : sources){
        this->addEdge(this->getNumVertex()-1, i, INF, "");
    }
    return this->getNumVertex()-1;
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
            int weight = top_v->getAdj().at(i)->getCost();
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
