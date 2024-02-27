//
// Created by hiram on 2/26/24.
//

#include "RoadLineInstancedGeometry.hpp"

namespace citty {
RoadLineInstancedGeometry::RoadLineInstancedGeometry(RoadModel const &roadModel,
                                                     Qt3DCore::QNode *parent)
    : Qt3DCore::QGeometry(parent), roadModel(roadModel) {
  using namespace Qt3DCore;

  m_positionAttribute->setName(QAttribute::defaultPositionAttributeName());
  m_positionAttribute->setVertexBaseType(QAttribute::Float);
  m_positionAttribute->setVertexSize(3);
  m_positionAttribute->setAttributeType(QAttribute::VertexAttribute);
  m_positionAttribute->setBuffer(m_vertexBuffer);
  m_positionAttribute->setByteStride(stride);
  m_positionAttribute->setCount(vertCount);
  m_texCoordAttribute->setName(
      QAttribute::defaultTextureCoordinateAttributeName());
  m_texCoordAttribute->setVertexBaseType(QAttribute::Float);
  m_texCoordAttribute->setVertexSize(2);
  m_texCoordAttribute->setAttributeType(QAttribute::VertexAttribute);
  m_texCoordAttribute->setBuffer(m_vertexBuffer);
  m_texCoordAttribute->setByteStride(stride);
  m_texCoordAttribute->setByteOffset(3 * sizeof(float));
  m_texCoordAttribute->setCount(vertCount);
  m_normalAttribute->setName(QAttribute::defaultNormalAttributeName());
  m_normalAttribute->setVertexBaseType(QAttribute::Float);
  m_normalAttribute->setVertexSize(3);
  m_normalAttribute->setAttributeType(QAttribute::VertexAttribute);
  m_normalAttribute->setBuffer(m_vertexBuffer);
  m_normalAttribute->setByteStride(stride);
  m_normalAttribute->setByteOffset(5 * sizeof(float));
  m_normalAttribute->setCount(vertCount);
  m_tangentAttribute->setName(QAttribute::defaultTangentAttributeName());
  m_tangentAttribute->setVertexBaseType(QAttribute::Float);
  m_tangentAttribute->setVertexSize(4);
  m_tangentAttribute->setAttributeType(QAttribute::VertexAttribute);
  m_tangentAttribute->setBuffer(m_vertexBuffer);
  m_tangentAttribute->setByteStride(stride);
  m_tangentAttribute->setByteOffset(8 * sizeof(float));
  m_tangentAttribute->setCount(vertCount);
  m_indexAttribute->setAttributeType(QAttribute::IndexAttribute);
  m_indexAttribute->setVertexBaseType(QAttribute::UnsignedShort);
  m_indexAttribute->setBuffer(m_indexBuffer);
  m_indexAttribute->setCount(lines);
  m_vertexBuffer->setData(generateVertexData());
  m_indexBuffer->setData(generateIndexData());

  m_instancePositionAttribute->setAttributeType(
      Qt3DCore::QAttribute::VertexAttribute);
  m_instancePositionAttribute->setBuffer(m_instanceBuffer);
  m_instancePositionAttribute->setVertexBaseType(Qt3DCore::QAttribute::Float);
  m_instancePositionAttribute->setVertexSize(3);
  m_instancePositionAttribute->setName(
      Qt3DCore::QAttribute::defaultPositionAttributeName());
  m_instancePositionAttribute->setName(QStringLiteral("instancePos"));
  m_instancePositionAttribute->setDivisor(1);
  m_instancePositionAttribute->setByteStride(3 * sizeof(float));

  m_vertexBuffer->setData(generateVertexData());
  m_indexBuffer->setData(generateIndexData());

  addAttribute(m_positionAttribute);
  addAttribute(m_texCoordAttribute);
  addAttribute(m_normalAttribute);
  addAttribute(m_tangentAttribute);
  addAttribute(m_instancePositionAttribute);
  addAttribute(m_indexAttribute);

  setBoundingVolumePositionAttribute(m_instancePositionAttribute);

  connect(&roadModel, &RoadModel::rowsInserted, this,
          &RoadLineInstancedGeometry::onInstancesAdded);
  connect(&roadModel, &RoadModel::rowsRemoved, this,
          &RoadLineInstancedGeometry::onInstancesRemoved);
  connect(&roadModel, &citty::RoadModel::dataChanged, this,
          &citty::RoadLineInstancedGeometry::onInstancesChanged);
}

Qt3DCore::QAttribute *RoadLineInstancedGeometry::positionAttribute() const {
  return m_positionAttribute;
}

Qt3DCore::QAttribute *RoadLineInstancedGeometry::normalAttribute() const {
  return m_normalAttribute;
}

Qt3DCore::QAttribute *RoadLineInstancedGeometry::texCoordAttribute() const {
  return m_texCoordAttribute;
}

Qt3DCore::QAttribute *RoadLineInstancedGeometry::tangentAttribute() const {
  return m_tangentAttribute;
}

Qt3DCore::QAttribute *RoadLineInstancedGeometry::indexAttribute() const {
  return m_indexAttribute;
}

QByteArray RoadLineInstancedGeometry::generateVertexData() const {
  QByteArray bufferBytes;
  bufferBytes.resize(stride * vertCount);

  auto *fptr = reinterpret_cast<float *>(bufferBytes.data());
  // position
  *fptr++ = 0.0f;
  *fptr++ = 0.0f;
  *fptr++ = 0.0f;
  // texture coordinates
  *fptr++ = 0.0f;
  *fptr++ = 0.5f;
  // normal
  *fptr++ = 0.0f;
  *fptr++ = 1.0f;
  *fptr++ = 0.0f;
  // tangent
  *fptr++ = 0.0f;
  *fptr++ = 0.0f;
  *fptr++ = 1.0f;
  *fptr++ = 1.0f;
  // position
  *fptr++ = 1.0f;
  *fptr++ = 0.0f;
  *fptr++ = 0.0f;
  // texture coordinates
  *fptr++ = 1.0f;
  *fptr++ = 0.5f;
  // normal
  *fptr++ = 0.0f;
  *fptr++ = 1.0f;
  *fptr++ = 0.0f;
  // tangent
  *fptr++ = 1.0f;
  *fptr++ = 0.0f;
  *fptr++ = 0.0f;
  *fptr = 1.0f;

  return bufferBytes;
}

QByteArray RoadLineInstancedGeometry::generateIndexData() const {
  QByteArray bufferBytes;
  bufferBytes.resize(2 * lines * sizeof(quint16));
  auto *indexPtr = reinterpret_cast<quint16 *>(bufferBytes.data());

  *indexPtr++ = 0;
  *indexPtr++ = 1;

  return bufferBytes;
}
int RoadLineInstancedGeometry::count() { return roadModel.rowCount(); }
void RoadLineInstancedGeometry::onInstancesAdded(const QModelIndex &parent,
                                                 int first, int last) {}
void RoadLineInstancedGeometry::onInstancesRemoved(const QModelIndex &parent,
                                                   int first, int last) {}
void RoadLineInstancedGeometry::onInstancesChanged(
    QModelIndex const &topLeft, QModelIndex const &bottomRight,
    QList<int> const &roles) {}

} // namespace citty
