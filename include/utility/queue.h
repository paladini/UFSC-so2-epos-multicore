// EPOS Queue Utility Declarations

// Queue is a traditional queue, with insertions at the tail
// and removals either from the head or from specific objects 

// Ordered_Queue is an ordered queue, i.e. objects are inserted
// in-order based on the integral value of "element.rank". Note that "rank"
// implies an order, but does not necessarily need to be "the absolute order"
// in the queue; it could, for instance, be a priority information or a 
// time-out specification. Insertions must first tag "element" with "rank".
// Removals are just like in the traditional queue. Elements of both Queues
// may be exchanged.
// Example: insert(B,7);insert(C,9);insert(A,4)
// +---+		+---+	+---+	+---+
// |obj|		| A |-->| B |-->| C |
// + - +       head -->	+ - +	+ - +	+ - + <-- tail
// |ord|		| 4 |<--| 7 |<--| 9 |
// +---+ 		+---+	+---+	+---+

// Relative Queue is an ordered queue, i.e. objects are inserted
// in-order based on the integral value of "element.rank" just like above.
// But differently from that, a Relative Queue handles "rank" as relative 
// offsets. This is very useful for alarm queues. Elements of Relative Queue
// cannot be exchanged with elements of the other queues described earlier.
// Example: insert(B,7);insert(C,9);insert(A,4)
// +---+		+---+	+---+	+---+
// |obj|		| A |-->| B |-->| C |
// + - +       head -->	+ - +	+ - +	+ - + <-- tail
// |ord|		| 4 |<--| 3 |<--| 2 |
// +---+ 		+---+	+---+	+---+

// Scheduling Queue is an ordered queue whose ordering criterion is externally
// definable and for which selecting methods are defined (e.g. choose). This
// utility is most useful for schedulers, such as CPU or I/O.

#ifndef __queue_h
#define	__queue_h

#include "list.h"

__BEGIN_UTIL

// Queue
template<typename T,
         typename El = List_Elements::Doubly_Linked<T> >
class Queue: public List<T, El> {};


// Ordered Queue
template<typename T,
         typename R = List_Element_Rank,
         typename El = List_Elements::Doubly_Linked_Ordered<T, R> >
class Ordered_Queue: public Ordered_List<T, R, El> {};


// Relatively-Ordered Queue
template<typename T, 
         typename R = List_Element_Rank,
         typename El = List_Elements::Doubly_Linked_Ordered<T, R> >
class Relative_Queue: public Relative_List<T, R, El> {};

__END_UTIL

#endif
