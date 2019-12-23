#ifndef TORUS_H
#define TORUS_H

#include <array>
#include <optional>

template<typename T>
class Torus {
  T *buffer;
  int start_i, start_j, end_i, end_j, cur_i, cur_j;
  int size_i, size_j;
 public:
  const int rows, cols;
  Torus(int _rows, int _cols) : rows(_rows), cols(_cols), start_(0), start_j(0), end_i(0), end_j(0), cur_i(0), cur_j(0), buffer(new T[rows * cols]) {}
  ~Torus() {
    delete[] buffer;
  }
  std::array<float, 4> range() const {
    int _si = start_i;
    if(_si == cur_i) {
      if(size_i != 0) {
        _si -= rows;
      }
    } else if(_si > cur_i) {
      _si -= rows;
    }
    int _ei = end_i;
    if(_ei == cur_i) {
      if(size_i != 0) {
        _ei += rows;
      }
    } else if(_ei < cur_i) {
      _ei += rows;
    }
    int _sj = start_j;
    if(_sj == cur_j) {
      if(size_j != 0) {
        _sj -= cols;
      }
    } else if(_sj > cur_j) {
      _sj -= cols;
    }
    int _ej = end_j;
    if(_ej == cur_j) {
      if(size_j != 0) {
        _ej += cols;
      }
    } else if(_ej < cur_j) {
      _ej += cols;
    }
    return std::array<float, 4>({_si - cur_i, _ei - cur_i, _sj - cur_j, _ej - cur_j});
  }
  std::optional<T&> get(i, j) {
    if(size_i * size_j == 0) {
      return std::null_opt;
    }
    auto r = range();
    if(i >= r[1] || i < r[0]) {
      return std::null_opt;
    }
    if(j >= r[3] || i < r[2]) {
      return std::null_opt;
    }
    int _i = cur_i + i;
    if(_i < 0) _i += rows;
    if(_i >= rows) _i -= rows;
    int _j = cur_j + j;
    if(_j < 0) _j += rows;
    if(_j >= rows) _j -= rows;
    return buffer[_i * cols + _j];
  }
  std::optional<T&> set(i, j, T v) {
    if(size_i * size_j == 0) {
      return std::null_opt;
    }
    auto r = range();
    if(i >= r[1] || i < r[0]) {
      return std::null_opt;
    }
    if(j >= r[3] || i < r[2]) {
      return std::null_opt;
    }
    int _i = cur_i + i;
    if(_i < 0) _i += rows;
    if(_i >= rows) _i -= rows;
    int _j = cur_j + j;
    if(_j < 0) _j += rows;
    if(_j >= rows) _j -= rows;
    return buffer[_i * cols + _j] = v;
  }
  bool addRow() {
    if(size_i == rows) {
      return false;
    }
    end_i++;
    if(end_i >= rows) {
      end_i -= rows;
    }
    return true;
  }
  bool addCol() {
    if(size_j == cols) {
      return false;
    }
    end_j++;
    if(end_j >= cols) {
      end_j -= cols;
    }
    return true;
  }
  bool removeRow() {
    if(size_i > 0) {
      return false;
    }
    start_i++;
    if(start_i >= rows) {
      start_i -= rows;
    }
    return true;
  }
  bool removeCol() {
    if(size_j > 0) {
      return false;
    }
    start_j++;
    if(start_j >= cols) {
      start_j -= cols;
    }
    return true;
  }
  bool shiftCur(int s, int t) {
    if(size_i * size_j == 0) {
      return false;
    }
    auto r = range();
    if(i >= r[1] || i < r[0]) {
      return false;
    }
    if(j >= r[3] || i < r[2]) {
      return false;
    }
    cur_i += s;
    if(cur_i < 0) cur_i += rows;
    if(cur_i >= rows) cur_i -= rows;
    cur_j += t;
    if(cur_j < 0) cur_j += cols;
    if(cur_j >= cols) cur_j -= cols;
  }
}

#endif // TORUS_H