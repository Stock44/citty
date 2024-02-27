//
// Created by hiram on 2/21/24.
//

#include "RoadModel.hpp"
#include <ranges>

namespace citty {

RoadModel::RoadModel(RoadNetwork &roadNetwork, QObject *parent)
    : QAbstractListModel(parent), roadNetwork(roadNetwork) {
  QObject::connect(&roadNetwork, &RoadNetwork::roadAdded, this,
                   &RoadModel::onRoadAdded);
  QObject::connect(&roadNetwork, &RoadNetwork::roadDeleted, this,
                   &RoadModel::onRoadDeleted);
  QObject::connect(&roadNetwork, &RoadNetwork::roadUpdated, this,
                   &RoadModel::onRoadUpdated);

  std::ranges::copy(roadNetwork.roads(), std::back_inserter(roads));
  for (auto const &[idx, roadId] : std::views::enumerate(roadNetwork.roads())) {
    roadIdRowMap.try_emplace(roadId, idx);
  }
}

int RoadModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);

  return roads.size();
}

QVariant RoadModel::data(const QModelIndex &index, int role) const {
  if (index.row() < 0)
    return {};

  auto roadRow = index.row();

  auto roadId = roads.at(roadRow);

  if (role == RoadRoles::lanes) {
    return roadNetwork.getRoad(roadId).lanes;
  } else if (role == RoadRoles::source) {
    auto source = roadNetwork.getNode(roadId.m_source);
    return source.position;
  } else if (role == RoadRoles::target) {
    auto target = roadNetwork.getNode(roadId.m_target);

    return target.position;
  } else if (role == RoadRoles::rotation) {
    auto source = roadNetwork.getNode(roadId.m_source);
    auto target = roadNetwork.getNode(roadId.m_target);

    if (qFuzzyCompare(target.position, source.position)) {
      return {};
    }

    auto road = target.position - source.position;

    auto rotation = std::atan2(road.x(), road.z());

    if (std::isnan(rotation)) {
      return 0;
    }

    return rotation * 180.0f / std::numbers::pi_v<float>;
  } else if (role == RoadRoles::length) {
    auto source = roadNetwork.getNode(roadId.m_source);
    auto target = roadNetwork.getNode(roadId.m_target);

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
  roles[RoadRoles::source] = "origin";
  roles[RoadRoles::target] = "target";
  roles[RoadRoles::rotation] = "rotation";
  roles[RoadRoles::length] = "length";

  return roles;
}
void RoadModel::onRoadAdded(RoadNetwork::RoadId id) {
  beginInsertRows(QModelIndex(), roads.size(), roads.size());
  roadIdRowMap.try_emplace(id, roads.size());
  roads.push_back(id);
  endInsertRows();
}

void RoadModel::onRoadDeleted(RoadNetwork::RoadId id) {
  auto roadRow = roadIdRowMap.at(id);
  beginRemoveRows(QModelIndex(), roadRow, roadRow);
  roadIdRowMap.erase(id);
  roads.erase(roads.begin() + roadRow);
  endRemoveRows();
}

void RoadModel::onRoadUpdated(RoadNetwork::RoadId id) {
  auto roadRow = roadIdRowMap.at(id);
  dataChanged(index(roadRow), index(roadRow));
}

} // namespace citty
