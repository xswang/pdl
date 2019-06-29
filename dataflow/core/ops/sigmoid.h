/*
 * sigmoid.h
 * Copyright (C) 2019 XiaoshuWang <2012wxs@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef SIGMOID_H
#define SIGMOID_H
#include <string>
#include <memory>

#include "core/ops/operator.h"

namespace dataflow{
class Sigmoid : public Operator {
 public:
  Sigmoid() {}
  ~Sigmoid() {}
  std::string get_op_name() {return "Sigmoid";}
  void initialize(Node* node) {
    std::cout << "hi" << std::endl;
  }
 private:
  Node* _node;
  std::shared_ptr<BlobMatrix> _input;

};

}

#endif /* !SIGMOID_H */
