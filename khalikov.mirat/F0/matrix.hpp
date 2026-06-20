#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <../common/top-it-vector.hpp>
#include <string>
#include <iostream>

namespace khalikov
{
  struct Matrix
  {
    Matrix();
    Matrix(size_t r, size_t c, int b);
    Matrix(const Matrix &other);
    Matrix(Matrix &&other) noexcept;
    ~Matrix() = default;

    Matrix &operator=(const Matrix &rhs);
    Matrix &operator=(Matrix &&rhs) noexcept;

    Vector< long long > &operator[](size_t r);
    const Vector< long long > &operator[](size_t r) const;

    size_t getRows() const noexcept;
    size_t getCols() const noexcept;
    int getBase() const noexcept;

    void setBase(int newBase);
    void swap(Matrix &other) noexcept;


  private:
    size_t rows_;
    size_t cols_;
    int base_;
    Vector< Vector< long long > > mx_;
  };
}

void khalikov::Matrix::swap(Matrix &other) noexcept
{
  std::swap(rows_, other.rows_);
  std::swap(cols_, other.cols_);
  std::swap(base_, other.base_);
  std::swap(mx_, other.mx_);
}

khalikov::Matrix::Matrix():
  rows_(0),
  cols_(0),
  base_(10),
  mx_()
{}

khalikov::Matrix::Matrix(size_t r, size_t c, int b):
  rows_(r),
  cols_(c),
  base_(b),
  mx_(r)
{
  for (size_t i = 0; i < r; ++i) {
    mx_[i] = Vector< long long >(c);
    for (size_t j = 0; j < c; ++j) {
      mx_[i][j] = 0;
    }
  }
}

khalikov::Matrix::Matrix(const Matrix &other):
  rows_(other.rows_),
  cols_(other.cols_),
  base_(other.base_),
  mx_(other.mx_)
{}

khalikov::Matrix::Matrix(Matrix &&other) noexcept:
  rows_(0),
  cols_(0),
  base_(10),
  mx_()
{
  swap(other);
}

khalikov::Matrix &khalikov::Matrix::operator=(const Matrix &rhs)
{
  if (this != std::addressof(rhs)) {
    Matrix temp(rhs);
    swap(temp);
  }
  return *this;
}

khalikov::Matrix &khalikov::Matrix::operator=(Matrix &&rhs) noexcept
{
  if (this != std::addressof(rhs)) {
    rows_ = 0;
    cols_ = 0;
    base_ = 10;
    mx_ = Vector< Vector< long long > >();
    swap(rhs);
  }
  return *this;
}

khalikov::Vector< long long > &khalikov::Matrix::operator[](size_t r)
{
  return mx_[r];
}

const khalikov::Vector< long long > &khalikov::Matrix::operator[](size_t r) const
{
  return mx_[r];
}

size_t khalikov::Matrix::getRows() const noexcept
{
  return rows_;
}

size_t khalikov::Matrix::getCols() const noexcept
{
  return cols_;
}

int khalikov::Matrix::getBase() const noexcept
{
  return base_;
}

void khalikov::Matrix::setBase(int newBase)
{
  base_ = newBase;
}

#endif
