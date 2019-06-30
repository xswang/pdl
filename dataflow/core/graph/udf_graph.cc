/*
 * udf_graph.cpp
 * Copyright (C) 2017 wangxiaoshu <2012wxs@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "core/graph/udf_graph.h"

#include "core/graph/graph_iter.h"

namespace dataflow {

UdfGraph::UdfGraph(dataflow::GraphDef& graph_def) 
  : graph_def_(graph_def) {
  GraphBuilder();
}
UdfGraph::~UdfGraph() {}

void UdfGraph::GraphBuilder() {
  for(const NodeDef ndef : graph_def_.node_def()){
      BuildNode(ndef);
  }
  NodeDef def;
  def.set_name("SOURCE");
}

void UdfGraph::BuildNode(const NodeDef node_def) {
  Node* node = AddNode(node_def);
  for(const std::string in : node_def.in()) {
    //std::cout<<"in = "<<in<<std::endl;
    std::size_t pos = in.find("/");
    //std::cout<<"pos = "<<pos<<std::endl;
    std::string input_name = in.substr(0, pos);
    // std::cout<<"input_name = "<<input_name<<std::endl;
    pos = in.find("_");
    std::string type = in.substr(pos+1);
    // std::cout << " in_type = " << type << std::endl;
    Node* InNode = nullptr;
    std::unordered_map<std::string, Node*>::iterator iter;
    iter = name_to_node_.find(input_name);
    if(iter != name_to_node_.end()){
      InNode = iter->second;
    } else {
      InNode = NewNode(input_name, type);
    }
    node->pre_nodes_.insert({InNode});
    InNode->post_nodes_.insert({node});
    ++node->input_num_;
    Edge* edge = AddEdge(InNode, node);
  }
}

// Node* UdfGraph::AddNode(const std::string& name, const std::string& type) {
Node* UdfGraph::AddNode(const NodeDef node_def) {
  Node* node = nullptr;
  std::string node_name = node_def.name();
  std::string node_type = node_def.type();
  node = NewNode(node_name, node_type);
  std::string key = node_type + "_proto";

  const google::protobuf::Descriptor *d = node_def.GetDescriptor();
  const google::protobuf::Reflection *r = node_def.GetReflection();
  std::vector<const google::protobuf::FieldDescriptor*> fields;
  r->ListFields(node_def, &fields);

  for (int i = 0; i < fields.size(); ++i) {
    std::cout << fields[i]->name() << std::endl;
    if (fields[i]->name() == key) {
      auto& weight_proto = r->GetMessage(node_def, fields[i]);
      const google::protobuf::Descriptor *d = weight_proto.GetDescriptor();
      const google::protobuf::Reflection *r = weight_proto.GetReflection();
      std::vector<const google::protobuf::FieldDescriptor*> fields;
      r->ListFields(weight_proto, &fields);
      for (int j = 0; j < fields.size(); ++j) {
        std::cout << "===" << fields[j]->name() << std::endl;
        if (fields[j]->name() == "name" || fields[j]->name() == "data" || fields[j]->name() == "label" || fields[j]->name() == "loss") {
          std::cout << "====== value " << r->GetString(weight_proto, fields[j]) << std::endl;
        } else {
          std::cout << fields[j]->name() << std::endl;
          std::cout << "===== value " << r->GetUInt32(weight_proto, fields[j]) << std::endl;
        }
      }
    }
  }
  std::cout << std::endl;
  
  

  nodes_.insert({node});
  name_to_node_.insert({node_name, node});
  return node;
}

Edge* UdfGraph::AddEdge(Node* src, Node* dst) {
  Edge* e = nullptr;
  e = NewEdge(src, dst);
  e->src_ = src;
  e->dst_ = dst;
  src->output_edges_.insert({e});
  dst->input_edges_.insert({e});
  edges_.insert({e});
  return e;
}

}//namespace dataflow


