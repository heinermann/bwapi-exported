#pragma once
#include <functional>
#include <limits.h>
#include "UnaryFilter.h"

#define BWAPI_COMPARE_FILTER_OP(op) UnaryFilterBase<_Param> operator op(const int &cmp) const               \
                                    {   return [&](_Param u)->bool{ return (*this)(u) op cmp; };   }
#define BWAPI_ARITHMATIC_FILTER_OP(op) template <typename _T>                                                 \
                                       CompareFilterBase<_Param> operator op(const _T &other) const     \
                                       {   return [&](_Param u)->int{ return (*this)(u) op other(u); };   }

namespace BWAPI
{
  /// @~English
  /// The CompareFilter is a temporary container
  /// in which the stored function predicate
  /// returns a value. If any relational
  /// operators are used, then it becomes
  /// a UnaryFilter.
  ///
  /// @~
  template<typename _Param>
  class CompareFilterBase
  {
  private:
    std::function<int(_Param)> pred;
  public:
    // Constructor
    template <typename _T>
    CompareFilterBase(const _T &predicate) : pred(predicate)
    {};

    // Assignment
    template <typename _T>
    CompareFilterBase &operator =(const _T& other)
    {
      this->pred.assign(other);
      return *this;
    };

    // Comparisons
    BWAPI_COMPARE_FILTER_OP(==);
    BWAPI_COMPARE_FILTER_OP(!=);
    BWAPI_COMPARE_FILTER_OP(<=);
    BWAPI_COMPARE_FILTER_OP(>=);
    BWAPI_COMPARE_FILTER_OP(<);
    BWAPI_COMPARE_FILTER_OP(>);

    BWAPI_ARITHMATIC_FILTER_OP(+);
    BWAPI_ARITHMATIC_FILTER_OP(-);
    BWAPI_ARITHMATIC_FILTER_OP(|);
    BWAPI_ARITHMATIC_FILTER_OP(&);
    BWAPI_ARITHMATIC_FILTER_OP(*);
    BWAPI_ARITHMATIC_FILTER_OP(^);

    template <typename _T>
    CompareFilterBase<_Param> operator /(const _T &other) const
    {   
      return [&](_Param u)->int{ int rval = other(u);
                                 return rval == 0 ? INT_MAX : (*this)(u) / rval;
                               };
    };
    template <typename _T>
    CompareFilterBase<_Param> operator %(const _T &other) const
    {   
      return [&](_Param u)->int{ int rval = other(u);
                                 return rval == 0 ? 0 : (*this)(u) % rval;
                               };
    };

    // call
    inline int operator()(_Param u) const
    {
      return pred(u);
    };
    
    inline operator bool() const
    {
      return (bool)pred;
    };
  };
}
