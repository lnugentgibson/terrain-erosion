template<typename V, typename T, typename F>
void VectorForEach(int d, const V& v, F f) const {
  T *A = new T[d];
  std::copy(v.begin(), v.end(), A);
  for (int i = 0; i < d; i++) {
    f(v[i], i, A);
  }
};

template<typename V, typename T, typename F>
V VectorMap(int d, const V& v, F f) const {
  T *A = new T[d];
  std::copy(v.begin(), v.end(), A);
  T *a = new T[d];
  for (int i = 0; i < d; i++) {
    a[i] = f(v[i], i, A);
  }
  return V(d, a);
};

template<typename V, typename T, typename F>
void VectorTransform(int d, V& v, F f) {
  T *A = new T[d];
  std::copy(v.begin(), v.end(), A);
  for (int i = 0; i < d; i++) {
    v[i] = f(v[i], i, A);
  }
};

template<typename V, typename T, typename F, typename S>
S VectorReduce(int d, const V& v, F f, S s) const {
  T *A = new T[d];
  std::copy(v.begin(), v.end(), A);
  S a = s;
  for (int i = 0; i < d; i++) {
    a = f(a, v[i], i, A);
  }
  return a;
};

template<typename V, typename T, typename F>
S VectorEvery(int d, const V& v, F f) const {
  T *A = new T[d];
  std::copy(v.begin(), v.end(), A);
  for (int i = 0; i < d; i++) {
    if(!f(v[i], i, A)) return false;
  }
  return true;
};

template<typename V, typename T, typename F>
S VectorSome(int d, const V& v, F f) const {
  T *A = new T[d];
  std::copy(v.begin(), v.end(), A);
  for (int i = 0; i < d; i++) {
    if(f(v[i], i, A)) return true;
  }
  return false;
};

template<typename V, typename T>
void VectorFill(int d, V& v, T& t) const {
  for (int i = 0; i < d; i++) {
    v[i] = t;
  }
};

template<typename V, typename T>
void negate(int d, V& v) {
  return VectorTransform(d, v, [](T e, int i, T *A) { return -e; });
}
template<typename V, typename T>
Vector VectorNegative(int d, const V& v) const {
  return VectorMap(d, v, [](T e, int i, T *A) { return -e; });
}

template<typename V, typename T>
void VectorAdd(int d, V& v, const T v) {
  VectorTransform(d, v, [v](T e, int i, T *A) { return e + v; });
}
template<typename V, typename T>
void VectorAdd(int d, V& v, const T *v) {
  VectorTransform(d, v, [v](T e, int i, T *A) { return e + v[i]; });
}
template<typename V, typename T>
void VectorAdd(int d, V& v, const V& v) {
  VectorTransform(d, v, [v](T e, int i, T *A) { return e + v[i]; });
}
template<typename V, typename T>
V VectorSum(int d, V& v, const T v) {
  return VectorMap(d, v, [v](T e, int i, T *A) { return e + v; });
}
template<typename V, typename T>
V VectorSum(int d, V& v, const T *v) {
  return VectorMap(d, v, [v](T e, int i, T *A) { return e + v[i]; });
}
template<typename V, typename T>
V VectorSum(int d, V& v, const V& v) {
  return VectorMap(d, v, [v](T e, int i, T *A) { return e + v[i]; });
}

template<typename Parent, typename T>
class VectorBase {
 private:
  VectorBase() {}
  friend Parent;
 public:
  template<typename F>
  void forEach(F f) const {
    Parent& p = static_cast<Parent&>(*this);
    VectorForEach(p.d, p, f);
  }
  template<typename F>
  Parent map(F f) const {
    Parent& p = static_cast<Parent&>(*this);
    return VectorMap(p.d, p, f);
  }
  template<typename F>
  Parent& transform(F f) {
    Parent& p = static_cast<Parent&>(*this);
    VectorTransform(p.d, p, f);
    return p;
  }
  template<typename F, typename S>
  S reduce(F f, S s) const {
    Parent& p = static_cast<Parent&>(*this);
    return VectorReduce(p.d, p, f, s);
  }
  template<typename F, typename S>
  bool every(F f, S s) const {
    Parent& p = static_cast<Parent&>(*this);
    return VectorEvery(p.d, p, f);
  }
  template<typename F, typename S>
  bool some(F f, S s) const {
    Parent& p = static_cast<Parent&>(*this);
    return VectorSome(p.d, p, f);
  }
  template<typename F>
  void fill(F f, T& t) const {
    Parent& p = static_cast<Parent&>(*this);
    VectorFill(p.d, p, t);
  }
}

template<typename T>
Vector& operator +=(T *c, const T v) {
  return transform(c, [v](T e, int i, T *A) { return e + v; });
}

template<typename T>
Vector& operator +=(T *c, const T *v) {
  return transform(c, [v](T e, int i, T *A) { return e + v[i]; });
}

template<typename T>
Vector& operator +=(T *c, const Vector& v) {
  // if (v.d != d) return util::Status(util::INVALID_ARGUMENT, "dimensions must be equal");
  return transform(c, [v](T e, int i, T *A) { return e + v[i]; });
}

template<typename T>
Vector sum(T *c, const T v) const {
  return map(c, [v](T e, int i, T *A) { return e + v; });
}

template<typename T>
Vector sum(T *c, const T *v) const {
  return map(c, [v](T e, int i, T *A) { return e + v[i]; });
}

template<typename T>
StatusOr<Vector> sum(T *c, const Vector& v) const {
  // if (v.d != d) return;
  return map(c, [v](T e, int i, T *A) { return e + v[i]; });
}

template<typename T>
Vector operator +(T *c, const T v) {
  return map(c, [v](T e, int i, T *A) { return e + v; });
}

template<typename T>
Vector operator +(T *c, const T *v) {
  return map(c, [v](T e, int i, T *A) { return e + v[i]; });
}

template<typename T>
Vector operator +(T *c, const Vector& v) {
  // if (v.d != d) return util::Status(util::INVALID_ARGUMENT, "dimensions must be equal");
  return map(c, [v](T e, int i, T *A) { return e + v[i]; });
}

template<typename T>
Vector& subtract(T *c, const T v) {
  return transform(c, [v](T e, int i, T *A) { return e - v; });
}

template<typename T>
Vector& subtract(T *c, const T *v) {
  return transform(c, [v](T e, int i, T *A) { return e - v[i]; });
}

template<typename T>
StatusOr<Vector> subtract(T *c, const Vector& v) {
  // if (v.d != d) return;
  return transform(c, [v](T e, int i, T *A) { return e - v[i]; });
}

template<typename T>
Vector& operator -=(T *c, const T v) {
  return transform(c, [v](T e, int i, T *A) { return e - v; });
}

template<typename T>
Vector& operator -=(T *c, const T *v) {
  return transform(c, [v](T e, int i, T *A) { return e - v[i]; });
}

template<typename T>
Vector& operator -=(T *c, const Vector& v) {
  // if (v.d != d) return util::Status(util::INVALID_ARGUMENT, "dimensions must be equal");
  return transform(c, [v](T e, int i, T *A) { return e - v[i]; });
}

template<typename T>
Vector difference(T *c, const T v) const {
  return map(c, [v](T e, int i, T *A) { return e - v; });
}

template<typename T>
Vector difference(T *c, const T *v) const {
  return map(c, [v](T e, int i, T *A) { return e - v[i]; });
}

template<typename T>
StatusOr<Vector> difference(T *c, const Vector& v) const {
  // if (v.d != d) return;
  return map(c, [v](T e, int i, T *A) { return e - v[i]; });
}

template<typename T>
Vector operator -(T *c, const T v) {
  return map(c, [v](T e, int i, T *A) { return e - v; });
}

template<typename T>
Vector operator -(T *c, const T *v) {
  return map(c, [v](T e, int i, T *A) { return e - v[i]; });
}

template<typename T>
Vector operator -(T *c, const Vector& v) {
  // if (v.d != d) return util::Status(util::INVALID_ARGUMENT, "dimensions must be equal");
  return map(c, [v](T e, int i, T *A) { return e - v[i]; });
}

template<typename T>
Vector& multiply(T *c, const T v) {
  return transform(c, [v](T e, int i, T *A) { return e * v; });
}

template<typename T>
Vector& multiply(T *c, const T *v) {
  return transform(c, [v](T e, int i, T *A) { return e * v[i]; });
}

template<typename T>
StatusOr<Vector> multiply(T *c, const Vector& v) {
  // if (v.d != d) return;
  return transform(c, [v](T e, int i, T *A) { return e * v[i]; });
}

template<typename T>
Vector& operator *=(T *c, const T v) {
  return transform(c, [v](T e, int i, T *A) { return e * v; });
}

template<typename T>
Vector& operator *=(T *c, const T *v) {
  return transform(c, [v](T e, int i, T *A) { return e * v[i]; });
}

template<typename T>
Vector& operator *=(T *c, const Vector& v) {
  // if (v.d != d) return util::Status(util::INVALID_ARGUMENT, "dimensions must be equal");
  return transform(c, [v](T e, int i, T *A) { return e * v[i]; });
}

template<typename T>
Vector product(T *c, const T v) const {
  return map(c, [v](T e, int i, T *A) { return e * v; });
}

template<typename T>
Vector product(T *c, const T *v) const {
  return map(c, [v](T e, int i, T *A) { return e * v[i]; });
}

template<typename T>
StatusOr<Vector> product(T *c, const Vector& v) const {
  // if (v.d != d) return;
  return map(c, [v](T e, int i, T *A) { return e * v[i]; });
}

template<typename T>
Vector operator *(T *c, const T v) {
  return map(c, [v](T e, int i, T *A) { return e * v; });
}

template<typename T>
Vector operator *(T *c, const T *v) {
  return map(c, [v](T e, int i, T *A) { return e * v[i]; });
}

template<typename T>
Vector operator *(T *c, const Vector& v) {
  // if (v.d != d) return util::Status(util::INVALID_ARGUMENT, "dimensions must be equal");
  return map(c, [v](T e, int i, T *A) { return e * v[i]; });
}

template<typename T>
Vector& divide(T *c, const T v) {
  return transform(c, [v](T e, int i, T *A) { return e / v; });
}

template<typename T>
Vector& divide(T *c, const T *v) {
  return transform(c, [v](T e, int i, T *A) { return e / v[i]; });
}

template<typename T>
StatusOr<Vector> divide(T *c, const Vector& v) {
  // if (v.d != d) return;
  return transform(c, [v](T e, int i, T *A) { return e / v[i]; });
}

template<typename T>
Vector& operator /=(T *c, const T v) {
  return transform(c, [v](T e, int i, T *A) { return e / v; });
}

template<typename T>
Vector& operator /=(T *c, const T *v) {
  return transform(c, [v](T e, int i, T *A) { return e / v[i]; });
}

template<typename T>
Vector& operator /=(T *c, const Vector& v) {
  // if (v.d != d) return util::Status(util::INVALID_ARGUMENT, "dimensions must be equal");
  return transform(c, [v](T e, int i, T *A) { return e / v[i]; });
}

template<typename T>
Vector quotient(T *c, const T v) const {
  return map(c, [v](T e, int i, T *A) { return e / v; });
}

template<typename T>
Vector quotient(T *c, const T *v) const {
  return map(c, [v](T e, int i, T *A) { return e / v[i]; });
}

template<typename T>
StatusOr<Vector> quotient(T *c, const Vector& v) const {
  // if (v.d != d) return;
  return map(c, [v](T e, int i, T *A) { return e / v[i]; });
}

template<typename T>
Vector operator /(T *c, const T v) {
  return map(c, [v](T e, int i, T *A) { return e / v; });
}

template<typename T>
Vector operator /(T *c, const T *v) {
  return map(c, [v](T e, int i, T *A) { return e / v[i]; });
}

template<typename T>
Vector operator /(T *c, const Vector& v) {
  // if (v.d != d) return util::Status(util::INVALID_ARGUMENT, "dimensions must be equal");
  return map(c, [v](T e, int i, T *A) { return e / v[i]; });
}

template<typename T>
bool equals(T *c, const Vector& v, T tolerance) const {
  return reduce([v, tolerance](bool a, T e, int i, T *A) { return a && TEquals(e, v[i], tolerance); }, true);
}

template<typename T>
StatusOr<T> dot(T *c, const Vector& v) const {
  return reduce([v](T a, T e, int i, T *A) { return a + e * v[i]; }, 0);
}