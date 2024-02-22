//
// Created by hiram on 2/21/24.
//

#pragma once

#include "RoadNetwork.hpp"
#include <QtCore>
namespace citty {

class RoadModel : public QAbstractListModel {
  Q_OBJECT
public:
  enum RoadRoles { lanes = Qt::UserRole + 1, source, target };

  RoadModel(RoadNetwork &roadNetwork, QObject *parent = nullptr);

  [[nodiscard]] int rowCount(const QModelIndex &parent) const;

  [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
  [[nodiscard]] QHash<int, QByteArray> roleNames() const;

private:
  RoadNetwork &roadNetwork;
};

} // namespace citty
