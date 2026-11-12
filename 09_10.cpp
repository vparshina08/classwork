#include <cstddef>
#include <exception>
#include <limits>
#include <stdexcept>
#include <utility>

struct p_t {
  int x;
  int y;
};

class Ints {
public:
  Ints();
  Ints(size_t size, int value);
  Ints(const int * arr, size_t arr_size);
  Ints(const Ints & rhs);
  Ints(size_t size);
  Ints(Ints && rhs) noexcept;

  Ints & operator=(const Ints & rhs);
  Ints & operator=(Ints && rhs) noexcept;
  Ints operator+(const Ints & rhs) const;

  ~Ints();

  size_t getSize() const noexcept;
  void set(size_t id, int i);
  int get(size_t id) const noexcept;
  int getSafe(size_t id) const;
  void setSafe(size_t id, int i);
  Ints append(int i) const;
  Ints append(const Ints & b) const;

private:
  size_t size;
  size_t capacity;
  int * data;
};

Ints::Ints():
  size(0), capacity(0), data(nullptr)
{}

Ints::Ints(size_t size, int value):
  size(size), capacity(size), data(new int[size])
{
  for (size_t i = 0; i < size; i++) {
    data[i] = value;
  }
}

Ints::Ints(const int * arr, size_t arr_size):
  size(arr_size), capacity(arr_size), data(new int[arr_size])
{
  if (arr == nullptr && arr_size != 0) {
    delete [] data;
    throw std::invalid_argument("Null array");
  }
  for (size_t i = 0; i < arr_size; i++) {
    data[i] = arr[i];
  }
}

Ints::Ints(size_t size):
  size(size), capacity(size), data(new int[size]{})
{}

Ints::Ints(const Ints & rhs):
  size(rhs.size), capacity(rhs.capacity), data(new int[rhs.capacity])
{
  for (size_t i = 0; i < size; i++) {
    data[i] = rhs.data[i];
  }
}

Ints::Ints(Ints && rhs) noexcept:
  size(rhs.size), capacity(rhs.capacity), data(rhs.data)
{
  rhs.size = 0;
  rhs.capacity = 0;
  rhs.data = nullptr;
}

Ints & Ints::operator=(const Ints & rhs) {
  if (this == &rhs) {
    return *this;
  }
  Ints temp(rhs);
  std::swap(size, temp.size);
  std::swap(capacity, temp.capacity);
  std::swap(data, temp.data);
  return *this;
}

Ints & Ints::operator=(Ints && rhs) noexcept {
  if (this != &rhs) {
    delete [] data;
    size = rhs.size;
    capacity = rhs.capacity;
    data = rhs.data;
    rhs.size = 0;
    rhs.capacity = 0;
    rhs.data = nullptr;
  }
  return *this;
}

Ints Ints::operator+(const Ints & rhs) const {
  if (size != rhs.size) {
    throw std::invalid_argument("Arrays have different sizes");
  }
  Ints result(size);
  for (size_t i = 0; i < size; i++) {
    int a = data[i];
    int b = rhs.data[i];
    if ((b > 0 && a > std::numeric_limits<int>::max() - b) ||
        (b < 0 && a < std::numeric_limits<int>::min() - b)) {
      throw std::overflow_error("Overflow");
    }
    result.data[i] = a + b;
  }
  return result;
}

Ints::~Ints() {
  delete [] data;
}

size_t Ints::getSize() const noexcept {
  return size;
}

void Ints::set(size_t id, int i) {
  data[id] = i;
}

int Ints::get(size_t id) const noexcept {
  return data[id];
}

void Ints::setSafe(size_t id, int i) {
  if (id >= size) {
    throw std::out_of_range("Out of range");
  }
  set(id, i);
}

int Ints::getSafe(size_t id) const {
  if (id >= size) {
    throw std::out_of_range("Out of range");
  }
  return get(id);
}

Ints Ints::append(int i) const {
  if (size == std::numeric_limits<size_t>::max()) {
    throw std::overflow_error("Overflow");
  }
  Ints temp(size + 1);
  for (size_t p = 0; p < size; p++) {
    temp.data[p] = data[p];
  }
  temp.data[size] = i;
  return temp;
}

Ints Ints::append(const Ints & b) const {
  if (b.size > std::numeric_limits<size_t>::max() - size) {
    throw std::overflow_error("Overflow");
  }
  Ints temp(size + b.size);
  for (size_t i = 0; i < size; i++) {
    temp.data[i] = data[i];
  }
  for (size_t i = 0; i < b.size; i++) {
    temp.data[size + i] = b.data[i];
  }
  return temp;
}

// m - number of rows, n - number of columns.
size_t matrixSize(size_t m, size_t n) {
  if (m != 0 && n > std::numeric_limits<size_t>::max() / m) {
    throw std::overflow_error("Matrix size overflow");
  }
  return m * n;
}

class Mtx {
  size_t m;
  size_t n;
  Ints data_mtx;

public:
  Mtx();
  Mtx(size_t m, size_t n);
  Mtx(size_t m, size_t n, const Ints & data);
  ~Mtx() = default;

  Mtx sum(const Mtx & rhs) const;
  Mtx mul(const Mtx & rhs) const;
  Ints getCol(size_t id) const;
  Ints getRow(size_t id) const;
  Mtx add_row(int init) const;
  Mtx addRow(int init) const;
  Mtx transpose() const;
  int det() const;
};

Mtx::Mtx():
  m(0), n(0)
{}

Mtx::Mtx(size_t m, size_t n):
  m(m), n(n), data_mtx(matrixSize(m, n), 0)
{}

Mtx::Mtx(size_t m, size_t n, const Ints & data):
  m(m), n(n), data_mtx(data)
{
  if (data.getSize() != matrixSize(m, n)) {
    throw std::invalid_argument("Wrong matrix data size");
  }
}

Mtx Mtx::sum(const Mtx & rhs) const {
  if (m != rhs.m || n != rhs.n) {
    throw std::invalid_argument("Different matrix sizes");
  }
  return Mtx(m, n, data_mtx + rhs.data_mtx);
}

Ints Mtx::getCol(size_t id) const {
  if (id >= n) {
    throw std::out_of_range("Column out of range");
  }
  Ints res;
  for (size_t i = 0; i < m; i++) {
    res = res.append(data_mtx.get(i * n + id));
  }
  return res;
}

Ints Mtx::getRow(size_t id) const {
  if (id >= m) {
    throw std::out_of_range("Row out of range");
  }
  Ints res;
  for (size_t i = 0; i < n; i++) {
    res = res.append(data_mtx.get(id * n + i));
  }
  return res;
}

Mtx Mtx::mul(const Mtx & rhs) const {
  if (n != rhs.m) {
    throw std::invalid_argument("Incompatible matrix sizes");
  }
  Mtx res(m, rhs.n);
  for (size_t i = 0; i < m; i++) {
    Ints row = getRow(i);
    for (size_t j = 0; j < rhs.n; j++) {
      Ints col = rhs.getCol(j);
      long long sm = 0;
      for (size_t k = 0; k < n; k++) {
        long long product = 1LL * row.get(k) * col.get(k);
        if ((product > 0 && sm > std::numeric_limits<long long>::max() - product) ||
            (product < 0 && sm < std::numeric_limits<long long>::min() - product)) {
          throw std::overflow_error("Overflow");
        }
        sm += product;
      }
      if (sm > std::numeric_limits<int>::max() ||
          sm < std::numeric_limits<int>::min()) {
        throw std::overflow_error("Overflow");
      }
      res.data_mtx.set(i * rhs.n + j, static_cast<int>(sm));
    }
  }
  return res;
}

Mtx Mtx::add_row(int init) const {
  if (m == std::numeric_limits<size_t>::max()) {
    throw std::overflow_error("Overflow");
  }
  Mtx res(m + 1, n);
  for (size_t i = 0; i < data_mtx.getSize(); i++) {
    res.data_mtx.set(i, data_mtx.get(i));
  }
  for (size_t i = 0; i < n; i++) {
    res.data_mtx.set(m * n + i, init);
  }
  return res;
}

Mtx Mtx::addRow(int init) const {
  return add_row(init);
}

Mtx Mtx::transpose() const {
  Mtx res(n, m);
  for (size_t i = 0; i < m; i++) {
    for (size_t j = 0; j < n; j++) {
      res.data_mtx.set(j * m + i, data_mtx.get(i * n + j));
    }
  }
  return res;
}

int Mtx::det() const {
  if (m != n || m == 0) {
    throw std::logic_error("Invalid matrix for determinant");
  }
  if (m == 1) {
    return data_mtx.get(0);
  }
  if (m == 2) {
    long long res = 1LL * data_mtx.get(0) * data_mtx.get(3) -
                    1LL * data_mtx.get(1) * data_mtx.get(2);
    if (res > std::numeric_limits<int>::max() ||
        res < std::numeric_limits<int>::min()) {
      throw std::overflow_error("Overflow");
    }
    return static_cast<int>(res);
  }

  long long res = 0;
  for (size_t i = 0; i < n; i++) {
    Mtx minor(m - 1, n - 1);
    size_t p = 0;
    for (size_t row = 1; row < m; row++) {
      for (size_t col = 0; col < n; col++) {
        if (col != i) {
          minor.data_mtx.set(p++, data_mtx.get(row * n + col));
        }
      }
    }
    long long term = 1LL * data_mtx.get(i) * minor.det();
    if (i % 2 != 0) {
      term = -term;
    }
    if ((term > 0 && res > std::numeric_limits<long long>::max() - term) ||
        (term < 0 && res < std::numeric_limits<long long>::min() - term)) {
      throw std::overflow_error("Overflow");
    }
    res += term;
  }

  if (res > std::numeric_limits<int>::max() ||
      res < std::numeric_limits<int>::min()) {
    throw std::overflow_error("Overflow");
  }
  return static_cast<int>(res);
}

// This class was in your original code, although it is not shown in the diagram.
class Mtxs {
public:
  Mtxs();
  Mtxs(size_t size, const Mtx & mtx);
  Mtxs(const Mtxs & rhs);
  Mtxs(Mtxs && rhs) noexcept;
  ~Mtxs();
  Mtxs & operator=(const Mtxs & rhs);
  Mtxs & operator=(Mtxs && rhs) noexcept;

private:
  size_t size;
  Mtx * arr_mtx;
};

Mtxs::Mtxs():
  size(0), arr_mtx(nullptr)
{}

Mtxs::Mtxs(size_t size, const Mtx & mtx):
  size(size), arr_mtx(new Mtx[size])
{
  try {
    for (size_t i = 0; i < size; i++) {
      arr_mtx[i] = mtx;
    }
  } catch (const std::exception &) {
    delete [] arr_mtx;
    throw;
  }
}

Mtxs::Mtxs(const Mtxs & rhs):
  size(rhs.size), arr_mtx(new Mtx[rhs.size])
{
  try {
    for (size_t i = 0; i < size; i++) {
      arr_mtx[i] = rhs.arr_mtx[i];
    }
  } catch (const std::exception &) {
    delete [] arr_mtx;
    throw;
  }
}

Mtxs::Mtxs(Mtxs && rhs) noexcept:
  size(rhs.size), arr_mtx(rhs.arr_mtx)
{
  rhs.size = 0;
  rhs.arr_mtx = nullptr;
}

Mtxs & Mtxs::operator=(const Mtxs & rhs) {
  if (this != &rhs) {
    Mtxs temp(rhs);
    std::swap(size, temp.size);
    std::swap(arr_mtx, temp.arr_mtx);
  }
  return *this;
}

Mtxs & Mtxs::operator=(Mtxs && rhs) noexcept {
  if (this != &rhs) {
    delete [] arr_mtx;
    size = rhs.size;
    arr_mtx = rhs.arr_mtx;
    rhs.size = 0;
    rhs.arr_mtx = nullptr;
  }
  return *this;
}

Mtxs::~Mtxs() {
  delete [] arr_mtx;
}

class Points {
  Ints data_points;

public:
  Points();
  Points(const Ints & data);
  void set(size_t id, p_t p);
  p_t get(size_t id) const;
  Points append(p_t p) const;
};

Points::Points():
  data_points()
{}

Points::Points(const Ints & data):
  data_points(data)
{
  if (data.getSize() % 2 != 0) {
    throw std::invalid_argument("Invalid points data");
  }
}

void Points::set(size_t id, p_t p) {
  if (id >= data_points.getSize() / 2) {
    throw std::out_of_range("Point out of range");
  }
  data_points.set(id * 2, p.x);
  data_points.set(id * 2 + 1, p.y);
}

p_t Points::get(size_t id) const {
  if (id >= data_points.getSize() / 2) {
    throw std::out_of_range("Point out of range");
  }
  p_t res;
  res.x = data_points.get(id * 2);
  res.y = data_points.get(id * 2 + 1);
  return res;
}

Points Points::append(p_t p) const {
  Ints tmp = data_points.append(p.x).append(p.y);
  return Points(tmp);
}

class Plot {
public:
  void set(int x, int y);
  int calc(int x) const;
  int solve(int y) const;
  int solve(Ints & buf, int y) const;
  Points append(p_t p) const;

private:
  Ints data_plot;
};

void Plot::set(int x, int y) {
  for (size_t i = 0; i < data_plot.getSize(); i += 2) {
    if (data_plot.get(i) == x) {
      data_plot.set(i + 1, y);
      return;
    }
  }
  data_plot = data_plot.append(x).append(y);
}

int Plot::calc(int x) const {
  for (size_t i = 0; i < data_plot.getSize(); i += 2) {
    if (data_plot.get(i) == x) {
      return data_plot.get(i + 1);
    }
  }
  throw std::logic_error("Point not found");
}

int Plot::solve(int y) const {
  for (size_t i = 1; i < data_plot.getSize(); i += 2) {
    if (data_plot.get(i) == y) {
      return data_plot.get(i - 1);
    }
  }
  throw std::logic_error("Solution not found");
}

int Plot::solve(Ints & buf, int y) const {
  int cnt = 0;
  for (size_t i = 1; i < data_plot.getSize(); i += 2) {
    if (data_plot.get(i) == y) {
      if (cnt == std::numeric_limits<int>::max()) {
        throw std::overflow_error("Overflow");
      }
      buf = buf.append(data_plot.get(i - 1));
      cnt++;
    }
  }
  return cnt;
}

Points Plot::append(p_t p) const {
  Ints tmp = data_plot.append(p.x).append(p.y);
  return Points(tmp);
}

int main() {
  int arr[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  Ints a(arr, 9);
  Mtx mtx1(3, 3, a);
  return 0;
}
