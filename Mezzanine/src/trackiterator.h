// © Copyright 2010 - 2014 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
#ifndef _trackiterator_h
#define _trackiterator_h

#include "countedptr.h"

#ifndef SWIG // STD headers are bad for Swig
    #include <cmath>
    #include <limits>

    #include "crossplatform.h"
#endif


namespace Mezzanine
{
    // Forward declation
    template <typename InterpolatableType> class TrackBase;

    /// @brief An Iterator that can take an arbitrary amount of steps through a track.
    /// @details This stores a value between 0.0 and 1.0 as the current location on a
    /// track, and another value between 0.0 an 1.0 as how muc to move when
    /// incremented. Everytime this is dereferenced it makes a call against the
    /// track it targets and gets the point on the track that corresponds with is current
    /// location. For example, presume SomeTrack is a valid track instance:
    /// @code
    /// SmoothTrackIterator< LinearInterpolator<Vector3> > Iter(&SomeTrack,0.0,1.0/200.0);
    ///
    /// for(Whole Counter = 0; Counter<200; Counter++
    /// {
    ///     std::cout << *Iter << std::endl;
    /// }
    /// @endcode
    /// This code will output 200 Vector3's lying on the path define by the track, which
    /// are each separated b approximately 0.5% ofthe tracks full length. If the track is
    /// small this could look pretty smooth
    template<typename InterpolatorType>
    class SmoothTrackIterator
    {
        public:
            /// @brief The type the interpolator this works with uses.
            typedef typename InterpolatorType::InterpolatableType InterpolatableType;
            /// @brief What type is this iterator working with.
            typedef InterpolatableType value_type;
            /// @brief When doing iterator math what is the type of math results
            typedef Real difference_type;
            /// @brief The type of a pointer to the iterated type
            typedef InterpolatableType* pointer;
            /// @brief The type of a reference to the iterated type
            typedef InterpolatableType& reference;
            /// @brief This almost supports random access iteration, it does not support any kind of writing to the container
            typedef std::random_access_iterator_tag iterator_category;
            /// @brief What kind of track with this iterate over
            typedef TrackBase<InterpolatorType> TargetTrackType;
            /// @brief The kind of this iterator.
            typedef SmoothTrackIterator<InterpolatorType> ThisType;

        protected:
            /// @brief The track this works against.
            const TargetTrackType* const TargetTrack;
            /// @brief Where on the track are we?
            Real Location;
            /// @brief How far should this
            Real Step;

            /// @brief Move the iterator in the direction of the step.
            void Increment()
                { Location += Step; }
            /// @brief Move the iterator in the opposite direction from the step.
            void Decrement()
                { Location -= Step; }
            /// @brief Move the iterator a multiple (including negative multiples) of the step.
            void StepAdjust(Integer Steps)
                { Location += Step * PreciseReal(Steps); }

        public:
            /// @brief The constructor for an iterator that can take an arbitrary amount steps through a series of data points
            /// @details If default constructed this cannot be dereferenced and is only useful for comparisons
            /// @param TrackToIterate Which track with this work against.
            /// @param WhereToStart Where on the track (range 0 to 1) Should iteration start.
            /// @param Increment When incremented how much should the location change? Defaults to .01 to create 100 steps.
            SmoothTrackIterator(const TargetTrackType* const TrackToIterate = 0, Real WhereToStart = 0.0, Real Increment = 0.01)
                : TargetTrack(TrackToIterate), Location(WhereToStart), Step(Increment)
                {}
            /// @brief Create a copy of an SmoothTrackIterator.
            /// @param Copy The SmoothTrackIterator to copy.
            SmoothTrackIterator(const ThisType& Copy)
                : TargetTrack(Copy.TargetTrack), Location(Copy.Location), Step(Copy.Step)
                {}

            /// @brief Change this SmoothTrackIterator to match another (Except for Track)
            /// @param Other The SmoothTrackIterator to copy, except for its target track
            /// @return A SmoothTrackIterator<InterpolatableType>
            SmoothTrackIterator<InterpolatorType>& operator=(const ThisType& Other)
            {
                Location=Other.Location;
                Step=Other.Step;
                return *this;
            }

            /// @brief Is this SmoothTrackIterator on the same track and in the same place as another.
            /// @param Other The Other SmoothTrackIterator to compare this one too.
            /// @return True if the track is the same and the location on the track is close enough to be within 1 epsilon.
            bool operator==(const ThisType& Other) const
            {
                return //TargetTrack==Other.TargetTrack &&
                       (Other.Location-std::numeric_limits<Real>::epsilon())<=Location &&
                       Location<=(Other.Location+std::numeric_limits<Real>::epsilon());
            }
            /// @brief Is this SmoothTrackIterator not on the same track and in the same place as another.
            /// @param Other The Other SmoothTrackIterator to compare this one too.
            /// @return False if the track is the same and the location on the track is close enough to be within 1 epsilon.
            bool operator!=(const ThisType& Other) const
                { return !operator==(Other); }

            /// @brief Get the current location on the SmoothTrackIterator
            /// @return An instance of InterpolatableType that is read only
            /// @warning Most iterators return a reference, to allow changes in the underlying container. This returns points that are not stored, so they cannot be changed.
            /// @note Everytime this is called this it calls the interpolator in the Target Track, This should run in constant time, but is much slower than normal pointer dereferences.
            virtual InterpolatableType operator*() const
                { return TargetTrack->GetInterpolated(Location); }
            /// @brief Derefernce this with the syntax for pointer member access.
            /// @return A Counted pointer to a temporary InterpolatableType instance.
            /// @warning This is read only because it is not stored anywhere.
            /// @note Everytime this is called it calls the interpolator in the Target Track.
            virtual CountedPtr<InterpolatableType> operator->() const
                { return CountedPtr<InterpolatableType> (new InterpolatableType(TargetTrack->GetInterpolated(Location))); }

            /// @brief Move the SmoothTrackIterator backwards on the track by on step.
            /// @details The iterator is moved to a new position by subtracting the
            /// step from the current location.
            /// @return A Reference to this iterator after the change has been made.
            ThisType& operator--()
            {
                Decrement();
                return *this;
            }
            /// @brief Move the SmoothTrackIterator backwards on the track and get a copy of its location before
            /// @details Like the prefix -- this moves the iterator, but this returns a copy
            /// of the iterator before being increment.
            /// @return An iterator that is a copy of this one before the decrement.
            /// @note Even though the results of this could be assignable doing so is useless without storing the results in a new iterator so this is made const.
            const ThisType operator--(int)
            {
                ThisType Results(*this);
                Decrement();
                return Results;
            }

            /// @brief Move the SmoothTrackIterator forwards on the track by on step.
            /// @details The iterator is moved to a new position by adding the
            /// step from the current location.
            /// @return A Reference to this iterator after the change has been made.
            ThisType& operator++()
            {
                Increment();
                return *this;
            }
            /// @brief Move the SmoothTrackIterator forwards on the track and get a copy of its location before
            /// @details Like the prefix ++ this moves the iterator, but this returns a copy
            /// of the iterator before being incremented.
            /// @return An iterator that is a copy of this one before the decrement.
            /// @note Even though the results of this could be assignable doing so is useless without storing the results in a new iterator so this is made const.
            const ThisType operator++(int)
            {
                ThisType Results(*this);
                Increment();
                return Results;
            }

            /// @brief Is the Iterator inside the track?
            /// @return This returns 0 if iterator is in the bounds of the track, -1 if before and 1 if after the bounds of the track.
            Integer BoundsCheck() const
            {
                if(1.0 < Location)
                    { return 1; }
                if(0.0 > Location)
                    { return -1; }
                return 0;
            }
            /// @brief If this is iterator is beyond the bounds of the track it target *wrap* it around to the other side
            /// @details Since the location on the track is stored as a value between 0.0 and 1.0 as long asthe step is
            /// less than ,subtracting or adding one will preserve the apparent offset from the last location on looped
            /// tracks
            /// @return True if the bounds where outside the track and false if they where not.
            bool BoundsCorrect()
            {
                Real Original = Location;
                while(1.0<Location)
                    { Location--; }
                while(0.0>Location)
                    { Location++; }
                return Original!=Location;
            }

            /// @brief Move a copy of this iterator a multiple of steps relative to the amount added
            /// @return A reference to this iterator to allow multiple math operations.
            /// @note Even though the results of this could be assignable doing so is useless without storing the results in a new iterator so this is made const.
            const ThisType operator+(Integer Steps)
            {
                ThisType Results(*this);
                Results.StepAdjust(Steps);
                return Results;
            }
            /// @brief Move a copy this iterator a negativemultiple of steps relative to the amount subtract
            /// @return A reference to this iterator to allow multiple math operations.
            /// @note Even though the results of this could be assignable doing so is useless without storing the results in a new iterator so this is made const.
            const ThisType operator-(Integer Steps)
            {
                ThisType Results(*this);
                Results.StepAdjust(-Steps);
                return Results;
            }

            /// @brief Move this iterator a given amount of steps forward
            /// @return A reference to this iterator so it could be used in other operations
            ThisType& operator+=(Integer Steps)
            {
                StepAdjust(Steps);
                return *this;
            }
            /// @brief Move this iterator a given amount of steps backwards
            /// @return A reference to this iterator so it could be used in other operations
            ThisType& operator-=(Integer Steps)
            {
                StepAdjust(-Steps);
                return *this;
            }

            /// @brief Compare which iterator is further along the track
            /// @note The target track and the step size are ignored. This allows for potentially non-sensensical comparison.
            /// @param Right The value on the right of the <.
            /// @return true if this iterator is closer to the start of its track than right one, false otherwise.
            bool operator<(const ThisType& Right)
                { return Location < Right.Location; }
            /// @brief Compare which iterator is further toward the track beginning.
            /// @note The target track and the step size are ignored. This allows for potentially non-sensensical comparison.
            /// @param Right The value on the right of the >.
            /// @return true if the other iterator is closer to the start of its track than right one, false otherwise.
            bool operator>(const ThisType& Right)
                { return Location > Right.Location; }
            /// @brief Compare which iterator is further along the track
            /// @note The target track and the step size are ignored. This allows for potentially non-sensensical comparison.
            /// @param Right The value on the right of the <=.
            /// @return true if this iterator is closer to the start of its track than right one(or they are equidistant), false otherwise.
            bool operator<=(const ThisType& Right)
                { return Location <= Right.Location; }
            /// @brief Compare which iterator is further toward the track beginning.
            /// @note The target track and the step size are ignored. This allows for potentially non-sensensical comparison.
            /// @param Right The value on the right of the >=.
            /// @return true if the other iterator is closer to the start of its track than right one(or they are equidistant), false otherwise.
            bool operator>=(const ThisType& Right)
                { return Location >= Right.Location; }

            /// @brief Get an iterator an arbitrary number of steps forward or backwards.
            /// @note Even though the results of this could be assignable doing so is useless without storing the results in a new iterator so this is made const.
            /// @param Steps How many times to increment or decrement the iterator.
            const ThisType operator[](Integer Steps)
                { return *this + Steps; }
    };

    /// @brief This allows for addition with and an Integer on the Left hand side, such as: 2 + Iter
    /// @param Steps The Integer on the left.
    /// @param Iter The SmoothTrackIterator on the right.
    template<typename InterpolatableType>
    SmoothTrackIterator<InterpolatableType>&  operator+(Integer Steps, SmoothTrackIterator<InterpolatableType>& Iter)
        { return Iter + Steps; }
    /// @brief This allows for subtraction with and an Integer on the Left hand side, such as: 2 + Iter
    /// @param Steps The Integer on the left.
    /// @param Iter The SmoothTrackIterator on the right.
    template<typename InterpolatableType>
    SmoothTrackIterator<InterpolatableType>&  operator-(Integer Steps, SmoothTrackIterator<InterpolatableType>& Iter)
        { return Iter - Steps; }




    /// @brief This will take the same amount of clock time to iterate over a range.
    /// @details This will spread movement over a range of a track evenly throughout
    /// a period of time. For example if you have a track of Real values ranging
    /// from 0.0 to 10.0, interpolating the point at 0.0 will return 0.0 and the
    /// point 1.0 will return 10.0. Continuing this example if we were to define an
    /// iterator over this range that will take exactly 1,000 milliseconds to
    /// complete its journey, if we increment and dereference this 500 milliseconds
    /// after it was instantiated it will return 5.0. This is the point halfway
    /// through the range of the track because the time is halfway consumed.
    /// @n @n
    /// Here is a code sample that takes 3/4 of a second to iterate over the first
    /// quarter of the data in a track
    /// @code
    /// TimedTrackIterator< LinearInterpolator<Vector3> > Iter(&SomeTrack,0.0,0.25,750);
    ///
    /// bool SimulationIsStillRunning = true;
    /// While(SimulationIsStillRunning)
    /// {
    ///   // This does not increment the iterator like normal iterators, it
    ///   // just updates an internal record of time
    ///   Iter++;
    ///
    ///   // When de-referencing the time is update
    ///   std::cout << *Iter << std::endl;
    ///
    ///   // DoOtherSimulationStuff will return false when the simulation end, and
    ///   // presumably it does all the other stuff
    ///   SimulationIsStillRunning = DoOtherSimulationStuff();
    /// }
    /// @endcode
    /// This code will emit to the standard output at least one Vector3 to the standard
    /// output. All output will be between(inclusive) 0.0 and 0.25 with times closer to
    /// the iterator creation time being near 0.0 and times closer to 750
    template<typename InterpolatorType>
    class TimedTrackIterator
    {
        public:
            /// @brief The type the interpolator this works with uses.
            typedef typename InterpolatorType::InterpolatableType InterpolatableType;
            /// @brief What type is this iterator working with.
            typedef InterpolatableType value_type;
            /// @brief The type of a pointer to the iterated type
            typedef InterpolatableType* pointer;
            /// @brief The type of a reference to the iterated type
            typedef InterpolatableType& reference;
            /// @brief This almost supports random access iteration, it does not support any kind of writing to the container
            typedef std::input_iterator_tag iterator_category;
            /// @brief What kind of track with this iterate over
            typedef TrackBase<InterpolatorType> TargetTrackType;
            /// @brief The kind of this iterator.
            typedef TimedTrackIterator<InterpolatorType> ThisType;

        protected:
            /// @brief Where Should Iteration Start
            Real StartRange;
            /// @brief Where should iteration stop
            Real EndRange;

            /// @brief What Time does iteration start at
            MaxInt StartTime;
            /// Where is this iterator now.
            MaxInt CurrentTime;
            /// @brief What Time does iteration ed at
            MaxInt EndTime;

            /// @brief The track this works against.
            const TargetTrackType* const TargetTrack;

            /// @brief Update the current location of this iterator based on the current time
            void Update()
            {
                MaxInt Now = crossplatform::GetTimeStamp();
                if(Now<StartTime)
                    { CurrentTime = StartTime; }
                if(EndTime<Now)
                    { CurrentTime = EndTime; }
                CurrentTime=Now;
            }

            InterpolatableType GetDereferenced() const
            {
                MaxInt Length(EndTime - StartTime);
                MaxInt Progress(CurrentTime - StartTime);
                Real RangeLength(EndRange - StartRange);

                Real AdjustedProgress( PreciseReal((Progress)/PreciseReal(Length)) * PreciseReal(RangeLength) + PreciseReal(StartRange) );
                return TargetTrack->GetInterpolated(AdjustedProgress);
            }

        public:
            /// @brief The constructor for and iterator
            /// @details Tracks
            /// @param TrackToIterate Which track with this work against.
            /// @param StartOnTrack In the range of 0.0 to 1.0 Where on the track should iteration start
            /// @param EndOnTrack In the range of 0.0 to 1.0 Where on the track should iteration end
            /// @param Duration
            /// @param WhenToStart
            TimedTrackIterator(const TargetTrackType* const TrackToIterate = 0,
                               Real StartOnTrack = 0.0,
                               Real EndOnTrack = 1.0,
                               MaxInt Duration = 1000,
                               MaxInt WhenToStart = crossplatform::GetTimeStamp())
                : StartRange(StartOnTrack),
                  EndRange(EndOnTrack),
                  StartTime(WhenToStart),
                  EndTime(Duration+StartTime),
                  TargetTrack(TrackToIterate)
                { Update(); }
            /// @brief Create a copy of an TimedTrackIterator.
            /// @param Copy The TimedTrackIterator to copy.
            TimedTrackIterator(const ThisType& Copy)
                : StartRange(Copy.StartRange),
                  EndRange(Copy.EndRange),
                  StartTime(Copy.StartTime),
                  CurrentTime(Copy.CurrentTime),
                  EndTime(Copy.EndTime),
                  TargetTrack(Copy.TargetTrack)
                { }
            /// @brief Change this TimedTrackIterator to match another (Except for Track)
            /// @param Other The TimedTrackIterator to copy, except for its target track
            /// @return A TimedTrackIterator<InterpolatableType>
            TimedTrackIterator<InterpolatorType>& operator= (const ThisType& Other)
            {
                StartRange=Other.StartRange;
                EndRange=Other.EndRange;
                StartTime=Other.StartTime;
                CurrentTime=Other.CurrentTime;
                EndTime=Other.EndTime;
                return *this;
            }

            /// @brief Is this TimedTrackIterator on the same track and in the same place as another.
            /// @param Other The Other TimedTrackIterator to compare this one too.
            /// @return True if the track is the same and the location on the same track.
            bool operator== (const ThisType& Other) const
            {
                return TargetTrack==Other.TargetTrack &&
                       StartRange==Other.StartRange &&
                       EndRange==Other.EndRange &&
                       StartTime==Other.StartTime &&
                       CurrentTime==Other.CurrentTime &&
                       EndTime==Other.EndTime;
            }
            /// @brief Is this TimedTrackIterator not on the same track and in the same place as another.
            /// @param Other The Other TimedTrackIterator to compare this one too.
            /// @return False if the track is the same and the location on the same track.
            bool operator!= (const ThisType& Other) const
                { return !operator==(Other); }

            /// @brief Get the location on track from the last time it was incremented
            /// @return An instance of InterpolatableType that is read only
            /// @warning Most iterators return a reference, to allow changes in the underlying container. This returns points that are not stored, so they cannot be changed.
            /// @note Everytime this is called this it calls the interpolator in the Target Track, This should run in constant time, but is much slower than normal pointer dereferences.
            virtual InterpolatableType operator*() const
                { return GetDereferenced(); }
            /// @brief Derefernce this with the syntax for pointer member access.
            /// @return A Counted pointer to a temporary InterpolatableType instance.
            /// @warning This is read only because it is not stored anywhere.
            /// @note Everytime this is called it calls the interpolator in the Target Track.
            virtual CountedPtr<InterpolatableType> operator->() const
                { return CountedPtr<InterpolatableType> (new InterpolatableType(GetDereferenced())); }

            /// @brief Move the SmoothTrackIterator forwards on the track by on step.
            /// @details The iterator is moved to a new position by adding the
            /// step from the current location.
            /// @return A Reference to this iterator after the change has been made.
            ThisType& operator++()
            {
                Update();
                return *this;
            }
            /// @brief Move the SmoothTrackIterator forwards on the track and get a copy of its location before
            /// @details Like the prefix ++ this moves the iterator, but this returns a copy
            /// of the iterator before being incremented.
            /// @return An iterator that is a copy of this one before the decrement.
            /// @note Even though the results of this could be assignable doing so is useless without storing the results in a new iterator so this is made const.
            const ThisType operator++(int)
            {
                ThisType Results(*this);
                Update();
                return Results;
            }
    };




}//Mezzanine

#endif

