//
// Created by hiram on 2/20/24.
//

#include "RoadNetwork.hpp"

namespace citty {

RoadId RoadNetwork::addRoad(NodeId n1, NodeId n2, Road road) {
  auto [roadId, ok] = boost::add_edge(n1, n2, graph);
  if (ok) {
    graph[roadId] = road;
    return roadId;
  }

  throw std::runtime_error("could not create edge");
}
NodeId RoadNetwork::addNode(Node node) {
  auto nodeId = boost::add_vertex(graph);

  graph[nodeId] = node;

  return nodeId;
}
Node &RoadNetwork::operator[](NodeId id) { return graph[id]; }
Node const &RoadNetwork::operator[](NodeId id) const { return graph[id]; }
Road &RoadNetwork::operator[](RoadId id) { return graph[id]; }
Road const &RoadNetwork::operator[](RoadId id) const { return graph[id]; }
} // namespace citty
