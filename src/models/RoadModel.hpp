//
// Created by hiram on 2/21/24.
//

#pragma once

#include "roadNetwork/RoadNetwork.hpp"
#include <QtCore>
#include <QtQmlIntegration>
namespace citty {

class RoadModel : public QAbstractListModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("passed from initial properties")
public:
  enum RoadRoles { lanes = Qt::UserRole + 1, source, target, rotation, length };

  /**
   * @brief Constructs a RoadModel object.
   *
   * This constructor creates a RoadModel object that is used to represent a
   *model of roads in a road network. It takes a reference to a RoadNetwork
   *object and an optional QObject parent . The RoadNetwork object is used to
   *provide the data for the road model.
   *
   * @param roadNetwork The RoadNetwork object used to provide the data for the
   *road model.
   * @param parent An optional QObject parent object. Defaults to nullptr.
   */
  explicit RoadModel(RoadNetwork &roadNetwork, QObject *parent = nullptr);

  /**
   * @brief Returns the number of rows in the model.
   *
   * This function returns the number of rows in the model under the given
   * parent index.
   *
   * @param parent The parent index.
   * @return The number of rows in the model.
   */
  [[nodiscard]] int
  rowCount(const QModelIndex &parent = QModelIndex()) const override;

  /**
   * @brief Returns the data stored under the given role for the item referred
   * to by the index.
   *
   * This function returns the data stored under the given `role` for the item
   * referred to by the `index`. The `role` determines what kind of data should
   * be returned. If the `role` is `RoadModel::lanes`, it returns the number of
   * lanes for the road at the given `index`. If the `role` is
   * `RoadModel::source`, it returns the position of the source node for the
   * road at the given `index`. If the `role` is `RoadModel::target`, it returns
   * the position of the target node for the road at the given `index`. For any
   * other `role`, it returns an empty QVariant.
   *
   * @param index The index of the item.
   * @param role The role for which to return the data.
   * @return The requested data.
   */
  [[nodiscard]] QVariant data(const QModelIndex &index,
                              int role = Qt::DisplayRole) const override;

protected:
  /**
   * @brief Returns the role names for the model.
   *
   * This function returns a QHash of role names for the model. The role names
   * map the integer roles to their corresponding string names.
   *
   * @return A QHash of role names.
   */
  [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

private:
  void onRoadAdded(RoadNetwork::RoadId id);

  void onRoadDeleted(RoadNetwork::RoadId id);

  void onRoadUpdated(RoadNetwork::RoadId id);

  RoadNetwork &roadNetwork;
  std::vector<RoadNetwork::RoadId> roads;
  boost::unordered_flat_map<RoadNetwork::RoadId, std::size_t> roadIdRowMap;
};

} // namespace citty
