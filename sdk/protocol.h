#pragma once

#include <iostream>

namespace iggy {
namespace models {
class Model {
public:
    Model() = default;
    virtual ~Model() = default;
};
}  // namespace models
}  // namespace iggy

namespace iggy {
namespace formats {

class WireFormat {
public:
    WireFormat(iggy::models::Model& model);
    virtual ~WireFormat() = default;
};

}  // namespace formats
}  // namespace iggy
