//
// Created by hiram on 5/6/22.
//

#ifndef CITYY_INDEXBUFFER_H
#define CITYY_INDEXBUFFER_H

#include <vector>
#include <cstdlib>
#include "Buffer.h"

namespace graphics {

    class IndexBuffer : public Buffer {
    public:
        [[nodiscard]] long getSize() const override;

        void enableAttribs() override;

        void addIndex(GLuint index);

        void addIndices(std::vector<GLuint> const &newIndices);

        void setIndices(std::vector<GLuint> const &newIndices);

        void setIndices(std::vector<GLuint> &&newIndices);

        void modifyIndices(std::vector<GLuint> modIndices, long modStart);

        void upload() override;

    private:
        std::vector<GLuint> indices;
    };

} // graphics

#endif //CITYY_INDEXBUFFER_H
