/*
 * Reader.h
 * Copyright (C) 2019 XiaoshuWang <2012wxs@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef READER_H
#define READER_H
#include <string>

#include "core/ops/operator.h"
#include "core/ops/matrix_blob.h"

namespace dataflow{
class Reader : public Operator {
 public:
  Reader() {}
  ~Reader() {}
  std::string get_op_name() {return "Reader";}
  void initialize(Node* node) {
    std::cout << "initialize " << node->name_ << std::endl;
  }
 
 private:
  Node* _node;
};

}

#endif /* !READER_H */
