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
   * This function creates a road between the specified nodes in the road
   * network. It uses the provided RoadProperties object to set the properties
   * of the road. If no properties are provided, a default RoadProperties object
   * will be used.
   *
   * If successful, this function returns the unique identifier (Id) of the
   * newly created road. The Id can be used to reference the road in other parts
   * of the code.
   *
   * @param n1 The identifier of the first node connected by the road.
   * @param n2 The identifier of the second node connected by the road.
   * @param properties The properties of the road (optional).
   * @return The unique identifier (Id) of the newly created road.
   *
   * @throws std::runtime_error if an error occurs during the addition of the
   * road
   */
  Id addRoad(std::size_t n1, std::size_t n2,
             RoadProperties properties = RoadProperties());

  /**
   * @brief Adds a new node to the road network with the specified properties.
   *
   * @param properties The properties of the new node (optional).
   * @return The ID of the new node.
   */
  Id addNode(NodeProperties properties = NodeProperties());

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
  void updateNode(Id id, NodeProperties update);

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
  void updateRoad(Id id, RoadProperties update);

  /**
   * @brief Retrieves the source node id of a road.
   *
   * This function returns the id of the source node of the road with the given
   * id in the road network.
   *
   * @param id The id of the road.
   * @return The id of the source node.
   */
  [[nodiscard]] Id source(Id id) const;

  /**
   * @brief Retrieves the target node id of a road.
   *
   * This function returns the id of the target node of the road with the given
   * id in the road network.
   *
   * @param id The id of the road.
   * @return Id The id of the target node.
   */
  [[nodiscard]] Id target(Id id) const;

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
  [[nodiscard]] NodeProperties const &getNode(Id id) const;

  /**
   * @brief Retrieves the properties of a road with the given id.
   *
   * This function returns a const reference to the RoadProperties object that
   * contains the properties of the road with the given id in the road*/
  [[nodiscard]] RoadProperties const &getRoad(Id id) const;

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
  void deleteNode(Id id);

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
  void deleteRoad(Id id);

signals:

  /**
   * @brief Emitted before a new node is created in the network.
   *
   * @param id The id of the new node.
   */
  void nodeAboutToBeCreated(Id id);

  /**
   * @brief Emitted after a new node is created in the network.
   *
   * @param id The id of the new node, or std::nullopt if the creation failed.
   */
  void nodeCreationFinished(std::optional<Id> id);

  /**
   * @brief Emitted before a new road is created in the network.
   *
   * @param id The id of the new road.
   */
  void roadAboutToBeCreated(Id id);

  /**
   * @brief Emitted after a new road is created in the network.
   *
   * @param id The id of the new road, or std::nullopt if the creation failed.
   */
  void roadCreationFinished(std::optional<Id> id);

  /**
   * @brief Emitted after a node is updated in the network.
   *
   * @param id The id of the updated node.
   */
  void nodeUpdated(Id id);

  /**
   * @brief Emitted after a road is updated in the network.
   *
   * @param id The id of the updated road.
   */
  void roadUpdated(Id id);

  /**
   * @brief Emitted before a node is deleted from the network.
   *
   * @param id The id of the node.
   */
  void nodeAboutToBeDeleted(Id id);

  /**
   * @brief Emitted after a node is deleted from the network.
   *
   * @param id The id of the deleted node, or std::nullopt if the deletion
   * failed.
   */
  void nodeDeletionFinished(std::optional<Id> id);

  /**
   * @brief Emitted before a road is deleted from the network.
   *
   * @param id The id of the road.
   */
  void roadAboutToBeDeleted(Id id);

  /**
   * @brief Emitted after a road is deleted from the network.
   *
   * @param id The id of the deleted road, or std::nullopt if the deletion
   * failed.
   */
  void roadDeletionFinished(std::optional<Id> id);

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
