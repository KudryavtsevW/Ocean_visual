#ifndef FISHMATRIX_H_INCLUDED
#define FISHMATRIX_H_INCLUDED

#include <vector>
#include <memory>
#include "Fish.h"

// Объявляем типы, чтобы они были видны везде
typedef std::vector<std::unique_ptr<Fish>> Row;
typedef std::vector<Row> FishMatrix;

#endif // FISHMATRIX_H_INCLUDED
