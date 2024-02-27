//
// Created by hiram on 2/20/24.
//

#include "RoadNetwork.hpp"
#include "NodeProperties.hpp"
#include "RoadProperties.hpp"

namespace citty {

RoadNetwork::RoadId RoadNetwork::addRoad(NodeId n1, NodeId n2,
                                         RoadProperties properties) {
  auto [id, ok] = boost::add_edge(n1, n2, graph);

  if (!ok) {
    throw std::runtime_error("could not add edge to graph");
  }

  graph[id] = properties;

  emit roadAdded(id);

  return id;
}

RoadNetwork::NodeId RoadNetwork::addNode(NodeProperties properties) {
  auto id = boost::add_vertex(graph);

  graph[id] = properties;

  emit nodeAdded(id);

  return id;
}

NodeProperties const &RoadNetwork::getNode(NodeId id) const {
  return graph[id];
}

RoadProperties const &RoadNetwork::getRoad(RoadId id) const {
  return graph[id];
}

RoadNetwork::NodeId RoadNetwork::source(RoadId id) const {
  return boost::source(id, graph);
}

RoadNetwork::NodeId RoadNetwork::target(RoadId id) const {
  return boost::target(id, graph);
}

void RoadNetwork::updateNode(NodeId id, NodeProperties update) {
  graph[id] = update;
  emit nodeUpdated(id);
}

void RoadNetwork::updateRoad(RoadId id, RoadProperties update) {
  graph[id] = update;
  emit roadUpdated(id);
}

void RoadNetwork::deleteNode(NodeId id) {
  auto [outEdgesBegin, outEdgesEnd] = boost::out_edges(id, graph);
  auto [inEdgesBegin, inEdgesEnd] = boost::in_edges(id, graph);

  std::for_each(outEdgesBegin, outEdgesEnd,
                [this](auto roadId) { deleteRoad(roadId); });

  std::for_each(inEdgesBegin, inEdgesEnd,
                [this](auto roadId) { deleteRoad(roadId); });

  boost::remove_vertex(id, graph);

  emit nodeDeleted(id);
}

void RoadNetwork::deleteRoad(RoadId id) {
  boost::remove_edge(id, graph);

  emit roadDeleted(id);
}
std::size_t RoadNetwork::roadCount() { return boost::num_edges(graph); }
std::size_t RoadNetwork::nodeCount() { return boost::num_vertices(graph); }


} // namespace citty
