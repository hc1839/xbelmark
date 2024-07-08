#ifndef XBELMARK_ITERATOR_H
#define XBELMARK_ITERATOR_H

namespace xbelmark {

/**
 *  Iterator over a collection.
 */
template <typename T>
class Iterator {
 public:
  virtual ~Iterator() = default;

  /**
   *  Whether the iteration has more elements.
   */
  virtual bool HasNext() const = 0;

  /**
   *  Next element in the iteration.
   *
   *  An exception is thrown if there are no more elements.
   */
  virtual T Next() = 0;
};

} // namespace xbelmark

#endif
