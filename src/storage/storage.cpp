#include "taco/storage/storage.h"

#include <iostream>
#include <string>

#include "taco/type.h"
#include "taco/format.h"
#include "taco/error.h"
#include "taco/storage/index.h"
#include "taco/storage/array.h"
#include "taco/storage/array_util.h"
#include "taco/util/strings.h"

using namespace std;

namespace taco {
namespace storage {

// class Storage
struct Storage::Content {
  Format format;
  Index  index;
  Array  values;
};

Storage::Storage() : content(nullptr) {
}

Storage::Storage(const Format& format) : content(new Content) {
  content->format = format;
}

void Storage::setValues(double* values) {
  content->values = makeArray(values, getIndex().getSize());
} 

const Format& Storage::getFormat() const {
  return content->format;
}

void Storage::setIndex(const Index& index) {
  content->index = index;
}

const Index& Storage::getIndex() const {
  return content->index;
}

Index Storage::getIndex() {
  return content->index;
}

const double* Storage::getValues() const {
  if (content->values.getType().getKind() == Type::Undefined) {
    return nullptr;
  }
  taco_iassert(content->values.getType() == type<double>());
  return static_cast<const double*>(content->values.getData());
}

double* Storage::getValues() {
  if (content->values.getType().getKind() == Type::Undefined) {
    return nullptr;
  }
  taco_iassert(content->values.getType() == type<double>());
  return static_cast<double*>(content->values.getData());
}

size_t Storage::getSizeInBytes() {
  size_t indexSizeInBytes = 0;
  const auto& index = getIndex();
  for (size_t i = 0; i < index.numDimensionIndices(); i++) {
    const auto& dimIndex = index.getDimensionIndex(i);
    for (size_t j = 0; j < dimIndex.numIndexArrays(); j++) {
      const auto& indexArray = dimIndex.getIndexArray(j);
      indexSizeInBytes += indexArray.getSize() *
                          indexArray.getType().getNumBytes();
    }
  }
  return indexSizeInBytes + index.getSize() * sizeof(double);
}

std::ostream& operator<<(std::ostream& os, const Storage& storage) {
  auto format = storage.getFormat();
  if (storage.getValues() == nullptr) {
    return os;
  }

  auto index = storage.getIndex();

  // Print index
  os << index << endl;

  // Print values
  auto values = storage.getValues();
  os << (values != nullptr
         ? "  [" + util::join(values, values + index.getSize()) + "]"
         : "none");

  return os;
}

}}
