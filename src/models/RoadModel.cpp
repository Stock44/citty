//
// Created by hiram on 2/21/24.
//

#include "RoadModel.hpp"

namespace citty {

RoadModel::RoadModel(RoadNetwork &roadNetwork, QObject *parent)
    : QAbstractListModel(parent), roadNetwork(roadNetwork) {
  QObject::connect(&roadNetwork, &RoadNetwork::roadAboutToBeCreated, this,
                   &RoadModel::beforeRoadCreation);
  QObject::connect(&roadNetwork, &RoadNetwork::roadCreationFinished, this,
                   &RoadModel::finishedRoadCreation);
  QObject::connect(&roadNetwork, &RoadNetwork::roadAboutToBeDeleted, this,
                   &RoadModel::beforeRoadDeletion);
  QObject::connect(&roadNetwork, &RoadNetwork::roadDeletionFinished, this,
                   &RoadModel::finishedRoadDeletion);
  QObject::connect(&roadNetwork, &RoadNetwork::roadUpdated, this,
                   &RoadModel::onRoadUpdate);
}

int RoadModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);

  return roadNetwork.roadCount();
}

QVariant RoadModel::data(const QModelIndex &index, int role) const {
  if (index.row() < 0)
    return {};

  auto roadId = index.row();

  auto road = roadNetwork.getRoad(roadId);

  if (role == RoadRoles::lanes) {
    return road.lanes;
  } else if (role == RoadRoles::center) {
    auto sourceId = roadNetwork.source(roadId);
    auto targetId = roadNetwork.target(roadId);
    auto source = roadNetwork.getNode(sourceId);
    auto target = roadNetwork.getNode(targetId);

    auto position =
        source.position + ((target.position - source.position) / 2.0f);
    return position;
  } else if (role == RoadRoles::rotation) {
    auto sourceId = roadNetwork.source(roadId);
    auto targetId = roadNetwork.target(roadId);
    auto source = roadNetwork.getNode(sourceId);
    auto target = roadNetwork.getNode(targetId);

    if (qFuzzyCompare(target.position, source.position)) {
      return {};
    }

    auto road = target.position - source.position;

    auto rotation = std::atan2(road.x(), road.z());

    if (std::isnan(rotation)) {
      return 0;
    }

    return rotation * 180.0f / std::numbers::pi_v<float> ;
  } else if (role == RoadRoles::length) {
    auto sourceId = roadNetwork.source(roadId);
    auto targetId = roadNetwork.target(roadId);
    auto source = roadNetwork.getNode(sourceId);
    auto target = roadNetwork.getNode(targetId);

    auto length = (target.position - source.position).length();

    if (length == 0) {
      return 0.1;
    }

    return length;
  }
  return {};
}

QHash<int, QByteArray> RoadModel::roleNames() const {
  QHash<int, QByteArray> roles;

  roles[RoadRoles::lanes] = "lanes";
  roles[RoadRoles::center] = "center";
  roles[RoadRoles::rotation] = "rotation";
  roles[RoadRoles::length] = "length";

  return roles;
}
void RoadModel::beforeRoadCreation(RoadNetwork::Id id) {
  beginInsertRows(QModelIndex(), id, id);
}
void RoadModel::finishedRoadCreation(std::optional<RoadNetwork::Id> id) {
  endInsertRows();
}
void RoadModel::beforeRoadDeletion(RoadNetwork::Id id) {
  beginRemoveRows(QModelIndex(), id, id);
}
void RoadModel::finishedRoadDeletion(std::optional<RoadNetwork::Id> id) {
  endRemoveRows();
}
void RoadModel::onRoadUpdate(RoadNetwork::Id id) {
  dataChanged(index(id), index(id));
}

} // namespace citty
