#ifndef _MANAK_RUN_TREE_HPP_INCLUDED
#define _MANAK_RUN_TREE_HPP_INCLUDED

#include <map>
#include <string>
#include <list>
#include <tuple>

#include "pmeasure.hpp"
#include "benchmark_case.hpp"

#include <manak/util/version.hpp>

namespace manak
{

struct RNode
{
  RNode(RNode* parent)
    : parent(parent)
  {

  }

  bool AddNext(const std::string& name,
               RNode*& n)
  {
    auto it = nexts.find(name);
    if(it != nexts.end())
    {
      n = it->second;
      return false;
    }
    n = new RNode(this);
    nexts[name] = n;
    return true;
  }

  void AddCase(BenchmarkCase* bc, size_t l_id)
  {
    children[l_id] = bc;
  }

  void Run()
  {
    for(auto n : nexts)
    {
      n.second->Run();
    }

    for(auto c : children)
    {
      std::cout << "Running " << c.second->Name() << "...";
      auto l = c.second->Run();
      results[c.first] = l;
      std::cout << " [DONE]" << std::endl;
    }
  }

  void PrintTXT(std::ostream& stream, size_t l_ids)
  {
    for(auto it : nexts)
    {
      it.second->PrintTXT(stream, l_ids);
    }

    if(children.size() != 0)
    {
      std::list<std::tuple<std::string, double, PMeasure>> dummy;
      std::list<std::tuple<std::string, double, PMeasure>>::iterator it_s[l_ids];

      for(size_t i = 0;i < l_ids;i++)
      {
        auto it = results.find(i);
        if(it != results.end())
          it_s[i] = it->second.begin();
        else it_s[i] = dummy.end();
      }

      size_t num_entries = 0;
      for(auto l : results)
      {
        if(l.second.size() > num_entries)
          num_entries = l.second.size();
      }

      if(num_entries > 1)
      {
        stream << children.begin()->second->Name() << std::endl;

        for(size_t index = 0;index < num_entries;index++)
        {
          std::stringstream s_values;
          s_values << std::setiosflags(std::ios::left);

          std::string sub_name = "";
          bool name_mismatch = false;

          for(size_t i = 0;i < l_ids;i++)
          {
            std::stringstream ss;

            if(it_s[i] != dummy.end())
            {
              if(it_s[i] != results.find(i)->second.end())
              {
                if(!name_mismatch)
                {
                  if(sub_name != "" && sub_name != std::get<0>(*it_s[i]))
                    name_mismatch = true;
                  else
                    sub_name = std::get<0>(*it_s[i]);
                }
                ss << std::get<2>(*it_s[i]);
                it_s[i]++;
              }
              else ss << "---";
            }
            else ss << "---";

            s_values << std::setw(20) << ss.str();
          }

          std::stringstream s_sub_name;
          s_sub_name << "  ";
          if(!name_mismatch)
          {
            s_sub_name << sub_name;
          }
          else
          {
            s_sub_name << "Parameter Set " << index;
          }

          stream << std::setw(30) << s_sub_name.str();
          stream << s_values.str();

          stream << std::endl;
        }
      }
      else
      {
        stream << std::setw(30) << children.begin()->second->Name();

        for(size_t i = 0;i < l_ids;i++)
        {
          std::stringstream ss;

          if(it_s[i] != dummy.end())
          {
            if(it_s[i] != results.find(i)->second.end())
            {
              ss << std::get<2>(*it_s[i]);
            }
            else ss << "---";
          }
          else ss << "---";

          stream << std::setw(20) << ss.str();
        }

        stream << std::endl;
      }
      stream << std::endl;
    }
  }

  RNode* parent;

  std::map<std::string, RNode*> nexts;

  std::map<size_t, BenchmarkCase*> children;
  std::map<size_t, std::list<std::tuple<std::string, double, PMeasure>>> results;
};

class RunTree
{
 public:
  RunTree()
    : root(new RNode(NULL)), current_node(root), total_nodes(0), current_l_id(0)
  {
  }

  static RunTree& GlobalRunTree()
  {
    static RunTree singleton;
    return singleton;
  }

  void AddSuite(const std::string& name)
  {
    RNode* temp;
    current_node->AddNext(name, temp);
    current_node = temp;
  }

  void CloseSuite()
  {
    current_node = current_node->parent;
  }

  void AddCase(BenchmarkCase* bc)
  {
    RNode* c;
    bool res = current_node->AddNext(bc->Name(), c);

    if(res)
      total_nodes++;

    size_t l_id;
    auto it = l_map.find(bc->LibraryName());
    if(it != l_map.end())
      l_id = it->second;
    else
    {
      l_map[bc->LibraryName()] = current_l_id++;
      l_id = current_l_id - 1;
    }

    c->AddCase(bc, l_id);
  }

  void Run()
  {
    std::cout << std::setiosflags(std::ios::left);
    std::cout << "######################################################################"
           << std::endl;
    std::cout << "#  " << std::setw(66) << "Manak C++ Benchmarking Library" << "#"
           << std::endl;

    std::stringstream ss;
    ss << "Version " << __MANAK_VERSION_MAJOR << "." << __MANAK_VERSION_MINOR
       << "." << __MANAK_VERSION_PATCH;
    std::cout << "#  " << std::setw(66) << ss.str() << "#" << std::endl;

    std::cout << "######################################################################"
           << std::endl << std::endl;

    std::cout << "Running " << total_nodes << " benchmarks with " << l_map.size()
              << " libraries." << std::endl << std::endl;

    root->Run();
  }

  void PrintTXT(std::ostream& stream)
  {
    stream << std::setiosflags(std::ios::left);
    stream << "######################################################################"
           << std::endl;
    stream << "#  " << std::setw(66) << "Manak C++ Benchmarking Library" << "#"
           << std::endl;

    std::stringstream ss;
    ss << "Version " << __MANAK_VERSION_MAJOR << "." << __MANAK_VERSION_MINOR
       << "." << __MANAK_VERSION_PATCH;
    stream << "#  " << std::setw(66) << ss.str() << "#" << std::endl;

    std::stringstream ss2;
    ss2 << "Created at " << Timer::getTimeStamp();

    stream << "#  " << std::setw(66) << ss2.str() << "#" << std::endl;

    stream << "######################################################################"
           << std::endl << std::endl;

    stream << std::setprecision(3);
    stream << std::setw(30) << "       Case Name";

    for(size_t i = 0;i < l_map.size();i++)
    {
      stream << std::setw(20);
      for(auto it : l_map)
      {
        if(it.second == i)
        {
          stream << it.first;
          break;
        }
      }
    }

    stream << std::endl;
    root->PrintTXT(stream, l_map.size());
  }

 private:
  RNode* root;
  RNode* current_node;

  std::map<std::string, size_t> l_map;

  size_t total_nodes;
  size_t current_l_id;
};

}


#endif // _MANAK_RUN_TREE_HPP_INCLUDED
