//
// Created by hiram on 2/20/24.
//

#include "RoadNetwork.hpp"
#include "NodeProperties.hpp"
#include "RoadProperties.hpp"

namespace citty {

RoadNetwork::Id RoadNetwork::addRoad(Id n1, Id n2, RoadProperties properties) {
  Id id;

  auto node1 = nodes.at(n1);

  auto node2 = nodes.at(n2);

  bool usedFreedId = !freedRoadIds.empty();

  if (freedRoadIds.empty()) {
    id = roads.size();
  } else {
    id = freedRoadIds.top();
    freedRoadIds.pop();
  }

  emit roadAboutToBeCreated(id);
  auto [road, ok] = boost::add_edge(node1, node2, graph);

  if (!ok) {
    emit roadCreationFinished({});
    throw std::runtime_error("could not add edge to graph");
  }

  auto [roadIt, roadOk] = roads.try_emplace(id, road);
  if (!roadOk) {
    if (usedFreedId) {
      freedRoadIds.push(id);
    }
    boost::remove_edge(road, graph);
    emit roadCreationFinished({});
    throw std::runtime_error("could not register road edge descriptor");
  }

  auto [roadIdIt, roadIdOk] = roadIds.try_emplace(road, roads.size());
  if (!roadIdOk) {
    if (usedFreedId) {
      freedRoadIds.push(id);
    }
    boost::remove_edge(road, graph);
    roads.erase(id);
    emit roadCreationFinished({});
    throw std::runtime_error("could not register road edge descriptor");
  }

  graph[road] = properties;

  emit roadCreationFinished(id);

  return id;
}

RoadNetwork::Id RoadNetwork::addNode(NodeProperties properties) {

  Id id;
  bool usedFreedId = !freedNodeIds.empty();

  if (freedNodeIds.empty()) {
    id = nodes.size();
  } else {
    id = freedNodeIds.top();
    freedNodeIds.pop();
  }

  emit nodeAboutToBeCreated(id);

  auto node = boost::add_vertex(graph);

  auto [nodeIt, nodeOk] = nodes.try_emplace(id, node);
  if (!nodeOk) {
    if (usedFreedId)
      freedNodeIds.push(id);
    boost::remove_vertex(node, graph);
    emit nodeCreationFinished({});
    throw std::runtime_error("could not register node vertex descriptor");
  }
  auto [nodeIdIt, nodeIdOk] = nodeIds.try_emplace(node, id);
  if (!nodeIdOk) {
    if (usedFreedId)
      freedNodeIds.push(id);
    boost::remove_vertex(node, graph);
    nodes.erase(id);
    emit nodeCreationFinished({});
    throw std::runtime_error("could not register node vertex descriptor");
  }

  graph[node] = properties;

  emit nodeCreationFinished(id);

  return id;
}

NodeProperties const &RoadNetwork::getNode(Id id) const {
  auto node = nodes.at(id);

  return graph[node];
}

RoadProperties const &RoadNetwork::getRoad(Id id) const {
  auto road = roads.at(id);

  return graph[road];
}

RoadNetwork::Id RoadNetwork::source(Id id) const {
  try {
    auto road = roads.at(id);

    auto source = boost::source(roads.at(id), graph);

    return nodeIds.at(source);
  } catch (...) {
    return {};
  }
}

RoadNetwork::Id RoadNetwork::target(Id id) const {
  auto road = roads.at(id);

  auto source = boost::target(roads.at(id), graph);

  return nodeIds.at(source);
}

void RoadNetwork::updateNode(Id id, NodeProperties update) {
  graph[nodes.at(id)] = update;
  emit nodeUpdated(id);
}

void RoadNetwork::updateRoad(Id id, RoadProperties update) {
  graph[roads.at(id)] = update;
  emit roadUpdated(id);
}

void RoadNetwork::deleteNode(Id id) {

  if (!nodes.contains(id))
    throw std::runtime_error("node does not exist");

  auto node = nodes[id];

  auto [outEdgesBegin, outEdgesEnd] = boost::out_edges(node, graph);
  auto [inEdgesBegin, inEdgesEnd] = boost::in_edges(node, graph);

  std::for_each(outEdgesBegin, outEdgesEnd,
                [this](auto roadId) { deleteRoad(roadIds.at(roadId)); });

  std::for_each(inEdgesBegin, inEdgesEnd,
                [this](auto roadId) { deleteRoad(roadIds.at(roadId)); });

  emit nodeAboutToBeDeleted(id);

  auto delCount = nodes.erase(id);
  if (delCount == 0) {
    emit nodeDeletionFinished({});
    throw std::runtime_error("could not delete node vertex descriptor");
  }

  delCount = nodeIds.erase(node);
  if (delCount == 0) {
    nodes.emplace(id, node);
    emit nodeDeletionFinished({});
    throw std::runtime_error("could not delete node vertex descriptor");
  }

  freedNodeIds.push(id);

  boost::remove_vertex(node, graph);

  emit nodeDeletionFinished(id);
}

void RoadNetwork::deleteRoad(Id id) {
  if (!roads.contains(id))
    throw std::runtime_error("road doesn't exist");

  emit roadAboutToBeDeleted(id);

  auto road = roads[id];

  auto delCount = roads.erase(id);

  if (delCount == 0) {
    emit roadDeletionFinished({});
    throw std::runtime_error("could not delete road edge descriptor");
  }

  delCount = roadIds.erase(road);

  if (delCount == 0) {
    roads.emplace(id, road);
    emit roadDeletionFinished({});
    throw std::runtime_error("could not delete road edge descriptor");
  }

  freedRoadIds.emplace(id);

  boost::remove_edge(road, graph);

  emit roadDeletionFinished(id);
}
std::size_t RoadNetwork::roadCount() { return roads.size(); }
std::size_t RoadNetwork::nodeCount() { return nodes.size(); }

} // namespace citty
