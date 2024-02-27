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
  // using lists for containers as they don't invalidate descriptors when
  // deleting vertices
  using Graph =
      boost::adjacency_list<boost::listS, boost::listS, boost::bidirectionalS,
                            NodeProperties, RoadProperties>;

  using RoadId = Graph::edge_descriptor;
  using NodeId = Graph::vertex_descriptor;

  /**
   * @brief Adds a road between two nodes in the road network.
   *
   * This function creates a road between the specified nodes in the road
   * network. It uses the provided RoadProperties object to set the
   * properties of the road. If no properties are provided, a default
   * RoadProperties object will be used.
   *
   * If successful, this function returns the unique identifier (Id) of the
   * newly created road. The Id can be used to reference the road in other
   * parts of the code.
   *
   * @param n1 The identifier of the first node connected by the road.
   * @param n2 The identifier of the second node connected by the road.
   * @param properties The properties of the road (optional).
   * @return The unique identifier (Id) of the newly created road.
   *
   * @throws std::runtime_error if an error occurs during the addition of
   * the road
   */
  RoadId addRoad(NodeId n1, NodeId n2,
                 RoadProperties properties = RoadProperties());

  /**
   * @brief Adds a new node to the road network with the specified properties.
   *
   * @param properties The properties of the new node (optional).
   * @return The ID of the new node.
   */
  NodeId addNode(NodeProperties properties = NodeProperties());

  /**
   * @brief Updates the properties of a node in the road network.
   *
   * This function updates the properties of a node with the given id in the
   * road network.
   *
   * @param id The id of the node to be updated.
   * @param update The updated node properties.
   *
   * @see NodeProperties, Id
   */
  void updateNode(NodeId id, NodeProperties update);

  /**
   * @brief Updates the properties of a road in the road network.
   *
   * This function updates the properties of the road with the given id in the
   * road network. The properties to be updated are specified by the `update`
   * parameter.
   *
   * @param id The id of the road to be updated.
   * @param update The updated road properties.
   *
   * @see RoadProperties, Id
   */
  void updateRoad(RoadId id, RoadProperties update);

  /**
   * @brief Retrieves the source node id of a road.
   *
   * This function returns the id of the source node of the road with the given
   * id in the road network.
   *
   * @param id The id of the road.
   * @return The id of the source node.
   */
  [[nodiscard]] NodeId source(RoadId id) const;

  /**
   * @brief Retrieves the target node id of a road.
   *
   * This function returns the id of the target node of the road with the given
   * id in the road network.
   *
   * @param id The id of the road.
   * @return Id The id of the target node.
   */
  [[nodiscard]] NodeId target(RoadId id) const;

  /**
   * @brief Retrieves the properties of a node with the given id.
   *
   * This function returns a reference to the NodeProperties struct that
   * contains the properties of the node with the given id in the road network.
   *
   * @param id The id of the node.
   * @return const NodeProperties& A reference to the NodeProperties struct of
   * the node.
   *
   * @see NodeProperties, Id
   */
  [[nodiscard]] NodeProperties const &getNode(NodeId id) const;

  /**
   * @brief Retrieves the properties of a road with the given id.
   *
   * This function returns a const reference to the RoadProperties object that
   * contains the properties of the road with the given id in the road*/
  [[nodiscard]] RoadProperties const &getRoad(RoadId id) const;

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
   * @brief Deletes a node from the road network.
   *
   * This function deletes the specified node from the road network.
   * Deleting a node also deletes all the roads connected to it.
   *
   * @param id The ID of the node to be deleted.
   *
   * @see deleteRoad(Id roadId)
   * @see nodeAboutToBeDeleted()
   * @see nodeDeletionFinished(Id nodeId)
   */
  void deleteNode(NodeId id);

  /**
   * @brief Deletes a road from the road network.
   *
   * This function deletes a road from the road network. It emits signals
   * indicating the deletion process. If the road is successfully deleted,
   * it removes the road from the internal data structures and emits the
   * 'roadDeletionFinished' signal with the deleted road ID.
   *
   * @param id The ID of the road to delete.
   */
  void deleteRoad(RoadId id);

  [[nodiscard]] auto roads() const {
    auto [it, end] = boost::edges(graph);

    return std::ranges::subrange(it, end);
  }

  [[nodiscard]] auto nodes() const {
    auto [it, end] = boost::vertices(graph);

    return std::ranges::subrange(it, end);
  }

signals:
  /**
   * @brief Emitted when a new node is created in the network.
   */
  void nodeAdded(NodeId id);

  /**
   * @brief Emitted when a new road is added to the network.
   */
  void roadAdded(RoadId id);

  /**
   * @brief Emitted after a node is updated in the network.
   *
   * @param id The id of the updated node.
   */
  void nodeUpdated(NodeId id);

  /**
   * @brief Emitted after a road is updated in the network.
   *
   * @param id The id of the updated road.
   */
  void roadUpdated(RoadId id);

  /**
   * @brief Emitted when a node is deleted from the network.
   *
   * @param id The id of the node to be deleted.
   */
  void nodeDeleted(NodeId);

  /**
   * @brief Emitted when a road is deleted from the network.
   *
   * @param id The id of the road to be deleted.
   */
  void roadDeleted(RoadId id);

private:
  Graph graph;
};

} // namespace citty
