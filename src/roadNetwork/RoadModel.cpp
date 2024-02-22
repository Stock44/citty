//
// Created by hiram on 2/21/24.
//

#include "RoadModel.hpp"

namespace citty {

Q_DECLARE_METATYPE(std::pa)

RoadModel::RoadModel(RoadNetwork &roadNetwork, QObject *parent)
    : roadNetwork(roadNetwork) {}

int RoadModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);

  return roadNetwork.roadCount();
}

QVariant RoadModel::data(const QModelIndex &index, int role) const {
  if (index.row() < 0)
    return {};

  auto roadId = index.row();

  auto road = roadNetwork.getRoad(roadId);

  if (road == nullptr) {
    return {};
  }

  if (role == RoadRoles::lanes)
    return road->lanes;
  else if (role == RoadRoles::source) {
    auto source = roadNetwork.source(roadId).transform(
        [this](auto nodeId) { return roadNetwork.getNode(nodeId); });

    if (!source || *source == nullptr) {
      return {};
    }

    return (*source)->position;
  } else if (role == RoadRoles::target) {
    auto target = roadNetwork.target(roadId).transform(
        [this](auto nodeId) { return roadNetwork.getNode(nodeId); });

    if (!target || *target == nullptr) {
      return {};
    }
    return (*target)->position;
  }
  return {};
}

QHash<int, QByteArray> RoadModel::roleNames() const {
  return QAbstractItemModel::roleNames();
}
} // namespace citty
