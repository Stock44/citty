//
// Created by hiram on 2/26/24.
//

#pragma once

#include "RoadModel.hpp"
#include <QAttribute>
#include <QByteArray>
#include <QGeometry>
#include <QObject>
#include <QList>

namespace citty {

class RoadLineInstancedGeometry : public Qt3DCore::QGeometry {
  Q_OBJECT
  Q_PROPERTY(
      Qt3DCore::QAttribute *positionAttribute READ positionAttribute CONSTANT)
  Q_PROPERTY(
      Qt3DCore::QAttribute *normalAttribute READ normalAttribute CONSTANT)
  Q_PROPERTY(
      Qt3DCore::QAttribute *texCoordAttribute READ texCoordAttribute CONSTANT)
  Q_PROPERTY(
      Qt3DCore::QAttribute *tangentAttribute READ tangentAttribute CONSTANT)
  Q_PROPERTY(Qt3DCore::QAttribute *indexAttribute READ indexAttribute CONSTANT)
  Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
  explicit RoadLineInstancedGeometry(RoadModel const &roadModel,
                                     QNode *parent = nullptr);

  [[nodiscard]] Qt3DCore::QAttribute *positionAttribute() const;
  [[nodiscard]] Qt3DCore::QAttribute *normalAttribute() const;
  [[nodiscard]] Qt3DCore::QAttribute *texCoordAttribute() const;
  [[nodiscard]] Qt3DCore::QAttribute *tangentAttribute() const;
  [[nodiscard]] Qt3DCore::QAttribute *indexAttribute() const;

  [[nodiscard]] int count();

signals:
  void countChanged(int count);

private:
  void onInstancesAdded(QModelIndex const &parent, int first, int last);

  void onInstancesRemoved(QModelIndex const &parent, int first, int last);

  void onInstancesChanged(QModelIndex const &topLeft,
                          QModelIndex const &bottomRight,
                          QList<int> const &roles);

  [[nodiscard]] QByteArray generateVertexData() const;
  [[nodiscard]] QByteArray generateIndexData() const;

  RoadModel const &roadModel;

  Qt3DCore::QAttribute *m_positionAttribute = new Qt3DCore::QAttribute(this);
  Qt3DCore::QAttribute *m_normalAttribute = new Qt3DCore::QAttribute(this);
  Qt3DCore::QAttribute *m_texCoordAttribute = new Qt3DCore::QAttribute(this);
  Qt3DCore::QAttribute *m_tangentAttribute = new Qt3DCore::QAttribute(this);
  Qt3DCore::QAttribute *m_indexAttribute = new Qt3DCore::QAttribute(this);
  Qt3DCore::QAttribute *m_instancePositionAttribute =
      new Qt3DCore::QAttribute(this);
  Qt3DCore::QBuffer *m_vertexBuffer = new Qt3DCore::QBuffer(this);
  Qt3DCore::QBuffer *m_indexBuffer = new Qt3DCore::QBuffer(this);
  Qt3DCore::QBuffer *m_instanceBuffer = new Qt3DCore::QBuffer(this);

  const int lines = 1;
  const int vertCount = 2;
  const int stride = (3 + 2 + 3 + 4) * sizeof(float);
};

} // namespace citty
