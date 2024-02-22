//
// Created by hiram on 2/20/24.
//

#pragma once

#include "NodeProperties.hpp"
#include "RoadProperties.hpp"
#include <QEntity>
#include <QVector3D>
#include <boost/graph/adjacency_list.hpp>
#include <boost/unordered/unordered_flat_map.hpp>
#include <deque>
#include <stack>

namespace citty {

/**
 * @brief RoadNetwork class represents a network of roads.
 *
 * The RoadNetwork class provides functionalities to add and update roads and
 * nodes in the network. It also provides functions to retrieve information
 * about the roads and nodes in the network.
 */
class RoadNetwork : public QObject {
  Q_OBJECT
public:
  using Id = std::size_t;

  /**
   * @brief Adds a road between two nodes in the road network.
   *
   * This function adds a road between the nodes with the given ids in the road
   * network. If no properties are provided, default properties are used.
   *
   * @param n1 The id of the first node.
   * @param n2 The id of the second node.
   * @param properties The properties of the road to be added. Defaults to an
   * empty RoadProperties object if not provided.
   * @return The id of the newly added road.
   *
   * @see RoadProperties
   */
  Id addRoad(std::size_t n1, std::size_t n2,
             RoadProperties properties = RoadProperties());

  /**
   * @brief Adds a node to the road network.
   *
   * This function adds a new node to the road network with the specified
   * properties. If no properties are provided, default properties are used.
   *
   * @param properties The properties of the node to be added. Defaults to an
   * empty NodeProperties object if not provided.
   * @return The id of the newly added node.
   *
   * @see NodeProperties
   */
  Id addNode(NodeProperties properties = NodeProperties());

  /**
   * @brief Update the properties of a node.
   *
   * This function updates the properties of the node with the given id using
   * the provided NodeProperties. The updated properties include the position of
   * the node. After updating the node properties, the function emits a signal
   * indicating that the nodes have been changed.
   *
   * @param id The id of the node to be updated.
   * @param update The updated NodeProperties.
   *
   * @see NodeProperties
   * @see nodesChanged
   */
  void updateNode(Id id, NodeProperties update);

  /**
   * @brief Update the properties of a road.
   *
   * This function updates the properties of the road with the given id using
   * the provided RoadProperties. The updated properties include the number of
   * lanes of the road. After updating the road properties, the function emits a
   * signal indicating that the roads have been changed.
   *
   * @param id The id of the road to be updated.
   * @param update The updated RoadProperties.
   *
   * @see RoadProperties
   * @see roadsChanged
   */
  void updateRoad(Id id, RoadProperties update);

  /**
   * @brief Retrieves the source node id of a road.
   *
   * This function returns the id of the source node of the road with the given id in the road network.
   *
   * @param id The id of the road.
   * @return std::optional<Id> The id of the source node if the road exists, otherwise returns an empty optional.
   */
  [[nodiscard]] std::optional<Id> source(Id id) const;

  /**
   * @brief Retrieves the target node id of a road.
   *
   * This function returns the id of the target node of the road with the given id in the road network.
   *
   * @param id The id of the road.
   * @return std::optional<Id> The id of the target node if the road exists, otherwise returns an empty optional.
   */
  [[nodiscard]] std::optional<Id> target(Id id) const;

  /**
   * @brief Get the properties of a node.
   *
   * This function returns a pointer to the NodeProperties struct that contains
   * the properties of the node with the given id. If the node with the given id
   * does not exist, nullptr is returned.
   *
   * @param id The id of the node.
   * @return const NodeProperties* A pointer to the NodeProperties struct of the
   * node or nullptr if the node does not exist.
   *
   * @see NodeProperties
   */
  [[nodiscard]] NodeProperties const *getNode(Id id) const;

  /**
   * @brief Get the properties of a road.
   *
   * This function returns a pointer to the RoadProperties struct that contains
   * the properties of the road with the given id. If the road with the given id
   * does not exist, nullptr is returned.
   *
   * @param id The id of the road.
   * @return const RoadProperties* A pointer to the RoadProperties struct of the
   * road or nullptr if the road does not exist.
   *
   * @see RoadProperties
   */
  [[nodiscard]] RoadProperties const *getRoad(Id id) const;

  /**
   * This function returns the total number of roads present in the RoadNetwork.
   *
   * @return std::size_t The number of roads in the RoadNetwork.
   */
  [[nodiscard]] std::size_t roadCount();

  /**
   * This function returns the total number of nodes present in the RoadNetwork.
   *
   * @return std::size_t The number of nodes in the RoadNetwork.
   */
  [[nodiscard]] std::size_t nodeCount();

  /**
   * @brief Delete a node from the road network.
   *
   * This function deletes the node with the given id from the road network.
   * It also deletes all the roads connected to the node.
   * After deleting the node and roads, the function emits a signal indicating
   * that the nodes have been changed.
   *
   * @param id The id of the node to be deleted.
   *
   * @see nodesChanged
   * @see deleteRoad
   */
  void deleteNode(Id id);

  /**
   * @brief Deletes a road from the road network.
   *
   * This function deletes the road with the given id from the road network.
   * After deleting the road, the function emits a signal indicating that the
   * roads have been changed.
   *
   * @param id The id of the road to be deleted.
   *
   * @see roadsChanged
   */
  void deleteRoad(Id id);

  /**
   * @brief The Operation enum class represents different operation types on the
   * road network.
   *
   * This enum class defines three types of operations: CREATE, DELETE, and
   * UPDATE. Each operation is used to indicate a specific type of change made
   * to either a node or a road within a signal.
   *
   */
  enum class Operation { CREATE, DELETE, UPDATE };

signals:
  /**
   * @brief These signals are emitted when the properties of a node or road in
   * the road network have changed.
   *
   * The nodesChanged and roadsChanged signals are emitted when the properties
   * of a node or road in the road network have been updated. They contain two
   * parameters: an Operation enum value representing the type of operation
   * (CREATE, DELETE, or UPDATE) and an Id representing the unique identifier of
   * the node or road that has been changed.
   *
   * @see RoadNetwork::Operation
   */

  void nodesChanged(citty::RoadNetwork::Operation op, Id id);

  void roadsChanged(citty::RoadNetwork::Operation op, Id id);

private:
  // using lists for containers as they don't invalidate descriptors when
  // deleting vertices
  using Graph =
      boost::adjacency_list<boost::listS, boost::listS, boost::bidirectionalS,
                            NodeProperties, RoadProperties>;

  Graph graph;

  std::stack<Id> freedRoadIds;

  std::stack<Id> freedNodeIds;

  boost::unordered_flat_map<Id, Graph::edge_descriptor> roads;
  boost::unordered_flat_map<Id, Graph::vertex_descriptor> nodes;

  boost::unordered_flat_map<Graph::edge_descriptor, Id> roadIds;
  boost::unordered_flat_map<Graph::vertex_descriptor, Id> nodeIds;
};

} // namespace citty
