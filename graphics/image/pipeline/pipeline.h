#ifndef GRAPHICS_IMAGE_PIPELINE_PIPELINE_H
#define GRAPHICS_IMAGE_PIPELINE_PIPELINE_H

template <typename Input, typename Output>
class Worker {
  
};

template <typename T>
class Tensor {
 public:
  size_t GetOrder();
  size_t GetDimension(size_t i);
  
  T Get(size_t *position);
  Tensor<T> SubTensor(size_t dimensions, size_t *start, size_t end);
  Tensor<T> SubTensor(size_t dimensions, size_t i);
  Tensor<T> SubTensor(size_t i);
};

enum JobState {
  JOB_WAITING,
  JOB_READY,
  JOB_RUNNING,
  JOB_COMPLETE
};

template <typename Input, typename Output>
class Job {
 public:
  JobState GetState();
};

template <typename Input>
class Source {
  
};

template <typename Output>
class Sink {
  
};

#endif // GRAPHICS_IMAGE_PIPELINE_PIPELINE_H