
#include "ThreadIndex.h"

ThreadIndex& ThreadIndex::operator++()
{
    Index++;
    return *this;
}

ThreadIndex ThreadIndex::operator+(const ThreadIndex::InternalType& Increment) const
{
    /// @todo Add bounds checking to ThreadIndex::operator+. This is not used internally and not part of any planned
    /// workflow so it shouldn't need strong optimization.
    return ThreadIndex{Index + Increment};
}

ThreadIndex::InternalType ThreadIndex::AsRaw() const
{
    return Index;
}

bool operator== (const ThreadIndex& LeftHand, const ThreadIndex& RightHand)
{
    return LeftHand.Index == RightHand.Index;
}

std::ostream& operator<< (std::ostream& OutputStream, const ThreadIndex& OneThreadIndex)
{
    return OutputStream << "Thread Index: " << OneThreadIndex.Index;
}

bool operator <(const ThreadIndex& LeftHand, const ThreadIndex& RightHand)
{
    return LeftHand.Index < RightHand.Index;
}
