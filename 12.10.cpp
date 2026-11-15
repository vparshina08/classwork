#include <cstddef>
#include <limits>
#include <stdexcept>

class Mtx
{
public:
  Mtx();
  Mtx(const Mtx & other);
  Mtx(Mtx && other) noexcept;
  Mtx(size_t rows, size_t cols, int value = 0);
  ~Mtx();

  Mtx & operator=(const Mtx & other);
  Mtx & operator=(Mtx && other) noexcept;
  Mtx operator+(const Mtx & other) const;
  bool operator==(const Mtx & other) const;

  void swap(Mtx & other) noexcept;

private:
  size_t rows_;
  size_t cols_;
  int * data_;
};

struct Mtxs
{
public:
  Mtxs();
  Mtxs(const Mtxs & other);
  Mtxs(Mtxs && other) noexcept;
  explicit Mtxs(size_t count);
  ~Mtxs();

  Mtxs & operator=(const Mtxs & other);
  Mtxs & operator=(Mtxs && other) noexcept;
  Mtx & operator[](size_t index);
  const Mtx & operator[](size_t index) const;
  Mtxs operator+(const Mtxs & other) const;
  bool operator==(const Mtxs & other) const;

  size_t getSize() const;
  bool isEmpty() const;
  bool isNotEmpty() const;
  void insert(size_t pos, const Mtx & value);
  void insert(size_t pos, const Mtx & value, size_t count);
  void insert(size_t pos, const Mtxs & other);
  void erase(size_t pos);
  void erase(size_t first, size_t last);
  size_t erase(const Mtx & value);
  Mtx conv(Mtx accumulator, Mtx (*operation)(Mtx, Mtx)) const;
  void swap(Mtxs & other) noexcept;

private:
  size_t k_;
  Mtx * mtxs_;
};

Mtx::Mtx():
  rows_(0),
  cols_(0),
  data_(nullptr)
{}

Mtx::Mtx(const Mtx & other):
  Mtx(other.rows_, other.cols_)
{
  for (size_t i = 0; i < rows_ * cols_; ++i)
  {
    data_[i] = other.data_[i];
  }
}

Mtx::Mtx(Mtx && other) noexcept:
  rows_(other.rows_),
  cols_(other.cols_),
  data_(other.data_)
{
  other.rows_ = 0;
  other.cols_ = 0;
  other.data_ = nullptr;
}

Mtx::Mtx(size_t rows, size_t cols, int value):
  rows_(rows),
  cols_(cols),
  data_(nullptr)
{
  if (cols != 0 && rows > std::numeric_limits<size_t>::max() / cols)
  {
    throw std::length_error("Matrix is too large");
  }
  const size_t count = rows * cols;
  if (count != 0)
  {
    data_ = new int[count];
  }
  for (size_t i = 0; i < count; ++i)
  {
    data_[i] = value;
  }
}

Mtx::~Mtx()
{
  delete[] data_;
}

Mtx & Mtx::operator=(const Mtx & other)
{
  if (this != &other)
  {
    Mtx copy(other);
    swap(copy);
  }
  return *this;
}

Mtx & Mtx::operator=(Mtx && other) noexcept
{
  if (this != &other)
  {
    delete[] data_;
    rows_ = other.rows_;
    cols_ = other.cols_;
    data_ = other.data_;
    other.rows_ = 0;
    other.cols_ = 0;
    other.data_ = nullptr;
  }
  return *this;
}

Mtx Mtx::operator+(const Mtx & other) const
{
  if (rows_ != other.rows_ || cols_ != other.cols_)
  {
    throw std::invalid_argument("Different matrix sizes");
  }
  Mtx result(rows_, cols_);
  for (size_t i = 0; i < rows_ * cols_; ++i)
  {
    const long long sum = static_cast<long long>(data_[i]) + other.data_[i];
    if (sum > std::numeric_limits<int>::max() || sum < std::numeric_limits<int>::min())
    {
      throw std::overflow_error("Matrix sum overflow");
    }
    result.data_[i] = static_cast<int>(sum);
  }
  return result;
}

bool Mtx::operator==(const Mtx & other) const
{
  if (rows_ != other.rows_ || cols_ != other.cols_)
  {
    return false;
  }
  for (size_t i = 0; i < rows_ * cols_; ++i)
  {
    if (data_[i] != other.data_[i])
    {
      return false;
    }
  }
  return true;
}

void Mtx::swap(Mtx & other) noexcept
{
  const size_t old_rows = rows_;
  const size_t old_cols = cols_;
  int * old_data = data_;

  rows_ = other.rows_;
  cols_ = other.cols_;
  data_ = other.data_;

  other.rows_ = old_rows;
  other.cols_ = old_cols;
  other.data_ = old_data;
}

Mtxs::Mtxs():
  k_(0),
  mtxs_(nullptr)
{}

Mtxs::Mtxs(const Mtxs & other):
  Mtxs()
{
  Mtxs copy(other.k_);
  for (size_t i = 0; i < other.k_; ++i)
  {
    copy.mtxs_[i] = other.mtxs_[i];
  }
  swap(copy);
}

Mtxs::Mtxs(Mtxs && other) noexcept:
  k_(other.k_),
  mtxs_(other.mtxs_)
{
  other.k_ = 0;
  other.mtxs_ = nullptr;
}

Mtxs::Mtxs(size_t count):
  k_(count),
  mtxs_(nullptr)
{
  if (count != 0)
  {
    mtxs_ = new Mtx[count];
  }
}

Mtxs::~Mtxs()
{
  delete[] mtxs_;
}

Mtxs & Mtxs::operator=(const Mtxs & other)
{
  if (this != &other)
  {
    Mtxs copy(other);
    swap(copy);
  }
  return *this;
}

Mtxs & Mtxs::operator=(Mtxs && other) noexcept
{
  if (this != &other)
  {
    delete[] mtxs_;
    k_ = other.k_;
    mtxs_ = other.mtxs_;
    other.k_ = 0;
    other.mtxs_ = nullptr;
  }
  return *this;
}

Mtx & Mtxs::operator[](size_t index)
{
  if (index >= k_)
  {
    throw std::out_of_range("Wrong array index");
  }
  return mtxs_[index];
}

const Mtx & Mtxs::operator[](size_t index) const
{
  if (index >= k_)
  {
    throw std::out_of_range("Wrong array index");
  }
  return mtxs_[index];
}

Mtxs Mtxs::operator+(const Mtxs & other) const
{
  Mtxs result(*this);
  result.insert(result.k_, other);
  return result;
}

bool Mtxs::operator==(const Mtxs & other) const
{
  if (k_ != other.k_)
  {
    return false;
  }
  for (size_t i = 0; i < k_; ++i)
  {
    if (!(mtxs_[i] == other.mtxs_[i]))
    {
      return false;
    }
  }
  return true;
}

size_t Mtxs::getSize() const
{
  return k_;
}

bool Mtxs::isEmpty() const
{
  return k_ == 0;
}

bool Mtxs::isNotEmpty() const
{
  return k_ != 0;
}

// 1: Insert one matrix.
void Mtxs::insert(size_t pos, const Mtx & value)
{
  insert(pos, value, 1);
}

// 2: Insert the same matrix several times.
void Mtxs::insert(size_t pos, const Mtx & value, size_t count)
{
  if (pos > k_)
  {
    throw std::out_of_range("Wrong insert position");
  }
  Mtxs block(count);
  for (size_t i = 0; i < count; ++i)
  {
    block.mtxs_[i] = value;
  }
  insert(pos, block);
}

// 3: Insert all matrices from another Mtxs object.
void Mtxs::insert(size_t pos, const Mtxs & other)
{
  if (pos > k_)
  {
    throw std::out_of_range("Wrong insert position");
  }
  if (other.k_ > std::numeric_limits<size_t>::max() - k_)
  {
    throw std::length_error("Array is too large");
  }
  if (other.k_ == 0)
  {
    return;
  }
  Mtxs result(k_ + other.k_);
  for (size_t i = 0; i < pos; ++i)
  {
    result.mtxs_[i] = mtxs_[i];
  }
  for (size_t i = 0; i < other.k_; ++i)
  {
    result.mtxs_[pos + i] = other.mtxs_[i];
  }
  for (size_t i = pos; i < k_; ++i)
  {
    result.mtxs_[other.k_ + i] = mtxs_[i];
  }
  swap(result);
}

// 1: Erase one matrix by its index.
void Mtxs::erase(size_t pos)
{
  if (pos >= k_)
  {
    throw std::out_of_range("Wrong erase position");
  }
  erase(pos, pos + 1);
}

// 2: Erase the range [first, last).
void Mtxs::erase(size_t first, size_t last)
{
  if (first > last || last > k_)
  {
    throw std::out_of_range("Wrong erase range");
  }
  if (first == last)
  {
    return;
  }
  Mtxs result(k_ - (last - first));
  for (size_t i = 0; i < first; ++i)
  {
    result.mtxs_[i] = mtxs_[i];
  }
  for (size_t i = last; i < k_; ++i)
  {
    result.mtxs_[i - (last - first)] = mtxs_[i];
  }
  swap(result);
}

// 3: Erase all matrices equal to value.
size_t Mtxs::erase(const Mtx & value)
{
  size_t removed = 0;
  for (size_t i = 0; i < k_; ++i)
  {
    if (mtxs_[i] == value)
    {
      ++removed;
    }
  }
  if (removed == 0)
  {
    return 0;
  }
  Mtxs result(k_ - removed);
  size_t j = 0;
  for (size_t i = 0; i < k_; ++i)
  {
    if (!(mtxs_[i] == value))
    {
      result.mtxs_[j] = mtxs_[i];
      ++j;
    }
  }
  swap(result);
  return removed;
}

Mtx Mtxs::conv(Mtx accumulator, Mtx (*operation)(Mtx, Mtx)) const
{
  if (operation == nullptr)
  {
    throw std::invalid_argument("Null operation");
  }
  for (size_t i = 0; i < k_; ++i)
  {
    accumulator = operation(accumulator, mtxs_[i]);
  }
  return accumulator;
}

void Mtxs::swap(Mtxs & other) noexcept
{
  const size_t old_size = k_;
  Mtx * old_data = mtxs_;

  k_ = other.k_;
  mtxs_ = other.mtxs_;

  other.k_ = old_size;
  other.mtxs_ = old_data;
}

int main()
{
  return 0;
}
