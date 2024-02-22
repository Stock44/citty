//
// Created by hiram on 2/20/24.
//

#include "RoadNetwork.hpp"
#include "NodeProperties.hpp"
#include "RoadProperties.hpp"

namespace citty {

RoadNetwork::Id RoadNetwork::addRoad(Id n1, Id n2, RoadProperties properties) {
  auto [road, ok] = boost::add_edge(nodes.at(n1), nodes.at(n2), graph);

  if (!ok) {
    throw std::runtime_error("could not create edge");
  }

  Id id;

  bool usedFreedId = !freedRoadIds.empty();

  if (freedRoadIds.empty()) {
    id = roads.size();
  } else {
    id = freedRoadIds.top();
    freedRoadIds.pop();
  }

  auto [roadIt, roadOk] = roads.try_emplace(id, road);
  if (!roadOk) {
    if (usedFreedId) {
      freedRoadIds.push(id);
    }
    boost::remove_edge(road, graph);
    throw std::runtime_error("could not create edge");
  }
  auto [roadIdIt, roadIdOk] = roadIds.try_emplace(road, roads.size());
  if (!roadIdOk) {
    if (usedFreedId) {
      freedRoadIds.push(id);
    }
    boost::remove_edge(road, graph);
    roads.erase(id);
    throw std::runtime_error("could not create edge");
  }

  graph[road] = properties;

  emit roadsChanged(Operation::CREATE, id);

  return id;
}

RoadNetwork::Id RoadNetwork::addNode(NodeProperties properties) {
  auto node = boost::add_vertex(graph);

  Id id;
  bool usedFreedId = !freedNodeIds.empty();

  if (freedNodeIds.empty()) {
    id = nodes.size();
  } else {
    id = freedNodeIds.top();
    freedNodeIds.pop();
  }

  auto [nodeIt, nodeOk] = nodes.try_emplace(id, node);
  if (!nodeOk) {
    if (usedFreedId)
      freedNodeIds.push(id);
    boost::remove_vertex(node, graph);
    throw std::runtime_error("could not create node");
  }
  auto [nodeIdIt, nodeIdOk] = nodeIds.try_emplace(node, id);
  if (!nodeIdOk) {
    if (usedFreedId)
      freedNodeIds.push(id);
    boost::remove_vertex(node, graph);
    nodes.erase(id);
    throw std::runtime_error("could not create node");
  }

  graph[node] = properties;

  emit nodesChanged(Operation::CREATE, id);

  return id;
}

NodeProperties const *RoadNetwork::getNode(Id id) const {
  try {
    auto node = nodes.at(id);

    return &graph[node];
  } catch (...) {
    return nullptr;
  }
}

RoadProperties const *RoadNetwork::getRoad(Id id) const {
  try {
    auto road = roads.at(id);

    return &graph[road];
  } catch (...) {
    return nullptr;
  }
}

std::optional<RoadNetwork::Id> RoadNetwork::source(Id id) const {
  try {
    auto road = roads.at(id);

    auto source = boost::source(roads.at(id), graph);

    return nodeIds.at(source);
  } catch (...) {
    return {};
  }
}

std::optional<RoadNetwork::Id> RoadNetwork::target(Id id) const {
  try {
    auto road = roads.at(id);

    auto source = boost::target(roads.at(id), graph);

    return nodeIds.at(source);
  } catch (...) {
    return {};
  }
}

void RoadNetwork::updateNode(Id id, NodeProperties update) {
  graph[nodes.at(id)] = update;
  emit nodesChanged(Operation::UPDATE, id);
}

void RoadNetwork::updateRoad(Id id, RoadProperties update) {
  graph[roads.at(id)] = update;
  emit roadsChanged(Operation::UPDATE, id);
}

void RoadNetwork::deleteNode(Id id) {
  auto [outEdgesBegin, outEdgesEnd] = boost::out_edges(nodes.at(id), graph);
  auto [inEdgesBegin, inEdgesEnd] = boost::in_edges(nodes.at(id), graph);

  std::for_each(outEdgesBegin, outEdgesEnd,
                [this](auto roadId) { deleteRoad(roadIds.at(roadId)); });
  std::for_each(inEdgesBegin, inEdgesEnd,
                [this](auto roadId) { deleteRoad(roadIds.at(roadId)); });

  auto node = nodes.at(id);

  nodes.erase(id);
  nodeIds.erase(node);
  freedNodeIds.emplace(id);

  emit nodesChanged(Operation::DELETE, id);

  boost::remove_vertex(node, graph);
}

void RoadNetwork::deleteRoad(Id id) {
  auto road = roads.at(id);

  roads.erase(id);
  roadIds.erase(road);
  freedRoadIds.emplace(id);

  emit roadsChanged(Operation::DELETE, id);

  boost::remove_edge(road, graph);
}
std::size_t RoadNetwork::roadCount() { return roads.size(); }
std::size_t RoadNetwork::nodeCount() { return nodes.size(); }

} // namespace citty
