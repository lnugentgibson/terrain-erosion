#ifndef NOISE_H
#define NOISE_H

float curve1(float x) {
  return x;
}

float curve3(float x) {
  return x * x * (-x * 2 + 3);
}

float curve(float x, int d) {
  switch(d) {
    case 3:
      return x * x * (-x * 2 + 3);
    default:
      return x;
  }
}

#endif // NOISE_H