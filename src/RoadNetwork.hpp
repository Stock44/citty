//
// Created by hiram on 2/20/24.
//

#pragma once

#include <QEntity>
#include <QVector3D>
#include <boost/graph/adjacency_list.hpp>

namespace citty {

struct Road {
  int lanes;
};

struct Node {
  QVector3D position;
};

using RoadNetworkGraph = boost::adjacency_list<boost::listS, boost::listS,
                                               boost::directedS, Node, Road>;

using NodeId = RoadNetworkGraph::vertex_descriptor;
using RoadId = RoadNetworkGraph::edge_descriptor;

class RoadNetwork {
public:
  RoadId addRoad(NodeId n1, NodeId n2, Road road = Road());

  NodeId addNode(Node node = Node());

  Node &operator[](NodeId id);

  Node const &operator[](NodeId id) const;

  Road &operator[](RoadId id);

  Road const &operator[](RoadId id) const;

private:
  RoadNetworkGraph graph;
};

} // namespace citty
